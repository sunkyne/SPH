#include "Field.h"

Field::Field() :
	_xRes(0), _yRes(0), _zRes(0), _data(NULL)
{
}

Field::Field(int xRes, int yRes, int zRes) :
	_xRes(xRes), _yRes(yRes), _zRes(zRes), _cellCount(xRes* yRes* zRes)
{
	_data = new particleVector[_xRes * _yRes * _zRes];
}

Field::~Field()
{
	if (_data) delete[] _data;
}