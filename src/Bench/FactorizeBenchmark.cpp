#include "FactorizeBenchmark.h"
#include <iostream>
#include <thread>

namespace FactorizeBenchmark {

    extern "C" void factorize(long long number, bool first) {
        for (long long i = 2; i * i <= number; ++i) {
            while (number % i == 0) {
                if (first) std::cout << i << " ";
                number /= i;
            }
        }
        if (first) {
            if (number > 1) std::cout << number;
            std::cout << std::endl;
        }
    }

    void run(int iterations, long long number) {
        bool first = true;
        for (int i = 0; i < iterations; ++i) {
            if (first) std::cout << "Factorizing " << number << ": ";
            factorize(number, first);
            first = false;
        }
    }

}
