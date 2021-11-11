#include "spline.h"

Spline::Spline()
{

}

std::ostream& operator<<(std::ostream& os, const Spline& s) {
    os << '[';
    const int z = s.basis.size();
    for(int i = 0; i < z; i++) {
       os << s.basis[i];
       if(i != z-1) {
           os << ",\n\n";
       }
    }
    os << "]";
    return os;
}

void Spline::linear() {
    _order = 0;
    basis = std::vector<PPoly>();

    for(int i = 0; i < size()-1; i++) {
        PPoly pp = PPoly();
        for(int k = 0; k < size()-1; k++) {
            Poly p = Poly(1);
            p << (i == k ? 1.0 : 0.0);
            Span s = {operator()(k), operator()(k+1)};
            pp << (Piece){s, p};
        }
        basis.emplace_back(pp);
    }
}


void Spline::interpolate() {
    _order++;
    size_t z = (int)size() - 1;
    if(z <= _order)
        return;
    for(size_t r = 0; r < z; r++) {
        for(size_t i = 0; i < z-_order; i++) {
            Poly a = basis[i][r] * lerp(i, _order);
            Poly b = basis[i+1][r] * lerp(i+1, _order, true);
            Poly n = a + b;
            basis[i][r] = n;
        }
    }
    basis.pop_back();
}
void Spline::cull() {
    for(int i = 0; i < basis.size(); i++) {
        basis[i].cull();
    }
}

void Spline::computeBasis(int order) {
    linear();
    for(int i = 0; i < order; i++) {
        interpolate();
    }
    cull();
}

void Spline::computeIntegrals(Float a, Float b) {
    integrals = VectorXFloat(basis.size());
    int i = 0;
    for(auto p : basis) {
        integrals(i++) = p.integral(a, b);
    }
}

void Spline::compute(int order, Float a, Float b) {
    computeBasis(order);
    computeIntegrals(a, b);
}

Float Spline::evaluate(Float x) {
    Float o = 0.0;
    for(auto b : basis) {
        o += b.evaluate(x);
    }
    return o;
}

Poly Spline::lerp(int i, int p, bool inverse) {
    Float a = 0.0;
    Float b = 0.0;
    auto o = Poly(2);

    if(knot(i) != knot(i+p)) {
        a = 1.0/(knot(i+p) - knot(i));
        b = -knot(i) * a;
    }
    if(inverse) {
        a = -a;
        b = 1-b;
    }
    o << b, a;
    return o;
}

Knot Spline::constructKnot(int N, int order) {
    Float step = 1.0/(Float)N;
    Knot k = Knot(N+(order*2)+1);
    for(int i = -order; i <= N+order; i++) {
        k(i+order) = i*step;
    }
    return k;
}
