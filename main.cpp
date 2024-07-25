#include <filesystem>
#include <iostream>
#include <vector>
#include <array>
#include "happly/happly.h"
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "usage: " << argv[0] << " FOLDER OUTPUT_PLY TRANSFORM" << std::endl;
        exit(EXIT_FAILURE);
    }

    char* folder = argv[1];
    char* output_ply = argv[2];
    char* transform = argv[3];

    std::cout << "FOLDER = " << folder << std::endl;
    std::cout << "OUTPUT_PLY = " << output_ply << std::endl;
    std::cout << "TRANSFORM = " << transform << std::endl;

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

    float m[4][4];
    std::ifstream transform_fs(transform, std::ios::binary);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            transform_fs.read(reinterpret_cast<char*>(&m[i][j]), sizeof(float));

    for (auto &v : v_pos) {
        double x = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2] + m[0][3];
        double y = m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2] + m[1][3];
        double z = m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2] + m[2][3];
        double w = m[3][0] * v[0] + m[3][1] * v[1] + m[3][2] * v[2] + m[3][3];
        v[0] = x / w;
        v[1] = y / w;
        v[2] = z / w;
    }

    happly::PLYData ply_out;
    ply_out.addVertexPositions(v_pos);
    ply_out.addFaceIndices(f_idx);
    ply_out.write(output_ply, happly::DataFormat::Binary);
}
