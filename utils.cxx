#include "inc/includes.hxx"

sf::RenderWindow window(sf::VideoMode(1280, 720),"",sf::Style::Close);
sf::Font font;
sf::Music music;
sf::RectangleShape fadeRect;
const types::u8 fontSize = 8;
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
types::u8 scaleFactor = 3;
const types::u8 maxScale = 5;
const sf::Vector2u screenSizes[] = {{426,240},{853,480},{1280,720},{1707,960},{2133,1200}};
const std::string errorMsgs[] = {"Generic error.","Feature (currently) unimplemented.","Level ID is invalid.","Assets are missing.","Scale factor is invalid."};
sf::Event e;
const types::u8 bsodTextLines = 2;
const std::string bsodText[] = {"Your game ran into a problem and needs to restart.","Press any key or joypad button to restart."};
const types::u8 bsodX = 6;
const types::u8 bsodY = 5;
const types::u8 bsodSubTxtLines[] = {2,4};
const std::string bsodSubTxt[] = {"For more information about this issue and possible fixes, visit","https://github.com/TheWindowsPro98/project-daisy-pc/blob/main/docs/errors.md","If you contact the developer (don't), give him this info:","Stop Code (String):","Stop Code (Hex):","What Failed:"};
const std::string bsodStopStrings[] = {"Generic error.","Requested feature was not found.","Level ID is outside valid range.","Game assets are missing.","Game scaling is not within the valid range.","Player ID is outside valid range."};
const std::string bsodFileStrings[] = {"utils.cxx","utils.cxx","game.cxx","resources.cxx","prefs.cxx","prefs.cxx"};
const char lineEnd = '\n';
const char space = ' ';

/// @brief Fades the screen in or out.
/// @param speed Speed of the fade. Bigger value = faster fade.
/// @param direction Direction of the fade. True for in, false for out.
/// @param fadeTarget Alpha value (0 - 255) that the fade routine stops at.
void screenFade(float speed, bool direction, float fadeTarget)
{
    float alpha = fadeRect.getFillColor().r;
    if (!direction)
    {
        alpha -= speed/2;
        if (alpha <= fadeTarget)
        {
            alpha = fadeTarget;
        }
    }
    else
    {
        alpha += speed*3;
        if (alpha >= fadeTarget)
        {
            alpha = fadeTarget;
        }
    }
    fadeRect.setFillColor(sf::Color(alpha,alpha,alpha,0xFF));
    window.draw(fadeRect,sf::BlendMultiply);
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
    return pos * (fontSize * scaleFactor);
}

static void resetGame()
{
    font.loadFromFile(blazeTTF);
    font.setSmooth(false);
    title();
}

static void fallback_printerr(int error)
{
    using namespace std;
    cout << ":(" << lineEnd << lineEnd;
    for (types::u8 i = 0; i < bsodTextLines; i++)
    {
        cout << bsodText[i] << lineEnd;
    }
    cout << lineEnd;
    for (types::u8 i = 0; i < bsodSubTxtLines[0]; i++)
    {
        cout << bsodSubTxt[i] << lineEnd;
    }
    cout << lineEnd;
    cout << bsodSubTxt[2] << lineEnd;
    cout << bsodSubTxt[3] << space << bsodStopStrings[error] << lineEnd;
    cout << bsodSubTxt[4] << space << hex << "0x" << setfill('0') << setw(types::varSizes::var32Bit*2) << error << lineEnd;
    cout << bsodSubTxt[5] << space << bsodFileStrings[error] << endl;
}

