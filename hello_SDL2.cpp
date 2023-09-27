#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

#include "SDL.h"
#include "SDL_ttf.h"

constexpr int gWINDOW_WEIGHT = 1152;
constexpr int gWINDOW_HEIGHT = 896;

constexpr int gFPS = 60;
constexpr Uint32 gFPS_TIME = 1000 / gFPS;
constexpr int gDEFAULT_PTSIZE = 70;
int main(int argc, char *argv[])
{
    int ret = 0;

    if ( ! argv[1] ) {
        std::cout << "Usage: ./main.exe *.ttf (中文字库)" << std::endl;
        return -1;
    }
    // 初始化SDL
    ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n",SDL_GetError());
        return -1;
    }
    if (TTF_Init() < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF: %s\n",SDL_GetError());
        SDL_Quit();
        return(2);
    }
    // 创建一个 SDL 窗口
    SDL_Window *screen = SDL_CreateWindow("Hello SDL",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gWINDOW_WEIGHT, gWINDOW_HEIGHT, 0);
    if (screen == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't Create SDL Window: %s\n",SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    // 加载一张图片
    SDL_Surface *surfaceIcon = SDL_LoadBMP("./hello_SDL2.bmp");
    if (surfaceIcon == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't SDL_LoadBMP: %s\n",SDL_GetError());
        SDL_DestroyWindow(screen);
        TTF_Quit();
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
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't SDL_CreateRenderer: %s\n",SDL_GetError());
        SDL_DestroyWindow(screen);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    int ptsize = gDEFAULT_PTSIZE;
    TTF_Font *font = TTF_OpenFont(argv[1], ptsize);
    if (font == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't TTF_OpenFont: %s\n",SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(screen);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Color forecol = {0, 0, 0, 0xff};
    SDL_Color backcol = {0xff, 0xff, 0xff, 0xff};
    // SDL_Log("Hello SDL! TTF_SetFontSize: %d\n", TTF_SetFontSize(font, 200)); // 在TTF_Render* 字符之前设置才生效，或者每次修改ptsize都重新执行 TTF_Render*
    // TTF_SetFontStyle(font, TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE | TTF_STYLE_STRIKETHROUGH);
    SDL_Surface *surface = TTF_RenderText_Solid(font, "Hello SDL!", forecol);
    // SDL_Surface *surface = TTF_RenderText_Shaded(font, "Hello SDL!", forecol, backcol);
    // SDL_Surface *surface = TTF_RenderText_Blended(font, "Hello SDL!", forecol);
    // SDL_Surface *surface = TTF_RenderText_LCD(font, "Hello SDL!", forecol, backcol);
    // SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, "Hello SDL!", forecol, 100);
    if (surface == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't TTF_Render***: %s\n",SDL_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(screen);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Log("Hello SDL! surface format: %d\n", surface->format->format);
    SDL_Log("Hello SDL! surface w: %d\n", surface->w);
    SDL_Log("Hello SDL! surface h: %d\n", surface->h);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
    if (texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't SDL_CreateTextureFromSurface: %s\n",SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(screen);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    int tigerHeadx = 400;
    int tigerHeady = 200;
    int helloWidth = surface->w;
    int helloHigh = surface->h;
    SDL_Rect helloRect = {tigerHeadx, tigerHeady, helloWidth, helloHigh};
    SDL_FreeSurface(surface);

    int keyboardEvent = 0;
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        std::chrono::time_point start = std::chrono::high_resolution_clock::now();
        Uint32 startMs = SDL_GetTicks();
        /* deal with event */
        const Uint8 *keyStatus = SDL_GetKeyboardState(&keyboardEvent);
        if (keyStatus[SDL_SCANCODE_RIGHT] == SDL_PRESSED) {
            tigerHeadx += 3;
        }
        if (keyStatus[SDL_SCANCODE_LEFT] == SDL_PRESSED) {
            tigerHeadx -= 3;
        }
        if (keyStatus[SDL_SCANCODE_DOWN] == SDL_PRESSED) {
            tigerHeady += 3;
        }
        if (keyStatus[SDL_SCANCODE_UP] == SDL_PRESSED) {
            tigerHeady -= 3;
        }
        if (keyStatus[SDL_SCANCODE_W] == SDL_PRESSED) {
            helloHigh += 3;
        }
        if (keyStatus[SDL_SCANCODE_S] == SDL_PRESSED) {
            helloHigh -= 3;
        }
        if (keyStatus[SDL_SCANCODE_A] == SDL_PRESSED) {
            helloWidth -= 3;
        }
        if (keyStatus[SDL_SCANCODE_D] == SDL_PRESSED) {
            helloWidth += 3;
        }
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        /* do your job */
        // 清屏
        SDL_SetRenderDrawColor(render, 65, 167, 225, 0xFF );
        SDL_RenderClear(render);
        // 绘制
        helloRect.x = tigerHeadx;
        helloRect.y = tigerHeady;
        helloRect.w = helloWidth;
        helloRect.h = helloHigh;
        SDL_RenderCopy(render, texture, nullptr, &helloRect);

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
    // 关闭 font
    TTF_CloseFont(font);
    // 销毁 render
    SDL_DestroyRenderer(render);
    // 销毁 SDL 窗口
    SDL_DestroyWindow(screen);
    // TTF 退出
    TTF_Quit();
    // SDL 退出
    SDL_Quit();

    return 0;
}
