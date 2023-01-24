#pragma once
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#define _USE_MATH_DEFINES
#include <memory>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

class MatrixStack;
class Program;
class Texture;

class Particle
{
public:
	
	Particle();
	Particle(int index, std::vector<float>& posBuf);
	virtual ~Particle();

	bool& obstacle() { return _obstacle; }
	float& density() { return _density; }
	float& restDensity() { return _restDensity; }
	float& pressure() { return _pressure; }
	float& viscosity() { return _viscosity; }
	Eigen::Map<Eigen::Vector3f>& x() { return _x; }
	Eigen::Vector3f& v() { return _v; }
	Eigen::Vector3f& a() { return _a; }

	void step();
	
private:
	bool _obstacle;
	float _restDensity;
	float _density;
	float _pressure;
	float _viscosity;
	
	// Properties that changes every frame
	Eigen::Map<Eigen::Vector3f> _x; // position (mapped to a location in posBuf)
	Eigen::Vector3f _v;             // velocity
	Eigen::Vector3f _a;             // acceleration
};

#endif
