#include <iostream>
#include <cmath>
#include "twoway.h"
#include "gamedata.h"
#include "frameFactory.h"

void TwoWay::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if(timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

TwoWay::TwoWay(const std::string& name):
  // Drawable(name, 
  //   Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
  //            Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
  //   Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), Gamedata::getInstance().getXmlInt(name+"/speedY"))
  // ),
 Sprite(name, 
    Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
             Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
    Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), Gamedata::getInstance().getXmlInt(name+"/speedY"))
  ),
  rightFrames( FrameFactory::getInstance().getFrames(name) ),
  leftFrames( FrameFactory::getInstance().getLeftFrames(name)),
  frames( rightFrames ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  //dt(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames")),
  frameInterval(Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame(0),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
  //spriteLeft(false)
{ }

TwoWay::TwoWay(const TwoWay& s) :
  // Drawable(s.getName(), s.getPosition(),s.getVelocity()),
  Sprite(s.getName(), s.getPosition(),s.getVelocity()), 
  rightFrames(s.rightFrames),
  leftFrames(s.leftFrames),
  frames(s.frames),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight),
  //dt(s.dt),
  currentFrame(s.currentFrame),
  numberOfFrames(s.numberOfFrames),
  frameInterval(s.frameInterval),
  timeSinceLastFrame(s.timeSinceLastFrame),
  frameWidth(s.frameWidth),
  frameHeight(s.frameHeight)
  //spriteLeft(s.spriteLeft)
{ }


void TwoWay::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x,y);
  // if (spriteLeft)
  //   leftFrames[currentFrame]->draw(x,y);
  // else
  //   frames[currentFrame]->draw(x,y);
}

void TwoWay::update(Uint32 ticks) { 
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
    // spriteLeft=0;
    frames = rightFrames;
  }

  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
    //spriteLeft =1;
    frames = leftFrames;
  }  
  //TO CHANGE THE DIRECTION OF VELOCITY WHEN IT COLLIDES WITH WALL
  // if(velocityX() > 0 && spriteLeft == 1) {
  //   velocityX( -abs( velocityX() ) );
  // }
  // if( velocityX() < 0 && spriteLeft == 0) {
  //   velocityX( abs( velocityX() ) );
  // }
}
