#include "stdafx.h"
#include "util.h"

SimpleRand::SimpleRand(int min, int max)
  :_rndInt(min, max)
{
}

int SimpleRand::getIntRnd()
{
  return _rndInt(_randEngine);
}

Point::Point()
{
  init(0,0);
}

void Point::init(float xp, float yp)
{
  x=xp,y=yp;
}
