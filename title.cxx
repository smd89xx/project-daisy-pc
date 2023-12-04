#include "inc/includes.hxx"

const types::u8 titleX = 6;
const types::u8 titleY = 22;
const types::u8 titleOptAmnt = 4;
const std::string comingSoonText = "Feature will be added soon!";

const structs::Option titleMenu[] = 
{
    {titleX,titleY,"Start Game"},
    {titleX+10,titleY,"Continue Game"},
    {titleX+22,titleY,"Statistics"},
    {titleX+31,titleY,"Preferences"},
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
                window.close();
                return;
            }
        }
    }
    switch (menuIndex)
    {
        case 0:
        {
            gameInit();
            break;
        }
        case 3:
        {
            prefsScreen();
            break;
        }
        default:
        {
            printerr(missingFuncErr);
            break;
        }
    }
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
        drawMenu(titleMenu,4);
        screenFade(volFadeSpeed,true,fadeDark);
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
                if (!window.hasFocus())
                {
                    break;
                }
                if (e.key.scancode == sf::Keyboard::Scan::Left)
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
                else if (e.key.scancode == sf::Keyboard::Scan::Right)
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
                if (e.key.scancode == sf::Keyboard::Scan::Enter)
                {
                    sndCnf.play();
                    selectMenuTitle();
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