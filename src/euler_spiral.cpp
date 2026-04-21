#include <stdafx.hpp>
#include <euler_spiral.hpp>
#include <fresnel_integral.hpp>
#include <limits>

namespace es
{

static constexpr double kEpsilon = 1.0e-10;
static constexpr double kConvergenceThreshold = 1.0e-5;
static constexpr double kInitialStepSize = 0.1;
static constexpr double kLargeError = 1.0e9;

static bool isNearZero(double v) {
  return std::fabs(v) < kEpsilon;
}

SpiralPoint getEndPointFromCurvature(double length, double curvStart, double curvEnd, double initX /*= 0.0*/, double initY /*= 0.0*/, double initTheta /*= 0.0*/) {

  if (length <= 0.0 || std::isnan(length) || std::isinf(length)) {
    SpiralPoint pos;
    pos.x = initX;
    pos.y = initY;
    pos.t = initTheta;
    return pos;
  }

  return getEndPoint(length, (curvEnd - curvStart) / length, curvStart, initX, initY, initTheta);
}

SpiralPoint getEndPoint(double length, double dCurv, double initCurv /*= 0.0*/, double initX /*= 0.0*/, double initY /*= 0.0*/, double initTheta /*= 0.0*/) {

  if (std::isnan(length) || std::isinf(length) || std::isnan(dCurv) || std::isinf(dCurv) ||
      std::isnan(initCurv) || std::isinf(initCurv)) {
    SpiralPoint pos;
    pos.x = initX;
    pos.y = initY;
    pos.t = initTheta;
    return pos;
  }

  SpiralPoint pos;

  if (isNearZero(dCurv) && isNearZero(initCurv)) {
    // Straight line.
    pos.x = length * std::cos(initTheta);
    pos.y = length * std::sin(initTheta);
  }
  else if (isNearZero(dCurv)) {
    // Arc
    double x = std::sin(initCurv * length) / initCurv;
    double y = (1.0 - std::cos(initCurv * length) ) / initCurv;

    pos.x = x * std::cos(initTheta) - y * std::sin(initTheta);
    pos.y = x * std::sin(initTheta) + y * std::cos(initTheta);
  }
  else {
    double a = 1.0 / std::sqrt(M_PI * std::fabs(dCurv));

    double x = fresnel_cos_integral((initCurv + dCurv * length) * a) - fresnel_cos_integral(initCurv * a);
    double y = fresnel_sin_integral((initCurv + dCurv * length) * a) - fresnel_sin_integral(initCurv * a);

    double theta = initTheta - initCurv * initCurv * 0.5 / dCurv;
    if (dCurv < 0.0) x *= -1.0;
    pos.x = M_PI * a * (x * std::cos(theta) - y * std::sin(theta));
    pos.y = M_PI * a * (x * std::sin(theta) + y * std::cos(theta));

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

  double denom = kGoal - kStart;
  if (isNearZero(denom)) {
    // Equal curvatures: return angle of chord as fallback.
    return getAngleOfVector(goal.x - start.x, goal.y - start.y);
  }

  return getAngleOfVector(
      (-kStart * kGoal * (goal.y - start.y) + kGoal * std::cos(start.t) - kStart * std::cos(goal.t)) / denom,
      ( kStart * kGoal * (goal.x - start.x) + kGoal * std::sin(start.t) - kStart * std::sin(goal.t)) / denom);
}



ShapeType calcBiarcSolution(const SpiralPoint& start, const SpiralPoint& goal, SpiralParameter& p1, SpiralParameter& p2) {

  ShapeType type = INVALID;
  double dx = goal.x - start.x;
  double dy = goal.y - start.y;

  double L = std::sqrt(dx*dx + dy*dy);

  // Same point: treat as zero-length line.
  if (isNearZero(L)) {
    p1.initCurv = 0.0;
    p1.dCurv = 0.0;
    p1.length = 0.0;
    p2.initCurv = 0.0;
    p2.dCurv = 0.0;
    p2.length = 0.0;
    return LINE;
  }

  double angle = getAngleOfVector(dx, dy);
  double angle_mean = (start.t + goal.t) * 0.5;

  while(angle_mean > 2.0 * M_PI) {
    angle_mean -= 2.0 * M_PI;
  }
  while(angle_mean < - 2.0 * M_PI) {
    angle_mean += 2.0 * M_PI;
  }

  if (isNearZero(angle - angle_mean)) {
    double k1 = 0.0;
    double k2 = 0.0;
    double L1 = 0.0;
    double L2 = 0.0;
    if (isNearZero(angle - start.t)) {
      // Straight line.
      type = LINE;
      k1 = k2 = 0.0;
      L2 = L1 = 0.5 * L;
    }
    else {
      // Same Arc.
      type = ARC;
      k2 = k1 = -2.0 * std::sin(0.5 * (start.t - goal.t)) / L;
      if (isNearZero(angle_mean)) {
        L2 = L1 = 0.25 * M_PI * L;
      }
      else {
        L2 = L1 = L * (goal.t + start.t)/ 4.0 / std::sin(angle_mean);
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
    // biarc.
    type = BIARC;
    double k1 = -4.0 * std::sin((3.0 * start.t + goal.t) / 4.0 - angle) * std::cos((goal.t - start.t) / 4.0) / L;
    double k2 =  4.0 * std::sin((start.t + 3.0 * goal.t) / 4.0 - angle) * std::cos((goal.t - start.t) / 4.0) / L;
    double k3 =  4.0 * std::cos((3.0 * start.t + goal.t) / 4.0 - angle) * std::sin((goal.t - start.t) / 4.0) / L;
    double k4 =  4.0 * std::cos((start.t + 3.0 * goal.t) / 4.0 - angle) * std::sin((goal.t - start.t) / 4.0) / L;

    double theta_join = calcJoinTheta(start, goal, k1, k2);
    double L1 = isNearZero(k1) ? L * std::sin(0.5 * (goal.t + start.t) - angle) / std::sin(0.5 * (start.t - goal.t)) : calcArcLen(start.t, theta_join, k1);
    double L2 = isNearZero(k2) ? L * std::sin(0.5 * (goal.t + start.t) - angle) / std::sin(0.5 * (start.t - goal.t)) : calcArcLen(theta_join, goal.t, k2);

    theta_join = calcJoinTheta(start, goal, k3, k4);
    double L3 = isNearZero(k3) ? L * std::sin(0.5 * (goal.t + start.t) - angle) / std::sin(0.5 * (goal.t - start.t)) : calcArcLen(start.t, theta_join, k3);
    double L4 = isNearZero(k4) ? L * std::sin(0.5 * (goal.t + start.t) - angle) / std::sin(0.5 * (start.t - goal.t)) : calcArcLen(theta_join, goal.t, k4);

    if (L1+L2 < L3+L4 || isNearZero(goal.t - start.t)) {
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
  double error = std::sqrt(dx * dx + dy * dy + std::fabs(dt));
  return error;
}

SpiralParameter solveIteratively(const SpiralPoint& start, const SpiralPoint& goal, const SpiralParameter& initParam, size_t num_itr) {

  double error = computeError(start, goal, initParam);
  double prevError = kLargeError;
  double errorStep = kInitialStepSize;

  double estK = initParam.initCurv;
  double estL = initParam.length;

  for (size_t i = 0; i < num_itr; i++) {
    if (error < kConvergenceThreshold) {
      break;
    }

    std::array<double, 4> errors;
    errors.fill(std::numeric_limits<double>::max());
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
  }

  SpiralParameter res;
  res.length = estL;
  res.initCurv = estK;

  return res;
}

SpiralParameter getParameter(const SpiralPoint& start, const SpiralPoint& goal, SpiralParameter* init, size_t maxItrNum) {

  // Same point guard
  double dx = goal.x - start.x;
  double dy = goal.y - start.y;
  if (isNearZero(dx) && isNearZero(dy)) {
    SpiralParameter p;
    p.length = 0.0;
    p.initCurv = 0.0;
    p.dCurv = 0.0;
    if (init) *init = p;
    return p;
  }

  SpiralParameter est, p2;
  ShapeType stype = calcInitialEstimate(start, goal, est, p2);
  est.dCurv = 0.0;
  if (init)
    *init = est;

  if (stype == BIARC) {
    est = solveIteratively(start, goal, est, maxItrNum);

    if (!isNearZero(est.length)) {
      est.dCurv = 2.0 * (goal.t - start.t - (est.initCurv * est.length)) / est.length / est.length;
    }
  }

  return est;
}

} // namespace es
