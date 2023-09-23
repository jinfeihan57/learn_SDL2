#include <iostream>
#include <SDL.h>
#include <chrono>
#include <thread>
#include <string>
#include "SDL_image.h"

constexpr int gWINDOW_WEIGHT = 1152;
constexpr int gWINDOW_HEIGHT = 896;

constexpr int gFPS = 60;
constexpr Uint32 gFPS_TIME = 1000 / gFPS;

int main(int argc, char *argv[])
{
    int ret = 0;
    if ( ! argv[1] ) {
        std::cout << "Usage: ./main.exe *.gif" << std::endl;
        return -1;
    }
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
        return -1;
    }
    // 创建一个 SDL 窗口
    SDL_Window *screen = SDL_CreateWindow("Hello SDL",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gWINDOW_WEIGHT, gWINDOW_HEIGHT, 0);
    if (screen == nullptr) {
        std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
        return -1;
    }
    // 加载一张图片
    SDL_Surface *surfaceIcon = SDL_LoadBMP("./hello_SDL2.bmp");
    if (surfaceIcon == nullptr) {
        std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
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
        return -1;
    }

    // 加载 gif 动图
    int imgW = 0;
    int imgH = 0;
    IMG_Animation *animation = IMG_LoadAnimation(argv[1]);
    std::cout << __LINE__ << animation->count << std::endl;
    std::cout << __LINE__ << animation->delays[0] << std::endl;
    std::cout << __LINE__ << animation->w << std::endl;
    std::cout << __LINE__ << animation->h << std::endl;
    imgW = animation->w;
    imgH = animation->h;
    // surface -> texture
    SDL_Texture **textures = (SDL_Texture **)SDL_calloc(animation->count, sizeof(SDL_Texture*));
    if (!textures) {
        SDL_Log("Couldn't allocate textures\n");
        IMG_FreeAnimation(animation);
        return -1;
    }
    for (int j = 0; j < animation->count; ++j) {
        textures[j] = SDL_CreateTextureFromSurface(render, animation->frames[j]);
    }

    int tigerHeadx = 400;
    int tigerHeady = 200;
    bool quit = false;
    SDL_Event event;
    int times = 0;
    int count = 0;
    while (!quit) {
        std::chrono::time_point start = std::chrono::high_resolution_clock::now();
        Uint32 startMs = SDL_GetTicks();
        /* deal with event */
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT: {
                quit = true;
                break;
            }
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym)
                    {
                        case SDLK_LEFT:  tigerHeadx-=15; break;
                        case SDLK_RIGHT: tigerHeadx+=15; break;
                        case SDLK_UP:    tigerHeady-=15; break;
                        case SDLK_DOWN:  tigerHeady+=15; break;

                        default:
                            break;
                    }
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                if ((event.button.button == SDL_BUTTON_LEFT) && (event.button.clicks == 2)) { // 双击
                    tigerHeadx = event.motion.x;
                    tigerHeady = event.motion.y;
                }
                break;
            }
            default:
                break;
        }
        /* do your job */
        // 清屏
        SDL_SetRenderDrawColor(render, 135, 206, 0, 0xFF );
        SDL_RenderClear(render);
        // 绘制
        SDL_Rect dRect{tigerHeadx, tigerHeady, imgW, imgH};
        times++;
        if ((times % 3) == 0) {
            count++;
            if(count >= animation->count){
                count = 0;
            }
        }
        SDL_RenderCopy(render, textures[count], nullptr, &dRect);
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

    // 销毁 textures animation
    for (int j = 0; j < animation->count; ++j) {
            SDL_DestroyTexture(textures[j]);
    }
    IMG_FreeAnimation(animation);
    SDL_free(textures);
    
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
