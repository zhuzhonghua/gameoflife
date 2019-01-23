#pragma once

class SimpleRand{
public:
  SimpleRand(int min, int max);

  int getIntRnd();
protected:
  boost::random::mt19937 _randEngine;
  boost::random::uniform_int_distribution<> _rndInt;
};

class Point{
public:
  Point();
protected:
  void init(float xp, float yp);
public:
  float x;
  float y;
};
