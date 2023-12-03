#include "inc/includes.hxx"

const types::u8 titleX = 2;
const types::u8 titleY = 22;
const types::u8 titleOptAmnt = 5;
const sf::IntRect titleCurRects[] = {{titleX,titleY,0,0}};

const structs::Option titleMenu[] = 
{
    {titleX,titleY,"Start Game"},
    {titleX+10,titleY,"Continue Game"},
    {titleX+22,titleY,"Statistics"},
    {titleX+31,titleY,"Preferences"},
    {titleX+41.75,titleY,"Exit Game"},
};

static void selectMenuTitle()
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
                updSRAM();
                window.close();
                return;
            }
        }
    }
    switch (menuIndex)
    {
        case 0:
        {
            level = 0;
            lives = 5;
            score = 0;
            gameInit();
            break;
        }
        case 1:
        {
            level = saveRAM[addrLevel];
            lives = saveRAM[addrLives];
            score = readSRAM_u32(addrScore);
            gameInit();
            break;
        }
        case 3:
        {
            prefsScreen();
            break;
        }
        case 4:
        {
            updSRAM();
            window.close();
            break;
        }
        default:
        {
            printerr(missingFuncErr,"selectMenuTitle()");
            break;
        }
    }
}

static void moveCursor(bool direction)
{
    if (!direction)
    {    
        sndHvr.play();
        if (menuIndex == 0)
        {
            menuIndex = titleOptAmnt-1;
        }
        else
        {
            menuIndex--;
        }
    }
    else
    {
        sndHvr.play();
        if (menuIndex >= titleOptAmnt-1)
        {
            menuIndex = 0;
        }
        else
        {
            menuIndex++;
        }
    }
}

static void callSelect()
{
    sndCnf.play();
    selectMenuTitle();
}

void title()
{
    fadeRect.setFillColor(sf::Color::Black);
    std::string vmajStr = std::to_string(versionMajor);
    std::string vminStr = std::to_string(versionMinor);
    std::string vrevStr = std::to_string(versionRevision);
    std::string vcStr = vmajStr + "." + vminStr + "." + vrevStr;
    std::string versionCombined = "Version " + releaseStageStringsS[releaseStage] + vcStr;
    music.openFromFile(titleTrack);
    music.setLoop(true);
    music.play();
    sf::Text copyInfo(templateText);
    sf::Text versionText(templateText);
    copyInfo.setString(L"©TheWindowsPro98 2023");
    copyInfo.setPosition(0,pixelToTile(29));
    versionText.setString(versionCombined);
    versionText.setPosition(pixelToTile(6.75),pixelToTile(6));
    sf::Texture titleTexture;
    titleTexture.loadFromFile(titleImg);
    titleTexture.setSmooth(true);
    sf::RectangleShape titleSprite(sf::Vector2f(titleTexture.getSize()));
    titleSprite.setTexture(&titleTexture);
    sf::Vector2f titleImgSize(320 * scaleFactor, 48 * scaleFactor);
    titleSprite.setSize(titleImgSize);
    titleSprite.setPosition(pixelToTile(7),0);
    float volume = 100;
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        window.draw(copyInfo);
        window.draw(versionText);
        window.draw(titleSprite);
        drawMenu(titleMenu,titleOptAmnt);
        screenFade(volFadeSpeed,true,fadeDark);
        window.display();
        while (window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::Closed:
            {
                updSRAM();
                window.close();
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (e.key.scancode == sf::Keyboard::Scan::Left)
                {
                    moveCursor(false);
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Right)
                {
                    moveCursor(true);
                }
                if (e.key.scancode == sf::Keyboard::Scan::Enter)
                {
                    sndCnf.play();
                    selectMenuTitle();
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Escape)
                {
                    menuIndex = 4;
                    sndBack.play();
                    selectMenuTitle();
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
                        selectMenuTitle();
                        break;
                    }
                    case buttonCircle:
                    {
                        menuIndex = 4;
                        sndBack.play();
                        selectMenuTitle();
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
                        moveCursor(false);
                    }
                    else if (e.joystickMove.position == 100)
                    {
                        moveCursor(true);
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