#include <omp.h>
#include <iostream>
#include <unordered_map>
#include "Scenes/Scene_1.h"
#include "Scenes/Scene_2.h"
#include "Scenes/Scene_3.h"
#include "Scenes/Scene_4.h"

int main(int argc, const char **argv) {
    std::unordered_map<std::string, std::string> cmdLineParams;
    for (int i = 0; i < argc; i++) {
        std::string key(argv[i]);
        if (key.size() > 0 && key[0] == '-') {
            if (i != argc - 1) // not last argument
            {
                cmdLineParams[key] = argv[i + 1];
                i++;
            } else
                cmdLineParams[key] = "";
        }
    }

    std::string outFilePath;
    if (cmdLineParams.find("-out") != cmdLineParams.end())
        outFilePath = cmdLineParams["-out"];

    int sceneId = 1;
    if (cmdLineParams.find("-scene") != cmdLineParams.end())
        sceneId = atoi(cmdLineParams["-scene"].c_str());

    int threads = -1;
    if (cmdLineParams.find("-threads") != cmdLineParams.end()) {
        threads = atoi(cmdLineParams["-threads"].c_str());
    }

    if (threads != -1) {
        omp_set_num_threads(threads);
    }

    switch (sceneId) {
        case 1: {
            if (outFilePath.empty()) {
                outFilePath = "../Output/Out_1.bmp";
            }
            scene_1(outFilePath);
            break;
        }
        case 2: {
            if (outFilePath.empty()) {
                outFilePath = "../Output/Out_2.bmp";
            }
            scene_2(outFilePath);
            break;
        }
        case 3: {
            if (outFilePath.empty()) {
                outFilePath = "../Output/Out_3.bmp";
            }
            scene_3(outFilePath);
            break;
        }
        case 4: {
            if (outFilePath.empty()) {
                outFilePath = "../Output/Out_4.bmp";
            }
            scene_4(outFilePath);
            break;
        }
        default: {
            break;
        }
    }
    std::cout << "Done!" << std::endl;
    return 0;
}