/// @brief Displays an error screen.
/// @param error Error code.
/// @param fallback Determines whether it should use the graphics window or stdout. Note that crashing to stdout is unrecoverable; the entire game process is destroyed.
void printerr(int error, bool fallback)
{
    sf::Texture bsodQRPx;
    sf::Texture bsodFrownPx;
    sf::SoundBuffer sbSad;
    bool assetsFailed = font.loadFromFile(scpTTF) & bsodFrownPx.loadFromFile(bsodFrown) & bsodQRPx.loadFromFile(bsodQR) & sbSad.loadFromFile(crashSFX);
    if (!assetsFailed || fallback)
    {
        fallback_printerr(error);
        exit(1);
    }
    sf::Sound sndSad;
    sndSad.setBuffer(sbSad);
    sndSad.play();
    font.setSmooth(true);
    sf::Text bsodDrawableText[bsodTextLines];
    sf::RectangleShape bsodFrownDrawable(sf::Vector2f(bsodFrownPx.getSize()));
    bsodFrownDrawable.setTexture(&bsodFrownPx);
    bsodFrownDrawable.setScale(sf::Vector2f(scaleFactor,scaleFactor));
    bsodFrownDrawable.setPosition(sf::Vector2f(pixelToTile(bsodX),pixelToTile(bsodY)));
    sf::RectangleShape bsodQRDrawable(sf::Vector2f(bsodQRPx.getSize()));
    bsodQRDrawable.setTexture(&bsodQRPx);
    bsodQRDrawable.setScale(sf::Vector2f(scaleFactor*1.5,scaleFactor*1.5));
    bsodQRDrawable.setPosition(sf::Vector2f(pixelToTile(bsodX),pixelToTile(bsodY+12)));
    types::u8 bsodSubTxtLinesCmb;
    for (types::u8 i = 0; i < sizeof(bsodSubTxtLines); i++)
    {
        bsodSubTxtLinesCmb += bsodSubTxtLines[i];
    }
    sf::Text bsodDrawableSubTxt[bsodSubTxtLinesCmb];
    for (types::u8 i = 0; i < bsodTextLines; i++)
    {
        bsodDrawableText[i] = templateText;
        bsodDrawableText[i].setString(bsodText[i]);
        bsodDrawableText[i].setPosition(sf::Vector2f(pixelToTile(bsodX),pixelToTile(bsodY+8+i)));
        bsodDrawableText[i].setStyle(sf::Text::Style::Bold);
    }
    for (types::u8 i = 0; i < bsodSubTxtLines[0]; i++)
    {
        bsodDrawableSubTxt[i] = templateText;
        bsodDrawableSubTxt[i].setString(bsodSubTxt[i]);
        bsodDrawableSubTxt[i].setPosition(sf::Vector2f(pixelToTile(bsodX+6.75),pixelToTile(bsodY+12+i)));
        bsodDrawableSubTxt[i].setStyle(sf::Text::Style::Bold);
        bsodDrawableSubTxt[i].setScale(0.65,0.65);
    }
    for (types::u8 i = 0; i < bsodSubTxtLines[1]; i++)
    {
        bsodDrawableSubTxt[i+2] = templateText;
        bsodDrawableSubTxt[i+2].setString(bsodSubTxt[i+2]);
        bsodDrawableSubTxt[i+2].setPosition(sf::Vector2f(pixelToTile(bsodX+6.75),pixelToTile(bsodY+15+i)));
        bsodDrawableSubTxt[i+2].setStyle(sf::Text::Style::Bold);
        bsodDrawableSubTxt[i+2].setScale(0.55,0.55);
    }
    sf::Text bsodDrawableStopText(bsodDrawableSubTxt[3]);
    bsodDrawableStopText.setString(bsodStopStrings[error]);
    bsodDrawableStopText.setPosition(pixelToTile(bsodX+13.75),pixelToTile(bsodY+16));
    std::stringstream hexStr;
    hexStr << "0x" << std::setfill('0') << std::setw(types::varSizes::var32Bit*2) << std::hex << error;
    sf::Text bsodDrawableStopHex(bsodDrawableStopText);
    bsodDrawableStopHex.setString(hexStr.str());
    bsodDrawableStopHex.setPosition(pixelToTile(bsodX+12.75),pixelToTile(bsodY+17));
    sf::Text bsodDrawablePtr(bsodDrawableStopHex);
    bsodDrawablePtr.setString(bsodFileStrings[error]);
    bsodDrawablePtr.setPosition(pixelToTile(bsodX+11.35),pixelToTile(bsodY+18));
    while (window.isOpen())
    {
        window.clear(sf::Color(0x00000000));
        window.draw(bsodFrownDrawable);
        window.draw(bsodQRDrawable);
        for (types::u8 i = 0; i < bsodTextLines; i++)
        {
            window.draw(bsodDrawableText[i]);
        }
        for (types::u8 i = 0; i < bsodSubTxtLines[0]; i++)
        {
            window.draw(bsodDrawableSubTxt[i]);
        }
        for (types::u8 i = 0; i < bsodSubTxtLines[1]; i++)
        {
            window.draw(bsodDrawableSubTxt[i+2]);
        }
        window.draw(bsodDrawableStopText);
        window.draw(bsodDrawableStopHex);
        window.draw(bsodDrawablePtr);
        window.display();
        while (window.pollEvent(e))
        {
            switch (e.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::KeyPressed:
                {
                    sndSad.stop();
                    resetGame();
                    break;
                }
                case sf::Event::JoystickButtonPressed:
                {
                    if (!window.hasFocus())
                    {
                        break;
                    }
                    sndSad.stop();
                    resetGame();
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
    
}

/// @brief Changes screen and viewport scale based on the 'scaleFactor' variable.
void updScreenSize()
{
    if (scaleFactor - 1 >= maxScale)
    {
        printerr(invalidScaleErr);
    }
    sf::Vector2u baseSize = screenSizes[scaleFactor-1];
    sf::View view;
    view.setSize(sf::Vector2f(baseSize));
    view.setCenter(baseSize.x / 2,baseSize.y / 2);
    window.setSize(baseSize);
    window.setView(view);
    fadeRect.setSize(sf::Vector2f(baseSize));
    templateText.setCharacterSize(fontSize * scaleFactor);
    templateText.setOutlineThickness((3.5 / 3) * scaleFactor);
}