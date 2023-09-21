#include <iostream>
#include <SDL.h>
#include <chrono>
#include <thread>
#include <string>

constexpr int gWINDOW_WEIGHT = 1152;
constexpr int gWINDOW_HEIGHT = 896;

constexpr int gFPS = 60;
constexpr Uint32 gFPS_TIME = 1000 / gFPS;

int main(int argc, char *argv[])
{
    // 初始化SDL
    SDL_Init(SDL_INIT_VIDEO); // SDL_INIT_TIMER?
    // 创建一个 SDL 窗口
    SDL_Window *screen = SDL_CreateWindow("Hello SDL",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gWINDOW_WEIGHT, gWINDOW_HEIGHT, 0);
    
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
    SDL_Renderer *render = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);  // SDL_RENDERER_PRESENTVSYNC
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

    Uint8 alph = 127;
    int tigerHeadx = 400;
    int tigerHeady = 200;
    bool quit = false;
    SDL_Event event;
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
                        case SDLK_LEFT:  tigerHeadx--; break;
                        case SDLK_RIGHT: tigerHeadx++; break;
                        case SDLK_UP:    tigerHeady--; break;
                        case SDLK_DOWN:  tigerHeady++; break;

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
        SDL_SetRenderDrawColor(render, 135, 206, 235, 0xFF );
        SDL_RenderClear(render);
        // 绘制

        SDL_SetRenderDrawColor(render, 255, 0, 0, 0xFF);
        SDL_RenderDrawLine(render, 0, 0, 500, 500);

        // SDL_Point points[] = {{500, 500}, {200, 300}, {300, 300}, {400, 200}, {500, 500}};
        // SDL_RenderDrawLines(render, points, sizeof(points) / sizeof(SDL_Point));

        // SDL_SetRenderDrawColor(render, 0, 0, 255, 0xFF);
        // SDL_Rect aRect{200, 200, 50, 70};
        // SDL_RenderDrawRect(render, &aRect);

        // SDL_SetRenderDrawColor(render, 0, 0, 255, 0xFF);
        // SDL_Rect rects[] = {{400, 300, 100, 200}, {401, 301, 98, 198}, {402, 302, 96, 196},
        //                     {403, 303, 94, 194}, {404, 304, 92, 192}};
        // SDL_RenderDrawRects(render, rects, sizeof(rects) / sizeof(SDL_Rect));

        // SDL_SetRenderDrawColor(render, 127, 0, 255, 0xFF);
        // SDL_Rect bRect{300, 200, 50, 70};
        // SDL_RenderFillRect(render, &bRect);

        // SDL_SetRenderDrawColor(render, 255, 0, 0, 0xFF);
        // for(int i = 500; i < 550; i++) {
        //     SDL_RenderDrawPoint(render, i, i+1);  // SDL_RenderDrawPoints
        // }

        // 显示
        SDL_RenderPresent(render);

        // 调整帧率
        Uint32 endMs = SDL_GetTicks();
        Uint32 consumeTime = endMs - startMs;
        SDL_Delay(consumeTime >= gFPS_TIME ? 0 : (gFPS_TIME - consumeTime)); // 调整帧率

        std::chrono::time_point end = std::chrono::high_resolution_clock::now();
        int fps = std::chrono::seconds(1) / (end - start);
        std::cout << "FPS:" << fps << std::endl;
        // 修改应用标题
        SDL_SetWindowTitle(screen, (std::string("fps: ") + std::to_string(fps)).c_str());
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
