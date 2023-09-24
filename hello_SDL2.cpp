#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"

constexpr int gWINDOW_WEIGHT = 1152;
constexpr int gWINDOW_HEIGHT = 896;

constexpr int gFPS = 60;
constexpr Uint32 gFPS_TIME = 1000 / gFPS;

int main(int argc, char *argv[])
{
    int ret = 0;

    // 初始化SDL
    ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret != 0) {
        std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
        return -1;
    }
    // 初始化IMG
    ret = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    if (ret != (IMG_INIT_PNG | IMG_INIT_JPG)) {
        std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
        SDL_Quit();
        return -1;
    }
    // 创建一个 SDL 窗口
    SDL_Window *screen = SDL_CreateWindow("Hello SDL",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gWINDOW_WEIGHT, gWINDOW_HEIGHT, 0);
    if (screen == nullptr) {
        std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }
    // 加载一张图片
    SDL_Surface *surfaceIcon = SDL_LoadBMP("./hello_SDL2.bmp");
    if (surfaceIcon == nullptr) {
        std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
        SDL_DestroyWindow(screen);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }
    // 设置应用图标
    SDL_SetWindowIcon(screen, surfaceIcon);
    // 释放 surfaceIcon
    SDL_FreeSurface(surfaceIcon);
    // 创建 render
    SDL_Renderer *render = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
    if (render == nullptr) {
        std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
        SDL_DestroyWindow(screen);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // 加载 texture
    SDL_Texture *texture = IMG_LoadTexture(render, "adventurer-sheet.png");
    if (texture == nullptr) {
        std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(screen);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }
    int maxDuration = 150;
    int widthSpr = 50;
    int heightSpr = 37;
    int imgW = widthSpr * 3;
    int imgH = heightSpr * 3;

    std::vector<std::pair<int, int>> idle1  { {0, 0}, {0, 1}, {0, 2}, {0, 3} };
    std::vector<std::pair<int, int>> crouch { {0, 4}, {0, 5}, {0, 6}, {1, 0} };
    std::vector<std::pair<int, int>> run    { {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6} };
    std::vector<std::pair<int, int>> jump   { {2, 0}, {2, 1}, {2, 2}, {2, 3} };
    std::vector<std::pair<int, int>> mid    { {2, 4}, {2, 5}, {2, 6}, {3, 0} };
    std::vector<std::pair<int, int>> fall   { {3, 1}, {3, 2} };
    std::vector<std::pair<int, int>> slide  { {3, 3}, {3, 4}, {3, 5}, {3, 6}, {4, 0} };
    std::vector<std::pair<int, int>> grab   { {4, 1}, {4, 2}, {4, 3}, {4, 4}};
    std::vector<std::pair<int, int>> climb  { {4, 5}, {4, 6}, {5, 0}, {5, 1}, {5, 2} };
    std::vector<std::pair<int, int>> idle2  { {5, 3}, {5, 4}, {5, 5}, {5, 6} };
    std::vector<std::pair<int, int>> attack1{ {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4} };
    std::vector<std::pair<int, int>> attack2{ {6, 5}, {6, 6}, {7, 0}, {7, 1}, {7, 2}, {7, 3} };
    std::vector<std::pair<int, int>> attack3{ {7, 4}, {7, 5}, {7, 6}, {8, 0}, {8, 1}, {8, 2} };
    std::vector<std::pair<int, int>> hurt   { {8, 3}, {8, 4}, {8, 5} };
    std::vector<std::pair<int, int>> die    { {8, 6}, {9, 0}, {9, 1}, {9, 2}, {9, 3}, {9, 4}, {9, 5} };
    std::vector<std::pair<int, int>> jump2  { {9, 6}, {10, 0}, {10, 1} };
    int index = 0;
    std::vector<std::pair<int, int>> current = idle1;
    int currentTime = SDL_GetTicks();
    int spriteChangeTime = currentTime + maxDuration;
    SDL_bool faceRight = SDL_TRUE;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    int keyboardEvent = 0;
    int tigerHeadx = 400;
    int tigerHeady = 200;
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        std::chrono::time_point start = std::chrono::high_resolution_clock::now();
        Uint32 startMs = SDL_GetTicks();
        /* deal with event */
        const Uint8 *keyStatus = SDL_GetKeyboardState(&keyboardEvent);
        // std::cout << keyboardEvent << std::endl;
        if (keyStatus[SDL_SCANCODE_Q] == SDL_PRESSED) {
            index = 0;
            current = idle1;
        } else if (keyStatus[SDL_SCANCODE_W] == SDL_PRESSED) {
            index = 0;
            current = crouch;
        } else if (keyStatus[SDL_SCANCODE_E] == SDL_PRESSED) {
            index = 0;
            current = run;
        } else if (keyStatus[SDL_SCANCODE_SPACE] == SDL_PRESSED) {
            index = 0;
            current = jump;
        } else if (keyStatus[SDL_SCANCODE_T] == SDL_PRESSED) {
            index = 0;
            current = mid;
        } else if (keyStatus[SDL_SCANCODE_Y] == SDL_PRESSED) {
            index = 0;
            current = fall;
        } else if (keyStatus[SDL_SCANCODE_U] == SDL_PRESSED) {
            index = 0;
            current = slide;
        } else if (keyStatus[SDL_SCANCODE_I] == SDL_PRESSED) {
            index = 0;
            current = grab;
        } else if (keyStatus[SDL_SCANCODE_O] == SDL_PRESSED) {
            index = 0;
            current = climb;
        } else if (keyStatus[SDL_SCANCODE_P] == SDL_PRESSED) {
            index = 0;
            current = idle2;
        } else if (keyStatus[SDL_SCANCODE_A] == SDL_PRESSED) {
            index = 0;
            current = attack1;
        } else if (keyStatus[SDL_SCANCODE_S] == SDL_PRESSED) {
            index = 0;
            current = attack2;
        } else if (keyStatus[SDL_SCANCODE_D] == SDL_PRESSED) {
            index = 0;
            current = attack3;
        } else if (keyStatus[SDL_SCANCODE_F] == SDL_PRESSED) {
            index = 0;
            current = hurt;
        } else if (keyStatus[SDL_SCANCODE_G] == SDL_PRESSED) {
            index = 0;
            current = die;
        } else if (keyStatus[SDL_SCANCODE_H] == SDL_PRESSED) {
            index = 0;
            current = jump2;
        }
        if (keyStatus[SDL_SCANCODE_RIGHT] == SDL_PRESSED) {
            tigerHeadx += 1;
            flip = SDL_FLIP_NONE;
        }
        if (keyStatus[SDL_SCANCODE_LEFT] == SDL_PRESSED) {
            tigerHeadx -= 1;
            flip = SDL_FLIP_HORIZONTAL;
        }
        if (keyStatus[SDL_SCANCODE_DOWN] == SDL_PRESSED) {
            tigerHeady += 1;
        }
        if (keyStatus[SDL_SCANCODE_UP] == SDL_PRESSED) {
            tigerHeady -= 1;
        }
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        /* do your job */
        // 清屏
        SDL_SetRenderDrawColor(render, 135, 206, 0, 0xFF );
        SDL_RenderClear(render);
        // 绘制
        SDL_Rect sRect = {current[index].second * widthSpr, current[index].first * heightSpr, widthSpr, heightSpr};
        SDL_Rect dRect = {tigerHeadx, tigerHeady, imgW, imgH};
        SDL_RenderCopyEx(render, texture, &sRect, &dRect, 0, nullptr, flip);

        currentTime = SDL_GetTicks();
        if (currentTime > spriteChangeTime) {
            spriteChangeTime += maxDuration;
            index++;
            if ((index % current.size()) == 0) {
                index = 0;
                current = run;
            }
        }
        // 显示
        SDL_RenderPresent(render);

        // 调整帧率
        Uint32 endMs = SDL_GetTicks();
        Uint32 consumeTime = endMs - startMs;
        SDL_Delay(consumeTime >= gFPS_TIME ? 0 : (gFPS_TIME - consumeTime)); // 调整帧率

        std::chrono::time_point end = std::chrono::high_resolution_clock::now();
        int fps = std::chrono::seconds(1) / (end - start);
        // 修改应用标题
        SDL_SetWindowTitle(screen, (std::string("fps: ") + std::to_string(fps)).c_str());
    }

    // 销毁 texture
    SDL_DestroyTexture(texture);
    // 销毁 render
    SDL_DestroyRenderer(render);
    // 销毁 SDL 窗口
    SDL_DestroyWindow(screen);
    // IMG 退出
    IMG_Quit();
    // SDL 退出
    SDL_Quit();

    return 0;
}
