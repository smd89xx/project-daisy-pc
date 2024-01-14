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
    {prefsX+diffXDelta, prefsY, getFileLine(strDiffs)},
    {prefsX+diffXDelta, prefsY+1.25, getFileLine(strDiffs+1)},
    {prefsX+diffXDelta, prefsY+2.5, getFileLine(strDiffs+2)},
    {prefsX+diffXDelta, prefsY+3.75, getFileLine(strDiffs+3)},
    {prefsX+plrXDelta, prefsY+6.25, getFileLine(strPlrNames)},
    {prefsX+plrXDelta, prefsY+7.5, getFileLine(strPlrNames+1)},
    {prefsX+jukeboXDelta,prefsY+10, getFileLine(strJukebox)},
    {prefsX+exitXDelta, prefsY+12.5, getFileLine(strExit)}
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
            printerr(missingFuncErr,getFileLine(strMenuPrefsFunc));
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
    bool joyConnected = sf::Joystick::isConnected(0);
    if (joyConnected)
    {
        scalePromptCmb.str(getFileLine(strEmpty));
        scalePromptCmb << btnPrompts[buttonL1] << getFileLine(strKeySlash) << btnPrompts[buttonR1] << getFileLine(strIncDec) << getFileLine(strScaling) << (types::u16)scaleFactor << getFileLine(strKeyRParam);
        drawBitmapFont(btnPrompts[dpadUp] + getFileLine(strKeySlash) + btnPrompts[dpadDown] + getFileLine(strHoverPrompt),{0,28});
        drawBitmapFont(btnPrompts[buttonCircle] + getFileLine(strExitPrompt),{0,27});
        drawBitmapFont(btnPrompts[buttonCross] + getFileLine(strDecisionPrompt),{0,26});
    }
    else
    {
        scalePromptCmb.str(getFileLine(strEmpty));
        scalePromptCmb << getFileLine(strKeyLArrow) << getFileLine(strKeySlash) << getFileLine(strKeyRArrow) << getFileLine(strIncDec) << getFileLine(strScaling) << (types::u16)scaleFactor << getFileLine(strKeyRParam);
        drawBitmapFont(getFileLine(strKeyUArrow) + getFileLine(strKeySlash) + getFileLine(strKeyDArrow) + getFileLine(strHoverPrompt),{0,28});
        drawBitmapFont(getFileLine(strKeyEscape) + getFileLine(strExitPrompt),{0,27});
        drawBitmapFont(getFileLine(strKeyEnter) + getFileLine(strDecisionPrompt),{0,26});
    }
    drawBitmapFont(scalePromptCmb.str(),{0,29});
}

void prefsScreen()
{
    music.openFromFile(getFileLine(resPrefsMus,&resList));
    music.play();
    menuIndex = 0;
    fadeRect.setFillColor(sf::Color::Black);
    types::u32 color = RGB4toRGB8(0x224);
    while (window.isOpen())
    {
        window.clear(sf::Color(color));
        drawBitmapFont(getFileLine(strDiffHdr),{prefsX,prefsY});
        drawBitmapFont(getFileLine(strPlrHdr),{prefsX,prefsY+6.25});
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