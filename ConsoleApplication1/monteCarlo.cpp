#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>

int main() {
    std::ifstream file("wynik.txt");
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku do odczytu." << std::endl;
        return 1;
    }

    std::vector<std::pair<double, double> > points;
    double x, y;
    while (file >> x >> y) {
        points.emplace_back(x, y);
    }

    int count = 0;
    size_t size = points.size();
    file.close();

    double start = omp_get_wtime();

#pragma omp parallel for reduction(+:count)
    for (size_t i = 0; i < size; i++) {
        const auto& point = points[i];
        if (point.first * point.first + point.second * point.second <= 1.0) {
            count++;
        }
    }

    double end = omp_get_wtime();

    double elapsed_time = (end - start) * 1000;
    double pi_estimate = 4.0 * count / size;
    std::cout << "Otrzymane przybliżenie Pi: " << pi_estimate << ", czas wykonywania programu [ms]: " << elapsed_time << std::endl;

    return 0;
}
