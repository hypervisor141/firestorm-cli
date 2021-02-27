#include "../include/ply.h"
#include "../include/wavefront.h"
#include <cerrno>
#include <iostream>
#include <string>
#include <filesystem>
#include <wordexp.h>

namespace fsystem = std::filesystem;

void printUsage(){
    std::string info(" FireStorm's official .fsm file generator, using .obj and .ply as inputs.\n \
    Usage : \n \
        --help      print this help message\n \
        --input=     path to an input .obj file\n \
        --output=    path to output the resulting FSM file\n");

    std::cout << info << std::endl;
};

void validateFile(std::string &input){
    fsystem::file_status results = fsystem::status(input);

    if(fsystem::is_directory(results)){
        std::cerr << input << " is a directory, but a file path is required." << std::endl;
        exit(3);

    }else if(fsystem::is_regular_file(results)){
        return;

    }else{
        std::cerr << input << " is not a file nor a directory." << std::endl;
        exit(3);
    }
}

std::string expandPath(std::string target){
    wordexp_t result;
    wordexp(target.c_str(), &result, 0);

    return std::string(result.we_wordv[0]);
}

void parseArgs(int size, char** args){
    if(size <= 1){
        printUsage();
        exit(1);
    }

    char *arg = NULL;
    std::string inputpath;
    std::string outputpath;

    inputpath.reserve(4096);
    outputpath.reserve(4096);
 
    for(int i = 1; i < size; i++){
        arg = args[i];

        if(arg != NULL){
            std::string param(arg);

            if(param.empty()){
                continue;

            }else if(param.rfind("--input=") == 0){
                inputpath.append(expandPath(param.substr(8, param.size() - 8).c_str()));
                validateFile(inputpath);

            }else if(param.rfind("--output=") == 0){
                outputpath.append(expandPath(param.substr(9, param.size() - 9).c_str()));

            }else{
                std::cerr << "[Error] Invalid command " << param << std::endl;
                exit(2);
            }
        }
    }

    if(inputpath.empty()){
        std::cerr << "[Error] No input file specified." << std::endl;

    }else if(outputpath.empty()){
        std::cerr << "[Error] No output file specified." << std::endl;

    }else{
        fs::loadconfig config(true, true, true, true, true, true);
        fs::fsm target;
        fs::wavefront obj;

        obj.load(target, config, inputpath, true);
        target.save(outputpath);
        target.clear();
    }
}

int main(int size, char **args){
    parseArgs(size, args);
    return 0;
}