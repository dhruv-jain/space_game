#include <cmath>
#include "enemy.h"
#include "viewport.h"
#include "gamedata.h"
#include "frameFactory.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

void Enemy::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

Enemy::Enemy( const std::string& name, const Player& p) :
  Sprite(name),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  player(p),
  currentMode(NORMAL),
  safeDistance ( Gamedata::getInstance().getXmlFloat("safeDistance") )
{ }

Enemy::Enemy(const Enemy& s) :
  Sprite(s), 
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  player(s.player),
  currentMode(s.currentMode),
  safeDistance(s.safeDistance)
  { }

void Enemy::goLeft() { 
  if (X() > 0) velocityX( -abs(velocityX()) ); 
}

void Enemy::goRight() { 
  velocityX( fabs(velocityX()) ); 
}

void Enemy::goUp() { 
  velocityY( -fabs(velocityY()) ); 
}

void Enemy::goDown() { 
  velocityY( fabs(velocityY()) ); 
}

void Enemy::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x,y);
  Sprite::draw();
}

void Enemy::update(Uint32 ticks) { 
  Sprite::update(ticks);

  float x= X()+getFrame()->getWidth()/2;
  float y= Y()+getFrame()->getHeight()/2;
  float ex= player.X()+player.getFrame()->getWidth()/2;
  float ey= player.Y()+player.getFrame()->getHeight()/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );

  if  ( currentMode == NORMAL ) {
    setVelocity(Vector2f(0,0));

    if(distanceToEnemy < safeDistance) currentMode = EVADE;
  }
  else if  ( currentMode == EVADE ) {
    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else {
      setVelocity(Vector2f(80,80));
      if ( x < ex ) goRight();
      if ( x > ex ) goLeft();
      if ( y < ey ) goDown();
      if ( y > ey ) goUp();
    }
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  

}
