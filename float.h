#ifndef FLOAT_H
#define FLOAT_H
//#include <quadmath.h>
#include <autodiff/forward/real.hpp>
#include <autodiff/forward/real/eigen.hpp>
#include <Eigen/Core>
#include <Eigen/Dense>

using Float = autodiff::detail::Real<1, long double>;
using VectorXFloat = Eigen::Matrix<Float, -1, 1, 0, -1, 1>;
using MatrixXFloat = Eigen::Matrix<Float, -1, -1, 0, -1, -1>;

#define LOG_STEPS false
#define LOG_ERROR false

#endif // FLOAT_H
