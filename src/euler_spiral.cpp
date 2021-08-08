#include <stdafx.hpp>
#include <boost/math/tools/minima.hpp>
#include <euler_spiral.hpp>
#include <fresnel_integral.hpp>

namespace es
{
  
SpiralPoint getEndPointFromCurvature(double length, double curvStart, double curvEnd, double initX /*= 0.0*/, double initY /*= 0.0*/, double initTheta /*= 0.0*/) {

  return getEndPoint(length, (curvEnd - curvStart) / length, curvStart, initX, initY, initTheta);
}

SpiralPoint getEndPoint(double length, double dCurv, double initCurv /*= 1000.0*/, double initX /*= 0.0*/, double initY /*= 0.0*/, double initTheta /*= 0.0*/) {

  SpiralPoint pos;

  if (dCurv == 0.0 && initCurv == 0.0) {
    // Straight line.
    pos.x = length * cos(initTheta);
    pos.y = length * sin(initTheta);
  }
  else if (dCurv == 0.0 && initCurv != 0.0) {
    // Arc
    double x = sin(initCurv * length) / initCurv;
    double y = (1.0 - cos(initCurv * length) ) / initCurv;

    pos.x = x * cos(initTheta) - y * sin(initTheta);
    pos.y = x * sin(initTheta) + y * cos(initTheta);
  }
  else {
    double a = 1.0 / sqrt(M_PI * abs(dCurv));

    double x = fresnel_cos_integral((initCurv + dCurv * length) * a) - fresnel_cos_integral(initCurv * a);
    double y = fresnel_sin_integral((initCurv + dCurv * length) * a) - fresnel_sin_integral(initCurv * a);

    double theta = initTheta - initCurv * initCurv * 0.5 / dCurv;
    if (dCurv < 0.0) x *= -1.0;
    pos.x = M_PI * a * (x * cos(theta) - y * sin(theta));
    pos.y = M_PI * a * (x * sin(theta) + y * cos(theta));

  }

  pos.x += initX;
  pos.y += initY;
  pos.t = dCurv * length * length / 2.0 + initCurv * length + initTheta;

  return pos;
}

double getAngleOfVector(double x, double y) {

  double angle = std::atan2(y,x);
  return angle < 0 ? angle + 2.0 * M_PI : angle;
}

double calcArcLen(double t0, double t1, double curv) {

  double num = t1 - t0;
  if (curv < 0 && num > 0) {
    num -= 2.0 * M_PI;
  }
  else if (curv > 0 && num < 0) {
    num += 2.0 * M_PI;
  }

  return num / curv;
}

double calcJoinTheta(const SpiralPoint& start, const SpiralPoint& goal, double kStart, double kGoal) {

  return getAngleOfVector(
      (-kStart * kGoal * (goal.y - start.y) + kGoal * cos(start.t) - kStart * cos(goal.t)) / (kGoal - kStart), 
      ( kStart * kGoal * (goal.x - start.x) + kGoal * sin(start.t) - kStart * sin(goal.t)) / (kGoal - kStart));
}



ShapeType calcBiarcSolution(const SpiralPoint& start, const SpiralPoint& goal, SpiralParameter& p1, SpiralParameter& p2) {

  ShapeType type = INVALID;
  double dx = goal.x - start.x;
  double dy = goal.y - start.y;

  double L = sqrt(dx*dx + dy*dy);
  double angle = getAngleOfVector(dx, dy);
  double angle_mean = (start.t + goal.t) * 0.5;

  while(angle_mean > 2.0 * M_PI) {
    angle_mean -= 2.0 * M_PI;
  }
  while(angle_mean < - 2.0 * M_PI) {
    angle_mean += 2.0 * M_PI;
  }

  if (angle == angle_mean) {
    double k1 = 0.0;
    double k2 = 0.0;
    double L1 = 0.0;
    double L2 = 0.0;
    if (angle == start.t) {
      // Straight line.
      type = LINE;
      k1 = k2 = 0.0;
      L2 = L1 = 0.5 * L;
      std::cout << "# straight" << std::endl;
    }
    else {
      // Same Arc.
      type = ARC;
      std::cout << "# Arc" << std::endl;
      k2 = k1 = -2.0 * sin(0.5 * (start.t - goal.t)) / L;
      if (angle_mean == 0.0) {
        L2 = L1 = 0.25 * M_PI * L;
      }
      else {
        L2 = L1 = L * (goal.t + start.t)/ 4.0 / sin(angle_mean);
      }
    }

    p1.initCurv = k1;
    p1.dCurv = 0.0;
    p1.length = L1;
    p2.initCurv = k2;
    p2.dCurv = 0.0;
    p2.length = L2;
  }
  else {
    std::cout << "# biarc" << std::endl;
    // biarc.
    type = BIARC;
    double k1 = -4.0 * sin((3.0 * start.t + goal.t) / 4.0 - angle) * cos((goal.t - start.t) / 4.0) / L;
    double k2 =  4.0 * sin((start.t + 3.0 * goal.t) / 4.0 - angle) * cos((goal.t - start.t) / 4.0) / L;
    double k3 =  4.0 * cos((3.0 * start.t + goal.t) / 4.0 - angle) * sin((goal.t - start.t) / 4.0) / L;
    double k4 =  4.0 * cos((start.t + 3.0 * goal.t) / 4.0 - angle) * sin((goal.t - start.t) / 4.0) / L;

    double theta_join = calcJoinTheta(start, goal, k1, k2);
    double L1 = k1 == 0 ? L * sin(0.5 * (goal.t + start.t) - angle) / sin(0.5 * (start.t - goal.t)) : calcArcLen(start.t, theta_join, k1);
    double L2 = k2 == 0 ? L * sin(0.5 * (goal.t + start.t) - angle) / sin(0.5 * (start.t - goal.t)) : calcArcLen(theta_join, goal.t, k2);

    theta_join = calcJoinTheta(start, goal, k3, k4);
    double L3 = k3 == 0 ? L * sin(0.5 * (goal.t + start.t) - angle) / sin(0.5 * (goal.t - start.t)) : calcArcLen(start.t, theta_join, k3);
    double L4 = k4 == 0 ? L * sin(0.5 * (goal.t + start.t) - angle) / sin(0.5 * (start.t - goal.t)) : calcArcLen(theta_join, goal.t, k4);

    if (L1+L2 < L3+L4 || goal.t == start.t) {
      p1.length = L1;
      p2.length = L2;
      p1.initCurv = k1;
      p2.initCurv = k2;
    }
    else {
      p1.length = L3;
      p2.length = L4;
      p1.initCurv = k3;
      p2.initCurv = k4;
    }
  }

  return type;
}

ShapeType calcInitialEstimate(const SpiralPoint& start, const SpiralPoint& goal, SpiralParameter& p1, SpiralParameter& p2) {
  
  ShapeType type = calcBiarcSolution(start, goal, p1, p2);
  p1.length += p2.length;
  return type;
}

double computeError(const SpiralPoint& start, const SpiralPoint& goal, const SpiralParameter& param) {

  double dCurv = 2.0 * (goal.t - start.t - param.initCurv * param.length) / param.length / param.length;
  SpiralPoint est = getEndPoint(param.length, dCurv, param.initCurv, start.x, start.y, start.t);
  double dx = est.x - goal.x;
  double dy = est.y - goal.y;
  double dt = 0.0;
   // est.t - goal.t;
  double error = sqrt(dx * dx + dy * dy + abs(dt));
  return error;
}

SpiralParameter solveIteratively(const SpiralPoint& start, const SpiralPoint& goal, const SpiralParameter& initParam, double num_itr) {

  double error = computeError(start, goal, initParam);
  double prevError = 1.0e9;
  double errorStep = 0.1;
  double epsilon = 1.0e-5;
  double epsError = 0.1;
  
  double estK = initParam.initCurv;
  double estL = initParam.length;

  for (size_t i = 0; i < num_itr; i++) {
    if (error < epsilon) {
      break;
    }

    std::vector<double> errors = {__DBL_MAX__, __DBL_MAX__, __DBL_MAX__, __DBL_MAX__};
    SpiralParameter stepParam;
    stepParam.length = estL;

    stepParam.initCurv = estK + errorStep; errors[0] = computeError(start, goal, stepParam);
    stepParam.initCurv = estK - errorStep; errors[1] = computeError(start, goal, stepParam);
    
    stepParam.initCurv = estK;
    stepParam.length   = estL + errorStep; errors[2] = computeError(start, goal, stepParam);

    errors[3] = *std::max_element(errors.begin(), errors.end()) + 1.0;

    if (estL > errorStep) {
      stepParam.initCurv = estK;
      stepParam.length = estL - errorStep;
      errors[3] = computeError(start, goal, stepParam);
    }
    int minErrorsIndex = std::min_element(errors.begin(),errors.end()) - errors.begin();
    error = errors[minErrorsIndex];

    if (error > prevError) {
      errorStep *= 0.5;
    }
    else if (minErrorsIndex == 0) {
      estK += errorStep;
    }
    else if (minErrorsIndex == 1) {
      estK -= errorStep;
    }
    else if (minErrorsIndex == 2) {
      estL += errorStep;
    }
    else if (estL > errorStep) {
      estL -= errorStep;
    }
    prevError = error;
    // std::cout << "## est erro l k " << error << " " << estL << " " << estK << std::endl;
  }

  SpiralParameter res;
  res.length = estL;
  res.initCurv = estK;

  return res;
}

SpiralParameter getParameter(const SpiralPoint& start, const SpiralPoint& goal, SpiralParameter* init) {

  SpiralParameter est, p2;
  ShapeType stype = calcInitialEstimate(start, goal, est, p2);
  est.dCurv = 0.0;
  if (init)
    *init = est;
  
  if (stype == BIARC) {
    est = solveIteratively(start, goal, est, 100000);

    est.dCurv = 2.0 * (goal.t - start.t - (est.initCurv * est.length)) / est.length / est.length;
  }

  return est;
}

} // namespace es