#include "tank.h"

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

//Tank Creation
Tank::Tank(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y)
{

	//activate the player
	active = true;

	//set the player number 0 or 1
	playerNum = pNum;

	//set float for player speed
	speed = 200.0f;

	//tank firing sound
	fire = Mix_LoadWAV((audioPath + "fire.wav").c_str());

	//tank path to image
	if(playerNum == 0)
	{
		playerPath = filePath + "tank.png";
	}else
	{
		playerPath = filePath + "tank2.png";
	}

	//load the surface into the texture
	texture = IMG_LoadTexture(renderer, playerPath.c_str());

	//set the SDL_Rect X and Y for the player
	posRect.x = x;
	posRect.y = y;

	//Use SDL_QueryTexture to get the W and H of the player's texture
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	//set the movement floats to the players original X and Y
	pos_X = x;
	pos_Y = y;

	//set the xDir and yDir for the joysticks
	xDir = 0;
	yDir = 0;

	//set initial values so the tank can shoot
	xDirOld = 1;
	yDirOld = 0;

	//center of the image rectangle
	center.x = posRect.w/2;
	center.y = posRect.h/2;

	//String to create the path to the player's bullet path
	string bulletPath;

	//bullet path to image
	if(playerNum == 0)
	{
		bulletPath = filePath + "redBullet.png";
	}else
	{
		bulletPath = filePath + "purpleBullet.png";
	}

	//Create the player's bullet pool
	for (int i = 0; i < 10; i++)
	{
		//create the bullet and move offscreen, out of the game play area
		TankBullet tmpBullet(renderer, bulletPath, -1000, -1000, 0, 0);

		//add to bulletList
		bulletList.push_back(tmpBullet);
	}
}

void Tank::Update(float deltaTime)
{
	//check for gamepad input
	if(Xvalue != 0 || Yvalue != 0)
	{
		//get the angle that tank needs to face
		//x = posRect.x - xDir;
		//y = posRect.y - yDir;
		tankangle = atan2(Yvalue, Xvalue) * 180/3.14;

		//set this as the old angle and dir some the player/tank can shoot when stopped
		oldAngle = tankangle;
		//xDirOld = xDir;
		//yDirOld = yDir;

		float radians = (tankangle * 3.14)/180;

		float move_x = speed * cos(radians);
		float move_y = speed * sin(radians);

		//Update floats for precision loss
		pos_X += (move_x) * deltaTime;
		pos_Y += (move_y) * deltaTime;

		//Update player position with code to account for precision loss
		posRect.x = (int)(pos_X + 0.5f);
		posRect.y = (int)(pos_Y + 0.5f);

	}else
	{
		tankangle = oldAngle;
	}

	//Adjust position floats based on speed, direction of joystick axis and deltaTime
	//pos_X += (speed * xDir) * deltaTime;
	//pos_Y += (speed * yDir) * deltaTime;

	//check if the tank is off screen and set it back
	if(posRect.x < 0)
	{
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if(posRect.x > 1024 - posRect.w)
	{
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}

	if(posRect.y < 0)
	{
		posRect.y = 0;
		pos_Y = posRect.y;
	}

	if(posRect.y > 768 - posRect.h)
	{
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}

	//Update the tank's bullets
	for(int i = 0; i < bulletList.size(); i++)
	{
		//check to see if the bullet is active
		if(bulletList[i].active)
		{
			//Update Bullet
			bulletList[i].Update(deltaTime);
		}
	}

}

void Tank::Draw(SDL_Renderer *renderer)
{
	//draw the player's bullets
	for(int i = 0; i < bulletList.size(); i++)
	{
		//check to see if the bullet is active
		if(bulletList[i].active)
		{
			//Draw Bullet
			bulletList[i].Draw(renderer);
		}
	}

	//draw the player texture using the vars texture and posRect
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, tankangle, &center, SDL_FLIP_NONE);
}

void Tank::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	//if the player's number is 0 and the joystick button is from joystick -
	if (event.which == 0 && playerNum == 0)
	{
		//if A Button
		if (event.button == 0)
		{
			//Create a bullet
			CreateBullet();
		}
	}
	//if the numbers are 1
	if (event.which == 1 && playerNum == 1)
	{
		//if A Button
		if (event.button == 0)
		{
			//Create a bullet
			CreateBullet();
		}
	}

}

void Tank::CreateBullet()
{
	//see if there is a bullet active to fire
	for (int i = 0; i < bulletList.size(); i++)
	{
		//see if bullet is not active
		if (bulletList[i].active == false)
		{
			//Play the Over Sound - plays fine through levels, must pause for QUIT
			Mix_PlayChannel(-1, fire, 0);

			//set bullet to active
			bulletList[i].active = true;

			//use some math in the x position to get the bullet close to
			//the center of the player using player width
			bulletList[i].posRect.x = (posRect.x + (posRect.w / 2));
			bulletList[i].posRect.y = (posRect.y + (posRect.h / 2));

			//finishing aligning to the player center using the texture width
			bulletList[i].posRect.x = bulletList[i].posRect.x - (bulletList[i].posRect.w / 2);
			bulletList[i].posRect.y = bulletList[i].posRect.y - (bulletList[i].posRect.h / 2); 

			//Set the x and y positions of the bullet's float positions
			bulletList[i].pos_X = bulletList[i].posRect.x;
			bulletList[i].pos_Y = bulletList[i].posRect.y;

			//if the tank is moving fire in the direction
			if (Xvalue != 0 || Yvalue != 0)
			{
				//Set the x and y positions of the bullet's float positions
				bulletList[i].tankangle = tankangle;
			}
			else
			{
				//if the tank is not moving, fire in the direction currently facing
				//Set the x and y positions of the bullet's float positions
				bulletList[i].tankangle = oldAngle;
			}

			//once bullet is found, break out of loop
			break;
		}
	}
}

//Player Joystick Axis Method
void Tank::OnControllerAxis(Sint16 X, Sint16 Y)
{
	Xvalue = X;
	Yvalue = Y;

	if (!(Xvalue < -JOYSTICK_DEAD_ZONE) && !(Xvalue > JOYSTICK_DEAD_ZONE))
		{
			Xvalue = 0.0f;
		}

	if(!(Yvalue < -JOYSTICK_DEAD_ZONE) && !(Yvalue > JOYSTICK_DEAD_ZONE))
		{
			Yvalue = 0.0f;
		}
}



