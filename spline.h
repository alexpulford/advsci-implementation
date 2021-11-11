#ifndef SPLINE_H
#define SPLINE_H
#include <vector>
#include <autodiff/forward/real.hpp>
#include <autodiff/forward/real/eigen.hpp>

#include "ppoly.h"
#include "float.h"

typedef VectorXFloat Knot;
class Spline : Knot
{
private:
    uint _order = 0;
    Poly lerp(int i, int p, bool inverse = false);
public:
    using Knot::Knot;
    using Knot::operator<<;
    using Knot::operator();
    using Knot::size;

    std::vector<PPoly> basis;
    VectorXFloat integrals;
    Spline();

    void linear();
    void interpolate();
    void cull();
    void compute(int order = 3,  Float a = 0.0, Float b = 1.0);
    void computeBasis(int order);
    void computeIntegrals(Float a, Float b);
    Float evaluate(Float x);
    uint order() { return _order; }
    Float knot(int i) { return operator()(i); }

    friend std::ostream& operator<<(std::ostream& os, const Spline& s);

    static Knot constructKnot(int N, int order);
};

#endif // SPLINE_H
