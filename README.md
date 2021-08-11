# Euler Spiral

An Euler spiral (clothoid) is a curve whose curvature changes linearly with its curve length.

This libraly provides two functions. 
- calculate the endpoint of Euler spiral at given length with given curveture deviation.

  ```cpp
  struct SpiralPoint {
    double x, y, t;
  };

  SpiralPoint getEndPoint(double length, double dCurv, double initCurv = 0.0, double initX = 0.0, double initY = 0.0, double initTheta = 0.0);
  ```
- estimate suitable parameters for Euler spiral, `length`, `initial curvature` and `curvature deviation` from given two points and directions at them.

  ```cpp
  struct SpiralParameter {
    double length;
    double initCurv;
    double dCurv;
  };

  SpiralParameter getParameter(const SpiralPoint& start, const SpiralPoint& goal, SpiralParameter* init = NULL);
  ```

## build & run demo

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## reference

- *Euler Spiral for Shape Completion.*  
  Benjamin B. Kimia, Ilana Frankel, and Ana-Maria Popescu. 2003.  
  Int. J. Comput. Vision 54, 1–3 (August-September 2003), 157–180.  
  DOI:https://doi.org/10.1023/A:1023713602895