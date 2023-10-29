#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

#include "SDL.h"

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
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n",SDL_GetError());
        return -1;
    }
    // 创建一个 SDL 窗口
    SDL_Window *screen = SDL_CreateWindow("Hello SDL",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gWINDOW_WEIGHT, gWINDOW_HEIGHT, 0);
    if (screen == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't Create SDL Window: %s\n",SDL_GetError());
        SDL_Quit();
        return -1;
    }
    // 加载一张图片
    SDL_Surface *surfaceIcon = SDL_LoadBMP("./hello_SDL2.bmp");
    if (surfaceIcon == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't SDL_LoadBMP: %s\n",SDL_GetError());
        SDL_DestroyWindow(screen);
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
        SDL_Quit();
        return -1;
    }

    int tigerHeadx = 0;
    int tigerHeady = 0;
    int partTextureW = gWINDOW_WEIGHT;
    int partTextureH = gWINDOW_HEIGHT;
    SDL_Texture *partTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888, 
                                                 SDL_TEXTUREACCESS_STREAMING, partTextureW, partTextureH);
    int32_t *pixels = new int32_t[partTextureW * partTextureH];
    int clickX = 0;
    int clickY = 0;
    int keyboardEvent = 0;
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        std::chrono::time_point start = std::chrono::high_resolution_clock::now();
        Uint32 startMs = SDL_GetTicks();
        /* deal with event */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            if (event.type == SDL_MOUSEMOTION) {
                if(event.motion.state == 1) {
                    clickX = event.motion.x > partTextureW ? 0 : event.motion.x;
                    clickY = event.motion.y > partTextureH ? 0 : event.motion.y;
                }
            }
        }
        /* do your job */
        // 清屏
        SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear(render);
        // 将pixels切换到texture
        int pitch= 0;
        // 读取显存中的像素信息到 pixels
        SDL_LockTexture(partTexture, nullptr, reinterpret_cast<void **>(&pixels), &pitch);
        // 绘制像素
        pixels[clickY * partTextureW + clickX] = 0xFF00FFFF;
        // 将内存中的 pixels 更新到显存
        SDL_UnlockTexture(partTexture);

        SDL_Point point = {0, 0};
        // 将已经渲染好的texture，渲染到窗口
        SDL_RenderCopyEx(render, partTexture, nullptr, nullptr, 0, &point, SDL_FLIP_NONE);

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
    delete [] pixels;
    // 销毁 texture
    SDL_DestroyTexture(partTexture);
    // 销毁 render
    SDL_DestroyRenderer(render);
    // 销毁 SDL 窗口
    SDL_DestroyWindow(screen);
    // SDL 退出
    SDL_Quit();

    return 0;
}