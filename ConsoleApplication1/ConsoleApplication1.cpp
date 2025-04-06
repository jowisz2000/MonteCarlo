//#include <iostream>
//#include <fstream>
//#include <random>
//
//int main() {
//    int n = 1000000;
//
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_real_distribution<> dis(-1.0, 1.0);
//
//    std::ofstream file("wynik.txt");
//    if (!file.is_open()) {
//        std::cerr << "Nie mo¿na otworzyæ pliku do zapisu." << std::endl;
//        return 1;
//    }
//    for (int i = 0; i < n; ++i) {
//        double liczba1 = dis(gen);
//        double liczba2 = dis(gen);
//        file << liczba1 << " " << liczba2 << "\n";
//    }
//
//    file.close();
//
//    return 0;
//}
