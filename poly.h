#ifndef POLY_H
#define POLY_H
#include <iostream>
#include "function.h"
#include "float.h"

class Poly : private VectorXFloat, public Function
{
public:
    using VectorXFloat::VectorXFloat;
    using VectorXFloat::operator<<;
    using VectorXFloat::isZero;

    Poly();

    Poly integral();
    Float evaluate(Float x) override;
    int degree() { return size()-1; };

    Poly& operator+=(Poly o);
    Poly& operator-=(Poly o);
    Poly& operator*=(Poly o);
    Poly& operator*=(Float o);
    Poly& operator-();

    Poly operator+(const Poly& o);
    Poly operator-(Poly& o);
    Poly operator*(Poly o);
    Poly operator*(Float o);

    friend std::ostream& operator<<(std::ostream& os, const Poly& p);
};

#endif // POLY_H
