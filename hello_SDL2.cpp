#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

constexpr int gWINDOW_WEIGHT = 1152;
constexpr int gWINDOW_HEIGHT = 896;

constexpr int gFPS = 60;
constexpr Uint32 gFPS_TIME = 1000 / gFPS;

constexpr int gDEFAULT_PTSIZE = 70;
const std::string gSTRING = {"正在播放: "};  // UTF8 编码

int main(int argc, char *argv[])
{
    int ret = 0;
    if ( ! argv[1] ) {
        std::cout << "Usage: ./main.exe *.ttf(中文字库) *.mp3(音频文件) *.wav(音效文件)" << std::endl;
        return -1;
    }
    // 初始化SDL
    ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if (ret != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n",SDL_GetError());
        return -1;
    }
    if (TTF_Init() < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF: %s\n",TTF_GetError());
        SDL_Quit();
        return -1;
    }
    ret = Mix_Init(MIX_INIT_MP3);
    if ( ret != MIX_INIT_MP3) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF: %s\n",Mix_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    ret = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    if (ret != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF: %s\n",Mix_GetError());
        Mix_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;

    }
    Mix_Music *music = Mix_LoadMUS(argv[2]);
    if (music == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF: %s\n",Mix_GetError());
        Mix_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    Mix_Chunk *scratch = Mix_LoadWAV(argv[3]);
    if (scratch == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF: %s\n",Mix_GetError());
        Mix_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
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
    std::string displayText = gSTRING + std::string(argv[2]);
    SDL_Color forecol = {0, 0, 0, 0xff};
    SDL_Color backcol = {0xff, 0xff, 0xff, 0xff};
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, displayText.c_str(), forecol);
    if (surface == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't TTF_Render***: %s\n",SDL_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(screen);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

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
    int tigerHeadx = 0;
    int tigerHeady = 0;
    int textsWidth = surface->w;
    int textsHigh = surface->h;
    SDL_FreeSurface(surface);
    int volume = MIX_MAX_VOLUME;
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
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            quit = 1;
            continue;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_a:
                    Mix_PlayChannel(-1, scratch, 0);
                    break;
                case SDLK_p:
                    if ( Mix_PlayingMusic() == 0 ) {        // 判断是否正在播放
                        //Play the music
                        // Mix_PlayMusic(music, -1);        // 从头播放音乐
                        Mix_FadeInMusic(music, -1, 2000);   // 渐入音乐
                    } else if (Mix_PausedMusic() == 1) {    // 判断是否暂停
                        Mix_ResumeMusic();                  // 继续播放
                    }
                    break;
                case SDLK_SPACE:
                    Mix_PauseMusic();                       // 暂停播放
                    break;
                case SDLK_s:
                    Mix_HaltMusic();                    // 停止播放
                    break;
                case SDLK_d: {
                    volume -= 20;
                    volume = volume < 0 ? 0 : volume;
                    int a = Mix_VolumeMusic(volume);
                    std::cout << "volume: " << a << std::endl;
                    break;
                }
                case SDLK_u: {
                    volume += 20;
                    volume = volume > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : volume;
                    int a = Mix_VolumeMusic(volume);
                    std::cout << "volume: " << a << std::endl;
                    break;
                }
                default:
                    break;
            }
        }
        /* do your job */
        // 清屏
        SDL_SetRenderDrawColor(render, 65, 167, 225, 0xFF);
        SDL_RenderClear(render);
        // 绘制
        // 绘画 点/线/面，copy texture 等
        SDL_Rect textRenderRect = {tigerHeadx, tigerHeady, textsWidth, textsHigh};
        SDL_SetTextInputRect(&textRenderRect); // 将输入法UI设置到，指定的矩形附近
        SDL_SetRenderDrawColor(render, 65, 57, 225, 0x00); // 设置输入框的颜色
        SDL_RenderFillRect(render, &textRenderRect);
        SDL_RenderCopyEx(render, texture, nullptr, &textRenderRect, 0, nullptr, SDL_FLIP_NONE);

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
    Mix_FadeOutMusic(500); // 音乐效果渐渐消失
    SDL_Delay(500);
    // 销毁 texture
    SDL_DestroyTexture(texture);
    // 关闭 font
    TTF_CloseFont(font);
    // 销毁 render
    SDL_DestroyRenderer(render);
    // 销毁 SDL 窗口
    SDL_DestroyWindow(screen);
    Mix_CloseAudio();
    Mix_FreeChunk(scratch);
    Mix_FreeMusic(music);
    Mix_Quit();
    // TTF 退出
    TTF_Quit();
    // SDL 退出
    SDL_Quit();

    return 0;
}