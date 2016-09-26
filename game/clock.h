#ifndef CLOCK__H
#define CLOCK__H
#include <SDL.h>
#include <string>
#include <deque>
#include "vector2f.h"

class Manager;
class Clock {
public:
  static Clock& getInstance();
  unsigned int getTicks() const;
  unsigned int getTotalTicks() const { return sumOfTicks; }
  int getFps() const;
  unsigned int getSeconds() const { return getTicks()/1000;  }

private:
 friend class Manager;
  bool started;
  bool paused;
  bool sloMo;
  const bool framesAreCapped;
  const int frameCap;

  unsigned int frames;
  std::deque<int> recentFrames;
  const unsigned int maxFramesToAvg;

  unsigned int totalTicks;
  unsigned int sumOfTicks;
  unsigned int startTime;
  unsigned int pauseTime;
  unsigned int currTicks;
  unsigned int prevTicks;
  unsigned int ticks;
  Vector2f pos;
  unsigned int getTicksSinceLastFrame();
  void update();
  void toggleSloMo();

  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  unsigned int getFrames() const  { return frames;  }
  unsigned int capFrameRate() const;
  int getAvgFps() const;
  void start();
  void pause();
  void unpause();
  void display() const;

  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
};
#endif