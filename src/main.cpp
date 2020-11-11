#include "../include/ply.h"
#include "../include/wavefront.h"
#include <iostream>
#include <string>

void printUsage(){
    std::string info("");
};

int main(int size, char **args){
    if(size <= 1){
        std::cerr << "[Error] expected an input wavefront .obj file path, and an output .fsm path."

        printUsage();
    }

    std::string loc("/home/shayan/data/blender/hive/hive.obj");
    std::string output("/home/shayan/data/android/hive/app/src/main/assets/meshes.fsm");

    fs::loadconfig config(true, true, true, true, true, true);

    fs::fsm target;
    fs::wavefront meshes;

    meshes.load(target, config, loc, true);
    target.save(output);

    loc.clear();
    output.clear();
    target.clear();

    return 0;
}