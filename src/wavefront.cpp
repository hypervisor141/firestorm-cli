#include "../include/wavefront.h"
#include "../include/utils.h"
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace fs;

static std::string PROGRESSTEXT;

face::face(int pindex, int uindex, int nindex){
    this->positionindex = pindex;
    this->uvindex = uindex;
    this->normalindex = nindex;
};

wavefront::wavefront(){

};

wavefront::~wavefront(){
    
};

void wavefront::load(fsm &fsm, loadconfig &config, std::string &location, bool showprogress){
    std::ifstream file(location.c_str());

    positioncount = 0;
    uvcount = 0;
    normalcount = 0;

    if(showprogress){
        std::cout << "Loading[" << location.c_str() << "]\n";
    }
    
    if(file.is_open()){
        std::string line;
        std::string type;
        entry *current = NULL;

        while(std::getline(file, line)){
            line.assign(utils::trim_copy(line));
            type.assign(line.substr(0, line.find(" ")));
            line.assign(line.substr(line.find(" ") + 1));
            
            if(type.compare("o") == 0){
                if(current != NULL){
                    resolve(current, positioncount, uvcount, normalcount, showprogress);
                    current = NULL;

                    if(showprogress){
                        std::cout << " [SUCCESS]" << std::endl;
                    }
                }

                current = new entry();
                fsm.entries.push_back(current);

                faces.clear();
                tracker.clear();

                if(config.names){
                    readName(current, line);

                    if(showprogress){
                        std::cout << "[" << current->name << "] ";
                    }
                }

            }else if(type.compare("v") == 0 && config.positions){
                readPosition(line, positioncount);

            }else if(type.compare("vt") == 0 && config.texcoords){
                readUV(line, uvcount);

            }else if(type.compare("vn") == 0 && config.normals){
                readNormal(line, normalcount);

            }else if(type.compare("f") == 0 && config.indices){
                readFaces(line);
            }
        }

        resolve(current, positioncount, uvcount, normalcount, showprogress);
        
        file.close();

        if(showprogress){
            std::cout << std::endl;
        }
        
    }else{
        std::cerr << "File could not be opened : " << location << std::endl;
        exit(1);
    }
};

void wavefront::readName(entry *data, std::string &line){
    std::string &name = data->name;
    name.assign(line);
    boost::algorithm::to_lower(name);
    utils::trim(name);
};

void wavefront::readPosition(std::string &line, int &positioncount){
    int index = line.find(" ");
    int secondindex = line.rfind(" ");

    tmppositions.push_back(std::stof(line.substr(0, index)));
    tmppositions.push_back(std::stof(line.substr(index + 1, secondindex)));
    tmppositions.push_back(std::stof(line.substr(secondindex + 1)));
    
    positioncount++;

};

void wavefront::readUV(std::string &line, int &uvcount){
    int index = line.find(" ");

    tmptexcoords.push_back(std::stof(line.substr(0, index)));
    tmptexcoords.push_back(1 - std::stof(line.substr(index + 1)));

    uvcount++;
};

void wavefront::readNormal(std::string &line, int &normalcount){
    int index = line.find(" ");
    int secondindex = line.rfind(" ");

    tmpnormals.push_back(std::stof(line.substr(0, index)));
    tmpnormals.push_back(std::stof(line.substr(index + 1, secondindex)));
    tmpnormals.push_back(std::stof(line.substr(secondindex + 1)));

    normalcount++;
};

void wavefront::readFaces(std::string &line){
    int separator1 = line.find("/");
    int separator2  = line.find("/", separator1 + 1);
    int start = 0;
    int space = line.find(" ", separator2);
    
    face *f = NULL;

    for(int i = 0; i < 3; i++){
        f = new face(
            std::stoi(line.substr(start, separator1 - start)),
            tmptexcoords.size() == 0 ? -1 : std::stoi(line.substr(separator1 + 1, separator2 - separator1)),
            tmpnormals.size() == 0 ? -1 : std::stoi(line.substr(separator2 + 1, space - separator2)));
            
        separator1 = line.find("/", space);
        separator2  = line.find("/", separator1 + 1);
        start = space + 1;
        space = line.find(" ", separator2);

        if(space == -1){
            space = line.length();
        }

        faces.push_back(f);
    }
};

void wavefront::resolve(entry *data, int &positioncount, int &uvcount, int &normalcount, bool showprogress){
    if(showprogress){
        std::cout << "resolving[";
    }

    int poffset = (positioncount * 3 - tmppositions.size()) / 3 + 1;
    int uoffset = (uvcount * 2 - tmptexcoords.size()) / 2 + 1;
    int noffset = (normalcount * 3 - tmpnormals.size()) / 3 + 1;

    std::vector<float> &positions = data->positions;
    std::vector<float> &texcoords = data->texcoords;
    std::vector<float> &normals = data->normals;
    std::vector<short> &indices = data->indices;

    tracker.reserve(faces.size());

    PROGRESSTEXT.clear();

    for(int i = 0; i < faces.size(); i++){
        face *f1 = faces.at(i);
        int found = -1;

        if(showprogress){
            int size = PROGRESSTEXT.size();

            for(int index = 0; index < size; index++){
                PROGRESSTEXT.replace(index, 1, "\b");
            }

            std::cout << PROGRESSTEXT;

            PROGRESSTEXT.clear();
            PROGRESSTEXT.append(std::to_string(i + 1));
            PROGRESSTEXT.append("/");
            PROGRESSTEXT.append(std::to_string(faces.size()));
            PROGRESSTEXT.append("]");

            std::cout << PROGRESSTEXT;
        }

        for(int i2 = 0; i2 < tracker.size(); i2++){
            face *f2 = tracker.at(i2);
            
            if(f1->positionindex == f2->positionindex && f1->uvindex == f2->uvindex && f1->normalindex == f2->normalindex){
                found = i2;
                break;
            }
        }

        if(found >= 0){
            indices.push_back((short)(found));

        }else{
            int positionindex = (f1->positionindex - poffset) * 3;
            int uvindex = (f1->uvindex - uoffset) * 2;
            int normalindex = (f1->normalindex - noffset) * 3;

            positions.push_back(tmppositions.at(positionindex));
            positions.push_back(tmppositions.at(positionindex + 1));
            positions.push_back(tmppositions.at(positionindex + 2));

            texcoords.push_back(tmptexcoords.at(uvindex));
            texcoords.push_back(tmptexcoords.at(uvindex + 1));

            normals.push_back(tmpnormals.at(normalindex));
            normals.push_back(tmpnormals.at(normalindex + 1));
            normals.push_back(tmpnormals.at(normalindex + 2));

            indices.push_back((short)(positions.size() / 3 - 1));
            tracker.push_back(f1);
        }
    }
};