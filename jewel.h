//needed includes
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;

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

class  Jewel
{
public:

	//is the jewel active
	bool active;

	//jewel's texture
	SDL_Texture *jewel;

	//rect
	SDL_Rect jewelRect;

	//float for jewel's position to avoid precision loss
	float posJ_X, posJ_Y;

	//jewel's creation method
	Jewel(SDL_Renderer *renderer, string filePath, int color, float x, float y);

	//draw
	void Draw(SDL_Renderer *renderer);

	//update jewel
	void Update(float deltaTime);

	void TankMoveX(float tankSpeed, float deltaTime);

	void TankMoveY(float tankSpeed, float deltaTime);
};
