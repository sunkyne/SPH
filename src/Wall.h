#pragma once
#ifndef _WALL_H_
#define _WALL_H_

#define _USE_MATH_DEFINES
#include <memory>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

class Wall
{
public:
	Wall(Eigen::Vector3f n, Eigen::Vector3f p);
	virtual ~Wall();
	
	Eigen::Vector3f& n() { return _n; }
	Eigen::Vector3f& p() { return _p; }

private:
	Eigen::Vector3f _n;
	Eigen::Vector3f _p;
};

#endif
