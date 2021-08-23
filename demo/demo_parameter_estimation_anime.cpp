#include <stdafx.hpp>
#include <euler_spiral.hpp>

int main(int argc, char const *argv[])
{
  es::SpiralPoint start, goal;
  start.x = start.y = 0.0;
  start.t = M_PI_4;
  goal.t = -M_PI_2;

  goal.x = 15;
  goal.y = 8;

  es::SpiralParameter p1, p2;
  es::calcBiarcSolution(start, goal, p1, p2);

  double x2, y2, t2;

  std::ofstream fout("demo_anime.dat");

  for (double s = 0.0; s <= p1.length; s += p1.length/100.0) {
    es::SpiralPoint pos;
    pos = es::getEndPoint(s, p1.dCurv, p1.initCurv, start.x, start.y, start.t);
    fout << pos.x << " " << pos.y << std::endl;
    x2 = pos.x;
    y2 = pos.y;
    t2 = pos.t;
  }
  for (double s = 0.0; s <= p2.length; s += p2.length / 100.0)
  {
    es::SpiralPoint pos;
    pos = es::getEndPoint(s, p2.dCurv, p2.initCurv, x2, y2, t2);
    fout << pos.x << " " << pos.y << std::endl;
  }
  fout  << std::endl;

  for (size_t i = 0; i < 100; i+=1) {
    es::SpiralParameter p = es::getParameter(start, goal, nullptr, i);

    for (double s = 0.0; s <= p.length; s += p.length/100.0) {
      es::SpiralPoint pos, posi;
      pos = es::getEndPoint(s, p.dCurv, p.initCurv, start.x, start.y, start.t);
      fout << pos.x << " " << pos.y << std::endl;
    }

    fout  << std::endl;

  }
  

  return 0;
}
