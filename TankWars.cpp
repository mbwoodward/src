#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <sstream>
#include "tank.h"
#include "turret.h"
#include "enemyTank.h"


using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;



#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
//string var to hold the current working directory on __APPLE__
string currentWorkingDirectory(getcwd(NULL,0));

//create a string to link to the images folder on __APPLE__
string images_dir = currentWorkingDirectory + "/src/";

//create a string to link to the audio folder on __APPLE__
string audio_dir = currentWorkingDirectory + "/src/";
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include <direct.h>
#define getcwd _getcwd

//string var to hold the current working directory on __APPLE__
string currentWorkingDirectory(getcwd(NULL,0));

//create a string to link to the images folder on __APPLE__
string images_dir = currentWorkingDirectory + "\\src\\";

//create a string to link to the audio folder on __APPLE__
string audio_dir = currentWorkingDirectory + "\\src\\";
#endif

#if defined(__linux__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <unistd>

//string var to hold the current working directory on __APPLE__
string currentWorkingDirectory(getcwd(NULL,0));

//create a string to link to the images folder on __APPLE__
string images_dir = currentWorkingDirectory + "/src/";

//create a string to link to the audio folder on __APPLE__
string audio_dir = currentWorkingDirectory + "/src/";
#endif

//Variable for font
TTF_Font *font;

//font color
SDL_Color colorP1 = {255,255,255,255};

//Surface for player hit text
SDL_Surface *playerSurface, *turretSurface;

//Textures for player hit
SDL_Texture *playerTexture, *turretTexture;

//SDL_Rects for player
SDL_Rect playerPos, turretPos;

//playerhealth
int playerHealth = 100;

string tempText = " ";

void PlayerText(SDL_Renderer *renderer)
{
	string Result;
	ostringstream convert;
	convert << playerHealth;
	Result = convert.str();

	//create text
	//tempText = "Player Health: " + Result;

	tempText = "Player Health: ";

	//surface for font string
	playerSurface = TTF_RenderText_Solid(font, tempText.c_str(), colorP1);

	//create player score
	playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);

	//get width and height of texture
	SDL_QueryTexture(playerTexture, NULL, NULL, &playerPos.w, &playerPos.h);

	SDL_FreeSurface(playerSurface);
}

void TurretText(SDL_Renderer *renderer, int turretNum)
{
	string Result;
	ostringstream convert;
	convert << turretNum;
	Result = convert.str();

	//create text
	tempText = "Turret " + Result + " was the last hit...";

	//set to no turret hit
	if(turretNum == 0)
	{
		//create text for font
		tempText = "No Turret has been hit...";
	}

	//surface for font string
	turretSurface = TTF_RenderText_Solid(font, tempText.c_str(), colorP1);

	//create player score
	turretTexture = SDL_CreateTextureFromSurface(renderer, turretSurface);

	//get width and height of texture
	SDL_QueryTexture(turretTexture, NULL, NULL, &turretPos.w, &turretPos.h);

	SDL_FreeSurface(turretSurface);
}
//deltaTime var initialization - for frame rate independence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;


