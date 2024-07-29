#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <cmath>

struct result_t {
    uint32_t intersected;
    float t;
    float u;
    float v;
};

struct ref_result_t {
    uint32_t intersected;
    float t;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "usage: " << argv[0] << " EPS RESULT REF_RESULT" << std::endl;
        exit(EXIT_FAILURE);
    }

    float eps = std::stof(argv[1]);
    char* result = argv[2];
    char* ref_result = argv[3];

    std::cout << "EPS = " << eps << std::endl;
    std::cout << "RESULT = " << result << std::endl;
    std::cout << "REF_RESULT = " << ref_result << std::endl;

    std::ifstream result_fs(result, std::ios::binary);
    std::ifstream ref_result_fs(ref_result, std::ios::binary);

    std::vector<result_t> result_;
    std::vector<ref_result_t> ref_result_;

    for (result_t result; result_fs.read(reinterpret_cast<char*>(&result), sizeof(result_t)); )
        result_.push_back(result);

    for (ref_result_t ref_result; ref_result_fs.read(reinterpret_cast<char*>(&ref_result), sizeof(ref_result_t)); )
        ref_result_.push_back(ref_result);

    assert(result_.size() == ref_result_.size());

    size_t num_correct = 0;
    for (size_t i = 0; i < ref_result_.size(); i++) {
        if (ref_result_[i].intersected) {
            if (result_[i].intersected && std::fabs(result_[i].t / ref_result_[i].t - 1.0f) < eps)
                num_correct++;
        } else {
            if (!result_[i].intersected)
                num_correct++;
        }
    }

    std::cout << double(num_correct) / double(ref_result_.size()) << std::endl;
}
