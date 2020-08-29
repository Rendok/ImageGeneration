#ifndef IMAGEGENERATION_RANDOMGENERATOR_H
#define IMAGEGENERATION_RANDOMGENERATOR_H

#include <cmath>
#include <vector>

struct Node {
public:
    std::unique_ptr<Node> left = nullptr;
    std::unique_ptr<Node> right = nullptr;
    std::function<double(double, double)> func;
    int arity;
};

class RandomGenerator {
private:
    int width;
    int height;
    int max_depth;
    std::unique_ptr<Node> root;

    static std::tuple<int, std::string, std::function<double(double, double)>> getFunction();

    static int getPlaceHolder();

    double evaluateNode(std::unique_ptr<Node> &node, double x, double y);

    static std::vector<double> getCoefficients();

    std::unique_ptr<Node> generateNode(int depth);

public:
    RandomGenerator(int width, int height, int max_depth);

    void generateTree();

    std::vector<uint16_t> evaluate();
};


#endif //IMAGEGENERATION_RANDOMGENERATOR_H
