#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdint.h>
#include "tank.h"
#include "turret.h"


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
	SDL_Window *window = nullptr;

	//Create a SDL RenderTarget - pointer
	SDL_Renderer *renderer = nullptr;

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

	SDL_Texture *bkgd=IMG_LoadTexture(renderer, (images_dir+"background.png").c_str());

	SDL_Rect bkgdRect;

	bkgdRect.x = 0;
	bkgdRect.y = 0;

	bkgdRect.w = 2048;
	bkgdRect.h = 1536;

	float X_pos = 0.0f;
	float Y_pos = 0.0f;

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
				//X_pos = bkgdRect.x;
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
				//X_pos = bkgdRect.x;
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
				//X_pos = bkgdRect.x;
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
				//X_pos = bkgdRect.x;
			}else
			{
				Y_pos=bkgdRect.y;
			}
		}

		//update turret
		turret1.Update(deltaTime, tank1.posRect);

		//DRAW SECTION
		//Clear the SDL RenderTarget
		SDL_RenderClear(renderer);

		//Draw Background
		SDL_RenderCopy(renderer, bkgd, NULL, &bkgdRect);

		//Draw player1 tank
		tank1.Draw(renderer);

		//Draw turret
		turret1.Draw(renderer);

		SDL_RenderPresent(renderer);
	}
	//Close and destroy the window
	SDL_DestroyWindow(window);

	//Clean up
	SDL_Quit();

	return 0;
}




