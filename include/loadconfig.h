#ifndef LOADCONFIG
#define LOADCONFIG

namespace fs{
    class loadconfig;
}

class fs::loadconfig{
    public:

    bool names;
    bool positions;
    bool colors;
    bool texcoords;
    bool normals;
    bool indices;
    
    explicit loadconfig(bool names, bool positions, bool colors, bool texcoords, bool normals, bool indices);
    ~loadconfig();
};

#endif