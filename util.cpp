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
