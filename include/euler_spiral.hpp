#pragma once

#include <vector>

namespace es {
  
struct SpiralPoint {
  double x, y, t;
};

SpiralPoint calcFromCurvature(double length, double curvStart, double curvEnd, double initX = 0.0, double initY = 0.0, double initTheta = 0.0);
SpiralPoint calc(double length, double dCurv, double initCurv = 0.0, double initX = 0.0, double initY = 0.0, double initTheta = 0.0);

} // namespace es