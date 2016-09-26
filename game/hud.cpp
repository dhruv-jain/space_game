#include "hud.h"


Hud::Hud() :
	 width( Gamedata::getInstance().getXmlInt("hud/Width") ),
	 height( Gamedata::getInstance().getXmlInt("hud/Height") ),
	 pos(Gamedata::getInstance().getXmlInt("hud/X"),
	 	Gamedata::getInstance().getXmlInt("hud/Y")),
	 io( IOManager::getInstance() ),
	 screen( io.getScreen() ),
	 clock( Clock::getInstance() ),
	 viewport( Viewport::getInstance() ),
	 GREEN( SDL_MapRGB(screen->format, 0, 0xff, 0) )
{}

void Hud::draw(unsigned int bcount, unsigned int fcount) const {
	Draw_AALine(screen, pos[0], pos[1]+height/2,
			    pos[0]+width, pos[1]+height/2,
			    height, 0xff, 0xff, 0xff, 0xff/2);
	Draw_AALine(screen, pos[0], pos[1],
			    pos[0]+width, pos[1], 1.0f, GREEN);
	Draw_AALine(screen, pos[0], pos[1]+height,
			    pos[0]+width, pos[1]+height, 1.0f, GREEN);
	Draw_AALine(screen, pos[0], pos[1],
			    pos[0], pos[1]+height, 2.0f, GREEN);
	Draw_AALine(screen, pos[0]+width, pos[1],
			    pos[0]+width, pos[1]+height, 2.0f, GREEN);
io.printMessageAt("HEADS UP DISPLAY", pos[0]+10, pos[1]+10);
io.printMessageValueAt("fps: ", clock.getFps(), pos[0]+10, pos[1]+30);
io.printMessageValueAt("Seconds: ", clock.getSeconds(), pos[0]+10, pos[1]+50);
io.printMessageAt("Rocket Movement", pos[0]+10, pos[1]+90);
io.printMessageAt("Press A to go LEFT", pos[0]+10, pos[1]+110);
io.printMessageAt("Press S to go DOWN", pos[0]+10, pos[1]+130);
io.printMessageAt("Press D to go RIGHT", pos[0]+10, pos[1]+150);
io.printMessageAt("Press W to go UP", pos[0]+10, pos[1]+170);
io.printMessageAt("Press SPACE to Shoot", pos[0]+10, pos[1]+190);
io.printMessageAt("Press R to reset", pos[0]+10, pos[1]+210);
io.printMessageAt("Press G for God Mode", pos[0]+10, pos[1]+230);
io.printMessageValueAt("Bullet list: ", bcount, pos[0]+10, pos[1]+300);
io.printMessageValueAt("Free list: ", fcount, pos[0]+10, pos[1]+270);
}


void Hud::game() const {
	Draw_AALine(screen, pos[0]+300, pos[1]+height/2,
			    pos[0]+width+310, pos[1]+height/2,
			    height, 0xff, 0xff, 0xff, 0xff/2);
	Draw_AALine(screen, pos[0]+300, pos[1],
			    pos[0]+width+310, pos[1], 1.0f, GREEN);
	Draw_AALine(screen, pos[0]+300, pos[1]+height,
			    pos[0]+width+310, pos[1]+height, 1.0f, GREEN);
	Draw_AALine(screen, pos[0]+300, pos[1],
			    pos[0]+300, pos[1]+height, 2.0f, GREEN);
	Draw_AALine(screen, pos[0]+width+310, pos[1],
			    pos[0]+width+310, pos[1]+height, 2.0f, GREEN);

io.printMessageAt("Destiny was destroyed", pos[0]+330, pos[1]+155);

io.printMessageAt("You could not save the Galaxy", pos[0]+300, pos[1]+175);

}

void Hud::gamewin() const {
	Draw_AALine(screen, pos[0]+300, pos[1]+height/2,
			    pos[0]+width+310, pos[1]+height/2,
			    height, 0xff, 0xff, 0xff, 0xff/2);
	Draw_AALine(screen, pos[0]+300, pos[1],
			    pos[0]+width+310, pos[1], 1.0f, GREEN);
	Draw_AALine(screen, pos[0]+300, pos[1]+height,
			    pos[0]+width+310, pos[1]+height, 1.0f, GREEN);
	Draw_AALine(screen, pos[0]+300, pos[1],
			    pos[0]+300, pos[1]+height, 2.0f, GREEN);
	Draw_AALine(screen, pos[0]+width+310, pos[1],
			    pos[0]+width+310, pos[1]+height, 2.0f, GREEN);
io.printMessageAt("You saved the Galaxy", pos[0]+330, pos[1]+155);

io.printMessageAt("All enemy ships are destroyed", pos[0]+300, pos[1]+175);

}