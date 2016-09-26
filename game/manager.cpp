#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include "twoway.h"
#include "vector2f.h"
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "player.h"
#include "enemy.h"

//Function object - Painters algorithm
class ScaledSpriteCompare {
public:
  bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs) {
    return lhs->getScale() < rhs->getScale();
  }
};

Manager::~Manager() { 
  std::vector<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    delete (*ptr);
    ++ptr;
  }

 SDL_FreeSurface(orbSurface);
  for (unsigned i = 0; i < orbs.size(); ++i) {
    delete orbs[i];
  }
  orbs.clear();
  delete destiny;
  ExplodingSprite* temp1;
  std::list<ExplodingSprite*>::iterator ptr1 = explosionsprites.begin();
  while (ptr1 != explosionsprites.end()) {
    temp1=(*ptr1);
    ++ptr1;
    delete temp1;
  }

  Sprite* temp2;
  std::list<Sprite*>::iterator ptr2 = eship.begin();
  while (ptr2 != eship.end()) {
    temp2=(*ptr2);
    ++ptr2;
    delete temp2;
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  orbSurface( io.loadAndSet(Gamedata::getInstance().getXmlStr("asteroid/file"), Gamedata::getInstance().getXmlBool("asteroid/transparency")) ),
  bar(),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  stars("stars",Gamedata::getInstance().getXmlInt("stars/factor") ),
  sun("sun", Gamedata::getInstance().getXmlInt("sun/factor") ),
  planets("planets",Gamedata::getInstance().getXmlInt("planets/factor") ),
  viewport( Viewport::getInstance() ),
  currentSprite(0),
  timer(0),
  hud(),
  drawHud(Gamedata::getInstance().getXmlBool("hud/ShowAtStart") ),
  gameOver(Gamedata::getInstance().getXmlBool("hudgame/ShowAtStart") ),
  gameWin(false),
  orbs(),
  sprites(),
  destiny(new Player("destiny")),
  eship(),
  sound(),
  godmode(false),
  explosionsprites(),
  shipexploded(false),
  destinyexploded(false),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") )
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);  

  //calling the orb functions - Painters algorithm
  makeOrbs();
  printOrbs();
  makeEnemies();

  std::vector<Drawable*>::iterator ptr = sprites.begin();
  for (int i=0; i<currentSprite; i++)
    ++ptr;
  viewport.setObjectToTrack(destiny);
}

void Manager::draw() const {
  world.draw();
  stars.draw();
  sun.draw();
  planets.draw();
  bar.draw();

  //draw orbs - Painters algorithm 
  for (unsigned i = 0; i < 15; ++i) {
    orbs[i]->draw();
  }
  std::vector<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->draw();
    ++ptr;
  }
  
  if(!gameOver) {
  destiny->draw();
  } 

 //Exploding sprite
 std::list<ExplodingSprite*>::const_iterator expIt = explosionsprites.begin();
  while( expIt != explosionsprites.end()) {
    (*expIt)->draw();
    ++expIt;
  }
  //Asteroids 
  for (unsigned i = 15; i < 30; ++i) {
    orbs[i]->draw();
  }

  std::list<Sprite*>::const_iterator eship_itr = eship.begin();
  while( eship_itr != eship.end() ) {
    (*eship_itr)->draw();
    ++eship_itr;
  }

  io.printMessageAt(title, 10, 450);
  viewport.draw();
  io.printMessageAt("Press F1 to toggle HUD", 15, 10);

  if ( drawHud ) 
  hud.draw(destiny->bulletCount(), destiny->freeCount());

  if (gameOver) {
    hud.game();
  }

  if (gameWin) {
    hud.gamewin();
  }

  SDL_Flip(screen);
}

// Move this to IOManager
void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
  currentSprite = ( currentSprite + 1 ) % sprites.size();
  std::vector<Drawable*>::iterator ptr = sprites.begin();
  for (int i = 0; i < currentSprite; i++)
    ++ptr;
  viewport.setObjectToTrack(*ptr);
}

