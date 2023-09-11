#pragma once

#include "includes.hxx"

extern sf::RenderWindow window;
extern sf::Font font;
extern sf::Music music;
extern sf::RectangleShape fadeRect;
extern const u8 fontSize;

void screenFade(float speed, bool direction);
void drawMenu(const Option* option, u16 length);
void fadeMusic(bool direction, float speed);