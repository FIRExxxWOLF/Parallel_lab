#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <numeric>

// Функция для вычисления суммы элементов вектора
int calculateSum(const std::vector<int>& vec) {
    return std::accumulate(vec.begin(), vec.end(), 0);
}

int main() {
    const int numVectors = 30000;
    const int vectorSize = 100;
    std::vector<std::vector<int>> vectors;

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


    // Один поток
    auto start = std::chrono::high_resolution_clock::now();

    std::thread V1([&]()
        {
            for (int i = 0; i < numVectors; i++) {
                int sum = calculateSum(vectors[i]);
            };
        });
    V1.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time with 1 threads: " << duration.count() << " seconds" << std::endl;


    // Два потока
    start = std::chrono::high_resolution_clock::now();

    std::thread V2_1([&]()
        {
            for (int i = 0; i < numVectors; i+=2) {
                int sum = calculateSum(vectors[i]);
            };
        });
    
    std::thread V2_2([&]()
        {
            for (int i = 1; i < numVectors; i+=2) {
                int sum = calculateSum(vectors[i]);
            };
        });

    V2_1.join();
    V2_2.join();


    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Time with 2 threads: " << duration.count() << " seconds" << std::endl;


    // Четыре потока
    start = std::chrono::high_resolution_clock::now();

    std::thread V4_1([&]()
        {
            for (int i = 0; i < numVectors; i += 4) {
                int sum = calculateSum(vectors[i]);
            };
        });
    

    std::thread V4_2([&]()
        {
            for (int i = 1; i < numVectors; i += 4) {
                int sum = calculateSum(vectors[i]);
            };
        });
    

    std::thread V4_3([&]()
        {
            for (int i = 2; i < numVectors; i += 4) {
                int sum = calculateSum(vectors[i]);
            };
        });

    std::thread V4_4([&]()
        {
            for (int i = 3; i < numVectors; i += 4) {
                int sum = calculateSum(vectors[i]);
            };
        });

    V4_1.join();
    V4_2.join();
    V4_3.join();
    V4_4.join();


    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Time with 4 threads: " << duration.count() << " seconds" << std::endl;
    return 0;
}
