#include <stdafx.hpp>
#include <euler_spiral.hpp>
#include <fresnel_integral.hpp>

namespace es
{
  
SpiralPoint calcFromCurvature(double length, double curvStart, double curvEnd, double initX /*= 0.0*/, double initY /*= 0.0*/, double initTheta /*= 0.0*/) {

  return calc(length, (curvEnd - curvStart) / length, curvStart, initX, initY, initTheta);
}

SpiralPoint calc(double length, double dCurv, double initCurv /*= 1000.0*/, double initX /*= 0.0*/, double initY /*= 0.0*/, double initTheta /*= 0.0*/) {

  SpiralPoint pos;

  if (dCurv == 0.0 && initCurv == 0.0) {
    // Straight line.
    pos.x = initX + length * cos(initTheta);
    pos.y = initY + length * sin(initTheta);
  }
  else if (dCurv == 0.0 && initCurv != 0.0) {
    // Arc
    double x = sin(initCurv * length) / initCurv;
    double y = (1.0 - cos(initCurv * length) ) / initCurv;

    pos.x = x * cos(initTheta) + y * sin(initTheta);
    pos.y = x * sin(initTheta) - y * cos(initTheta);

    pos.x += initX;
    pos.y += initY;
  }
  else {
    double a = 1.0 / sqrt(M_PI * abs(dCurv));

    double x = fresnel_cos((initCurv + dCurv * length) * a) - fresnel_cos(initCurv * a);
    double y = fresnel_sin((initCurv + dCurv * length) * a) - fresnel_sin(initCurv * a);

    double theta = initTheta - initCurv * initCurv * 0.5 / dCurv;
    pos.x = M_PI * a * (x * cos(theta) + y * sin(theta));
    pos.y = M_PI * a * (x * sin(theta) - y * cos(theta));

    pos.x += initX;
    pos.y += initY;

    if (dCurv < 0.0)
      pos.y *= -1.0;

  }

  pos.t = dCurv * length * length / 2.0 + initCurv * length + initTheta;

  return pos;
}

} // namespace es