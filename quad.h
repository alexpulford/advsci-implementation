#ifndef QUAD_H
#define QUAD_H
#include "function.h"
#include "spline.h"
#include "float.h"
#include <sstream>
#include <iomanip>

class Quad : public VectorXFloat {
private:
    static Quad system(const VectorXFloat& q, Spline& s);
    Quad _newtons(Spline s);
public:
    using VectorXFloat::VectorXFloat;
    Float apply(Function* f) const;
    Quad newtons(Spline s, Float error);
    Quad newtons_limit(Spline s, int limit = 5);
    Quad newtons_min(Spline s, int iterations);
    Float error(Spline s);
    Quad continuation(Knot a, Knot b, const int steps, const Float err);
    Quad continuation_edge(Knot a, Knot b, const int steps, const Float err);
    VectorXFloat nodal_pattern(Spline s);
    friend std::ostream& operator<<(std::ostream& os, const Quad& q);

    Quad truncate();
    static Quad constructGuess(int N, int order, Knot k);
};

#endif // QUAD_H
