#include "inc/includes.hxx"

const types::u8 titleOptAmnt = 4;

static void selectMenuTitle(bool exit)
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
    if (!exit)
    {
        saveScreen();
    }
    else
    {
        updSRAM();
        window.close();
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

void title()
{
    fadeRect.setFillColor(sf::Color::Black);
    std::string versionCombined = getFileLine(strVersionHdr) + getFileLine(strRSShort+releaseStage) + getFileLine(strVersionNum);
    music.openFromFile(getFileLine(resTitleMus,&resList));
    music.setLoop(true);
    music.play();
    sf::Texture titleTexture;
    titleTexture.loadFromFile(getFileLine(resTitleLogo,&resList));
    sf::RectangleShape titleSprite(sf::Vector2f(titleTexture.getSize()));
    titleSprite.setTexture(&titleTexture);
    sf::Vector2f titleImgSize(320 * scaleFactor, 48 * scaleFactor);
    titleSprite.setSize(titleImgSize);
    titleSprite.setPosition(pixelToTile(7),0);
    types::u32 color = RGB4toRGB8(0x0224);
    sf::Texture titleBGTexture;
    titleBGTexture.loadFromFile(getFileLine(resTitleBG,&resList));
    sf::RectangleShape titleRect;
    titleRect.setTexture(&titleBGTexture);
    titleRect.setSize(sf::Vector2f(window.getSize()));
    types::u8 startButton = startBtnID();
    while (window.isOpen())
    {
        window.clear(sf::Color(color));
        window.draw(titleRect);
        drawBitmapFont(getFileLine(strCopyInfo),{0,29});
        drawBitmapFont(versionCombined,{6.75,6});
        if (sf::Joystick::isConnected(0))
        {
            drawBitmapFont(getFileLine(strStartTxt),{17.625,15});
        }
        else
        {
            drawBitmapFont(getFileLine(strStartTxtKB),{19.125,15});
        }
        window.draw(titleSprite);
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
                if (e.key.scancode == sf::Keyboard::Scan::Enter)
                {
                    sndCnf.play();
                    selectMenuTitle(false);
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Escape)
                {
                    sndBack.play();
                    selectMenuTitle(true);
                }
                break;
            }
            case sf::Event::JoystickButtonPressed:
            {
                if (!window.hasFocus())
                {
                    break;
                }
                if (e.joystickButton.button == buttonCircle)
                {
                    sndBack.play();
                    selectMenuTitle(true);
                }
                else if (e.joystickButton.button == startButton)
                {
                    sndCnf.play();
                    selectMenuTitle(false);
                }
            }
            default:
            {
                break;
            }
            }

        }
    }
}