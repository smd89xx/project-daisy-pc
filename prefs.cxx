#include "inc/includes.hxx"

types::u8* menuIndex;
const types::u8 prefsX = 10;
const types::u8 prefsY = 12;
const types::u8 diffXDelta = 9;
const types::u8 plrXDelta = 7;
const types::u8 plrXDeltaGlobal = 6;
const types::u8 prefsMenuAmnt = 8;
bool player = true; // False = Lucy, True = Stephanie
types::u8 difficulty = 1; // 0 = Easy, 1 = Normal, 2 = Hard, 3 = Legend (all names subject to change)
const structs::Option prefsMenu[] = 
{
    {prefsX+diffXDelta, prefsY, "Easy"},
    {prefsX+5+diffXDelta, prefsY, "Normal"},
    {prefsX+12+diffXDelta, prefsY, "Hard"},
    {prefsX+17+diffXDelta, prefsY, "Legend"},
    {prefsX+plrXDelta + plrXDeltaGlobal, prefsY+2, "Lucy"},
    {prefsX+5+plrXDelta + plrXDeltaGlobal, prefsY+2, "Stephanie"},
    {prefsX+11,prefsY+4,"The Jukebox"},
    {prefsX+14.25, prefsY+6, "Exit"}
};

static void updConfOutline()
{
    types::u8 playerIndex = player + 4;
    sf::Text diffConf(templateText);
    sf::Text plrConf(templateText);
    plrConf.setOutlineColor(playerColors[!player]);
    diffConf.setOutlineColor(playerColors[!player]);
    plrConf.setOutlineThickness(3.5);
    diffConf.setOutlineThickness(3.5);
    plrConf.setString(prefsMenu[playerIndex].label);
    plrConf.setPosition(pixelToTile(prefsMenu[playerIndex].x),pixelToTile(prefsMenu[playerIndex].y));
    diffConf.setString(prefsMenu[difficulty].label);
    diffConf.setPosition(pixelToTile(prefsMenu[difficulty].x),pixelToTile(prefsMenu[difficulty].y));
    window.draw(plrConf);
    window.draw(diffConf);
}

static void selectMenuPrefs()
{   
    if (*menuIndex >= 6)
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
    }
    switch (*menuIndex)
    {
        case 0:
        {
            difficulty = 0;
            break;
        }
        case 1:
        {
            difficulty = 1;
            break;
        }
        case 2:
        {
            difficulty = 2;
            break;
        }
        case 3:
        {
            difficulty = 3;
            break;
        }
        case 4:
        {
            player = false;
            break;
        }
        case 5:
        {
            player = true;
            break;
        }
        case 6:
        {
            *menuIndex = 0;
            jukebox();
            break;
        }
        case 7:
        {
            *menuIndex = 3;
            title();
            break;
        }
        default:
        {
            std::cerr << comingSoonText << std::endl;
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
    sf::Text diffStr(templateText);
    diffStr.setString("Difficulty:");
    diffStr.setPosition(pixelToTile(prefsX),pixelToTile(prefsY));
    sf::Text plrStr(templateText);
    plrStr.setString("Player:");
    plrStr.setPosition(pixelToTile(prefsX+plrXDeltaGlobal),pixelToTile(prefsY+2));
    while (window.isOpen())
    {
        sf::Event e;
        screenFade(volFadeSpeed*3,true);
        window.clear(sf::Color::Black);
        window.draw(diffStr);
        window.draw(plrStr);
        updConfOutline();
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
                    if (!window.hasFocus() || fadeRect.getFillColor().a != 0)
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
                            (*menuIndex)--;
                        }
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::Right)
                    {
                        sndHvr.play();
                        if (*menuIndex < prefsMenuAmnt-1)
                        {
                            (*menuIndex)++;
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