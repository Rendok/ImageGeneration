#include <random>
#include <vector>
#include <iostream>
#include "RandomGenerator.h"
#include "omp.h"

using namespace std;

RandomGenerator::RandomGenerator(int const width, int const height, int const max_depth) :
        max_depth(max_depth), width(width), height(height) {
}

unique_ptr<Node> RandomGenerator::generateNode(int const depth) {

    if (depth == max_depth) {
        auto node = make_unique<Node>();
        node->func = [=](double x, double y) { return getPlaceHolder(); };
        node->arity = 0;
        node->left = nullptr;
        node->right = nullptr;

        return node;
    }

    auto node = make_unique<Node>();
    auto[arity, notation, func] = getFunction();
//    cout << notation << " -> ";
    node->func = func;
    node->arity = arity;
    node->left = generateNode(depth + 1);
    node->right = generateNode(depth + 1);

    return node;
}

void RandomGenerator::generateTree() {
    root = generateNode(0);
}

tuple<int, string, function<double(double, double)>> RandomGenerator::getFunction() {
    std::random_device rd;
    std::mt19937 gen(rd());
    normal_distribution<double> dist(0, 1);
    auto const rnd = dist(gen);

    vector<tuple<int, string, function<double(double, double)>>> functions = {
            make_tuple(1, "cos()", [=](double x, double y) { return cos(x); }),
            make_tuple(1, "sin()", [=](double x, double y) { return sin(x); }),
            make_tuple(-1, "rnd", [=](double x, double y) { return rnd; }),
            make_tuple(1, "x", [=](double x, double y) { return x; }),
            make_tuple(1, "x^2", [=](double x, double y) { return x * x; }),
            make_tuple(1, "x^3", [=](double x, double y) { return x * x * x; }),
            make_tuple(2, "x + y", [=](double x, double y) { return x + y; }),
            make_tuple(2, "x - y", [=](double x, double y) { return x - y; }),
            make_tuple(2, "x * y", [=](double x, double y) { return x * y; })};

    uniform_int_distribution<int> distribution(0, functions.size() - 1);
    auto i = distribution(gen);
    return functions.at(i);
}

int RandomGenerator::getPlaceHolder() {
    vector<int> placeholders = {0, 1};

    std::random_device rd;
    std::mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, placeholders.size() - 1);

    return placeholders.at(distribution(gen));
}

double RandomGenerator::evaluateNode(unique_ptr<Node> &node, double const x, double const y) {

    // return a random value
    if (node->arity == -1) {
        return node->func(0, 0);
    }

    // return a placeholder
    if (node->arity == 0) {
        return node->func(0, 0) == 0 ? x : y;
    }

    // return a unary function
    if (node->arity == 1) {
        auto const left = evaluateNode(node->left, x, y);
        return node->func(left, 0);
    }

    // return a binary function
    auto const left = evaluateNode(node->left, x, y);
    auto const right = evaluateNode(node->right, x, y);
    return node->func(left, right);
}

vector<uint16_t> RandomGenerator::evaluate() {

    auto const coeffs = getCoefficients();
    vector<uint16_t> ans(height * width * 3);

    #pragma omp parallel for
    for (int y = 0; y < height; ++y) {
        #pragma omp parallel for
        for (int x = 0; x < width; ++x) {
            auto z = evaluateNode(root,
                                  static_cast<double>(x) / static_cast<double>(width - 1),
                                  static_cast<double>(y) / static_cast<double>(height - 1));

            auto r = round((coeffs[0] * z * z + coeffs[1] * z + coeffs[2]) * 0xffff);
            ans.at(3 * x + 3 * width * y) = static_cast<uint16_t>(r);

            auto g = round((coeffs[3] * z * z + coeffs[4] * z + coeffs[5]) * 0xffff);
            ans.at(3 * x + 1 + 3 * width * y) = static_cast<uint16_t>(g);

            auto b = round((coeffs[6] * z * z + coeffs[7] * z + coeffs[8]) * 0xffff);
            ans.at(3 * x + 2 + 3 * width * y) = static_cast<uint16_t>(b);
        }
    }
    return ans;
}

std::vector<double> RandomGenerator::getCoefficients() {
    vector<double> coefficients(9);

    std::random_device rd;
    std::mt19937 gen(rd());
    normal_distribution<double> distribution(0, 1);

    for (auto &v : coefficients) {
        v = distribution(gen);
    }

    return coefficients;
}
