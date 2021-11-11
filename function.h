#ifndef FUNCTION_H
#define FUNCTION_H

#include <autodiff/forward/real.hpp>
#include <autodiff/forward/real/eigen.hpp>

#include "float.h"

class Function {
public:
    virtual Float evaluate(Float x) {
        return 1;
    }
};

#endif // FUNCTION_H
