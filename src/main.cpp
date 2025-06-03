// Copyright 2022 NNTU-CS
#include "tree.h"

#include "tree.h"
#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;
using namespace std::chrono;

void runExperiment() {
    const int MAX_N = 10;  // Максимальный размер входных данных
    const int SAMPLES = 100; // Количество замеров для каждого n

    ofstream data_file("results.csv");
    data_file << "n,getAllPerms,getPerm1,getPerm2\n";

    random_device rd;
    mt19937 gen(rd());

    cout << "Running performance experiment..." << endl;
    cout << setw(5) << "n" << setw(15) << "getAllPerms"
        << setw(15) << "getPerm1" << setw(15) << "getPerm2" << endl;

    for (int n = 1; n <= MAX_N; ++n) {
        // Создаем вектор элементов
        vector<char> elements(n);
        for (int i = 0; i < n; ++i) {
            elements[i] = 'a' + i;
        }

        PMTree tree(elements);

        // Генерируем случайные номера перестановок для тестирования
        vector<int> perm_numbers(SAMPLES);
        uniform_int_distribution<> distr(1, tree.getTotalPermutations());
        for (int i = 0; i < SAMPLES; ++i) {
            perm_numbers[i] = distr(gen);
        }

        // Измеряем время для getAllPerms()
        auto start = high_resolution_clock::now();
        auto all_perms = getAllPerms(tree);
        auto end = high_resolution_clock::now();
        auto all_perms_time = duration_cast<microseconds>(end - start).count();

        // Измеряем время для getPerm1()
        start = high_resolution_clock::now();
        for (int num : perm_numbers) {
            auto perm = getPerm1(tree, num);
        }
        end = high_resolution_clock::now();
        auto get_perm1_time = duration_cast<microseconds>(end - start).count() / SAMPLES;

        // Измеряем время для getPerm2()
        start = high_resolution_clock::now();
        for (int num : perm_numbers) {
            auto perm = getPerm2(tree, num);
        }
        end = high_resolution_clock::now();
        auto get_perm2_time = duration_cast<microseconds>(end - start).count() / SAMPLES;

        // Записываем результаты
        data_file << n << "," << all_perms_time << "," << get_perm1_time << "," << get_perm2_time << "\n";

        // Выводим в консоль
        cout << setw(5) << n
            << setw(15) << all_perms_time
            << setw(15) << get_perm1_time
            << setw(15) << get_perm2_time << endl;
    }

    data_file.close();
    cout << "Experiment completed. Results saved to results.csv" << endl;

}

void printPermutations(const vector<char>& elements) {
    PMTree tree(elements);
    auto perms = getAllPerms(tree);

    cout << "\nAll permutations for [";
    for (size_t i = 0; i < elements.size(); ++i) {
        if (i != 0) cout << ", ";
        cout << elements[i];
    }
    cout << "]:" << endl;

    for (size_t i = 0; i < perms.size(); ++i) {
        cout << i + 1 << ": ";
        for (char c : perms[i]) {
            cout << c;
        }
        cout << endl;
    }
}

int main() {
    {
        cout << "Example 1: permutations of [1, 2, 3]" << endl;
        PMTree tree({ '1', '2', '3' });

        cout << "All permutations:" << endl;
        auto all = getAllPerms(tree);
        for (const auto& perm : all) {
            printPermutations(perm);
        }

        cout << "\nSpecific permutations:" << endl;
        cout << "Permutation 1: ";
        printPermutations(getPerm1(tree, 1));

        cout << "Permutation 2: ";
        printPermutations(getPerm2(tree, 2));

        cout << "Permutation 6: ";
        printPermutations(getPerm1(tree, 6));

        cout << "Permutation 8 (should be empty): ";
        printPermutations(getPerm2(tree, 8));
    }

    // Демонстрация работы
   // printPermutations({ '1', '2', '3' });
   // printPermutations({ 'a', 'b', 'c', 'd' });

    // Проведение вычислительного эксперимента
    runExperiment();

    return 0;
}
