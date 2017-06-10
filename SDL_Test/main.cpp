#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include"Player.h"
#include"MapObjects.h"

//Texture image for background level1.
SDL_Texture *LoadTexture(std::string filePath, SDL_Renderer *renderTarget)
{
	SDL_Texture *texture = nullptr;
	SDL_Surface *surface = IMG_Load(filePath.c_str());
	if (surface == NULL)
		std::cout << "error1" << std::endl;
	else
	{
		//error control if failed to load level1 background.
		texture = SDL_CreateTextureFromSurface(renderTarget, surface);
		if (texture == NULL)
			std::cout << "error2" << std::endl;
	}
	SDL_FreeSurface(surface);

	return texture;
}

int main(int argc, char *argv[])
{


	SDL_Window *window = nullptr;
	SDL_Renderer *renderTarget = nullptr;
	int currentTime = 0;
	int prevTime = 0;
	float delta = 0.0f;
	const Uint8 *keyState;
	SDL_Rect cornerRect = { 0, 0, 640, 480 };
	

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("GAME1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

	renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Player player1(renderTarget, "hero3.png", 0, 0, 7, 4);
	Player player2(renderTarget, "bad1.png", 600, 400, 7, 4);
	MapObjects object1(renderTarget, "darkball.png", 0, 0);

	SDL_Texture *texture = LoadTexture("map.png", renderTarget);

	
	bool isRunning = true;
	SDL_Event ev;

	while (isRunning)
	{
		prevTime = currentTime;
		currentTime = SDL_GetTicks();
		delta = (currentTime - prevTime) / 1000.0f;
		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_QUIT)
				isRunning = false;
		}

		keyState = SDL_GetKeyboardState(NULL);

		player1.Update(delta, keyState);
		player2.Update(delta, keyState);
		//object1.Update(delta, keyState);

		//cornerRect.x = player1.Get

		player1.IntersectsWith(player2);


		SDL_RenderClear(renderTarget);
		SDL_RenderCopy(renderTarget, texture, &cornerRect, NULL);
		player1.Draw(renderTarget);
		player2.Draw(renderTarget);
		object1.Draw(renderTarget);
		SDL_RenderPresent(renderTarget);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderTarget);
	SDL_DestroyTexture(texture);

	texture = nullptr;
	window = nullptr;
	renderTarget = nullptr;

	IMG_Quit(); //Destroys the image window to free memory
	SDL_Quit(); //Ends SDL windows to free memory

	return 0;
}