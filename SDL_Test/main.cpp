//
//  main.cpp
//  SDL_Test
//
//  Created by henry briseno on 6/8/17.
//  Copyright © 2017 henry briseno. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "Cleanup.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 40;

void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if (texture == nullptr) {
        logSDLError(std::cout,"LoadTexture");
    }
    return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;
    destination.w = w;
    destination.h = h;
    SDL_RenderCopy(ren, tex, NULL, &destination);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
    int w, h;
    SDL_QueryTexture(tex,NULL, NULL, &w, &h);
    renderTexture(tex, ren, x, y, w, h);
}

int main(int, char**)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }
    
    SDL_Window *window = SDL_CreateWindow("Lesson 2", 100, 100, SCREEN_WIDTH,
                                          SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        SDL_Quit();
        return 1;
    }
    
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        logSDLError(std::cout, "IMG_Init");
        SDL_Quit();
        return 1;
    }
    
    SDL_Texture *background = loadTexture("background.png", renderer);
    SDL_Texture *image = loadTexture("image.png", renderer);
    if (background == nullptr || image == nullptr) {
        cleanup(background, image, renderer, window);
        SDL_Quit();
        return 1;
    }
    
    SDL_Event e;
    bool quit =false;
    while(!quit)
    {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                quit = true;
            }
        }
    SDL_RenderClear(renderer);
        int xTiles = SCREEN_WIDTH / TILE_SIZE;
        int yTiles = SCREEN_HEIGHT / TILE_SIZE;
        for (int i = 0; i < xTiles * yTiles; ++i) {
            int x = i % xTiles;
            int y = i / xTiles;
            renderTexture(background, renderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
    
    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;
    renderTexture(image, renderer, x, y);
    SDL_RenderPresent(renderer);
    }
    cleanup(background, image, renderer, window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
