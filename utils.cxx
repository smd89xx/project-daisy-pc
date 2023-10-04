#include "inc/includes.hxx"

sf::RenderWindow window(sf::VideoMode(1280, 720),"",sf::Style::Close);
sf::Font font;
sf::Music music;
sf::RectangleShape fadeRect;
const types::u8 fontSize = 24;
sf::SoundBuffer sbHvr;
sf::Sound sndHvr;
sf::SoundBuffer sbCnf;
sf::Sound sndCnf;
sf::SoundBuffer sbBack;
sf::Sound sndBack;
sf::Text templateText;
const float volFadeSpeed = 1.75;
const float scrnFadeSpeed = 2;
const sf::Color playerColors[] = {sf::Color(0xAA22EEFF),sf::Color(0x00CC66FF)};
const types::u8 volMin = 0;
const types::u8 volMax = 100;
const types::u8 fadeDark = 0xFF;
const types::u8 fadeLight = 0x00;

/// @brief Fades the screen in or out.
/// @param speed Speed of the fade. Bigger value = faster fade.
/// @param direction Direction of the fade. True for in, false for out.
/// @param fadeTarget Alpha value (0 - 255) that the fade routine stops at.
void screenFade(float speed, bool direction, float fadeTarget)
{
    float alpha = fadeRect.getFillColor().a;
    if (!direction)
    {
        alpha += speed;
        if (alpha >= fadeTarget)
        {
            alpha = fadeTarget;
        }
    }
    else
    {
        alpha -= speed*3;
        if (alpha <= fadeTarget)
        {
            alpha = fadeTarget;
        }
    }
    fadeRect.setFillColor(sf::Color(0,0,0,alpha));
}

/// @brief Renders an Option structure.
/// @param option Option structure.
/// @param length Amount of items inside the option structure.
void drawMenu(const structs::Option* option, types::u8 length)
{
    for (types::u8 i = 0; i < length; i++)
    {
        structs::Option o(option[i]);
        sf::Text optionLabel(templateText);
        optionLabel.setString(o.label);
        optionLabel.setPosition(pixelToTile(o.x),pixelToTile(o.y));
        window.draw(optionLabel);
        sf::Text selectedLabel(templateText);
        selectedLabel.setString(option[menuIndex].label);
        selectedLabel.setPosition(pixelToTile(option[menuIndex].x),pixelToTile(option[menuIndex].y));
        selectedLabel.setOutlineThickness(3.5);
        selectedLabel.setOutlineColor(playerColors[player]);
        window.draw(selectedLabel);
    }
}

/// @brief Fades music in or out.
/// @param direction False = in, True = out
/// @param speed Smaller = slower
/// @param targetVolume The volume that the function stops at.
void fadeMusic(bool direction, float speed, float targetVolume)
{
    float volume = music.getVolume();
    if (direction)
    {
        volume -= speed;
        if (volume <= targetVolume)
        {
            volume = targetVolume;
        }
    }
    else
    {
        volume += speed;
        if (volume >= targetVolume)
        {
            volume = targetVolume;
        }
    }
    music.setVolume(volume);
}

/// @brief Converts a pixel co-ordinate to a tile co-ordinate (font size).
/// @param pos Position (in tiles)
/// @return Position in tiles
float pixelToTile(float pos)
{
    return pos * fontSize;
}

void printerr(types::u8 error)
{
    std::string errorMsgs[] = {"Generic error.","Feature (currently) unimplemented.","Level ID is invalid.","Assets are missing."};
    std::cerr << "The game has crashed!" << std::endl;
    std::cerr << "Error Code: " << std::showbase << std::hex << (int)error << " - ";
    std::cerr << errorMsgs[error] << std::endl;
    exit(1);
}