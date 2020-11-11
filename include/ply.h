#ifndef FSPLY
#define FSPLY

#include "fsm.h"
#include "loadconfig.h"

#include <string>
#include <vector>

namespace fs{
    class ply;
};

class fs::ply{
    public:

    class element;
    class property;
    class floatproperty;
    class shortproperty;

    std::string format;
    std::vector<element*> elements;

    explicit ply();
    ~ply();

    void load(entry *data, loadconfig &config, std::string &location);
    void release();

    class element{
        public:

        inline static int VERTEX = 912477;
        inline static int INDEX = 912478;
        
        int type;
        std::vector<property*> properties;

        int size;
        int read;

        explicit element(int &type, int size);
        ~element();

        bool needsmore();
        void increment();
        void parse(std::string &line);
    };
    class property{
        public:

        explicit property();
        virtual ~property();

        virtual void add(float value);
    };
    class floatproperty : public property{
        public:

        std::vector<float> *data;

        explicit floatproperty(std::vector<float> *data);
        ~floatproperty();

        void add(float value);
    };
    class shortproperty : public property{
        public:

        std::vector<short> *data;

        explicit shortproperty(std::vector<short> *data);
        ~shortproperty();

        void add(float value);
    };
};

#endif