void Manager::update() {
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();

  std::vector<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->update(ticks);
    ++ptr;
  }

  std::list<ExplodingSprite*>::iterator expIt = explosionsprites.begin();
  while( expIt != explosionsprites.end()) {
    (*expIt)->update(ticks);
    if ( (*expIt)->done()) {
     delete *expIt;
     expIt = explosionsprites.erase(expIt);
     shipexploded = false;

    }
    else ++expIt;
  }
 
  if(!gameOver) {
  destiny->update(ticks);
  }

  //updating enemy ship list
  std::list<Sprite*>::const_iterator eship_itr = eship.begin();
  while( eship_itr != eship.end() ) {
    (*eship_itr)->update(ticks);
    ++eship_itr;
  }
  
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }

   if ( (timer = clock.getSeconds() ) == 3 ) {
    drawHud = false;
  }

  //Collision bw bullet and enemy list
  std::list <Sprite*>::iterator eship_exp_itr = eship.begin();
  while( eship_exp_itr != eship.end() ) {
    if ( destiny->collidedWith(*eship_exp_itr) ) {
      explosionsprites.push_back(new ExplodingSprite(**eship_exp_itr));
      eship_exp_itr = eship.erase(eship_exp_itr);
    }
    ++eship_exp_itr;
  }

  //Collision bw destiny and enemy list
  std::list <Sprite*>::iterator eship_exp_itr1 = eship.begin();
  while( eship_exp_itr1 != eship.end() ) {
  if ( destiny-> destinyCollidedWith( *eship_exp_itr1 ) ) {
    explosionsprites.push_back( new ExplodingSprite(**eship_exp_itr1));
    if( !godmode){
          destinyexploded = true;
              bar.update();


    }

    sound[1];
    eship_exp_itr1 = eship.erase(eship_exp_itr1);
  }
  ++eship_exp_itr1;
}

  if ( bar.isEmpty() == true ) {
    if( gameOver == false ) {
      gameOver = true;
    explosionsprites.push_back( new ExplodingSprite(*destiny));
    }
  }

//Enemy list - check if the enemy list is empty
    if ( eship.empty()) {
      if ( gameWin == false ) {
        gameWin = true;
      }
    }

  //Update orbs - Painters algorithm
  for (unsigned i = 0; i < orbs.size(); ++i) {
    orbs[i]->update(ticks);
  }
  world.update();
  stars.update();
  sun.update();
  planets.update();
  viewport.update(); // always update viewport last
}

void Manager::makeOrbs() {
  unsigned numberOfOrbs = ( Gamedata::getInstance().getXmlInt("numberOfOrbs") );
  orbs.reserve( numberOfOrbs );
  srand(time(NULL));
  for (unsigned i = 0; i < numberOfOrbs; ++i) {
     ScaledSprite *s1 = new ScaledSprite("asteroid", orbSurface);
     s1->setVelocity(Vector2f(50, rand()%250 + 100));
     s1->setPosition(Vector2f(rand()%1900 + 100, rand()%800 + 100));
    orbs.push_back(s1);
  }
  sort(orbs.begin(), orbs.end(), ScaledSpriteCompare());
}

void Manager::makeEnemies() {
  unsigned numberofEnemies = ( Gamedata::getInstance().getXmlInt("numberOfEnemies") );
  srand(time(NULL));
  for (unsigned i = 0; i < numberofEnemies; ++i) {
    Enemy* e = new Enemy("enemyship",*destiny);
    e->setPosition (Vector2f(350 * (i+1), rand()%300 + 80) );
    eship.push_back( e );
  }
}

void Manager::printOrbs() const {
  for (unsigned i = 0; i < orbs.size(); ++i) {
    std::cout << orbs[i]->getScale() << std::endl;
  }
}

int Manager::play() {
  SDL_Event event;
  bool done = false;
  bool keyCatch = false;

  while ( not done ) {
    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false; 
    }
    if(event.type == SDL_KEYDOWN) {
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
        done = true;
        break;
      }
      if (keystate[SDLK_t] && !keyCatch) {
        keyCatch = true;
        //switchSprite();
        viewport.setObjectToTrack(destiny); 
      }
      if (keystate[SDLK_l] && !keyCatch) {
        keyCatch = true;
        clock.toggleSloMo();
      }
      if (keystate[SDLK_p] && !keyCatch) {
          keyCatch = true;
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
      if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }

      if (keystate[SDLK_r] && !keyCatch) {
          keyCatch = true;
        }

      if (keystate[SDLK_g] && !keyCatch) {
          keyCatch = true;
          godmode = !godmode;
        }

      if (keystate[SDLK_F1] && !keyCatch) {
          keyCatch = true;
          if ( drawHud ) drawHud = false;
          else drawHud = true;
         }

      if (keystate[SDLK_SPACE]) {
        destiny->shoot();
        sound[0];
      }          
    }

    if (keystate[SDLK_a]) {
      destiny->left();
    }

    if (keystate[SDLK_d]) {
      destiny->right();
    }

    if (keystate[SDLK_w]) {
      destiny->up();
    }

    if (keystate[SDLK_s]) {
      destiny->down();
    }

    if (keystate[SDLK_r]) {
       keyCatch = true;
       return 1;
    }
    draw();
    update();
  }
  return 0;
}