//Main Start***************************
int main()
{
	//**********Create the SDL Window-START**********
	//Start SDL2
	SDL_Init(SDL_INIT_EVERYTHING);

	//Create a SDL Window reference - pointer
	SDL_Window *window = NULL;

	//Create a SDL RenderTarget - pointer
	SDL_Renderer *renderer = NULL;

	//Create a SDL Window in the middle of the screen
	window = SDL_CreateWindow("Tank Wars!",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

	//Create an SDL RenderTarget
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//**********Create the SDL Window-END**********

	//******variable creation-START**********
	//Main loop flag
	bool quit=false;

	//Event handler
	SDL_Event e;
	//******variable creation-END**********

	//****Turn on Game Controller Events****
	SDL_GameControllerEventState(SDL_ENABLE);

	//****set up a Game Controller variable for player 1****
	SDL_GameController* gGameController0 = NULL;

	//****Open Game Controller for player 1****
	gGameController0 = SDL_GameControllerOpen(0);

	//Open Audio Channel
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//Load a MUSIC file
	Mix_Music *bgm = Mix_LoadMUS((audio_dir + "background.mp3").c_str());

	//If the MUSIC file is not playing - Play it
	if(!Mix_PlayingMusic())
		Mix_PlayMusic(bgm,-1);

	//******Create Player START**************
	Tank tank1 = Tank(renderer, 0, images_dir.c_str(),audio_dir.c_str(), 50.0f, 50.0f);

	//********Create Turret**********
	Turret turret1 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 800.0f, 500.0f);
	Turret turret2 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 1600.0f, 250.0f);
	Turret turret3 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 400.0f, 1000.0f);
	Turret turret4 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 1600.0f, 1250.0f);

	//************Create enemyTank***************
	EnemyTank eTank1 = EnemyTank(renderer, images_dir.c_str(), audio_dir.c_str(), 400.0f, 200.0f);
	EnemyTank eTank2 = EnemyTank(renderer, images_dir.c_str(), audio_dir.c_str(), 1400.0f, 300.0f);
	EnemyTank eTank3 = EnemyTank(renderer, images_dir.c_str(), audio_dir.c_str(), 600.0f, 1200.0f);
	EnemyTank eTank4 = EnemyTank(renderer, images_dir.c_str(), audio_dir.c_str(), 1700.0f, 600.0f);

	SDL_Texture *bkgd=IMG_LoadTexture(renderer, (images_dir+"background.png").c_str());

	SDL_Rect bkgdRect;

	bkgdRect.x = 0;
	bkgdRect.y = 0;

	bkgdRect.w = 2048;
	bkgdRect.h = 1536;

	float X_pos = 0.0f;
	float Y_pos = 0.0f;

	//init the ttf engine
	TTF_Init();

	//load Font
	font = TTF_OpenFont((images_dir + "Gsubadaslowly.ttf").c_str(), 40);

	//x and y for player's text
	playerPos.x = 10;
	playerPos.y = 10;

	//x and y for turret
	turretPos.x = 600;
	turretPos.y = 10;

	//create initial player text
	PlayerText(renderer);

	//create initial turret text
	TurretText(renderer, 0);

	//********MAIN GAME LOOP START*****************************************

	while(!quit)
	{
		//Create deltaTime - for frame rate independence
		thisTime = SDL_GetTicks();
		deltaTime = (float) (thisTime - lastTime) / 1000;
		lastTime = thisTime;

		//Handle Events on Queue - Keypresses and such START*************
		while(SDL_PollEvent(&e)!= 0)
		{
			//User requests quit
			if(e.type == SDL_QUIT)
			{
				quit = true;
			}

			switch(e.type)
			{
			case SDL_CONTROLLERBUTTONDOWN:

				if(e.cdevice.which == 0)
				{
					if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
					{
						tank1.OnControllerButton(e.cbutton);
						break;
					}
					//QUICK EXIT///DON'T FORGET TO TAKE OUT*******************			-----IMPORTANT-----
					if(e.cbutton.button == SDL_CONTROLLER_BUTTON_B)
					{
						quit = true;
						break;
					}
				}

				break;

			case SDL_CONTROLLERAXISMOTION:
				//tank1.OnControllerAxis(e.caxis);
				break;

			}//SWITCH (E.TYPE)

		}// POLL EVENT

		const Sint16 Xvalue = SDL_GameControllerGetAxis(gGameController0,SDL_CONTROLLER_AXIS_LEFTX);
		const Sint16 Yvalue = SDL_GameControllerGetAxis(gGameController0,SDL_CONTROLLER_AXIS_LEFTY);
		tank1.OnControllerAxis(Xvalue, Yvalue);

		//UPDATE SECTION
		//update player 1 tank
		tank1.Update(deltaTime);


		//move background
		if((tank1.posRect.x >= 1024 - tank1.posRect.w) && (tank1.Xvalue > 8000))
		{
			//Adjust position floats based on speed, direction and deltaTime
			X_pos -=(tank1.speed) * deltaTime;

			if(bkgdRect.x > -1024){
				//Update bullet position with code to account for precision loss
				bkgdRect.x = (int)(X_pos + 0.5f);
				//move turret
				turret1.TankMoveX(-tank1.speed, deltaTime);
				turret2.TankMoveX(-tank1.speed, deltaTime);
				turret3.TankMoveX(-tank1.speed, deltaTime);
				turret4.TankMoveX(-tank1.speed, deltaTime);

				//move enemy tank
				eTank1.eTankMoveX(-tank1.speed, deltaTime);
				eTank2.eTankMoveX(-tank1.speed, deltaTime);
				eTank3.eTankMoveX(-tank1.speed, deltaTime);
				eTank4.eTankMoveX(-tank1.speed, deltaTime);

			}else
			{
				X_pos=bkgdRect.x;
			}
		}
		if((tank1.posRect.x <= 0) && (tank1.Xvalue < 8000))
		{
			X_pos += (tank1.speed) * deltaTime;

			if(bkgdRect.x < 0){
				//Update bullet position with code to account for precision loss
				bkgdRect.x = (int)(X_pos + 0.5f);
				turret1.TankMoveX(tank1.speed, deltaTime);
				turret2.TankMoveX(tank1.speed, deltaTime);
				turret3.TankMoveX(tank1.speed, deltaTime);
				turret4.TankMoveX(tank1.speed, deltaTime);

				//move enemy tank
				eTank1.eTankMoveX(tank1.speed, deltaTime);
				eTank2.eTankMoveX(tank1.speed, deltaTime);
				eTank3.eTankMoveX(tank1.speed, deltaTime);
				eTank4.eTankMoveX(tank1.speed, deltaTime);

			}else
			{
				X_pos=bkgdRect.x;
			}
		}

		if((tank1.posRect.y >= 768 - tank1.posRect.h) && (tank1.Yvalue > 8000))
		{
			//Adjust position floats based on speed, direction and deltaTime
			Y_pos -=(tank1.speed) * deltaTime;

			if(bkgdRect.y > -768){
				//Update bullet position with code to account for precision loss
				bkgdRect.y = (int)(Y_pos + 0.5f);
				turret1.TankMoveY(-tank1.speed, deltaTime);
				turret2.TankMoveY(-tank1.speed, deltaTime);
				turret3.TankMoveY(-tank1.speed, deltaTime);
				turret4.TankMoveY(-tank1.speed, deltaTime);

				//move enemy tank
				eTank1.eTankMoveY(-tank1.speed, deltaTime);
				eTank2.eTankMoveY(-tank1.speed, deltaTime);
				eTank3.eTankMoveY(-tank1.speed, deltaTime);
				eTank4.eTankMoveY(-tank1.speed, deltaTime);


			}else
			{
				Y_pos=bkgdRect.y;
			}
		}
		if((tank1.posRect.y <= 0) && (tank1.Yvalue < 8000))
		{
			Y_pos += (tank1.speed) * deltaTime;

			if(bkgdRect.y < 0){
				//Update bullet position with code to account for precision loss
				bkgdRect.y = (int)(Y_pos + 0.5f);
				turret1.TankMoveY(tank1.speed, deltaTime);
				turret2.TankMoveY(tank1.speed, deltaTime);
				turret3.TankMoveY(tank1.speed, deltaTime);
				turret4.TankMoveY(tank1.speed, deltaTime);

				//move enemy tank
				eTank1.eTankMoveY(tank1.speed, deltaTime);
				eTank2.eTankMoveY(tank1.speed, deltaTime);
				eTank3.eTankMoveY(tank1.speed, deltaTime);
				eTank4.eTankMoveY(tank1.speed, deltaTime);



			}else
			{
				Y_pos=bkgdRect.y;
			}
		}

		//update turret
		turret1.Update(deltaTime, tank1.posRect);
		turret2.Update(deltaTime, tank1.posRect);
		turret3.Update(deltaTime, tank1.posRect);
		turret4.Update(deltaTime, tank1.posRect);

		//update enemy tank
		eTank1.Update(deltaTime, tank1.posRect);
		eTank2.Update(deltaTime, tank1.posRect);
		eTank3.Update(deltaTime, tank1.posRect);
		eTank4.Update(deltaTime, tank1.posRect);


		//check for hit from turrets
		//1
		for(int i =0; i<turret1.bulletList.size(); i++)
		{
			if(SDL_HasIntersection(&tank1.posRect, &turret1.bulletList[i].posRect))
			{
				turret1.bulletList[i].Reset();
				tank1.eBulletHit();
				//playerHealth--;
				//PlayerText(renderer);
				break;
			}
		}

		//2
		for(int i =0; i<turret2.bulletList.size(); i++)
		{
			if(SDL_HasIntersection(&tank1.posRect, &turret2.bulletList[i].posRect))
			{
				turret2.bulletList[i].Reset();
				tank1.eBulletHit();
				//playerHealth--;
				//PlayerText(renderer);
				break;
			}
		}

		//3
		for(int i =0; i<turret3.bulletList.size(); i++)
		{
			if(SDL_HasIntersection(&tank1.posRect, &turret3.bulletList[i].posRect))
			{
				turret3.bulletList[i].Reset();
				tank1.eBulletHit();
				//playerHealth--;
				//PlayerText(renderer);
				break;
			}
		}

		//4
		for(int i =0; i<turret4.bulletList.size(); i++)
		{
			if(SDL_HasIntersection(&tank1.posRect, &turret4.bulletList[i].posRect))
			{
				turret4.bulletList[i].Reset();
				tank1.eBulletHit();
				//playerHealth--;
				//PlayerText(renderer);
				break;
			}
		}

		//check for hit from tanks
		//1
			if(SDL_HasIntersection(&tank1.posRect, &eTank1.eTankRect))
			{
				tank1.eTankHit();
				//playerHealth--;
				//PlayerText(renderer);
				//break;
			}

		//2
			if(SDL_HasIntersection(&tank1.posRect, &eTank2.eTankRect))
			{
				tank1.eTankHit();
				//playerHealth--;
				//PlayerText(renderer);
				//break;
			}

		//3
			if(SDL_HasIntersection(&tank1.posRect, &eTank3.eTankRect))
			{
				tank1.eTankHit();
				//playerHealth--;
				//PlayerText(renderer);
				//break;
			}

		//4
			if(SDL_HasIntersection(&tank1.posRect, &eTank4.eTankRect))
			{
				tank1.eTankHit();
				//playerHealth--;
				//PlayerText(renderer);
				//break;
			}

		//check if player hit turret
		for (int i=0; i < tank1.bulletList.size(); i++)
		{
			//turret 1
			if(SDL_HasIntersection(&turret1.baseRect, &tank1.bulletList[i].posRect))
			{
				tank1.bulletList[i].Reset();
				TurretText(renderer, 1);
				break;
			}
			//turret 2
			if(SDL_HasIntersection(&turret2.baseRect, &tank1.bulletList[i].posRect))
			{
				tank1.bulletList[i].Reset();
				TurretText(renderer, 2);
				break;
			}
			//turret 3
			if(SDL_HasIntersection(&turret3.baseRect, &tank1.bulletList[i].posRect))
			{
				tank1.bulletList[i].Reset();
				TurretText(renderer, 3);
				break;
			}
			//turret 4
			if(SDL_HasIntersection(&turret4.baseRect, &tank1.bulletList[i].posRect))
			{
				tank1.bulletList[i].Reset();
				TurretText(renderer, 4);
				break;
			}
			//enemy tank
			//1
			if(SDL_HasIntersection(&eTank1.eTankRect, &tank1.bulletList[i].posRect))
			{
				tank1.bulletList[i].Reset();
				if(eTank1.active == true)
				{
				eTank1.RemoveHealth();
				}
			}
			//2
			if(SDL_HasIntersection(&eTank2.eTankRect, &tank1.bulletList[i].posRect))
			{
				tank1.bulletList[i].Reset();
				if(eTank2.active == true)
				{
				eTank2.RemoveHealth();
				}
			}
			//3
			if(SDL_HasIntersection(&eTank3.eTankRect, &tank1.bulletList[i].posRect))
			{
				tank1.bulletList[i].Reset();
				if(eTank3.active == true)
				{
				eTank3.RemoveHealth();
				}
			}
			//4
			if(SDL_HasIntersection(&eTank4.eTankRect, &tank1.bulletList[i].posRect))
			{
				tank1.bulletList[i].Reset();
				if(eTank4.active == true)
				{
				eTank4.RemoveHealth();
				}
			}
		}

		//DRAW SECTION
		//Clear the SDL RenderTarget
		SDL_RenderClear(renderer);

		//Draw Background
		SDL_RenderCopy(renderer, bkgd, NULL, &bkgdRect);

		//Draw player1 tank
		tank1.Draw(renderer);

		//Draw turret
		turret1.Draw(renderer);
		turret2.Draw(renderer);
		turret3.Draw(renderer);
		turret4.Draw(renderer);

		eTank1.Draw(renderer);
		eTank2.Draw(renderer);
		eTank3.Draw(renderer);
		eTank4.Draw(renderer);

		//Draw player hit texture
		SDL_RenderCopy(renderer, playerTexture, NULL, &playerPos);

		//Draw turret hit texture
		SDL_RenderCopy(renderer, turretTexture, NULL, &turretPos);

		SDL_RenderPresent(renderer);
	}
	//Close and destroy the window
	SDL_DestroyWindow(window);

	//Clean up
	SDL_Quit();

	return 0;
}




