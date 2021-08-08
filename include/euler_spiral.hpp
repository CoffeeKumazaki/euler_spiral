#pragma once

#include <vector>

namespace es {
  
struct SpiralPoint {
  double x, y, t;
};

struct SpiralParameter {
  double length    = 0.0;
  double initCurv  = 0.0;
  double dCurv     = 0.0;
};

enum ShapeType
{
  LINE,
  ARC,
  BIARC,
  INVALID = -1
};

SpiralPoint getEndPoint(double length, double dCurv, double initCurv = 0.0, double initX = 0.0, double initY = 0.0, double initTheta = 0.0);
SpiralPoint getEndPointFromCurvature(double length, double curvStart, double curvEnd, double initX = 0.0, double initY = 0.0, double initTheta = 0.0);
SpiralParameter getParameter(const SpiralPoint& start, const SpiralPoint& goal, SpiralParameter* init = NULL);
ShapeType calcBiarcSolution(const SpiralPoint &start, const SpiralPoint &goal, SpiralParameter &p1, SpiralParameter &p2);

} // namespace es