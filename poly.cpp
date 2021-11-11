#include "poly.h"

Float Poly::evaluate(Float x) {
    Float y = 0;
    Float t = 1;
    for(int p = 0; p <= degree(); p++) {
        y += t*operator()(p);
        t *= x;
    }
    return y;
}

Poly& Poly::operator+=(Poly o) {
    resize(std::max(size(), o.size()));
    for(int i = 0; i < o.size(); i++) {
        operator()(i) += o(i);
    }
    return *this;
}

Poly& Poly::operator-=(Poly o) {
    resize(std::max(size(), o.size()));
    for(int i = 0; i < o.size(); i++) {
        operator()(i) -= o(i);
    }
    return *this;
}

Poly& Poly::operator*=(Poly o) {
    auto a = Poly(*this);
    const int z = a.size() + o.size();
    resize(z);
    setZero();

    for(int x = 0; x < a.size(); x++) {
        for(int y = 0; y < o.size(); y++) {
            operator()(x+y) += a(x) * o(y);
        }
    }
    return *this;
}
Poly& Poly::operator*=(Float o) {
    for(int i = 0; i < size(); i++) {
        operator()(i) *= o;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Poly& p) {
    os << '[';
    for(Float c : p)
        os << c << ',';
    os << "\b]";
    return os;
}

Poly Poly::operator-(Poly& o) {
    const int z = std::max(size(), o.size());
    auto p = Poly(z);
    for(int i = 0; i < z; i++) {
        p[i] = operator()(i) + o(i);
    }
    return p;
}

Poly Poly::operator+(const Poly& o) {
    const int z = std::max(size(), o.size());
    auto p = Poly(z);
    for(int i = 0; i < z; i++) {
        p[i] = operator()(i) + o(i);
    }
    return p;
}
Poly Poly::operator*(Float o) {
    const int z = size();
    auto p = Poly(z);
    for(int i = 0; i < z; i++) {
        p[i] = operator()(i) * o;
    }
    return p;
}


Poly Poly::operator*(Poly o) {
    const int z = size() + o.size();
    auto p = Poly(z);
    p.setZero();
    for(int x = 0; x < size(); x++) {
        for(int y = 0; y < o.size(); y++) {
            Float v = operator()(x) * o(y);
            p[x+y] += v;
        }
    }
    return p;
}

Poly& Poly::operator-() {
    *this *= -1;
    return *this;
}

Poly Poly::integral() {
    int z = degree();
    auto o = Poly(z+2);
    o.setZero();
    for (int x = 0; x <= z; x++) {
        o[x+1] = operator()(x)/(x+1);
    }
    return o;
}

Poly::Poly()
{

}
