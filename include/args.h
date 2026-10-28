#ifndef ARGS_H
#define ARGS_H

#include <iostream>
#include <string>

struct Config {
    unsigned int size = 3;
    unsigned int seed = 4;
    unsigned int opt_mode = 1;
    float tilt1 = 0;
    float tilt2 = 0;
    bool is3D = false;
    bool showFPS = false;
    bool useBlend = false;
    bool useTrail = false;
    std::string modeCstl = "default";
};

inline int ParseArgs(int argc, char* argv[], Config &cfg) {
    bool sizetmp = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-3D" || arg == "-3d") {
            cfg.is3D = true;
        } else if (arg == "-glxy") {
            cfg.modeCstl = "glxy";
        } else if (arg == "-fps") {
            cfg.showFPS = true;
        } else if (arg == "-blend" || arg == "-b") {
            cfg.useBlend = true;
        } else if (arg == "-trail" || arg == "-t") {
            cfg.useTrail = true;
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
                std::cerr << "Missing value for --tilt1\n";
                return 1;
            }
            try {
                cfg.tilt1 = std::stof(argv[++i]);
            } catch (...) {
                std::cerr << "Invalid inclination\n";
                return 1;
            }
        } else if (arg == "--tilt2") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --tilt2\n";
                return 1;
            }
            try {
                cfg.tilt2 = std::stof(argv[++i]);
            } catch (...) {
                std::cerr << "Invalid inclination\n";
                return 1;
            }
        } else if (arg == "--opt" || arg == "-O") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --opt\n";
                return 1;
            }
            try {
                cfg.opt_mode = std::stof(argv[++i]);
            } catch (...) {
                std::cerr << "Invalid Optimation mode\n";
                return 1;
            }
            if (std::stof(argv[i]) > 1) {
                std::cerr << "Error value for --opt\n";
                return 1;
            }
        } else if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: nbody [NUMBER_OF_BODIES] [OPTIONS]\n";
            std::cout << " > 'the order is not important.'\n\n";
            std::cout << "Arguments:\n";
            std::cout << "  NUMBER_OF_BODIES     Number of celestial bodies (default: 3)\n\n";
            std::cout << "Options:\n";
            std::cout << "  -3D                  Enable 3D visualization\n";
            std::cout << "  -glxy                Enable galaxy collision mode\n";
            std::cout << "  -fps                 Show FPS on screen\n";
            std::cout << "  -blend, -b           Enable blending mode\n";
            std::cout << "  -trail, -t           Draw Trail\n";
            std::cout << "  -opt <M>, -O <M>     Set mode [0-1]\n";
            std::cout << "  --tilt1 <DEG>        Tilt angle for the first galaxy (in degrees)\n";
            std::cout << "  --tilt2 <DEG>        Tilt angle for the second galaxy (in degrees)\n";
            std::cout << "  --seed <N>           Set the seed for random generation (default: 4)\n";
            std::cout << "  -h, --help           Show help message\n";
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
