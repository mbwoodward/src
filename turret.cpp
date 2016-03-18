#include "turret.h"

//Turret creation method
Turret::Turret(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{
	//activate the turret
	active = true;

	//fire sound
	fire = Mix_LoadWAV((audioPath + "fire.wav").c_str());

	//Create the turret base file path
	string basePath = filePath + "turretBase.png";

	//load image into texture
	tBase = IMG_LoadTexture(renderer, basePath.c_str());

	//Create turret barrel file path
	string barrelPath = filePath + "turretBarrel.png";

	//load image into texture
	tBarrel = IMG_LoadTexture(renderer, barrelPath.c_str());

	//set the SDL_Rect X and Y for base
	baseRect.x = x;
	baseRect.y = y;

	//get width and height
	int w, h;
	SDL_QueryTexture(tBase, NULL, NULL, &w, &h);
	baseRect.w = w;
	baseRect.h = h;

	//set SDL_Rect X and Y for barrel
	barrelRect.x = x+56;
	barrelRect.y = y+56;

	//get width and height
	SDL_QueryTexture(tBarrel, NULL, NULL, &w, &h);
	barrelRect.w = w;
	barrelRect.h = h;

	center.x = 8;
	center.y = 8;

	//string to create bullet path
	string bulletPath = filePath + "tBullet.png";

	//Create the turret's bullet pool
	for(int i = 0; i < 10; i++)
	{
		//create the bullet and stuff
		TurretBullet tmpBullet(renderer, bulletPath, 2000, 2000);

		//add to bulletList
		bulletList.push_back(tmpBullet);
	}

	//random null init
	srand(time(NULL));

	//update float values for movement
	posB_X = baseRect.x;
	posB_Y = baseRect.y;

	posT_X = barrelRect.x;
	posT_Y = barrelRect.y;
}

void Turret::TankMoveX(float tankSpeed, float deltaTime)
{
	posB_X += (tankSpeed) * deltaTime;
	posT_X += (tankSpeed) * deltaTime;

	//Update position
	baseRect.x = (int) (posB_X + 0.5f);
	barrelRect.x = (int) (posT_X + 0.5f);
}

void Turret::TankMoveY(float tankSpeed, float deltaTime)
{
	posB_Y += (tankSpeed) * deltaTime;
	posT_Y += (tankSpeed) * deltaTime;

	//Update position
	baseRect.y = (int) (posB_Y + 0.5f);
	barrelRect.y = (int) (posT_Y + 0.5f);
}

//Turret Draw
void Turret::Draw(SDL_Renderer *renderer)
{
	//draw the turret's bullets
	for (int i = 0; i < bulletList.size(); i++)
	{
		//check too see active
		if(bulletList[i].active)
		{
			//Draw
			bulletList[i].Draw(renderer);
		}
	}

	//Draw base
	SDL_RenderCopy(renderer, tBase, NULL, &baseRect);

	//Draw barrel
	SDL_RenderCopyEx(renderer, tBarrel, NULL, &barrelRect, turretangle, &center, SDL_FLIP_NONE);

}

//Turret Update
void Turret::Update(float deltaTime, SDL_Rect tankRect)
{
	//get angle between the tank and turret
	x = (tankRect.x + (tankRect.w/2)) - (baseRect.x + (baseRect.w/2));
	y = (tankRect.y + (tankRect.h/2)) - (baseRect.y + (baseRect.h/2));
	turretangle = atan2(y,x) * 180 / 3.14;

	if(SDL_GetTicks() > fireTime)
	{
		if(baseRect.x > 0 && baseRect.x < 1024 && baseRect.y > 0 && baseRect.y < 768)
		{
			CreateBullet(tankRect);
		}

		fireTime = SDL_GetTicks() + (rand() % 3 + 1) * 1000;
	}

	//Update the turret's bullets
		for (int i = 0; i < bulletList.size(); i++)
		{
			//check too see active
			if(bulletList[i].active)
			{
				//Draw
				bulletList[i].Update(deltaTime);
			}
		}

}

void Turret::CreateBullet(SDL_Rect target)
{
	//see if active bullet
	for(int i=0; i<bulletList.size();i++)
	{
		//see if not active
		if(bulletList[i].active == false)
		{
			bulletList[i].Start(target, baseRect);

			//Play fire sound
			Mix_PlayChannel(-1, fire, 0);

			//set to active
			bulletList[i].active = true;

			//use math to get bullet close to center of turret
			bulletList[i].posRect.x = ((baseRect.x + (baseRect.w/2)) - (bulletList[i].posRect.w/2));
			bulletList[i].posRect.y = ((baseRect.y + (baseRect.h/2)) - (bulletList[i].posRect.h/2));

			//set x and y positions of bullet
			bulletList[i].pos_X = bulletList[i].posRect.x;
			bulletList[i].pos_Y = bulletList[i].posRect.y;

			//break
			break;

		}
	}
}


