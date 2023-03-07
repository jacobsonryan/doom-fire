#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 320 
#define HEIGHT 168 
#define NUM_COLORS 37

unsigned char rgb_pallete[NUM_COLORS][3] = {
    { 0x07,0x07,0x07 }, // near black
    { 0x1F,0x07,0x07 },
    { 0x2F,0x0F,0x07 },
    { 0x47,0x0F,0x07 },
    { 0x57,0x17,0x07 },
    { 0x67,0x1F,0x07 },
    { 0x77,0x1F,0x07 },
    { 0x8F,0x27,0x07 },
    { 0x9F,0x2F,0x07 },
    { 0xAF,0x3F,0x07 },
    { 0xBF,0x47,0x07 },
    { 0xC7,0x47,0x07 },
    { 0xDF,0x4F,0x07 },
    { 0xDF,0x57,0x07 },
    { 0xDF,0x57,0x07 },
    { 0xD7,0x5F,0x07 },
    { 0xD7,0x5F,0x07 },
    { 0xD7,0x67,0x0F },
    { 0xCF,0x6F,0x0F },
    { 0xCF,0x77,0x0F },
    { 0xCF,0x7F,0x0F },
    { 0xCF,0x87,0x17 },
    { 0xC7,0x87,0x17 },
    { 0xC7,0x8F,0x17 },
    { 0xC7,0x97,0x1F },
    { 0xBF,0x9F,0x1F },
    { 0xBF,0x9F,0x1F },
    { 0xBF,0xA7,0x27 },
    { 0xBF,0xA7,0x27 },
    { 0xBF,0xAF,0x2F },
    { 0xB7,0xAF,0x2F },
    { 0xB7,0xB7,0x2F },
    { 0xB7,0xB7,0x37 },
    { 0xCF,0xCF,0x6F },
    { 0xDF,0xDF,0x9F },
    { 0xEF,0xEF,0xC7 },
    { 0xFF,0xFF,0xFF }, // white
};

int fire_pixels[WIDTH * HEIGHT];
int buf[WIDTH * HEIGHT];

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture; 
int running = 1;

void setup() {
  window = SDL_CreateWindow(
      "DOOM FIRE",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      WIDTH * 4,
      HEIGHT * 4,
      0
  );
  renderer = SDL_CreateRenderer(window, -1, 0);

  for(int c = 0; c < WIDTH; c++) {
    fire_pixels[(HEIGHT - 1) * WIDTH + c] = NUM_COLORS - 1;
  }
}

void poll_events() {
  SDL_Event windowEvent;
  while(SDL_PollEvent(&windowEvent)) {
    if(windowEvent.type == SDL_QUIT) {
      running = 0;
    }
  }
}

void spread_fire(int from) {
  if(fire_pixels[from] == 0) {
    fire_pixels[from - WIDTH] = 0;
  } else {
    int random = rand() % 3;
    int to = from - random + 1;
    fire_pixels[to - WIDTH] = fire_pixels[from] - (random & 1);
  }
}


void do_fire() {
  for(int r = 1; r < HEIGHT; r++) {
    for(int c = 0; c < WIDTH; c++) {
      spread_fire(r * WIDTH + c);
    }
  }
}

void draw_fire() {
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
  for(int r = 0; r < HEIGHT; r++) {
    for(int c = 0; c < WIDTH; c++) {
      int index = fire_pixels[r * WIDTH + c];
      buf[r * WIDTH + c] = (255 << 24) | (rgb_pallete[index][0] << 16) | (rgb_pallete[index][1] << 8) | rgb_pallete[index][2];
    }
  }
  
  SDL_UpdateTexture(texture, NULL, buf, WIDTH * sizeof(Uint32));
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
  Uint32 frameStart;
  int fps = 60;
  int delay = 1000 / fps;
  int frameTime;

  setup();

  while(running) {
    frameStart = SDL_GetTicks();
    poll_events();
    do_fire();
    draw_fire();
    frameTime = SDL_GetTicks() - frameStart;
    if(delay > frameTime) {
      SDL_Delay(delay);
    }
  }
  return 0;
}
