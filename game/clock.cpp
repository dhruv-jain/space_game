#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"


Clock& Clock::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  started(false), 
  paused(false), 
  sloMo(false), 
  framesAreCapped(Gamedata::getInstance().getXmlBool("framesAreCapped")), 
  frameCap(Gamedata::getInstance().getXmlInt("frameCap")), 
  frames(0), 
  recentFrames(),
  maxFramesToAvg(100),
  totalTicks(0),
  sumOfTicks(0),
  startTime(0), pauseTime(0),
  currTicks(0), prevTicks(0), ticks(0),
  pos( Gamedata::getInstance().getXmlInt("clock/locX"),
       Gamedata::getInstance().getXmlInt("clock/locY"))
  {
  start();
}

void Clock::update() { 
  if ( !paused ) {
    ++frames; 
    if ( recentFrames.size() >= maxFramesToAvg ) { 
      totalTicks -= recentFrames.front();
      recentFrames.pop_front(); 
    }
    recentFrames.push_back( ticks );
    totalTicks += ticks;
  }
 }

 unsigned int Clock::getTicksSinceLastFrame() { 
  if (paused) return 0;
  else if ( sloMo ) return 1;

  currTicks = getTicks();
  ticks = currTicks-prevTicks;
  unsigned int delay = capFrameRate();
  prevTicks = currTicks + delay;
  ticks += delay;
  sumOfTicks += ticks;
  return ticks;
}

void Clock::toggleSloMo() {
  if( started && !sloMo ) {
    pauseTime = SDL_GetTicks() - startTime;
    sloMo = true;
  }
  else if ( started && sloMo ) {
    startTime = SDL_GetTicks() - pauseTime;
    sloMo = false;
  }
}

int Clock::getFps() const { 
  if ( getSeconds() > 0 ) return frames/getSeconds();  
  else if ( getTicks() > 5000 and getFrames() == 0 ) {
    throw std::string("Can't getFps if you don't increment the frames");
  }
  else return 0;
}

unsigned int Clock::getTicks() const { 
  if (paused) return pauseTime;
  else if ( sloMo ) return 1;
  else return SDL_GetTicks() - startTime; 
}



unsigned int Clock::capFrameRate() const { 
  if ( !framesAreCapped ) return 0;
  unsigned int delay = std::max(0.0,1000.0/frameCap+0.5 - ticks);
  SDL_Delay( delay );
  return delay;
}


int Clock::getAvgFps() const { 
  if ( getSeconds() > 0 && recentFrames.size() > maxFramesToAvg/2) {
    return recentFrames.size()/(totalTicks/1000.0);  
  }
  if ( getTicks() > 5000 and getFrames() == 0 ) {
    throw std::string("Can't getFps if you don't increment the frames");
  }
  return 0;
}

void Clock::start() { 
  started = true; 
  paused = false; 
  frames = 0;
  pauseTime = startTime = SDL_GetTicks(); 
}
void Clock::pause() {
  if( started && !paused ) {
    pauseTime = SDL_GetTicks() - startTime;
    paused = true;
  }
}
void Clock::unpause() {
  if( started && paused ) {
    startTime = SDL_GetTicks() - pauseTime;
    paused = false;
  }
}

