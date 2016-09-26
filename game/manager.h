#include <list>
#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "scaledSprite.h"
#include "player.h"
#include "multisprite.h"
#include "explodingSprite.h"
#include "health.h"
#include "sound.h"

class Manager {
public:
  Manager (); 
  ~Manager ();
  int play();
  void switchSprite();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  SDL_Surface * const orbSurface;
  Health bar;
  World world;
  World stars;
  World sun;
  World planets;
  Viewport& viewport;
  int currentSprite;

  int timer;
  Hud hud;
  bool drawHud;
  bool gameOver;
  bool gameWin;
  
  std::vector<ScaledSprite*> orbs;
  std::vector<Drawable*> sprites;
  Player* destiny;
  std::list<Sprite*> eship;

  SDLSound sound;
  bool godmode;
  std::list<ExplodingSprite*> explosionsprites;
  bool shipexploded;
  bool destinyexploded;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;

  void draw() const;
  void update();

  void makeOrbs();
  void printOrbs() const;
  void makeEnemies();
  
  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
