#include "inc/includes.hxx"

sf::RenderWindow window(sf::VideoMode(1280, 720),"",sf::Style::Close);
sf::Font font;
sf::Music music;
sf::RectangleShape fadeRect;
const u8 fontSize = 24;

/// @brief Fades the screen in or out.
/// @param speed Speed of the fade. Bigger value = faster fade.
/// @param direction Direction of the fade. True for in, false for out.
void screenFade(float speed, bool direction)
{
    float alpha = fadeRect.getFillColor().a;
    if (!direction)
    {
        alpha += speed;
        if (alpha >= 255)
        {
            alpha = 255;
        }
    }
    else
    {
        alpha -= speed;
        if (alpha <= 0)
        {
            alpha = 0;
        }
    }
    fadeRect.setFillColor(sf::Color(0,0,0,alpha));
}

/// @brief Renders an Option structure.
/// @param option Option structure.
/// @param length Amount of items inside the option structure.
void drawMenu(const Option* option, u16 length)
{
    for (u16 i = 0; i < length; i++)
    {
        Option o = option[i];
        sf::Text optionLabel;
        optionLabel.setFont(font);
        optionLabel.setString(o.label);
        optionLabel.setPosition(o.x*fontSize,o.y*fontSize);
        optionLabel.setCharacterSize(fontSize);
        optionLabel.setOutlineThickness(3.5);
        window.draw(optionLabel);
        sf::Text selectedLabel;
        selectedLabel.setFont(font);
        selectedLabel.setString(option[*menuIndex].label);
        selectedLabel.setPosition(option[*menuIndex].x*fontSize,option[*menuIndex].y*fontSize);
        selectedLabel.setCharacterSize(fontSize);
        selectedLabel.setOutlineThickness(3.5);
        selectedLabel.setOutlineColor(sf::Color(0x00CC66FF));
        window.draw(selectedLabel);
    }
}

/// @brief Fades music in or out.
/// @param direction False = in, True = out
/// @param speed Smaller = slower
void fadeMusic(bool direction, float speed)
{
    float volume = music.getVolume();
    if (direction)
    {
        volume -= speed;
        if (volume <= 0)
        {
            volume = 0;
        }
    }
    else
    {
        volume += speed;
        if (volume >= 100)
        {
            volume = 100;
        }
    }
    music.setVolume(volume);
}