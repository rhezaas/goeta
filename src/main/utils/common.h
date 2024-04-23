#ifndef COMMON_H
#define COMMON_H

#include <cmath>


inline double sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

inline double sigmoidPrime(double x) {
    return sigmoid(x) * (1.0 - sigmoid(x));
}

#endif