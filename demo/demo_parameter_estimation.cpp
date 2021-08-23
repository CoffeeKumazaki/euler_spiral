#include <stdafx.hpp>
#include <euler_spiral.hpp>

int main(int argc, char const *argv[])
{
  es::SpiralPoint starts[] = {
    {0, 0, 0},
    {30, 30, M_PI_4},
    {10, 10, 0},
    {20, 5, -M_PI_2},
  };
  es::SpiralPoint goals[] = {
    {15, 6, M_PI_2},
    {10, 35, 3.0*M_PI_4},
    {10, 38, 0},
    {30, 15, -M_PI},
  };

  std::ofstream fout("demo.dat");
  for (size_t i = 0; i < sizeof(starts)/sizeof(es::SpiralPoint); i++)
  {
    es::SpiralPoint start = starts[i], goal = goals[i];

    es::SpiralParameter p = es::getParameter(start, goal);

    for (double s = 0.0; s <= p.length; s += p.length/100.0) {
      es::SpiralPoint pos;
      pos = es::getEndPoint(s, p.dCurv, p.initCurv, start.x, start.y, start.t);
      fout << pos.x << " " << pos.y << std::endl;
    }

    fout  << std::endl;

  }

  return 0;
}
