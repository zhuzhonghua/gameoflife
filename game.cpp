#include "stdafx.h"
#include "game.h"
#include "util.h"

Game::Game()
{
  _width = 960;
  _height = 640;

  _column = 96;
  _row = 64;

  _wSize = 10;
  _hSize = 10;

  _map.resize(_column);
  SimpleRand rnd(0, 1);
  for(int i = 0; i < _column; i++){
    _map[i].resize(_row);

    for(int j=0;j<_row;j++){
      _map[i][j] = rnd.getIntRnd();
    }
  }
  
  _gameExit = false;
}

SDL_Texture* Game::createSolid()
{
  Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  SDL_Surface* img = SDL_CreateRGBSurface(0, 2, 1, 32, rmask, gmask, bmask, amask);

  r1.x = 0;
  r1.y = 0;
  r1.w = 1;
  r1.h = 1;  
  SDL_FillRect(img, &r1, SDL_MapRGB ( img->format, 255, 255, 255 ));

  r2 = r1;
  r2.x = 1;
  SDL_FillRect(img, &r2, SDL_MapRGB ( img->format, 0, 0, 0 ));
  
  SDL_Texture* tex = SDL_CreateTextureFromSurface(_renderer, img);
  
  SDL_FreeSurface(img);

  return tex;
}

void Game::init()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  _window = SDL_CreateWindow( "Game Of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
  _renderer = SDL_CreateRenderer(_window, -1, 0);

  //_texture = IMG_LoadTexture(_renderer, "test.jpg");
  _texture = createSolid();
  _fpsLimiter.setMaxFPS(60.0f);
}

void Game::run()
{
  while(_gameExit == false){
    _fpsLimiter.begin();

    SDL_RenderClear(_renderer);
    
    processInput();
    update();
  
    SDL_RenderPresent(_renderer);
    
    _fps = _fpsLimiter.end();
  }

  SDL_DestroyWindow(_window);
  SDL_DestroyRenderer(_renderer);
  SDL_Quit();
}

void Game::update()
{
  
  for(int i = 0; i < _column; i++){
    for(int j=0;j<_row;j++){
      int v = _map[i][j];
      SDL_Rect dst;
      dst.x = i*_wSize;
      dst.y = j*_hSize;
      dst.w = _wSize;
      dst.h = _hSize;
      
      if(v==0){
  	SDL_RenderCopy(_renderer, _texture, &r1, &dst);
      }
      else{
  	SDL_RenderCopy(_renderer, _texture, &r2, &dst);
      }
    }
  }
}

void Game::processInput()
{
  SDL_Event e;
  while(SDL_PollEvent(&e) != 0){
    switch(e.type){
    case SDL_QUIT:
      _gameExit = true;
      break;
    }
  }
}
