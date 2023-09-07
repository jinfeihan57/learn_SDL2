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

    // 睡眠 2s ，来保持窗口
    // std::chrono::milliseconds ms{2000};
    // std::this_thread::sleep_for(ms);

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT : {
                quit = true;
                break;
            }

            default:
                break;
        }
    }
    // 销毁 SDL 窗口
    SDL_DestroyWindow(screen);
    // SDL 退出
    SDL_Quit();

	return 0;
}
