#include <stdafx.hpp>
#include <euler_spiral.hpp>

int main(int argc, char const *argv[])
{
  es::SpiralPoint start, goal;
  start.x = start.y = 0.0;
  start.t = 0.0;
  goal.t = 0.0;
  for (double x = 5; x <= 14; x += 3) {
    goal.x = x;
    for (double y = -5; y <= 5; y += 2) {
      goal.y = y;
      std::cout << "# x:y = " << x << " " << y << std::endl;

      es::SpiralParameter p1, p2;
      es::calcBiarcSolution(start, goal, p1, p2);
      std::cout << "# p1 " << p1.length << " " << p1.initCurv << std::endl;
      std::cout << "# p2 " << p2.length << " " << p2.initCurv << std::endl;

      double x2, y2, t2;

      for (double s = 0.0; s <= p1.length; s += p1.length/100.0) {
        es::SpiralPoint pos;
        pos = es::getEndPoint(s, p1.dCurv, p1.initCurv, start.x, start.y, start.t);
        std::cout << pos.x << " " << pos.y << std::endl;
        x2 = pos.x;
        y2 = pos.y;
        t2 = pos.t;
      }
      for (double s = 0.0; s <= p2.length; s += p2.length / 100.0)
      {
        es::SpiralPoint pos;
        pos = es::getEndPoint(s, p2.dCurv, p2.initCurv, x2, y2, t2);
        std::cout << pos.x << " " << pos.y << std::endl;
      }
      std::cout << std::endl
                << std::endl;
    }
  }

  return 0;
}
