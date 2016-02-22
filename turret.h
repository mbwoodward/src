
//needed includes
#include <string>
#include <iostream>

using namespace std;
#include <vector>

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#endif

#if defined(__linux__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#endif

//#include "turretbullet.h"

class Turret
{
public:
	//is the turret active - still alive?
	bool active;

	//variable to hold the list of bullets
	//vector<TurretBullet> bulletList;

	//string to hold the path to the turret's texture
	string turretPath;

	//turret's texture to hold base and barrel images
	SDL_Texture *tBase;
	SDL_Texture *tBarrel;

	//turret's SDL_Rect for the X, Y, W and H of base texture
	SDL_Rect baseRect;

	//turret's SDL_Rect for the X, Y, W, and H of barrel texture
	SDL_Rect barrelRect;
};
