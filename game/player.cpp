#include <iostream>
#include <cmath>
#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"

/**/
CollisionStrategy* selectStrategy(const string& name) {
  std::string sName = Gamedata::getInstance().getXmlStr(name+"/strategy");
  if( sName == "midpoint") return new MidPointCollisionStrategy;
  if( sName == "rectangular") return new RectangularCollisionStrategy;
  if( sName == "perpixel") return new PerPixelCollisionStrategy;
  throw std::string("No strategy in getStrategy");
}
/**/
void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Player::Player( const std::string& name) :
  TwoWay(name),
  keyPressedX(false),
  keyPressedY(false),
  initialVelocity(350,350),
  slowDown(Gamedata::getInstance().getXmlFloat(name+"/slowDown")),
  bulletName("bullet"),
  bulletInterval(Gamedata::getInstance().getXmlFloat(bulletName+"/bulletinterval")),
  timeSinceLastBullet(0),
  minBulletSpeed(Gamedata::getInstance().getXmlFloat(bulletName+"/minbulletspeed")),
  bullets(bulletName),
  strategy( selectStrategy(name)) 
{ }

Player::Player(const Player& s) :
  TwoWay(s.getName()),
  keyPressedX(s.keyPressedX),
  keyPressedY(s.keyPressedY),
  initialVelocity(s.initialVelocity),
  slowDown(s.slowDown),
  bulletName(s.bulletName),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(s.timeSinceLastBullet),
  minBulletSpeed(s.minBulletSpeed),
  bullets(s.bullets),
  strategy(s.strategy)
  { }

void Player::stop() {
  if (!keyPressedX) velocityX( slowDown*velocityX() );
  if (!keyPressedY) velocityY( slowDown*velocityY() );
}

void Player::right() {
  keyPressedX = true;
  if (X() < worldWidth-frameWidth) {
    velocityX(initialVelocity[0]);
  }
}

void Player::left() {
  keyPressedX = true;
  if (X() > 0) {
    velocityX(-initialVelocity[0]);
  }
}

void Player::up() {
keyPressedY = true;
if ( Y() > 0 )
  velocityY(-initialVelocity[0]);
}

void Player::down() {
keyPressedY = true;
if (Y() < worldHeight-frameHeight )
  velocityY(initialVelocity[0]);
}

bool Player::collidedWith(const Sprite* obj) const {
  return bullets.collidedWith(obj);
 }


 bool Player::destinyCollidedWith(const Sprite* obj) const {
    if ( strategy->execute(*this,*obj)) {
      return true;
    }
  return false;
}

void Player::shoot(){
  if (velocityX() > 0) { frames = rightFrames;}
  else { frames=leftFrames;}

  if (timeSinceLastBullet > bulletInterval) {
  Vector2f vel =getVelocity();
  float x;
  float y = Y()+frameHeight/4;
  if (vel[0] > 0) {
  x = X()+ frameWidth;
  vel[0] += minBulletSpeed;    
  }
  else {
  x=X();
  vel[0] -= minBulletSpeed;
  }
  std::cout <<"computed vel:" << vel << std::endl;
  bullets.shoot(Vector2f(x,y),vel);
  timeSinceLastBullet = 0;

  }
}

void Player::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x,y);
  bullets.draw();
}

void Player::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() <= 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() >= worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }
  //changing directions for the player
  if ( velocityX() > 0) {
    frames = rightFrames;
  }
  if ( velocityX() < 0) { 
    frames = leftFrames;
  } 
  if (!keyPressedX ) {
   stop();
  }
  keyPressedX = false;

   if (!keyPressedY ) {
   stop();
  }
  keyPressedY = false;

  timeSinceLastBullet+=ticks;
  bullets.update(ticks);

}