#include <iostream>
#include <SDL.h>
#include <chrono>
#include <thread>

// using namespace std::literals::chrono_literals; std::this_thread::sleep_for(2000ms);

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
    
    // 加载一张图片
    SDL_Surface *surface = SDL_LoadBMP("./hello_SDL2.bmp");
    if (surface == nullptr) {
        std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
        return -1;
    }
    // 由 surface 转换成 texture
    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
    // 释放 surface
    SDL_FreeSurface(surface);
    // 睡眠 2s ，来保持窗口
    // std::chrono::milliseconds ms{2000};
    // std::this_thread::sleep_for(ms);
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        /* deal with event */
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT: {
                    // std::cout << __LINE__ << std::endl;
                    quit = true;
                    break;
                }

            default:
                break;
        }
        /* do your job */
        SDL_Rect sRect{0, 0, 500, 500};
        SDL_Rect s1Rect{500, 0, 500, 500};
        SDL_RenderCopy(render, texture, &sRect, &sRect);
        SDL_RenderCopy(render, texture, &sRect, &s1Rect);

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
