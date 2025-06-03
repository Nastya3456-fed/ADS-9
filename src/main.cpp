// Copyright 2022 NNTU-CS
#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include "tree.h"

// Using declarations вместо directives
using std::cout;
using std::endl;
using std::vector;
using std::ofstream;
using std::setw;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
using std::chrono::duration_cast;

void runExperiment() {
  const int MAX_N = 10;
  const int SAMPLES = 100;

  ofstream data_file("results.csv");
  data_file << "n,getAllPerms,getPerm1,getPerm2\n";

  random_device rd;
  mt19937 gen(rd());

  cout << "Running performance experiment..." << endl;
  cout << setw(5) << "n" << setw(15) << "getAllPerms" 
       << setw(15) << "getPerm1" << setw(15) << "getPerm2" << endl;

  for (int n = 1; n <= MAX_N; ++n) {
    vector<char> elements(n);
    for (int i = 0; i < n; ++i) {
      elements[i] = 'a' + i;
    }
    PMTree tree(elements);
    vector<int> perm_numbers(SAMPLES);
    uniform_int_distribution<> distr(1, tree.getTotalPermutations());
    for (int i = 0; i < SAMPLES; ++i) {
      perm_numbers[i] = distr(gen);
    }

    // Measure getAllPerms()
    auto start = high_resolution_clock::now();
    auto all_perms = getAllPerms(tree);
    auto end = high_resolution_clock::now();
    auto all_perms_time = duration_cast<microseconds>(end - start).count();

    // Measure getPerm1()
    start = high_resolution_clock::now();
    for (int num : perm_numbers) {
      auto perm = getPerm1(tree, num);
    }
    end = high_resolution_clock::now();
    auto get_perm1_time = duration_cast<microseconds>(end - start).count() / 
                         SAMPLES;

    // Measure getPerm2()
    start = high_resolution_clock::now();
    for (int num : perm_numbers) {
      auto perm = getPerm2(tree, num);
    }
    end = high_resolution_clock::now();
    auto get_perm2_time = duration_cast<microseconds>(end - start).count() / 
                         SAMPLES;

    // Save results
    data_file << n << "," << all_perms_time << "," << get_perm1_time 
              << "," << get_perm2_time << "\n";

    cout << setw(5) << n << setw(15) << all_perms_time 
         << setw(15) << get_perm1_time << setw(15) << get_perm2_time << endl;
  }

  data_file.close();
  cout << "Experiment completed. Results saved to results.csv" << endl;
}

void printPermutation(const vector<char>& perm) {
  cout << "[";
  for (size_t i = 0; i < perm.size(); ++i) {
    if (i != 0) cout << ", ";
    cout << perm[i];
  }
  cout << "]" << endl;
}

int main() {
  {
    cout << "Example 1: permutations of [1, 2, 3]" << endl;
    PMTree tree({'1', '2', '3'});

    cout << "All permutations:" << endl;
    auto all = getAllPerms(tree);
    for (const auto& perm : all) {
      printPermutation(perm);
    }

    cout << "\nSpecific permutations:" << endl;
    cout << "Permutation 1: ";
    printPermutation(getPerm1(tree, 1));

    cout << "Permutation 2: ";
    printPermutation(getPerm2(tree, 2));

    cout << "Permutation 6: ";
    printPermutation(getPerm1(tree, 6));

    cout << "Permutation 8 (should be empty): ";
    printPermutation(getPerm2(tree, 8));
  }

  runExperiment();
  return 0;
}
