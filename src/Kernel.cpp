#include "Kernel.h"

#include <iostream>

using namespace std;
using namespace Eigen;

Kernel::Kernel() 
{
}

Kernel::~Kernel()
{
}

float Kernel::W(float rSquared) {

    float coeff = 315.0 / (64.0 * M_PI * pow(H, 9));
    float hSquared = H * H;

    return coeff * pow(hSquared - rSquared, 3);
}

float Kernel::WGrad(float r) {

    //float coeff = -945.0 / (32.0 * M_PI * pow(H, 9));
    float coeff = -45.0 / (M_PI * pow(H, 6));

    return coeff * pow(H - r, 2);
}

float Kernel::WLap(float r) {

    float coeff = 45.0 / (M_PI * pow(H, 6));

    return coeff * (H - r);
}