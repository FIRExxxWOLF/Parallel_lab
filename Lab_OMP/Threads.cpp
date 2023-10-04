#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <numeric>
#include <omp.h>

// Функция для вычисления суммы элементов вектора
int calculateSum(const std::vector<int>& vec) {
    return std::accumulate(vec.begin(), vec.end(), 0);
}

int main() {
    int tid, time;
    const int numVectors = 30000;
    const int vectorSize = 1000;
    const int numThreads[] = { 1, 2, 4 };
    std::vector<std::vector<int>> vectors;
    std::vector<int> res(numVectors, 0);

    // Генератор случайных чисел
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(-1000, 1000);

    // Заполнение 30000 векторов
    for (int i = 0; i < numVectors; i++) {
        std::vector<int> vec;
        for (int j = 0; j < vectorSize; j++) {
            int num = dist(mt);
            vec.push_back(num);
        }
        vectors.push_back(vec);
    }


    for (int k = 0; k < 3; k++) {
        omp_set_num_threads(numThreads[k]);
#pragma omp parallel private(time,tid)
        {
            tid = omp_get_thread_num();
            auto start = std::chrono::high_resolution_clock::now();
#pragma omp for
            for (int i = 0; i < numVectors; i++) {
                res[i] = calculateSum(vectors[i]);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            std::cout << "Time with threads: " << duration.count() << " seconds" << std::endl;
            time = omp_get_wtime();
            std::cout << "Threads: " << numThreads[k] << " Time: " << omp_get_wtime() - time << std::endl;
        }
    }
    return 0;
}
