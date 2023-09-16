#include "inc/includes.hxx"

u8* menuIndex;
const u8 prefsX = 10;
const u8 prefsY = 12;
const u8 diffXDelta = 9;
const u8 plrXDelta = 7;
const u8 plrXDeltaGlobal = 6;
const u8 prefsMenuAmnt = 8;
bool player = true; // False = Lucy, True = Stephanie
const structs::Option prefsMenu[] = 
{
    {prefsX+diffXDelta, prefsY, "Easy"},
    {prefsX+5+diffXDelta, prefsY, "Normal"},
    {prefsX+12+diffXDelta, prefsY, "Hard"},
    {prefsX+17+diffXDelta, prefsY, "Legend"},
    {prefsX+plrXDelta + plrXDeltaGlobal, prefsY+2, "Lucy"},
    {prefsX+5+plrXDelta + plrXDeltaGlobal, prefsY+2, "Stephanie"},
    {prefsX+11,prefsY+4,"The Jukebox"},
    {prefsX+14.5, prefsY+6, "Exit"}
};

static void selectMenuPrefs()
{
    float volume;
    while (window.isOpen())
    {
        if (volume == 0)
        {
            music.stop();
            music.setVolume(100);
            break;
        }
        sf::Event e;
        volume = music.getVolume();
        fadeMusic(true,volFadeSpeed);
        screenFade(volFadeSpeed,false);
        window.draw(fadeRect);
        window.display();
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
        }
    }
    switch (*menuIndex)
    {
        case 7:
        {
            *menuIndex = 3;
            title();
            break;
        }
        default:
        {
            fprintf(stderr,"Feature will be added soon!");
            exit(EXIT_FAILURE);
            break;
        }
    }
}

void prefsScreen()
{
    music.openFromFile(lsTrack);
    music.play();
    *menuIndex = 0;
    fadeRect.setFillColor(sf::Color::Black);
    sf::Text diffStr;
    diffStr.setFont(font);
    diffStr.setString("Difficulty:");
    diffStr.setCharacterSize(fontSize);
    diffStr.setPosition(prefsX*fontSize,prefsY*fontSize);
    sf::Text plrStr;
    plrStr.setFont(font);
    plrStr.setString("Player:");
    plrStr.setCharacterSize(fontSize);
    plrStr.setPosition((prefsX+plrXDeltaGlobal)*fontSize,(prefsY+2)*fontSize);
    while (window.isOpen())
    {
        sf::Event e;
        screenFade(volFadeSpeed*2,true);
        window.clear(sf::Color::Black);
        window.draw(diffStr);
        window.draw(plrStr);
        drawMenu(prefsMenu,prefsMenuAmnt);
        window.draw(fadeRect);
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
                    if (!window.hasFocus() or fadeRect.getFillColor().a != 0)
                    {
                        break;
                    }
                    if (e.key.scancode == sf::Keyboard::Scan::Left)
                    {
                        sndHvr.play();
                        if (*menuIndex == 0)
                        {
                            *menuIndex = prefsMenuAmnt-1;
                        }
                        else
                        {
                            *menuIndex -= 1;
                        }
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::Right)
                    {
                        sndHvr.play();
                        if (*menuIndex < prefsMenuAmnt-1)
                        {
                            *menuIndex += 1;
                        }
                        else
                        {
                            *menuIndex = 0;
                        }
                    }
                    if (e.key.scancode == sf::Keyboard::Scan::Enter)
                    {
                        sndCnf.play();
                        selectMenuPrefs();
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::Escape)
                    {
                        sndCnf.play();
                        *menuIndex = 7;
                        selectMenuPrefs();
                    }
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