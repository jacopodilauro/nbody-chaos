#ifndef ARGS_H
#define ARGS_H

#include <iostream>
#include <string>

struct Config {
    unsigned int size = 3;
    unsigned int seed = 4;
    float g1tilt = 0;
    float g2tilt = 0;
    bool is3D = false;
    bool showFPS = false;
    bool useBlend = false;
    std::string modeCstl = "default";
};

inline int ParseArgs(int argc, char* argv[], Config &cfg) {
    bool sizetmp = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-3D") {
            cfg.is3D = true;
        } else if (arg == "-glxy") {
            cfg.modeCstl = "glxy";
        } else if (arg == "-fps") {
            cfg.showFPS = true;
        } else if (arg == "-blend") {
            cfg.useBlend = true;
        } else if (arg == "--seed") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --seed\n";
                return 1;
            }
            try {
                cfg.seed = std::stoi(argv[++i]);
            } catch (...) {
                std::cerr << "Invalid seed\n";
                return 1;
            }
        } else if (arg == "--tilt1") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --g1tilt\n";
                return 1;
            }
            try {
                cfg.g1tilt = std::stof(argv[++i]);
            } catch (...) {
                std::cerr << "Invalid inclination\n";
                return 1;
            }
        } else if (arg == "--tilt2") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --g2tilt\n";
                return 1;
            }
            try {
                cfg.g2tilt = std::stof(argv[++i]);
            } catch (...) {
                std::cerr << "Invalid inclination\n";
                return 1;
            }
        } else if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: nbody [NUMBER_OF_BODIES] [OPTIONS]\n\n";
            std::cout << "Arguments:\n";
            std::cout << "  NUMBER_OF_BODIES     Number of celestial bodies (optional, default: 3)\n\n";
            std::cout << "Options:\n";
            std::cout << "  -3D                  Enable 3D visualization\n";
            std::cout << "  -glxy               Enable galaxy collision mode\n";
            std::cout << "  -fps                 Show FPS on screen\n";
            std::cout << "  -blend               Enable additive blending mode\n";
            std::cout << "  --g1tilt <DEG>       Tilt angle for the first galaxy (in degrees)\n";
            std::cout << "  --g2tilt <DEG>       Tilt angle for the second galaxy (in degrees)\n";
            std::cout << "  --seed <N>           Set the seed for random generation\n";
            std::cout << "  -h, --help           Show this help message\n";
            return 0;
        } else if (!arg.empty() && arg[0] == '-') {
            std::cerr << "Unknown flag: " << arg << '\n';
            return 1;
        } else if (!sizetmp) {
            try {
                int n = std::stoi(arg);
                if (n < 1) {
                    std::cerr << "Number of bodies must be at least 1\n";
                    return 1;
                }
                cfg.size = n;
                sizetmp = true;
            } catch (...) {
                std::cerr << "Invalid number\n";
                return 1;
            }
        } else {
            std::cerr << "Unexpected argument: " << arg << '\n';
            return 1;
        }
    }
    return -1;
}

#endif