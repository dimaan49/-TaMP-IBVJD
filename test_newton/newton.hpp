#ifndef NEWTON_HPP
#define NEWTON_HPP

#include <stdexcept>
#include <cmath>

class Task1 {
public:
    static double newton(double A) {
        if (A < 0) {
            throw std::invalid_argument("Input must be non-negative");
        }

        if (A == 0) return 0;

        double K = 1;
        double Kold;

        do {
            Kold = K;
            K = (A / K + K) / 2.0;
        } while (std::abs(Kold - K) > 1e-10);

        return K;
    }
};

#endif // NEWTON_HPP
