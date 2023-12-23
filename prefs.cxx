#include "inc/includes.hxx"

types::u8 menuIndex;
const types::u8 prefsX = 14;
const types::u8 prefsY = 9;
const types::u8 diffXDelta = 15;
const types::u8 plrXDelta = 15;
const types::u8 jukeboXDelta = 6;
const float exitXDelta = jukeboXDelta + 3.75;
const types::u8 prefsMenuAmnt = 8;
const structs::Option prefsMenu[] = 
{
    {prefsX+diffXDelta, prefsY, "Easy"},
    {prefsX+diffXDelta, prefsY+1.25, "Normal"},
    {prefsX+diffXDelta, prefsY+2.5, "Hard"},
    {prefsX+diffXDelta, prefsY+3.75, "Legend"},
    {prefsX+plrXDelta, prefsY+6.25, "Lucy"},
    {prefsX+plrXDelta, prefsY+7.5, "Stephanie"},
    {prefsX+jukeboXDelta,prefsY+10,"The Jukebox"},
    {prefsX+exitXDelta, prefsY+12.5, "Exit"}
};

static void updConfOutline()
{
    types::u8 playerIndex = saveSlots[slotIndex].player + 4;
    sf::RectangleShape diffConf({8,8});
    diffConf.setScale(scaleFactor,scaleFactor);
    diffConf.setTexture(&cursorTexture);
    diffConf.setFillColor(playerColors[!saveSlots[slotIndex].player]);
    sf::RectangleShape plrConf(diffConf);
    plrConf.setPosition(pixelToTile(prefsMenu[playerIndex].x - 1),pixelToTile(prefsMenu[playerIndex].y));
    diffConf.setPosition(pixelToTile(prefsMenu[saveSlots[slotIndex].difficulty].x - 1),pixelToTile(prefsMenu[saveSlots[slotIndex].difficulty].y));
    window.draw(plrConf);
    window.draw(diffConf);
}

static void exitEvent()
{
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
            saveSlots[slotIndex].difficulty = 0;
            break;
        }
        case 1:
        {
            saveSlots[slotIndex].difficulty = 1;
            break;
        }
        case 2:
        {
            saveSlots[slotIndex].difficulty = 2;
            break;
        }
        case 3:
        {
            saveSlots[slotIndex].difficulty = 3;
            break;
        }
        case 4:
        {
            saveSlots[slotIndex].player = false;
            break;
        }
        case 5:
        {
            saveSlots[slotIndex].player = true;
            break;
        }
        case 6:
        {
            menuIndex = 0;
            jukebox();
            break;
        }
        case 7:
        {
            saveRAM[addrScaling] = scaleFactor;
            menuIndex = 0;
            if (slotIndex == maxSlots)
            {
                slotIndex = 0;
            }
            else
            {
                saveSlots[slotIndex].writeToSRAM(addrSaves+(slotIndex*10));
            }
            saveScreen();
            break;
        }
        default:
        {
            printerr(missingFuncErr,"selectMenuPrefs()");
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
        if (scaleFactor == 1)
        {
            scaleFactor = maxScale;
        }
        else
        {
            scaleFactor--;
        }
        updScreenSize();
    }
    else
    {
        sndHvr.play();
        if (scaleFactor == maxScale)
        {
            scaleFactor = 1;
        }
        else
        {
            scaleFactor++;
        }
        updScreenSize();
    }
}

static void drawBtnHints()
{
    std::stringstream scalePromptCmb;
    std::string emptyString = "";
    std::string scalePromptMainTxt = ": -/+ scaling";
    std::string scaleFactorStr = " (Current scale factor: ";
    bool joyConnected = sf::Joystick::isConnected(0);
    if (joyConnected)
    {
        scalePromptCmb.str(emptyString);
        scalePromptCmb << btnPrompts[buttonL1] << "/" << btnPrompts[buttonR1] << scalePromptMainTxt << scaleFactorStr << (types::u16)scaleFactor << ")";
        drawBitmapFont(btnPrompts[dpadUp] + "/" + btnPrompts[dpadDown] + ": Change selection",{0,28});
        drawBitmapFont(btnPrompts[buttonCircle] + ": Exit",{0,27});
        drawBitmapFont(btnPrompts[buttonCross] + ": Decision",{0,26});
    }
    else
    {
        scalePromptCmb.str(emptyString);
        scalePromptCmb << "Left/Right" << scalePromptMainTxt << scaleFactorStr << (types::u16)scaleFactor << ")";
        drawBitmapFont("Up/Down: Change selection",{0,28});
        drawBitmapFont("Escape: Exit",{0,27});
        drawBitmapFont("Enter: Decision",{0,26});
    }
    drawBitmapFont(scalePromptCmb.str(),{0,29});
}

void prefsScreen()
{
    music.openFromFile(lsTrack);
    music.play();
    menuIndex = 0;
    fadeRect.setFillColor(sf::Color::Black);
    types::u32 color = RGB4toRGB8(0x224);
    while (window.isOpen())
    {
        window.clear(sf::Color(color));
        drawBitmapFont("Difficulty:",{prefsX,prefsY});
        drawBitmapFont("Player:",{prefsX,prefsY+6.25});
        drawBtnHints();
        updConfOutline();
        drawMenu(prefsMenu, prefsMenuAmnt);
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
                    if (e.key.scancode == sf::Keyboard::Scan::Up)
                    {
                        cursorMove(false);
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::Down)
                    {
                        cursorMove(true);
                    }
                    if (e.key.scancode == sf::Keyboard::Scan::Left)
                    {
                        changeScale(false);
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::Right)
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