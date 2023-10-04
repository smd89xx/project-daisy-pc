#include "inc/includes.hxx"

sf::RectangleShape lvlFG(sf::Vector2f(window.getSize()));
sf::RectangleShape lvlBG(lvlFG);
sf::Texture lvlPxBG, lvlPxFG;
sf::Color bgColor(0x000000FF);
types::u8 level = 0;
float cam_x = 0;
float cam_y = 0;
const sf::Vector2f mapSizes[] = 
{
    {3840,2160},
};

static void camPos()
{
    if (cam_x < 0)
    {
        cam_x = 0;
    }
    else if (cam_x > mapSizes[level].x - window.getSize().x)
    {
        cam_x = mapSizes[level].x - window.getSize().x;
    }
    if (cam_y < 0)
    {
        cam_y = 0;
    }
    else if (cam_y > mapSizes[level].y - window.getSize().y)
    {
        cam_y = mapSizes[level].y - window.getSize().y;
    }
    lvlFG.setPosition(sf::Vector2f(-cam_x,-cam_y));
    lvlBG.setPosition(sf::Vector2f(-cam_x/2,-cam_y/2));
}

static const types::u8* loadMap()
{
    lvlPxBG.setSmooth(true);
    lvlPxFG.setSmooth(true);
    switch (level)
    {
    case 0:
    {
        lvlPxBG.loadFromFile(testLvlBG);
        lvlPxFG.loadFromFile(testLvlFG);
        lvlBG.setTexture(&lvlPxBG);
        lvlFG.setTexture(&lvlPxFG);
        lvlFG.setSize(mapSizes[level]);
        lvlBG.setSize(mapSizes[level]);
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

static void gameInputHdl()
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
        cam_x -= 2.5;
    }
    else if (rightPressed)
    {
        cam_x += 2.5;
    }
    if (upPressed)
    {
        cam_y -= 2.5;
    }
    else if (downPressed)
    {
        cam_y += 2.5;
    }
}

static void drawHUD()
{
    sf::Text camXLabel(templateText);
    sf::Text camYLabel(templateText);
    camYLabel.setPosition(0, 27.5);
    std::string cxStr = std::to_string(cam_x);
    std::string cyStr = std::to_string(cam_y);
    camXLabel.setString("CX: " + cxStr);
    camYLabel.setString("CY: " + cyStr);
    window.draw(camXLabel);
    window.draw(camYLabel);
}

void gameInit()
{
    fadeRect.setFillColor(sf::Color::Black);
    const types::u8* collisionArray = loadMap();
    while (window.isOpen())
    {
        sf::Event e;
        screenFade(volFadeSpeed,true);
        gameInputHdl();
        camPos();
        window.clear(bgColor);
        window.draw(lvlBG);
        window.draw(lvlFG);
        drawHUD();
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
            default:
            {
                break;
            }
            }
        }
    }
}