#include "../include/uttype.h"
#include <cstring>

short uttype::makeShort(char *input){
    short result = 0;

    for(long i = 0, i2 = (uttype::SIZE_SHORT * 8) - 8; i < uttype::SIZE_SHORT; i++, i2 -= 8){
        result |= (short)((input[i] & (0xFF)) << i2);
    }

    return result;
};

int uttype::makeInt(char *input){
    int result = 0;

    for(long i = 0, i2 = (uttype::SIZE_INT * 8) - 8; i < uttype::SIZE_INT; i++, i2 -= 8){
        result |= (int)((input[i] & (0xFF)) << i2);
    }

    return result;
};

long uttype::makeLong(char *input){
    long result = 0;

    for(long i = 0, i2 = (uttype::SIZE_LONG * 8) - 8; i < uttype::SIZE_LONG; i++, i2 -= 8){
        result |= (long)((input[i] & (0xFF)) << i2);
    }

    return result;
};

long long uttype::makeLongLong(char *input){
    long long result = 0;

    for(long i = 0, i2 = (uttype::SIZE_LONG_LONG * 8) - 8; i < uttype::SIZE_LONG_LONG; i++, i2 -= 8){
        result |= (long long)((input[i] & (0xFFL)) << i2);
    }

    return result;
};

float uttype::makeFloat(char *input){
    float result = 0;
    memcpy(&result, input, uttype::SIZE_FLOAT);
    return result;
};

double uttype::makeDouble(char *input){
    double result = 0;
    memcpy(&result, input, uttype::SIZE_DOUBLE);

    return result;
};

long double uttype::makeLongDouble(char *input){
    long double result = 0;
    memcpy(&result, input, uttype::SIZE_LONG_DOUBLE);

    return result;
};

void uttype::parseShort(short input, char *results){
    for(long i = 0, i2 = (uttype::SIZE_SHORT * 8) - 8; i < uttype::SIZE_SHORT; i++, i2 -= 8){
        results[i] = (char)(input >> i2);
    }
};

void uttype::parseInt(int input, char *results){
    for(long i = 0, i2 = (uttype::SIZE_INT * 8) - 8; i < uttype::SIZE_INT; i++, i2 -= 8){
        results[i] = (char)(input >> i2);
    }
};

void uttype::parseLong(long input, char *results){
    for(long i = 0, i2 = (uttype::SIZE_LONG * 8) - 8; i < uttype::SIZE_LONG; i++, i2 -= 8){
        results[i] = (char)(input >> i2);
    }
};

void uttype::parseLongLong(long long input, char *results){
    for(long i = 0, i2 = (uttype::SIZE_LONG_LONG * 8) - 8; i < uttype::SIZE_LONG_LONG; i++, i2 -= 8){
        results[i] = (char)(input >> i2);
    }
};

void uttype::parseFloat(float input, char *data){
    memcpy(data, &input, uttype::SIZE_FLOAT);
};

void uttype::parseDouble(double input, char *data){
    memcpy(data, &input, uttype::SIZE_DOUBLE);
};

void uttype::parseLongDouble(long double input, char *data){
    memcpy(data, &input, uttype::SIZE_LONG_DOUBLE);
};

