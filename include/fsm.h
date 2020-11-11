#ifndef FSM
#define FSM

#include "loadconfig.h"

#include <string>
#include <vector>

namespace fs{
    class fsm;
    class entry;
};

class fs::entry{
    public:

    std::string name;
    std::vector<float> positions;
    std::vector<float> colors;
    std::vector<float> texcoords;
    std::vector<float> normals;
    std::vector<short> indices;

    entry();
    ~entry();
};

class fs::fsm{
    public:

    std::vector<entry*> entries;

    explicit fsm();
    ~fsm();
    
    void save(const std::string &location);
    void load(const loadconfig &config, const std::string &location);
    void clear();
};


#endif