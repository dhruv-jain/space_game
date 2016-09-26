#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include <iostream>
#include <vector>
#include "twoway.h"
#include "multibullets.h"


class Player : public TwoWay {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual ~Player() { }
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void stop();
  void right();
  void left();
  void up();
  void down();
  void shoot();
  bool collidedWith(const Sprite* obj) const;
  bool destinyCollidedWith(const Sprite* obj)const;

  unsigned int bulletCount() const {return bullets.bulletCount();}
  unsigned int freeCount() const { return bullets.freeCount();}

protected:
  bool keyPressedX;
  bool keyPressedY;

  Vector2f initialVelocity;
  const float slowDown;

  std::string bulletName;
  float bulletInterval;
  float timeSinceLastBullet;
  float minBulletSpeed;
  MultiBullets bullets;

  Player& operator=(const Player&);
  void advanceFrame(Uint32);
  CollisionStrategy* strategy;
};
#endif