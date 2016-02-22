//needed includes
#include <string>
#include <iostream>
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

class TankBullet
{
public:
	//Boolean for the state of the bullet
	bool active;

	//Bullet texture
	SDL_Texture *texture;

	//Bullet rectangle for position (X,Y) and size (W,H)
	SDL_Rect posRect;

	//movement direction values
	float xDir, yDir;

	//bullet speed
	float speed;

	//Bullet float positions to prevent precision loss
	float pos_X, pos_Y;

	//tank angle passed in
	float tankangle;

	//Bullet creation method
	TankBullet(SDL_Renderer *renderer, string filePath, float x, float y, int dirX, int dirY);

	//Bullet Update
	void Update(float deltaTime);

	//Bullet Draw
	void Draw(SDL_Renderer *renderer);

	//Bullet Reset
	void Reset();
};
