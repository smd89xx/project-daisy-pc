#include "inc/includes.hxx"

types::u8 menuIndex;
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
sf::Text* diffStr;
sf::Text* plrStr;

static void updConfOutline()
{
    types::u8 playerIndex = player + 4;
    sf::Text diffConf(templateText);
    sf::Text plrConf(templateText);
    plrConf.setOutlineColor(playerColors[!player]);
    diffConf.setOutlineColor(playerColors[!player]);
    plrConf.setString(prefsMenu[playerIndex].label);
    plrConf.setPosition(pixelToTile(prefsMenu[playerIndex].x),pixelToTile(prefsMenu[playerIndex].y));
    diffConf.setString(prefsMenu[difficulty].label);
    diffConf.setPosition(pixelToTile(prefsMenu[difficulty].x),pixelToTile(prefsMenu[difficulty].y));
    window.draw(plrConf);
    window.draw(diffConf);
}

static void clearMem()
{
    delete diffStr;
    delete plrStr;
}

static void exitEvent()
{
    saveRAM[addrDifficulty] = difficulty;
    saveRAM[addrPlayer] = player;
    saveRAM[addrScaling] = scaleFactor;
    updSRAM();
    window.close();
}

static void selectMenuPrefs()
{   
    if (menuIndex >= 6)
    {   
        fadeRect.setFillColor(sf::Color::White);
        float volume = music.getVolume();
        while (window.isOpen())
        {
            if (volume == 0)
            {
                music.stop();
                music.setVolume(100);
                break;
            }
            volume = music.getVolume();
            fadeMusic(true,volFadeSpeed,volMin);
            screenFade(volFadeSpeed,false,fadeLight);
            window.display();
            while (window.pollEvent(e))
            {
                if (e.type == sf::Event::Closed)
                {
                    exitEvent();
                    return;
                }
            }
        }
    }
    switch (menuIndex)
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
            menuIndex = 0;
            clearMem();
            jukebox();
            break;
        }
        case 7:
        {
            saveRAM[addrDifficulty] = difficulty;
            saveRAM[addrPlayer] = player;
            saveRAM[addrScaling] = scaleFactor;
            menuIndex = 3;
            clearMem();
            title();
            break;
        }
        default:
        {
            clearMem();
            printerr(missingFuncErr);
            break;
        }
    }
}

static void cursorMove(bool direction)
{
    if (!direction)
    {
        sndHvr.play();
        if (menuIndex == 0)
        {
            menuIndex = prefsMenuAmnt-1;
        }
        else
        {
            menuIndex--;
        }
    }
    else
    {
        sndHvr.play();
        if (menuIndex < prefsMenuAmnt-1)
        {
            menuIndex++;
        }
        else
        {
            menuIndex = 0;
        }
    }
}

static void changeScale(bool direction)
{
    if (!direction)
    {
        sndHvr.play();
        if (scaleFactor == 2)
        {
            scaleFactor = maxScale;
        }
        else
        {
            scaleFactor--;
        }
        plrStr->setCharacterSize(fontSize * scaleFactor);
        diffStr->setCharacterSize(fontSize * scaleFactor);
        diffStr->setPosition(pixelToTile(prefsX),pixelToTile(prefsY));
        plrStr->setPosition(pixelToTile(prefsX+plrXDeltaGlobal),pixelToTile(prefsY+2));
        updScreenSize();
    }
    else
    {
        sndHvr.play();
        if (scaleFactor == maxScale)
        {
            scaleFactor = 2;
        }
        else
        {
            scaleFactor++;
        }
        plrStr->setCharacterSize(fontSize * scaleFactor);
        diffStr->setCharacterSize(fontSize * scaleFactor);
        diffStr->setPosition(pixelToTile(prefsX),pixelToTile(prefsY));
        plrStr->setPosition(pixelToTile(prefsX+plrXDeltaGlobal),pixelToTile(prefsY+2));
        updScreenSize();
    }
}

void prefsScreen()
{
    diffStr = new sf::Text(templateText);
    plrStr = new sf::Text(templateText);
    music.openFromFile(lsTrack);
    music.play();
    menuIndex = 0;
    fadeRect.setFillColor(sf::Color::Black);
    diffStr->setString("Difficulty:");
    diffStr->setPosition(pixelToTile(prefsX),pixelToTile(prefsY));
    plrStr->setString("Player:");
    plrStr->setPosition(pixelToTile(prefsX+plrXDeltaGlobal),pixelToTile(prefsY+2));
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        window.draw(*diffStr);
        window.draw(*plrStr);
        drawMenu(prefsMenu, prefsMenuAmnt);
        updConfOutline();
        screenFade(volFadeSpeed,true,fadeDark);
        window.display();
        while (window.pollEvent(e))
        {
            switch (e.type)
            {
                case sf::Event::Closed:
                {
                    exitEvent();
                    break;
                }
                case sf::Event::KeyPressed:
                {
                    if (e.key.scancode == sf::Keyboard::Scan::Left)
                    {
                        cursorMove(false);
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::Right)
                    {
                        cursorMove(true);
                    }
                    if (e.key.scancode == sf::Keyboard::Scan::Up)
                    {
                        changeScale(false);
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::Down)
                    {
                        changeScale(true);
                    }
                    if (e.key.scancode == sf::Keyboard::Scan::Enter)
                    {
                        sndCnf.play();
                        selectMenuPrefs();
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::Escape)
                    {
                        menuIndex = 7;
                        sndBack.play();
                        selectMenuPrefs();
                    }
                    break;
                }
                case sf::Event::JoystickButtonPressed:
                {
                    if (!window.hasFocus())
                    {
                        break;
                    }
                    switch (e.joystickButton.button)
                    {
                        case buttonCross:
                        {
                            sndCnf.play();
                            selectMenuPrefs();
                            break;
                        }
                        case buttonCircle:
                        {
                            menuIndex = 7;
                            sndBack.play();
                            selectMenuPrefs();
                            break;
                        }
                        case buttonL1:
                        {
                            changeScale(false);
                            break;
                        }
                        case buttonR1:
                        {
                            changeScale(true);
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case sf::Event::JoystickMoved:
                {
                    if (!window.hasFocus())
                    {
                        break;
                    }
                    if (e.joystickMove.axis == axisDPADX)
                    {
                        if (e.joystickMove.position == -100)
                        {
                            cursorMove(false);
                        }
                        else if (e.joystickMove.position == 100)
                        {
                            cursorMove(true);
                        }
                    }
                    else if (e.joystickMove.axis == axisDPADY)
                    {
                        if (e.joystickMove.position == -100)
                        {
                            cursorMove(false);
                        }
                        else if (e.joystickMove.position == 100)
                        {
                            cursorMove(true);
                        }
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