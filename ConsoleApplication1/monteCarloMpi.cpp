#include <fstream>
#include <vector>
#include <mpi.h>
#include <iostream>
#include <utility>
#include <cmath>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    double start_time, end_time;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::pair<double, double>> points;
    int total_points = 0;

    if (rank == 0) {
        std::ifstream file("wynik.txt");
        if (!file.is_open()) {
            std::cerr << "Cannot open file for reading." << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        double x, y;
        while (file >> x >> y) {
            points.emplace_back(x, y);
        }
        file.close();
        total_points = points.size();
        std::cout << total_points << " points loaded." << std::endl;
    }

    MPI_Bcast(&total_points, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int points_per_proc = total_points / size;
    int remainder = total_points % size;
    int start_idx = rank * points_per_proc + std::min(rank, remainder);
    int end_idx = start_idx + points_per_proc + (rank < remainder ? 1 : 0);
    int local_size = end_idx - start_idx;

    std::vector<double> local_x(local_size);
    std::vector<double> local_y(local_size);
    std::vector<double> all_x;
    std::vector<double> all_y;
    std::vector<int> counts;
    std::vector<int> displs;

    if (rank == 0) {
        all_x.resize(total_points);
        all_y.resize(total_points);
        for (int i = 0; i < total_points; ++i) {
            all_x[i] = points[i].first;
            all_y[i] = points[i].second;
        }

        counts.resize(size);
        displs.resize(size);
        for (int i = 0; i < size; i++) {
            counts[i] = (i < remainder) ? points_per_proc + 1 : points_per_proc;
            displs[i] = i * points_per_proc + std::min(i, remainder);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);  // synchronizacja przed startem mierzenia
    if (rank == 0) {
        start_time = MPI_Wtime();  // start pomiaru po przygotowaniu danych
    }

    MPI_Scatterv(all_x.data(), counts.data(), displs.data(), MPI_DOUBLE, local_x.data(), local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(all_y.data(), counts.data(), displs.data(), MPI_DOUBLE, local_y.data(), local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int local_count = 0;
    for (int i = 0; i < local_size; ++i) {
        if (local_x[i] * local_x[i] + local_y[i] * local_y[i] <= 1.0) {
            local_count++;
        }
    }

    int global_count = 0;
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        end_time = MPI_Wtime();
        double elapsed_ms = (end_time - start_time) * 1000.0;
        double pi_estimate = 4.0 * global_count / total_points;

        std::cout << "Otrzymane przybliżenie Pi: " << pi_estimate
            << ", czas wykonywania programu [ms]: " << elapsed_ms << "\n";

        std::ofstream output("dane.csv", std::ios::app);
        if (output.is_open()) {
            output << size << ";" << elapsed_ms << "\n";
            output.close();
        }
        else {
            std::cerr << "Nie można otworzyć pliku do zapisu.\n";
        }
    }

    MPI_Finalize();
    return 0;
}
