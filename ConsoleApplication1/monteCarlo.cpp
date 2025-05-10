#include <omp.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>

int main() {
    const size_t size = 1'000'000;
    std::vector<std::pair<double, double>> points(size);

    std::mt19937 gen(42);
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    for (auto& p : points) {
        p = { dis(gen), dis(gen) };
    }

    std::ofstream file("dane.csv", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku!" << std::endl;
        return 1;
    }

    for (int threads = 1; threads <= 16; threads++) {
        omp_set_num_threads(threads);

        int count = 0;
        double start = omp_get_wtime();

#pragma omp parallel for reduction(+:count)
        for (int i = 0; i < size; i++) {
            const auto& point = points[i];
            if (point.first * point.first + point.second * point.second <= 1.0) {
                count++;
            }
        }

        double end = omp_get_wtime();
        double elapsed_time = (end - start) * 1000;
        double pi_estimate = 4.0 * count / size;

        std::cout << "Watki: " << threads
            << ", Pi ~ " << pi_estimate
            << ", czas [ms]: " << elapsed_time << std::endl;

        file << threads << "," << elapsed_time << "\n";
    }

    file.close();
    return 0;
}
