#include "Wall.h"

#include <iostream>

#include "GLSL.h"

using namespace std;
using namespace Eigen;

// Before this constructor is called, posBuf must be a valid vector<float>.
// I.e., Particle::init(n) must be called first.
Wall::Wall(Vector3f n, Vector3f p) :
	_n(n),
	_p(p)
{

}

Wall::~Wall()
{
}