#ifndef UTTYPE
#define UTTYPE

namespace uttype{
    
    static const long SIZE_CHAR = sizeof(char);
    static const long SIZE_SHORT = sizeof(short);
    static const long SIZE_INT = sizeof(int);
    static const long SIZE_LONG = sizeof(long);
    static const long SIZE_LONG_LONG = sizeof(long long);
    static const long SIZE_FLOAT = sizeof(float);
    static const long SIZE_DOUBLE = sizeof(double);
    static const long SIZE_LONG_DOUBLE = sizeof(long double);
    
    extern short makeShort(char *data);
    extern int makeInt(char *data);
    extern long makeLong(char *data);
    extern long long makeLongLong(char *data);
    extern float makeFloat(char *data);
    extern double makeDouble(char *data);
    extern long double makeLongDouble(char *data);
    extern void parseShort(short input, char *data);
    extern void parseInt(int input, char *data);
    extern void parseLong(long input, char *data);
    extern void parseLongLong(long long input, char *data);
    extern void parseFloat(float input, char *data);
    extern void parseDouble(double input, char *data);
    extern void parseLongDouble(long double input, char *data);
};

#endif
