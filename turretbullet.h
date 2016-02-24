#include <string>
#include <iostream>
#include <math.h>
using namespace std;

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#endif

#if defined(__linux__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

class TurretBullet
{
public:
	//Boolean for state
	bool active;

	//Bullet texture
	SDL_Texture *texture;

	//Bullet rect
	SDL_Rect posRect;

	//Bullet speed
	float speed;

	//Bullet float positions
	float pos_X, pos_Y;

	//position for turret and player
	SDL_Point turret, player;

	//distance
	float distance;

	//bullet creation method
	TurretBullet(SDL_Renderer *renderer, string filePath, float x, float y);

	//bullet Update
	void Update(float deltaTime);

	//bullet draw
	void Draw(SDL_Renderer *renderer);

	//bullet reset
	void Reset();

	//bullet start
	void Start(SDL_Rect playerPos, SDL_Rect turretPos);
};
