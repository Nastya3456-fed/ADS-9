// Copyright 2022 NNTU-CS
#include  <iostream>
#include  <fstream>
#include  <locale>
#include  <cstdlib>
#include  "tree.h"

#include "tree.h"
#include <algorithm>
#include <stack>
#include <stdexcept>

PMTree::PMTree(const std::vector<char>& elements) {
    if (elements.empty()) {
        root = nullptr;
        total_permutations = 0;
        return;
    }

    root = std::make_unique<Node>('\0'); // Корневой узел не содержит значения
    total_permutations = 1;
    for (size_t i = elements.size(); i > 0; --i) {
        total_permutations *= i;
    }

    buildTree(root.get(), elements);
}

void PMTree::buildTree(Node* parent, const std::vector<char>& remaining_elements) {
    if (remaining_elements.empty()) {
        return;
    }

    for (size_t i = 0; i < remaining_elements.size(); ++i) {
        parent->children.emplace_back(std::make_unique<Node>(remaining_elements[i]));

        std::vector<char> new_remaining = remaining_elements;
        new_remaining.erase(new_remaining.begin() + i);
        buildTree(parent->children.back().get(), new_remaining);
    }
}

void traverseTree(const PMTree::Node* node, std::vector<char>& current, std::vector<std::vector<char>>& result) {
    if (node->value != '\0') {
        current.push_back(node->value);
    }

    if (node->children.empty()) {
        if (!current.empty()) {
            result.push_back(current);
        }
    }
    else {
        for (const auto& child : node->children) {
            traverseTree(child.get(), current, result);
        }
    }

    if (node->value != '\0') {
        current.pop_back();
    }
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    std::vector<std::vector<char>> result;
    if (!tree.getRoot()) {
          return result;
    }

    std::vector<char> current;
    traverseTree(tree.getRoot(), current, result);
    return result;
}

std::vector<char> getPerm1(const PMTree& tree, int num) {
    if (num < 1 || static_cast<size_t>(num) > tree.getTotalPermutations()) {
        return {};
    }

    auto all_perms = getAllPerms(tree);
    return all_perms[num - 1];
}

std::vector<char> getPerm2(const PMTree& tree, int num) {
    if (num < 1 || static_cast<size_t>(num) > tree.getTotalPermutations() || !tree.getRoot()) {
        return {};
    }

    std::vector<char> result;
    const PMTree::Node* current = tree.getRoot();
    num--; // Переходим к 0-индексации

    // Для вычисления факториалов на лету
    std::vector<int> factorials(current->children.size() + 1);
    factorials[0] = 1;
    for (size_t i = 1; i < factorials.size(); ++i) {
        factorials[i] = factorials[i - 1] * i;
    }

    std::stack<const PMTree::Node*> stack;
    stack.push(current);

    while (!stack.empty()) {
        current = stack.top();
        stack.pop();

        if (current->value != '\0') {
            result.push_back(current->value);
        }

        if (current->children.empty()) {
            if (num == 0) {
                return result;
            }
            num--;
            result.pop_back();
        }
        else {
            size_t n = current->children.size();
            int fact = factorials[n - 1];
            int idx = num / fact;
            num %= fact;

            // Добавляем детей в обратном порядке, кроме выбранного
            for (int i = n - 1; i >= 0; --i) {
                if (static_cast<int>(i) != idx) {
                    stack.push(current->children[i].get());
                }
            }
            stack.push(current->children[idx].get());
        }
    }

    return {};
}
