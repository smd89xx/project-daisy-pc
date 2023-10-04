#include "inc/includes.hxx"

sf::RectangleShape* lvlFG;
sf::RectangleShape* lvlBG;
sf::Texture* lvlPxBG;
sf::Texture* lvlPxFG;
sf::Color* bgColor;
types::u8 level = 0;
float* cam_x;
float* cam_y;
const sf::Vector2f mapSizes[] = 
{
    {3840,2160},
};
bool* isPaused;
const types::u8 pauseX = 20;
const types::u8 pauseY = 13;
const types::u8 pauseOptsAmnt = 3;
const structs::Option pauseMenu[] = 
{
    {pauseX,pauseY,"Return to Game"},
    {pauseX,pauseY+2,"Save and Exit"},
    {pauseX,pauseY+4,"Exit Without Saving"}
};

static void camPos()
{
    if (*cam_x < 0)
    {
        *cam_x = 0;
    }
    else if (*cam_x > mapSizes[level].x - window.getSize().x)
    {
        *cam_x = mapSizes[level].x - window.getSize().x;
    }
    if (*cam_y < 0)
    {
        *cam_y = 0;
    }
    else if (*cam_y > mapSizes[level].y - window.getSize().y)
    {
        *cam_y = mapSizes[level].y - window.getSize().y;
    }
    lvlFG->setPosition(sf::Vector2f(-*cam_x,-*cam_y));
    lvlBG->setPosition(sf::Vector2f(-*cam_x/2,-*cam_y/2));
}

static const types::u8* loadMap()
{
    lvlPxBG->setSmooth(true);
    lvlPxFG->setSmooth(true);
    switch (level)
    {
    case 0:
    {
        lvlPxBG->loadFromFile(testLvlBG);
        lvlPxFG->loadFromFile(testLvlFG);
        lvlBG->setTexture(lvlPxBG);
        lvlFG->setTexture(lvlPxFG);
        lvlFG->setSize(mapSizes[level]);
        lvlBG->setSize(mapSizes[level]);
        music.openFromFile(lfTrack);
        music.setLoop(true);
        music.play();
        return testLvlCollision;
        break;
    }
    default:
    {
        printerr(badLevelErr);
        return nullptr;
        break;
    }
    }
}

static void gameInputHdl_KB()
{
    if (!window.hasFocus())
    {
        return;
    }
    bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left);
    bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right);
    bool upPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up);
    bool downPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down);
    if (leftPressed)
    {
        *cam_x -= 2.5;
    }
    else if (rightPressed)
    {
        *cam_x += 2.5;
    }
    if (upPressed)
    {
        *cam_y -= 2.5;
    }
    else if (downPressed)
    {
        *cam_y += 2.5;
    }
}

static void drawHUD()
{
    sf::Text camXLabel(templateText);
    sf::Text camYLabel(templateText);
    camYLabel.setPosition(0, 27.5);
    std::string cxStr = std::to_string(*cam_x);
    std::string cyStr = std::to_string(*cam_y);
    camXLabel.setString("CX: " + cxStr);
    camYLabel.setString("CY: " + cyStr);
    window.draw(camXLabel);
    window.draw(camYLabel);
}

static void selectMenuPause()
{
    if (menuIndex >= 1)
    {
        sf::Texture windowCapture;
        float volume = music.getVolume();
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
            fadeMusic(true,volFadeSpeed,volMin);
            screenFade(volFadeSpeed,false,fadeDark);
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
    switch (menuIndex)
    {
    case 0:
    {
        *isPaused = false;
        break;
    }
    case 2:
    {
        menuIndex = 0;
        delete cam_x;
        delete cam_y;
        delete isPaused;
        delete bgColor;
        delete lvlPxBG;
        delete lvlPxFG;
        title();
        break;
    }
    default:
    {
        printerr(missingFuncErr);
        break;
    }
    }
}

void gameInit()
{
    fadeRect.setFillColor(sf::Color::Black);
    cam_x = new float(0);
    cam_y = new float(0);
    isPaused = new bool(false);
    bgColor = new sf::Color(0x000000FF);
    lvlPxBG = new sf::Texture;
    lvlPxFG = new sf::Texture;
    lvlBG = new sf::RectangleShape(sf::Vector2f(window.getSize()));
    lvlFG = new sf::RectangleShape(*lvlBG);
    const types::u8* collisionArray = loadMap();
    while (window.isOpen())
    {
        sf::Event e;
        if (!*isPaused)
        {
            gameInputHdl_KB();
            camPos();
            fadeMusic(false,volFadeSpeed,volMax);
            screenFade(volFadeSpeed,true,fadeLight);
        }
        else
        {
            fadeMusic(true,volFadeSpeed,50);
            screenFade(volFadeSpeed*3,false,0x7F);
        }
        window.clear(*bgColor);
        window.draw(*lvlBG);
        window.draw(*lvlFG);
        drawHUD();
        window.draw(fadeRect);
        if (*isPaused)
        {
            drawMenu(pauseMenu,pauseOptsAmnt);
        }
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
                if (!*isPaused)
                {
                    if (e.key.scancode == sf::Keyboard::Scan::Enter)
                    {
                        sndCnf.play();
                        *isPaused = true;
                    }
                }
                else
                {
                    if (e.key.scancode == sf::Keyboard::Scan::Up)
                    {
                        sndHvr.play();
                        if (menuIndex <= 0)
                        {
                            menuIndex = pauseOptsAmnt - 1;
                        }
                        else
                        {
                            menuIndex--;
                        }
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::Down)
                    {
                        sndHvr.play();
                        if (menuIndex >= pauseOptsAmnt - 1)
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
                        selectMenuPause();
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::Escape)
                    {
                        sndBack.play();
                        menuIndex = 0;
                        selectMenuPause();
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