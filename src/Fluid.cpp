#include "Fluid.h"

#include <iostream>
#include <omp.h>
#include <cmath>
#include <chrono>

#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Texture.h"


using namespace std;
using namespace Eigen;

Fluid::Fluid() :
	count(0)
{
}

Fluid::~Fluid()
{
}

//#pragma omp parallel num_threads(100)
void Fluid::step(float h)
{
	auto start = chrono::high_resolution_clock::now();
	#pragma omp parallel for
		for (int i = 0; i < count; i++) {
			auto particle = particles[i];

			particle->density() = 0.0;

			//Kdtree::KdNodeVector result;
			//tree.range_nearest_neighbors(particle, 1.0, &result);

			//for (int j = 0; j < result.size(); j++) {
			//	auto neighbor = result[j].point;
			//
			//	if (particle == neighbor) {
			//		continue;
			//	}

			//	Vector3f diffPos = particle->x() - neighbor->x();

			//	float rSquared = diffPos.dot(diffPos);

			//	if (rSquared <= H * H && rSquared >= 0)
			//		particle->density() += M * w.W(rSquared);
			//}

			for (int j = 0; j < count; j++) {

				if (i == j) {
					continue;
				}

				auto neighbor = particles[j];

				Vector3f diffPos = particle->x() - neighbor->x();

				float rSquared = diffPos.dot(diffPos);

				if (rSquared <= H * H && rSquared >= 0)
					particle->density() += M * w.W(rSquared);
			}

			particle->pressure() = K * (particle->density() - RHO);
		}

	#pragma omp parallel for
		for (int i = 0; i < count; i++) {
			auto particle = particles[i];
			

			Vector3f pressureF = Vector3f::Zero();
			Vector3f viscosityF = Vector3f::Zero();
			Vector3f gravityF = Vector3f::Zero();
			gravityF(1) = -9.8;

			//Kdtree::KdNodeVector result;
			//tree.range_nearest_neighbors(particle, 1.0, &result);

			//for (int j = 0; j < result.size(); j++) {
			//	auto neighbor = result[j].point;

			//	if (particle == neighbor) {
			//		continue;
			//	}

			//	Vector3f diffPos = particle->x() - neighbor->x();

			//	float r = diffPos.norm();

			//	if (r <= H && r >= 0) {
			//		//pressureF += (particle->pressure() / pow(particle->density(), 2) + neighbor->pressure() / pow(neighbor->density(), 2)) * w.WGrad(diffPos, rSquared);
			//		pressureF += - M * ((particle->pressure() + neighbor->pressure()) / (2 * neighbor->density())) * w.WGrad(r) * (diffPos / r);
			//		viscosityF += V * M * (neighbor->v() - particle->v()) / neighbor->density() * w.WLap(r);
			//	}
			//}

			for (int j = 0; j < count; j++) {

				if (i == j) {
					continue;
				}

				auto neighbor = particles[j];

				Vector3f diffPos = particle->x() - neighbor->x();

				float r = diffPos.norm();

				if (r <= H && r >= 0) {
						//pressureF += (particle->pressure() / pow(particle->density(), 2) + neighbor->pressure() / pow(neighbor->density(), 2)) * w.WGrad(diffPos, rSquared);
					pressureF += - M * ((particle->pressure() + neighbor->pressure()) / (2 * neighbor->density())) * w.WGrad(r) * (diffPos / r);
					viscosityF += V * M * (neighbor->v() - particle->v()) / neighbor->density() * w.WLap(r);
				}

			}
			if (particle->obstacle()) {
				continue;
			}

			gravityF *= particle->density();

			Vector3f a = (pressureF + viscosityF + gravityF) / particle->density();

			particle->v() += h * a;
			particle->x() += h * particle->v();

			if (particle->x()(0) < -.7) {
				particle->x()(0) = -.7;
				particle->v()(0) *= WALL_DAMPING;
			}
			else if (particle->x()(0) > .7) {
				particle->x()(0) = .7;
				particle->v()(0) *= WALL_DAMPING;
			}
			if (particle->x()(2) < -.5) {
				particle->x()(2) = -.5;
				particle->v()(2) *= WALL_DAMPING;
			}
			else if (particle->x()(2) > .5) {
				particle->x()(2) = .5;
				particle->v()(2) *= WALL_DAMPING;
			}
			if (particle->x()(1) < 0) {
				particle->x()(1) = 0;
				particle->v()(1) *= WALL_DAMPING;
			}
		}

	//addParticle();
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	//cout << duration.count() << endl;
}

float Fluid::randFloat(float l, float h)
{
	float r = rand() / (float)RAND_MAX;
	return (1.0f - r) * l + r * h;
}

