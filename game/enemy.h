#ifndef ENEMY__H
#define ENEMY__H
#include <string>
#include <vector>
#include "sprite.h"
#include "ioManager.h"
#include "player.h"

class Player;

class Enemy : public Sprite {
public:
  Enemy(const std::string&, const Player& p);
  Enemy(const Enemy&);
  virtual ~Enemy() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void goLeft();
  void goRight();
  void goUp();
  void goDown();

  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }

 void incrSafeDistance() {++safeDistance; }
 void decrSafeDistance() { --safeDistance; }
 float getSafeDistance() { return safeDistance; }

protected:
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  const Player & player;

  enum MODE {NORMAL, EVADE};
  MODE currentMode;

  float safeDistance;


  void advanceFrame(Uint32 ticks);
};
#endif
