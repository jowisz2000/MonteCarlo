#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>

int main() {

    std::ifstream file("wynik.txt");
    if (!file.is_open()) {
        std::cerr << "Nie mo¿na otworzyæ pliku do odczytu." << std::endl;
        return 1;
    }

    std::vector<std::pair<double, double>> points;
    double x, y;
    while (file >> x >> y) {
        points.emplace_back(x, y);
    }

    int count = 0;
    int size = points.size();
    file.close();

    double start = omp_get_wtime();

#pragma omp parallel for reduction(count++)
    for (int i = 0; i < size; i++) {
        x = points.at(i).first;
        y = points.at(i).second;
        if (x*x+y*y <= 1.0) count++;
    }

    double end = omp_get_wtime();
    std::cout <<  "Otrzymane przyblizenie pi: " << 4.0*count/size <<", czas wykonywania programu [ms]: " << (end-start)*1000;


    return 0;
}
