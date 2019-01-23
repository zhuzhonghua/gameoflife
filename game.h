#pragma once

#include "timing.h"

class Game{
public:
  Game();
  void init();
  void run();

protected:
  void update();
  void processInput();

  SDL_Texture* createSolid();
  
protected:
  bool _gameExit;
  FPSLimiter _fpsLimiter;

  float _fps;
protected:
  SDL_Window* _window;
  SDL_Renderer* _renderer;

  SDL_Texture* _texture;
  SDL_Rect r1;
  SDL_Rect r2;
  
  std::vector<std::vector<int> > _map;
  int _width;
  int _height;
  int _column;
  int _row;
  int _wSize;
  int _hSize;
};