void Fluid::init(int n)
{
	n += X;
	this->n = n;

	posBuf.resize(3*n);
	colBuf.resize(3*n);
	scaBuf.resize(n);
	
	for(int i = 0; i < n; ++i) {
		posBuf[3*i+0] = 0.0f;
		posBuf[3*i+1] = 0.0f;
		posBuf[3*i+2] = 0.0f;
		if (i >= n - X) {
			colBuf[3 * i + 0] = 1.0f;
			colBuf[3 * i + 1] = 0.0f;
			colBuf[3 * i + 2] = 0.0f;
		}
		else {
			colBuf[3*i+0] = 0.3f;
			colBuf[3*i+1] = 0.3f;
			colBuf[3*i+2] = 1.0f;
		}
		scaBuf[i] = 0.1f;
	}

	// Generate buffer IDs
	GLuint bufs[3];
	glGenBuffers(3, bufs);
	posBufID = bufs[0];
	colBufID = bufs[1];
	scaBufID = bufs[2];
	
	// Send color buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, colBufID);
	glBufferData(GL_ARRAY_BUFFER, colBuf.size()*sizeof(float), &colBuf[0], GL_STATIC_DRAW);
	
	// Send scale buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, scaBufID);
	glBufferData(GL_ARRAY_BUFFER, scaBuf.size()*sizeof(float), &scaBuf[0], GL_STATIC_DRAW);
	
	assert(glGetError() == GL_NO_ERROR);

	Vector3f nor, pos;
	nor << 0, 1, 0;
	pos << 0, 0, 0;
	auto floor = make_shared<Wall>(nor, pos);
	walls.push_back(floor);
	nor << 1, 0, 0;
	pos << -1, 0, 0;
	auto lWall = make_shared<Wall>(nor, pos);
	walls.push_back(lWall);
	nor << -1, 0, 0;
	pos << 1, 0, 0;
	auto rWall = make_shared<Wall>(nor, pos);
	walls.push_back(rWall);
}

void Fluid::createParticles(int n) {
	n += X;
	this->count = n;
	for (int i = 0; i < n - X; i++) {
		auto p = make_shared<Particle>(i, posBuf);
		Vector3f pos;
		pos << -0.45f + (i % 10) * 0.1f, 0.5f + ((i % 100) / 10) * 0.1f, -0.25f + (i / 100) * 0.1f;
		//pos << -0.5f + (i % 10) * 0.1f, 0.5f + (i / 10) * 0.1f, 0.0f;
		p->x() = pos;
		particles.push_back(p);
		nodes.push_back(Kdtree::KdNode(p));
	}
	for (int i = n - X; i < n; i++) {
		auto p = make_shared<Particle>(i, posBuf);
		Vector3f pos;
		pos << -0.25f + (i % 5) * 0.1f, 0.25f, 0.0f;
		p->x() = pos;
		p->obstacle() = true;
		particles.push_back(p);
		nodes.push_back(Kdtree::KdNode(p));
	}
	tree = Kdtree::KdTree(&nodes, 2);

	//float totalDensity = 0;
	//for (int i = 0; i < count; i++) {
	//	auto particle = particles[i];

	//	particle->density() = 0.0;

	//	for (int j = 0; j < count; j++) {

	//		if (i == j) {
	//			continue;
	//		}

	//		auto neighbor = particles[j];

	//		Vector3f diffPos = particle->x() - neighbor->x();

	//		float rSquared = diffPos.dot(diffPos);

	//		if (rSquared <= H * H && rSquared >= 0)
	//			particle->density() += M * w.W(rSquared);
	//	}
	//	totalDensity += particle->density();
	//	std::cout << particle->density() << std::endl;
	//	std::cout << "-----------" << std::endl;
	//}
	//std::cout << "average " << totalDensity / n << std::endl;
	//175.52
}

void Fluid::addParticle()
{
	if (count < n) {
		auto p = make_shared<Particle>(count, posBuf);
		p->v() << randFloat(-2, 0), randFloat(-0.1, 0.1), 0;
		particles.push_back(p);
		count++;
	}
}

void Fluid::draw(shared_ptr<Program> prog)
{
	// Enable, bind, and send position array
	glEnableVertexAttribArray(prog->getAttribute("aPos"));
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(prog->getAttribute("aPos"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Enable and bind color array
	glEnableVertexAttribArray(prog->getAttribute("aCol"));
	glBindBuffer(GL_ARRAY_BUFFER, colBufID);
	glVertexAttribPointer(prog->getAttribute("aCol"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Enable and bind scale array
	glEnableVertexAttribArray(prog->getAttribute("aSca"));
	glBindBuffer(GL_ARRAY_BUFFER, scaBufID);
	glVertexAttribPointer(prog->getAttribute("aSca"), 1, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Draw
	glDrawArrays(GL_POINTS, 0, 3*particles.size());
	
	// Disable and unbind
	glDisableVertexAttribArray(prog->getAttribute("aSca"));
	glDisableVertexAttribArray(prog->getAttribute("aCol"));
	glDisableVertexAttribArray(prog->getAttribute("aPos"));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
