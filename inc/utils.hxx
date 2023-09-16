#pragma once

#include "includes.hxx"

extern sf::RenderWindow window;
extern sf::Font font;
extern sf::Music music;
extern sf::RectangleShape fadeRect;
extern const u8 fontSize;
extern sf::SoundBuffer sbHvr;
extern sf::Sound sndHvr;
extern sf::SoundBuffer sbCnf;
extern sf::Sound sndCnf;
extern const float volFadeSpeed;

void screenFade(float speed, bool direction);
void drawMenu(const structs::Option* option, u16 length);
void fadeMusic(bool direction, float speed);