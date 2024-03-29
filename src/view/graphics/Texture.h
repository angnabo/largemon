/**
 * Testure class to load textures and font from files and represent them
 * as SDL textures to be rendered to the screen.
 *
 * Created by angelica on 22/10/17.
 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

#ifndef LARGEMON_LTEXTURE_H
#define LARGEMON_LTEXTURE_H

using namespace std;

class Texture {
public:
    Texture();

    ~Texture();

    void setSize(int, int);

    bool loadFromFile(SDL_Renderer *renderer, std::string path);

    void free();

    void render(SDL_Renderer *renderer, int x, int y);

    void renderSprite(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip);

    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    int getWidth();

    int getHeight();

    int getOriginalWidth();

    void setHidden(bool b) { hidden = b; }

    bool isHidden() { return hidden; }

    void setBlendMode(SDL_BlendMode blending);

    void setAlpha(Uint8 alpha);

    bool loadFont(SDL_Renderer *renderer, TTF_Font *gFont, std::string textureText, SDL_Color textColor);

private:
    SDL_Texture *mTexture;
    int mWidth;
    int mHeight;
    int originalWidth;
    int originalHeight;
    bool hidden = false;
};

#endif //LARGEMON_LTEXTURE_H
