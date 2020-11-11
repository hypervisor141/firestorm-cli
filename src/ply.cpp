#include "../include/ply.h"
#include "../include/utils.h"

#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace fs;

void parseVertex(ply::element *element, std::string &line);
void parseFace(ply::element *element, std::string &line);

ply::ply(){
    
};

ply::~ply(){
    release();
};

void ply::load(entry *data, loadconfig &config, std::string &location){
    std::ifstream file(location.c_str());
    
    if(config.names){
        int startindex = location.rfind("/") + 1;
        data->name.assign(location.substr(startindex, location.rfind(".") - startindex));
    }
    
    if(file.is_open()){
        element *current = NULL;
        std::string line;
        bool headerfinished = false;
        int elementindex = 0;

        while(std::getline(file, line)){
            line.assign(utils::trim_copy(line));
            boost::to_lower(line);

            if(!headerfinished){
                if(line.find("format") == 0){
                    format.assign(line.substr(7));

                }else if(line.find("element") == 0){
                    int from = line.find(" ") + 1;
                    int lastspace = line.rfind(" ");
                    int size = std::stoi(line.substr(lastspace + 1));

                    std::string type(line.substr(from, lastspace - from));
                    boost::to_lower(type);
                    int itype = 0;

                    if(type.compare("vertex") == 0){
                        itype = element::VERTEX;

                    }else if(type.compare("face") == 0){
                        itype = element::INDEX;

                    }else{
                        std::string err("Invalid element type : ");
                        err.append(type);
                        throw new std::runtime_error(err);
                    }

                    current = new element(itype, size);
                    elements.push_back(current);

                }else if(line.find("property") == 0){
                    std::string type = line.substr(line.rfind(" ") + 1);
                    property *p = NULL;

                    if(type.compare("x") == 0 && config.positions){
                        p = new floatproperty(&data->positions);

                    }else if(type.compare("y") == 0 && config.positions){
                        p = new floatproperty(&data->positions);
                        
                    }else if(type.compare("z") == 0 && config.positions){
                        p = new floatproperty(&data->positions);
                        
                    }else if(type.compare("nx") == 0 && config.normals){
                        p = new floatproperty(&data->normals);
                        
                    }else if(type.compare("ny") == 0 && config.normals){
                        p = new floatproperty(&data->normals);
                        
                    }else if(type.compare("nz") == 0 && config.normals){
                        p = new floatproperty(&data->normals);
                        
                    }else if(type.compare("s") == 0 && config.texcoords){
                        p = new floatproperty(&data->texcoords);
                        
                    }else if(type.compare("t") == 0 && config.texcoords){
                        p = new floatproperty(&data->texcoords);
                        
                    }else if(type.compare("red") == 0 && config.colors){
                        p = new floatproperty(&data->colors);

                    }else if(type.compare("green") == 0 && config.colors){
                        p = new floatproperty(&data->colors);

                    }else if(type.compare("blue") == 0 && config.colors){
                        p = new floatproperty(&data->colors);

                    }else if(type.compare("alpha") == 0 && config.colors){
                        p = new floatproperty(&data->colors);

                    }else if(type.compare("vertex_indices") == 0 && config.indices){
                        p = new shortproperty(&data->indices);
                    }

                    current->properties.push_back(p);

                }else if(line.compare("end_header") == 0){
                    headerfinished = true;
                    current = elements.at(0);
                }

            }else{
                if(current->needsmore()){
                    current->parse(line);

                }else{
                    elementindex++;
                    
                    if(elementindex == elements.size()){
                        return;
                    }

                    current = elements.at(elementindex);
                    current->parse(line);
                }
            }
        }

        release();
        
    }else{
        std::cerr << "File could not be opened : " << location << std::endl;
        exit(1);
    }
};

void ply::release(){
    for(int i = 0; i < elements.size(); i++){
        delete elements.at(i);
    }

    elements.clear();
    elements.reserve(10);
};

ply::element::element(int &type, int size){
    this->type = type;
    this->size = size;
    this->read = 0;

    properties.reserve(size);
};

ply::element::~element(){
    property *p;

    for(int i = 0; i < properties.size(); i++){
        delete properties.at(i);
    }
};

bool ply::element::needsmore(){
    return read < size;
};

void ply::element::increment(){
    read++;
};

void ply::element::parse(std::string &line){
    if(type == VERTEX){
        parseVertex(this, line);

    }else if(type == INDEX){
        parseFace(this, line);
    }
};

void parseVertex(ply::element *element, std::string &line){
    int fromindex = 0;
    int toindex = line.find(" ");
    int propertyindex = 0;
    float value = 0;

    std::vector<ply::property*> &properties = element->properties;
    ply::floatproperty *p = NULL;
    
    while(true){
        value = stof(line.substr(fromindex, toindex));
        p = (ply::floatproperty*)properties.at(propertyindex);

        if(p != NULL){
            if(toindex > 0){
                p->add(value);

            }else{
                value = stof(line.substr(fromindex));
                properties.at(propertyindex)->add(value);

                break;
            }
        }

        fromindex = line.find(" ", fromindex + 1) + 1;
        toindex = line.find(" ", fromindex);
        propertyindex++;
    }

    element->increment();
};

void parseFace(ply::element *element, std::string &line){
    ply::property *indexproperty = element->properties.at(0);

    if(indexproperty != NULL){
        int fromindex = line.find(" ");
        int toindex = line.find(" ");
        int value = stoi(line.substr(0, toindex));

        if(value != 3){
            std::string error("Only triangulated meshes are allowed, current value : ");
            error.append(std::to_string(value));
            error.append(", current line : [ ");
            error.append(line);
            error.append(" ].");
            
            throw std::runtime_error(error);

        }else{
            fromindex = line.find(" ", toindex);
            toindex = line.find(" ", fromindex + 1);
            value = stoi(line.substr(fromindex, toindex - fromindex));
            
            indexproperty->add(value);
            
            fromindex = line.find(" ", toindex);
            toindex = line.find(" ", fromindex + 1);
            value = stoi(line.substr(fromindex, toindex - fromindex));

            indexproperty->add(value);
            
            fromindex = line.find(" ", toindex);
            value = stoi(line.substr(fromindex));
            
            indexproperty->add(value);
        }

        element->read += element->properties.size();
    }
};

ply::property::property(){
    
};

ply::property::~property(){
    
};

void ply::property::add(float value){

};

ply::floatproperty::floatproperty(std::vector<float> *source){
    this->data = source;
};

void ply::floatproperty::add(float value){
    data->push_back(value);
};

ply::floatproperty::~floatproperty(){
    
};

ply::shortproperty::shortproperty(std::vector<short> *source){
    this->data = source;
};

void ply::shortproperty::add(float value){
    data->push_back((short)value);
};

ply::shortproperty::~shortproperty(){
    
};