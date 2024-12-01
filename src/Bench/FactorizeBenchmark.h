#ifndef FACTORIZE_BENCHMARK_H
#define FACTORIZE_BENCHMARK_H

namespace FactorizeBenchmark {
    extern "C" void factorize(long long number, bool first);
    void run(int iterations, long long number);
}

#endif
