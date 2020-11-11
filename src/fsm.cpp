#include "../include/fsm.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/stat.h>

using namespace fs;

int SIZE_SHORT = sizeof(short);
int SIZE_INT = sizeof(int);
int SIZE_FLOAT = sizeof(float);
int SIZE_CHAR = sizeof(char);

entry::entry(){

};

entry::~entry(){

};

fsm::fsm(){
    
};

fsm::~fsm(){
    for(unsigned int i = 0; i < entries.size(); i++){
        delete entries.at(i);
    }
};

void fsm::save(const std::string &location){
    struct stat statinfo;
    
    if(stat(location.c_str(), &statinfo) == 0){
        std::remove(location.c_str());
    }
    
    std::ofstream file(location, std::fstream::binary | std::fstream::out);

    int sv = 0;
    int iv = 0;
    float fv = 0.0;

    iv = entries.size();
    file.write((char*)&iv, SIZE_INT);

    entry *e;

    for(unsigned int i = 0; i < entries.size(); i++){
        e = entries.at(i);

        iv = e->name.size();
        file.write((char*)&iv, SIZE_INT);

        iv = e->positions.size();
        file.write((char*)&iv, SIZE_INT);

        iv = e->colors.size();
        file.write((char*)&iv, SIZE_INT);

        iv = e->texcoords.size();
        file.write((char*)&iv, SIZE_INT);

        iv = e->normals.size();
        file.write((char*)&iv, SIZE_INT);
        
        iv = e->indices.size();
        file.write((char*)&iv, SIZE_INT);
        
        file.write(e->name.c_str(), e->name.size());

        for(unsigned int i2 = 0; i2 < e->positions.size(); i2++){
            fv = e->positions.at(i2);
            file.write((char*)&fv, SIZE_FLOAT);
        }
        for(unsigned int i2 = 0; i2 < e->colors.size(); i2++){
            fv = e->colors.at(i2);
            file.write((char*)&fv, SIZE_FLOAT);
        }
        for(unsigned int i2 = 0; i2 < e->texcoords.size(); i2++){
            fv = e->texcoords.at(i2);
            file.write((char*)&fv, SIZE_FLOAT);
        }
        for(unsigned int i2 = 0; i2 < e->normals.size(); i2++){
            fv = e->normals.at(i2);
            file.write((char*)&fv, SIZE_FLOAT);
        }
        for(unsigned int i2 = 0; i2 < e->indices.size(); i2++){
            sv = e->indices.at(i2);
            file.write((char*)&sv, SIZE_SHORT);
        }
    }

    file.flush();
};

void fsm::load(const loadconfig &config, const std::string &location){
    std::ifstream file(location, std::fstream::binary | std::fstream::in);

    short *sbuffer = NULL;
    int *ibuffer = NULL;
    float *fbuffer = NULL;

    if(file.is_open()){
        file.seekg(0, std::fstream::end);
        int filesize = file.tellg();
        char buffer[filesize];
        
        file.seekg(0);
        file.read(buffer, filesize);

        char *b = buffer;

        ibuffer = (int*)b;
        int entrysize = *ibuffer;
        b += SIZE_INT;

        entries.reserve(entrysize);

        for(int i = 0; i < entrysize; i++){
            entry *e = new entry();

            ibuffer = (int*)b;
            int namesize = *ibuffer;
            int positionsize = *(ibuffer + 1);
            int colorsize = *(ibuffer + 2);
            int texcoordsize = *(ibuffer + 3);
            int normalsize = *(ibuffer + 4);
            int indexsize = *(ibuffer + 5);
            b += SIZE_INT * 6;

            if(config.names && namesize != 0){
                e->name.resize(namesize);
                memcpy((void*)e->name.c_str(), b, namesize);
                b += namesize * SIZE_CHAR;
            }
            if(config.positions && positionsize != 0){
                fbuffer = (float*)b;
                e->positions.reserve(positionsize);

                for(int i2 = 0; i2 < positionsize; i2++){
                    e->positions.push_back(*(fbuffer + i2));
                }

                b += positionsize * SIZE_FLOAT;
            }
            if(config.colors && colorsize != 0){
                fbuffer = (float*)b;
                e->colors.reserve(colorsize);

                for(int i2 = 0; i2 < colorsize; i2++){
                    e->colors.push_back(*(fbuffer + i2));
                }

                b += colorsize * SIZE_FLOAT;
            }
            if(config.texcoords && texcoordsize != 0){
                fbuffer = (float*)b;
                e->texcoords.reserve(texcoordsize);
                
                for(int i2 = 0; i2 < texcoordsize; i2++){
                    e->texcoords.push_back(*(fbuffer + i2));
                }

                b += texcoordsize * SIZE_FLOAT;
            }
            if(config.normals && normalsize != 0){
                fbuffer = (float*)b;
                e->normals.reserve(normalsize);

                for(int i2 = 0; i2 < normalsize; i2++){
                    e->normals.push_back(*(fbuffer + i2));
                }

                b += normalsize * SIZE_FLOAT;
            }
            if(config.indices && indexsize != 0){
                sbuffer = (short*)b;
                e->indices.reserve(indexsize);
                
                for(int i2 = 0; i2 < indexsize; i2++){
                    e->indices.push_back(*(sbuffer + i2));
                }

                b += indexsize * SIZE_SHORT;
            }

            entries.push_back(e);
        }

    }else{
        std::string error("File could not be opened for reading, location : ");
        error.append(location);

        throw std::runtime_error(error);
    }
};

void fsm::clear(){
    for(unsigned int i = 0; i < entries.size(); i++){
        delete entries.at(i);
    }

    entries.clear();
    entries.reserve(10);
};