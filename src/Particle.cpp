#include "Particle.h"

#include <iostream>

#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Texture.h"

using namespace std;
using namespace Eigen;

Particle::Particle() :
	_x(0)
{
}

// Before this constructor is called, posBuf must be a valid vector<float>.
// I.e., Particle::init(n) must be called first.
Particle::Particle(int index, std::vector<float>& posBuf) :
	_obstacle(false),
	_density(0),
	_pressure(0),
	_viscosity(0),
	_x(&posBuf[3 * index])
{
	_v = Vector3f::Zero();
	_a = Vector3f::Zero();
}

Particle::~Particle()
{
}

void Particle::step()
{
	
}