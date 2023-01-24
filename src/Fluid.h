#pragma once
#ifndef _FLUID_H_
#define _FLUID_H_

#define _USE_MATH_DEFINES
#include <memory>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

#include "Particle.h"
#include "Wall.h"
#include "Kernel.h"
#include "Program.h"
#include "kdtree.h"

#define M 1.0 //mass
#define K 15 //stiffness parameter 15
#define RHO 500 //175 //reference density
#define V 1.1 //viscosity

#define WALL_K 10000.0 // wall spring constant
#define WALL_DAMPING -0.5 // wall damping constant

#define X 0

class Fluid
{
public:
	
	Fluid();
	virtual ~Fluid();

	void init(int n);
	void draw(std::shared_ptr<Program> prog);
	void createParticles(int n);
	void addParticle();
	void step(float h);
	
	float randFloat(float l, float h);
	
private:
	Kernel w;
	int n;
	int count;
	std::vector< std::shared_ptr< Particle> > particles;
	Kdtree::KdNodeVector nodes;
	Kdtree::KdTree tree;
	std::vector< std::shared_ptr< Wall> > walls;
	std::vector<float> posBuf;
	std::vector<float> colBuf;
	std::vector<float> scaBuf;
	GLuint posBufID;
	GLuint colBufID;
	GLuint scaBufID;
};

#endif
