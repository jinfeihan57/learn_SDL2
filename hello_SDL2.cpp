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
    // 睡眠 2s ，来保持窗口
    // std::chrono::milliseconds ms{2000};
    // std::this_thread::sleep_for(ms);
    int tigerHeadx = 400;
    int tigerHeady = 200;
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
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym)
                    {
                        case SDLK_LEFT:  tigerHeadx--; break;
                        case SDLK_RIGHT: tigerHeadx++; break;
                        case SDLK_UP:    tigerHeady--; break;
                        case SDLK_DOWN:  tigerHeady++; break;

                        case SDLK_a:  tigerHeadx--; break;
                        case SDLK_d: tigerHeadx++; break;
                        case SDLK_w:    tigerHeady--; break;
                        case SDLK_s:  tigerHeady++; break;
                    }
                break;
            }
            case SDL_MOUSEMOTION: {
                // 获取鼠标左边方法1
                // tigerHeadx = event.motion.x;
                // tigerHeady = event.motion.y;
                // 获取鼠标左边方法2
                SDL_GetMouseState(&tigerHeadx, &tigerHeady);
                if (event.motion.state != 0) { // 按住鼠标拖动
                    std::cout << __LINE__ << ": " << event.motion.state << std::endl;
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                // if ((event.button.button == SDL_BUTTON_LEFT) && (event.button.clicks == 2)) { // 双击
                //     std::cout << __LINE__ << std::endl;
                //     tigerHeadx = event.motion.x;
                //     tigerHeady = event.motion.y;
                // }
                // if (event.button.button == SDL_BUTTON_MIDDLE) {
                //     std::cout << __LINE__ << std::endl;
                //     tigerHeadx = event.motion.x;
                //     tigerHeady = event.motion.y;
                // }
                // if (event.button.button == SDL_BUTTON_RIGHT) {
                //     std::cout << __LINE__ << std::endl;
                //     tigerHeadx = event.motion.x;
                //     tigerHeady = event.motion.y;
                // }
                // if (event.button.button == SDL_BUTTON_X1) {
                //     std::cout << __LINE__ << std::endl;
                //     tigerHeadx = event.motion.x;
                //     tigerHeady = event.motion.y;
                // }
                // if (event.button.button == SDL_BUTTON_X2) {
                //     std::cout << __LINE__ << std::endl;
                //     tigerHeadx = event.motion.x;
                //     tigerHeady = event.motion.y;
                // }
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
        SDL_Rect s1Rect{300, 50, 400, 500};
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
