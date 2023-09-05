#include <iostream>
#include <SDL.h>
#include <chrono>
#include <thread>

constexpr int gWINDOW_WEIGHT = 1152;
constexpr int gWINDOW_HEIGHT = 896;

int main(int argc, char *argv[])
{
    // 初始化SDL
    SDL_Init(SDL_INIT_VIDEO);
    // 创建一个 SDL 窗口
    SDL_Window *screen = SDL_CreateWindow("Hello SDL",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gWINDOW_WEIGHT, gWINDOW_HEIGHT, 0);
    // 创建 render
    SDL_Renderer *render = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
    int renderW = 0;
    int renderH = 0;
    SDL_GetRendererOutputSize(render, &renderW, &renderH);
    // 加载一张图片
    SDL_Surface *surface = SDL_LoadBMP("./hello_SDL2.bmp");
    if (surface == nullptr) {
        std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
        return -1;
    }
    int imgW = surface->w;
    int imgH = surface->h;
    // 由 surface 转换成 texture
    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
    // 释放 surface
    SDL_FreeSurface(surface);

    int tigerHeadx = 400;
    int tigerHeady = 200;
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        /* deal with event */
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT: {
                    quit = true;
                    break;
                }
            case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {
                            case SDLK_LEFT:  tigerHeadx--; break;
                            case SDLK_RIGHT: tigerHeadx++; break;
                            case SDLK_UP:    tigerHeady--; break;
                            case SDLK_DOWN:  tigerHeady++; break;

                            case SDLK_a: tigerHeadx--; break;
                            case SDLK_d: tigerHeadx++; break;
                            case SDLK_w: tigerHeady--; break;
                            case SDLK_s: tigerHeady++; break;
                            default:
                                break;
                        }
                    break;
            }
            default:
                break;
        }
        /* do your job */
        SDL_Rect sRect{0, 0, imgW, imgH};
        SDL_Rect dRect{0, 0, renderW, renderH};
        SDL_RenderCopy(render, texture, &sRect, &dRect);

        SDL_Rect d1Rect{tigerHeadx, tigerHeady, 400, 500};
        SDL_Rect s1Rect{300, 50, 400, 500}; // tigerhead img
        SDL_RenderCopy(render, texture, &s1Rect, &d1Rect);

        SDL_RenderPresent(render);
    }

    // 销毁 texture
    SDL_DestroyTexture(texture);
    // 销毁 render
    SDL_DestroyRenderer(render);
    // 销毁 SDL 窗口
    SDL_DestroyWindow(screen);
    // SDL 退出
    SDL_Quit();

	return 0;
}
