#include "ppoly.h"

PPoly::PPoly() {
    pieces = std::vector<std::shared_ptr<Piece>>();
}

const Poly PPoly::zero = Poly(1);

std::ostream& operator<<(std::ostream& os, const PPoly& p) {
    os << '[';
    for(std::shared_ptr<Piece> c : p.pieces) {
       os << "(" << c->span.a << "," << c->span.b << ")" << " ";
       os << c->poly << ",";
    }
    os << "\b]";
    return os;
}

Float PPoly::operator()(Float x) {
    if(x >= domain.a and x <= domain.b) {
        for(size_t i = 0; i < pieces.size(); i++) {
            if(pieces[i]->span.a <= x and pieces[i]->span.b >= x) {
                return pieces[i]->poly.evaluate(x);
            }
        }
    }
    return 0.0;
}

Poly& PPoly::operator[](int i) {
    if(i >= 0 and i < (int)pieces.size()) {
        return pieces[i]->poly;
    }
    return pieces[0]->poly;
}

PPoly& PPoly::operator<<(Piece p) {
    pieces.emplace_back(std::make_shared<Piece>(p));
    return *this;
}

Float PPoly::integral(Float a, Float b) {
    Float o = 0.0;
    for(std::shared_ptr<Piece> p : pieces) {
        //asumes theres a knot at a and b (might not be!)
        if(p->span.a >= a and p->span.b <= b) {
            auto i = p->poly.integral();
            o += i.evaluate(p->span.b) - i.evaluate(p->span.a);
        }
    }
    return o;
}

void PPoly::cull() {
    for(size_t i = 0; i < pieces.size(); i++) {
        if(pieces[i]->poly.isZero(0)) {
            pieces.erase(pieces.begin()+i);
            i--;
        }
    }
    domain.a = 1;
    domain.b = 0;
    for(auto p : pieces) {
        domain.a = std::min(domain.a, p->span.a);
        domain.b = std::max(domain.b, p->span.b);
    }
    return;
}

