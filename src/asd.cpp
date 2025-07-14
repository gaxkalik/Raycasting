#include "raycast.hpp"
#include <filesystem>

namespace fs = std::filesystem;
void oASD() {
	 std::string path = "/Users/hrant/Desktop/Raycasting/textures/ezgif-split";

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::vector<std::string> tmp = strSplit(entry.path().string(), '/');
            std::string addddd = "magick " + entry.path().string()+ " -resize 64x64! -depth 8 RGBA:" +strSplit(tmp[7], '.')[0] + ".raw";
            system(addddd.c_str());
            std::cout << tmp[7] << std::endl;
        }
    }
}