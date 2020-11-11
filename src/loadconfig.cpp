#include "../include/loadconfig.h"

using namespace fs;

loadconfig::loadconfig(bool names, bool positions, bool colors, bool texcoords, bool normals, bool indices){
    this->names = names;
    this->positions = positions;
    this->colors = colors;
    this->texcoords = texcoords;
    this->normals = normals;
    this->indices = indices;
};

loadconfig::~loadconfig(){

};
