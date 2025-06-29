#include "raycast.hpp"
// #include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <map_file>" << std::endl;
		return 1;
	}
	raycast *game = new raycast();

	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
	if (game->initGame(argv[1])) {
		delete game;
		std::cerr << "Failed to initialize game" << std::endl;
		return 1;
	}
	if (game->startGame()) {
		delete game;
		std::cerr << "Failed to start game" << std::endl;
		return 1;
	}
	delete game;
	// system("leaks rc");
	return 0;
}
// #include <iostream>
// #include <fstream>
// #include <cstdint>
// #include <iomanip>

// const int WIDTH = 64;
// const int HEIGHT = 64;

// int main() {
//     const char* filename = "textures/a2zconverter_X2025-06-28_X8c832f76a2f3cfa3e8f63e33f7e7f9c3-wool_colored_magenta.raw";

//     std::ifstream file(filename, std::ios::binary);
//     if (!file) {
//         std::cerr << "Cannot open file: " << filename << std::endl;
//         return 1;
//     }

//     uint32_t pixels[WIDTH * HEIGHT];

//     for (int i = 0; i < WIDTH * HEIGHT; ++i) {
//         uint8_t argb[4];
//         if (!file.read(reinterpret_cast<char*>(argb), 4)) {
//             std::cerr << "Unexpected end of file or read error." << std::endl;
//             return 2;
//         }

//         // Store as 0xAARRGGBB
//         pixels[i] = (argb[0] << 24) | (argb[1] << 16) | (argb[2] << 8) | argb[3];
//     }

//     // Print pixel data
//     for (int y = 0; y < HEIGHT; ++y) {
//         for (int x = 0; x < WIDTH; ++x) {
//             uint32_t color = pixels[y * WIDTH + x];
//             std::cout << std::hex << std::setw(8) << std::setfill('0') << color << " ";
//         }
//         std::cout << std::endl;
//     }

//     return 0;
// }

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"

// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <cstdint>

// const int WIDTH = 64;
// const int HEIGHT = 64;

// int main() {
//     uint8_t image[HEIGHT][WIDTH][4];  // RGBA output
//     std::ifstream file("asd.txt");
//     if (!file) {
//         std::cerr << "Can't open texture.txt\n";
//         return 1;
//     }

//     std::string line;
//     int y = 0;
//     while (std::getline(file, line) && y < HEIGHT) {
//         std::istringstream ss(line);
//         for (int x = 0; x < WIDTH; ++x) {
//             std::string hex;
//             ss >> hex;
//             if (hex.empty()) {
//                 std::cerr << "Not enough data at line " << y << "\n";
//                 return 2;
//             }

//             uint32_t argb = std::stoul(hex, nullptr, 16);
//             image[y][x][0] = (argb >> 16) & 0xFF; // R
//             image[y][x][1] = (argb >> 8) & 0xFF;  // G
//             image[y][x][2] = argb & 0xFF;         // B
//             image[y][x][3] = (argb >> 24) & 0xFF; // A
//         }
//         y++;
//     }

//     if (y < HEIGHT) {
//         std::cerr << "File too short.\n";
//         return 3;
//     }

//     if (!stbi_write_png("output.png", WIDTH, HEIGHT, 4, image, WIDTH * 4)) {
//         std::cerr << "Failed to write output.png\n";
//         return 4;
//     }

//     std::cout << "Saved: output.png\n";
//     return 0;
// }

// #include <iostream>
// #include <fstream

// const int WIDTH = 64;
// const int HEIGHT = 64;
// const int CHANNELS = 3;
// const int TEXTURE_SIZE = WIDTH * HEIGHT * CHANNELS;

// bool loadTexture(const char* filename, unsigned char texture[WIDTH][HEIGHT][CHANNELS]) {
//     std::ifstream file(filename, std::ios::binary);
//     if (!file) return false;
//     file.read(reinterpret_cast<char*>(texture), TEXTURE_SIZE);
//     return file.gcount() == TEXTURE_SIZE;
// }
// void printAllColorsWithPreview(unsigned char texture[WIDTH][HEIGHT][CHANNELS]) {
//     std::cout << "64x64 Texture Dump with ASCII Preview:\n\n";
    
//     // ASCII Preview (scaled down to fit terminal)
//     std::cout << "ASCII Preview (32x32):\n";
//     for (int y = 0; y < HEIGHT; ++y) {
//         for (int x = 0; x < WIDTH; ++x) {
//             auto& p = texture[y][x];
//             std::cout << "\033[48;2;" << (int)p[0] << ";" << (int)p[1] << ";" << (int)p[2] << "m ";
//             std::cout << "\033[0m";
//         }
//         std::cout << "\n";
//     }
    
//     // Detailed color data
//     std::cout << "\nAll Pixel Data [X,Y] R G B (Hex)\n";
//     // std::cout << std::setfill('0');
//     // for (int y = 0; y < HEIGHT; y++) {
//     //     for (int x = 0; x < WIDTH; x++) {
//     //         auto& p = texture[x][y];
//     //         std::cout << std::hex << std::setw(2) << (int)p[0]
//     //                   << std::setw(2) << (int)p[1]
//     //                   << std::setw(2) << (int)p[2] << " ";
//     //     }
// 	// 	std::cout << "\n";
//     // }
// }

// int main() {
//     unsigned char texture[WIDTH][HEIGHT][CHANNELS];
    
//     if (!loadTexture("textures/output.raw", texture)) {
//         std::cerr << "Error loading texture.raw (expected 64x64 RGB, 12288 bytes)\n";
//         return 1;
//     }

//     printAllColorsWithPreview(texture);
//     return 0;
// }