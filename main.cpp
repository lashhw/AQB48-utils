#include <filesystem>
#include <iostream>
#include <vector>
#include <array>
#include "happly/happly.h"
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " FOLDER OUTPUT_PLY" << std::endl;
        exit(EXIT_FAILURE);
    }

    char* folder = argv[1];
    char* output_ply = argv[2];

    std::cout << "FOLDER = " << folder << std::endl;
    std::cout << "OUTPUT_PLY = " << output_ply << std::endl;

    std::vector<std::array<double, 3>> v_pos;
    std::vector<std::vector<size_t>> f_idx;

    for (const auto &entry : fs::directory_iterator(folder)) {
        happly::PLYData ply_data(entry.path());

        size_t offset = v_pos.size();
        std::vector<std::array<double, 3>> v_pos_tmp = ply_data.getVertexPositions();
        v_pos.insert(v_pos.end(), v_pos_tmp.begin(), v_pos_tmp.end());

        std::vector<std::vector<size_t>> f_idx_tmp = ply_data.getFaceIndices<size_t>();
        for (auto &x : f_idx_tmp)
            for (auto &y : x)
                y += offset;
        f_idx.insert(f_idx.end(), f_idx_tmp.begin(), f_idx_tmp.end());
    }

    happly::PLYData ply_out;
    ply_out.addVertexPositions(v_pos);
    ply_out.addFaceIndices(f_idx);
    ply_out.write(output_ply);
}
