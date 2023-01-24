#ifndef FIELD_H
#define FIELD_H

#include <cstdlib>
#include <iostream>
#include "assert.h"
#include "Particle.h"

using namespace std;

class Field {

    typedef vector<Particle> particleVector;

public:

    Field();
    Field(int xRes, int yRes, int zRes);
    virtual ~Field();

    inline particleVector& operator()(int x, int y, int z) {

        /*

        assert(x >= 0);
        assert(x < _xRes);
        assert(y >= 0);
        assert(y < _yRes);
        assert(z >= 0);
        assert(z < _zRes); // i*length*width + j*width + k

         */
        return _data[x + y * _xRes + z * _xRes * _yRes];
    }

    // accessors
    int xRes() const { return _xRes; };
    int yRes() const { return _yRes; };
    int zRes() const { return _zRes; };
    int cellCount() const { return _cellCount; };
    particleVector* data() const { return _data; };

private:

    int _xRes;
    int _yRes;
    int _zRes;
    int _cellCount;

    particleVector* _data;

};

#endif