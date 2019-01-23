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
  for(int x = 0; x < _column; x++){
    _map[x].resize(_row);

    for(int y=0;y<_row;y++){
      _map[x][y] = rnd.getIntRnd();
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
  //_fpsLimiter.setMaxFPS(60.0f);
  _fpsLimiter.setMaxFPS(2);
}

void Game::run()
{
  SDL_StartTextInput();	
  while(_gameExit == false){
    _fpsLimiter.begin();

    SDL_RenderClear(_renderer);
    
    _inputMgr.update();
    
    processInput();
    update();
    draw();
    
    SDL_RenderPresent(_renderer);
    
    _fps = _fpsLimiter.end();
  }
  SDL_StopTextInput();

  SDL_DestroyWindow(_window);
  SDL_DestroyRenderer(_renderer);
  SDL_Quit();
}

void Game::draw()
{
  for(int x = 0; x < _column; x++){
    for(int y=0;y<_row;y++){
      int v = _map[x][y];
      SDL_Rect dst;
      dst.x = x*_wSize;
      dst.y = y*_hSize;
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

void Game::update()
{  
  for(int x = 0; x < _column; x++){
    for(int y=0;y<_row;y++){
      int v = _map[x][y];
      int alive = getAliveAround(_map, x, y, _column, _row);
      // alive
      if(v == 1){
  	if(alive < 2){
  	  _map[x][y] = 0;
  	}
  	else if(alive == 2 || alive == 3){
  	  // do nothing
  	}
  	else if(alive > 3){
  	  _map[x][y]=0;
  	}
      }
      else if(v==0){
  	if(alive==3){
  	  _map[x][y]=1;
  	}
      }
    }
  }

  // click to set cell dead
  if(_inputMgr.isKeyDown(SDL_BUTTON_LEFT)){
    Point p = _inputMgr.getMouseCoords();
    int x = p.x / _wSize;
    int y = p.y / _hSize;

    _map[x][y] = 0;
  }
}

int Game::getAliveAround(std::vector<std::vector<int> > map, int x, int y, int width, int height)
{
  int alive=0;
  // up
  if(y-1>=0 && map[x][y-1]==1){
    alive++;
  }
  // down
  if(y+1<height && map[x][y+1]==1){
    alive++;
  }
  // left
  if(x-1>=0 && map[x-1][y]==1){
    alive++;
  }
  // right
  if(x+1<width && map[x+1][y]==1){
    alive++;
  }
  // up left
  if(y-1>=0 && x-1>=0&& map[x-1][y-1]==1){
    alive++;
  }
  // up right
  if(y-1>=0 && x+1<width && map[x+1][y-1]==1){
    alive++;
  }
  // down left
  if(y+1<height && x-1>=0 && map[x-1][y+1]==1){
    alive++;
  }
  // down right
  if(y+1<height && x+1<width && map[x+1][y+1]==1){
    alive++;
  }
  return alive;
}

void Game::processInput()
{
  SDL_Event e;
  while(SDL_PollEvent(&e) != 0){
    switch(e.type){
    case SDL_QUIT:
      _gameExit = true;
      break;
    case SDL_MOUSEMOTION:
      _inputMgr.setMouseCoords(e.motion.x, e.motion.y);
      break;
    case SDL_KEYDOWN:
      _inputMgr.pressKey(e.key.keysym.sym);
      break;
    case SDL_KEYUP:
      _inputMgr.releaseKey(e.key.keysym.sym);
      break;
    case SDL_MOUSEBUTTONDOWN:
      _inputMgr.pressKey(e.button.button);
      break;
    case SDL_MOUSEBUTTONUP:
      _inputMgr.releaseKey(e.button.button);
      break;
    }
  }
}
