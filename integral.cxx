#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// user defined function below
float f (float x){
  return exp(cos(x));
}

//function to calculate a definite integral given bounds of integration (xmin/max) & bounds of function (ymin/ymax)
float integral (float (*f)(float), float xmin, float xmax, float ymin, float ymax)
{
  int total = 0, inBox = 0;
  for (int count=0; count < 1000000; ++count)
  {
    float u1 = (float)rand() / RAND_MAX;
    float u2 = (float)rand() / RAND_MAX;

    float xcoord = ((xmax - xmin)*u1) + xmin;
    float ycoord = ((ymax - ymin)*u2) + ymin;
    float val = f(xcoord);

    ++total;

    if (val > ycoord) ++inBox;
  }

  float density = (float)inBox/total;

  std::cout << (xmax - xmin) * (ymax - ymin) * density << std::endl;
}

int main(int argc, char* argv[])
{
  if (argc < 3) {
    if (argc > 1) std::cout << argv[1] << " :1 \n";
    if (argc > 2) std::cout << argv[2] << " :2 \n";
    std::cout << "USAGE: integral <xmin> <xmax>" << "\n";
    return 1;
  }
  float xmin = atof(argv[1]);
  float xmax = atof(argv[2]);
  integral(f, xmin, xmax, 0, 4);
}