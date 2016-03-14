#include "enemyTank.h"

//Tank create
EnemyTank::EnemyTank(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{
	//activate
	active = false;

	//fire sound
	explode = Mix_LoadWAV((audioPath + "fire.wav").c_str());

	//create the tank
	string enemyPath = filePath + "tank2.png";

	//load image
	eTank = IMG_LoadTexture(renderer, enemyPath.c_str());

	//set the SDL_Rect
	eTankRect.x = x;
	eTankRect.y = y;

	//use SDL_QueryTexture
	int w,h;
	SDL_QueryTexture(eTank, NULL, NULL, &w, &h);
	eTankRect.w = w;
	eTankRect.h = h;

	posT_X = eTankRect.x;
	posT_Y = eTankRect.y;

	speed = 100;

	center.x = 32;
	center.y = 32;

	health = 10;
}

void EnemyTank::Reset()
{
	//reset the x pos
	eTankRect.x = -1000;

	//update pos_X
	posT_X = eTankRect.x;

	health = 10;

	//deactivate
	active = false;
}

void EnemyTank::RemoveHealth()
{
	health --;

	if(health <= 0)
	{
		Reset();
	}
}

//tank moves X
void EnemyTank::eTankMoveX(float tankSpeed, float deltaTime)
{
	posT_X += (tankSpeed) * deltaTime;

	//update bullet pos
	eTankRect.x = (int) (posT_X + 0.5f);
}

//tank moves Y
void EnemyTank::eTankMoveY(float tankSpeed, float deltaTime)
{
	posT_Y += (tankSpeed) * deltaTime;

	//update bullet pos
	eTankRect.y = (int) (posT_Y + 0.5f);
}

//turret draw
void EnemyTank::Draw(SDL_Renderer *renderer)
{
	//draw
	SDL_RenderCopyEx(renderer, eTank, NULL, &eTankRect, tankangle, &center, SDL_FLIP_NONE);
}

//tank update
void EnemyTank::Update(float deltaTime, SDL_Rect tankRect)
{

	double distancex = (eTankRect.x - tankRect.x) * (eTankRect.x - tankRect.x);
	double distancey = (eTankRect.y - tankRect.y) * (eTankRect.y - tankRect.y);

	double calcdistance = sqrt(distancex + distancey);

	if(calcdistance <= 500)
	{
		active = true;
	}
	else
	{
		active = false;
	}
	if(active)
	{
		//get angle
		x = (tankRect.x + (tankRect.w/2)) - (eTankRect.x + (eTankRect.w/2));
		y = (tankRect.y + (tankRect.h/2)) - (eTankRect.y + (eTankRect.h/2));
		tankangle = atan2(y,x) * 180 / 3.14;

		//gives radians
		float radians = (tankangle * 3.14) / 180;

		//get x and y
		float move_x = speed * cos(radians);
		float move_y = speed * sin(radians);

		//update floats
		posT_X += (move_x) * deltaTime;
		posT_Y += (move_y) * deltaTime;

		//update player pos
		eTankRect.x = (int)(posT_X + 0.5f);
		eTankRect.y = (int)(posT_Y + 0.5f);
	}
}



