#ifndef PPOLY_H
#define PPOLY_H
#include <memory>
#include <vector>
#include "poly.h"
#include "function.h"
#include "float.h"

typedef struct {
    Float a;
    Float b;
} Span;

typedef struct {
    Span span;
    Poly poly;
} Piece;

class PPoly : public Function
{
private:
    std::vector<std::shared_ptr<Piece>> pieces;
public:
    PPoly();
    Span domain = {0, 1};
    void cull();
    Float integral(Float a, Float b);
    Float evaluate(Float x) override {
        return operator()(x);
    }

    Float operator()(Float x);
    Poly& operator[](int i);
    PPoly& operator<<(Piece p);
    friend std::ostream& operator<<(std::ostream& os, const PPoly& p);
    const static Poly zero;
};

#endif // PPOLY_H
