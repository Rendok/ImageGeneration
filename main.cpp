#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include "RandomGenerator.h"
#include "opencv2/core.hpp"
#include <chrono>

using namespace std;

int main() {
    int width = 255;
    int height = 255;
    auto g = RandomGenerator(width, height, 6);

    auto start = chrono::steady_clock::now();

    for (int i = 0; i < 5; ++i) {
        g.generateTree();
        auto ans = g.evaluate();

//        int ind = 0;
//        for (auto v : ans)
//        {
//            if (ind % (3 * width) == 0)
//                std::cout << std::endl;
//            std::cout << v << " ";
//            ind++;
//        }

        cv::Mat M(width,height, CV_16UC3, ans.data());
//        std::cout << "M = " << std::endl << " " << M << std::endl << std::endl;
        cv::imwrite("alpha" + std::to_string(i) + ".png", M);
    }

    auto stop = chrono::steady_clock::now();
    std::cout << "Time passed: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << std::endl;


    return 0;
}
