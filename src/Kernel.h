#pragma once
#ifndef _KERNEL_H_
#define _KERNEL_H_

#define _USE_MATH_DEFINES
#include <memory>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

#define H .55
//.55
class Kernel
{
public:
	
	Kernel();
	virtual ~Kernel();
	
	float W(float rSquared);
	float WGrad(float r);
	float WLap(float r);
	
private:
};

#endif
