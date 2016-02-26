#include "turretbullet.h"

	//bullet creation method
	TurretBullet::TurretBullet(SDL_Renderer *renderer, string filePath, float x, float y)
	{
		//activate the bullet
		active = true;

		//set bullet speed
		speed = 800.0f;

		//load image into texture
		texture = IMG_LoadTexture(renderer, filePath.c_str());

		//get width and height
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		posRect.w = w;
		posRect.h = h;

		//set x and y positions
		posRect.x = x;
		posRect.y = y;

		//set float positions
		pos_X = x;
		pos_Y = y;
	}

	//bullet Update
	void TurretBullet::Update(float deltaTime)
	{
		//get direction values to move with
		float vX = ((player.x - 32) - turret.x) / distance;
		float vY = ((player.y - 32) - turret.y) / distance;

		//Increment the bullet position by vX and vY each frame
		pos_X += (vX * speed * deltaTime);
		pos_Y += (vY * speed * deltaTime);

		//Update bullet pos
		posRect.x = (int) (pos_X + 0.5f);
		posRect.y = (int) (pos_Y + 0.5f);

		//check to see if bullet is off screen
		if((posRect.y < (0 - posRect.h)) || (posRect.y > 768) || (posRect.x < (0 - posRect.w)) || (posRect.x > 1024))
		{
			Reset();
		}
	}

	//bullet draw
	void TurretBullet::Draw(SDL_Renderer *renderer)
	{
		SDL_RenderCopy(renderer, texture, NULL, &posRect);
	}

	//bullet reset
	void TurretBullet::Reset()
	{
		posRect.x = -1000;

		//update pos_X
		pos_X = posRect.x;

		//deactivate
		active = false;
	}

	//bullet start
	void TurretBullet::Start(SDL_Rect playerPos, SDL_Rect turretPos)
	{
		player.x = playerPos.x;
		player.y = playerPos.y;

		turret.x = turretPos.x;
		turret.y = turretPos.y;

		distance = sqrt((player.x - turret.x) * (player.x - turret.x) + (player.y - turret.y) * (player.y - turret.y));
	}



