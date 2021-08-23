#include <stdafx.hpp>
#include <euler_spiral.hpp>

int main(int argc, char const *argv[])
{
  es::SpiralPoint start, goal;
  start.x = start.y = 0.0;
  start.t = -M_PI;
  goal.t = -M_PI/2.0;
  for (double x = 5; x <= 14; x += 3) {
    goal.x = x;
    for (double y = -5; y <= 5; y += 2) {
      goal.y = y;
      std::cout << "# x:y = " << x << " " << y << std::endl;

      es::SpiralParameter ip;
      es::SpiralParameter p = es::getParameter(start, goal, &ip);

      std::cout << "# iparam " <<  ip.length << " " << ip.dCurv << " " << ip.initCurv << std::endl;
      std::cout << "# param  " <<  p.length << " " << p.dCurv << " " << p.initCurv << std::endl;

      for (double s = 0.0; s <= p.length; s += p.length/100.0) {
        es::SpiralPoint pos, posi;
        posi = es::getEndPoint(s, 0.0, ip.initCurv, start.x, start.y, start.t);
        pos = es::getEndPoint(s, p.dCurv, p.initCurv, start.x, start.y, start.t);
        std::cout << pos.x << " " << pos.y << " " << posi.x << " " << posi.y << std::endl;
      }
      std::cout << std::endl
                << std::endl;
    }
  }

  return 0;
}
