#include "jewel.h"

Jewel::Jewel(SDL_Renderer *renderer, string filePath, int color, float x, float y)
{
	active = true;

	if(color==0)
	{
		//Create the jewel file path
		string basePath = filePath + "purple.png";

		//load image
		jewel = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if(color==1)
	{
		//Create the jewel file path
		string basePath = filePath + "red.png";

		//load image
		jewel = IMG_LoadTexture(renderer, basePath.c_str());
	}
	else if(color==2)
	{
		//Create the jewel file path
		string basePath = filePath + "blue.png";

		//load image
		jewel = IMG_LoadTexture(renderer, basePath.c_str());
	}else if(color==3)
	{
		//Create the jewel file path
		string basePath = filePath + "folderPickup.png";

		//load image
		jewel = IMG_LoadTexture(renderer, basePath.c_str());
	}

	//set SDL_Rect x and y for image
	jewelRect.x = x;
	jewelRect.y = y;

	//use SDL_QueryTexture
	int w, h;
	SDL_QueryTexture(jewel, NULL, NULL, &w, &h);
	jewelRect.w = w/2;
	jewelRect.h = h/2;

	posJ_X = jewelRect.x;
	posJ_Y = jewelRect.y;
}

//jewel draw method
void Jewel::Draw(SDL_Renderer *renderer)
{
	//draw
	SDL_RenderCopy(renderer, jewel, NULL, &jewelRect);
}

void Jewel::TankMoveX(float tankSpeed, float deltaTime)
{
	posJ_X += (tankSpeed) * deltaTime;

	//Update jewel position
	jewelRect.x = (int) (posJ_X + 0.5f);
}

void Jewel::TankMoveY(float tankSpeed, float deltaTime)
{
	posJ_Y += (tankSpeed) * deltaTime;

	//Update jewel position
	jewelRect.y = (int) (posJ_Y + 0.5f);
}

