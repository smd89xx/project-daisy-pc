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
    {1280,720},
};
bool* isPaused;
const types::u8 pauseX = 19;
const types::u8 pauseY = 12;
const types::u8 pauseOptsAmnt = 3;
const structs::Option pauseMenu[] = 
{
    {pauseX,pauseY,"Return to Game"},
    {pauseX,pauseY+2,"Save and Exit"},
    {pauseX,pauseY+4,"Exit Without Saving"}
};
sf::Texture* playerTexture;
sf::Sprite* playerSprite;
float* player_x;
float* player_y;
float* velocity_x;
float* velocity_y;
float* maxSpeed;

enum cameraBoundaries {leftCamBnd = 144, rightCamBnd = 144, topCamBnd = 104, bottomCamBnd = 104};

static void camPos()
{
    sf::Vector2f rectSize(lvlBG->getSize());
    float px_scrn = *player_x - *cam_x;
    float py_scrn = *player_y - *cam_y;
    if (px_scrn > rightCamBnd * scaleFactor)
    {
        *cam_x = (*player_x - rightCamBnd);
    }
    else if (px_scrn < leftCamBnd * scaleFactor)
    {
        *cam_x = (*player_x - leftCamBnd);
    }
    if (py_scrn > bottomCamBnd * scaleFactor)
    {
        *cam_y = (*player_y - bottomCamBnd);
    }
    else if (py_scrn < topCamBnd * scaleFactor)
    {
        *cam_y = (*player_y - topCamBnd);
    }
    if (*cam_x < 0)
    {
        *cam_x = 0;
    }
    else if (*cam_x > (rectSize.x - window.getSize().x) / scaleFactor)
    {
        *cam_x = (rectSize.x - window.getSize().x) / scaleFactor;
    }
    if (*cam_y < 0)
    {
        *cam_y = 0;
    }
    else if (*cam_y > (rectSize.y - window.getSize().y) / scaleFactor)
    {
        *cam_y = (rectSize.y - window.getSize().y) / scaleFactor;
    }
    lvlFG->setPosition(sf::Vector2f(-*cam_x * scaleFactor,-*cam_y * scaleFactor));
    lvlBG->setPosition(sf::Vector2f((-*cam_x * scaleFactor)/2,(-*cam_y * scaleFactor)/2));
}

static const types::u8* loadMap()
{
    sf::Vector2f scaledMapSize(mapSizes[level]);
    scaledMapSize.x *= scaleFactor;
    scaledMapSize.y *= scaleFactor;
    switch (level)
    {
    case 0:
    {
        lvlPxBG->loadFromFile(testLvlBG);
        lvlPxFG->loadFromFile(testLvlFG);
        lvlBG->setTexture(lvlPxBG);
        lvlFG->setTexture(lvlPxFG);
        lvlFG->setSize(scaledMapSize);
        lvlBG->setSize(scaledMapSize);
        music.openFromFile(lfTrack);
        music.setLoop(true);
        music.play();
        *bgColor = sf::Color(0xFF00FFFF);
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

static void updatePlayer()
{
    *player_x += *velocity_x;
    *player_y += *velocity_y;
    sf::Vector2f rectSize(lvlBG->getSize());
    if (*player_x < playerSprite->getOrigin().x)
    {
        *player_x = playerSprite->getOrigin().x;
    }
    else if (*player_x > (rectSize.x - playerTexture->getSize().x) / scaleFactor)
    {
        *player_x = (rectSize.x - playerTexture->getSize().x) / scaleFactor;
    }
    if (*player_y < playerSprite->getOrigin().y)
    {
        *player_y = playerSprite->getOrigin().y;
    }
    else if (*player_y > (rectSize.y) / scaleFactor)
    {
        *player_y = (rectSize.y) / scaleFactor;
    }
    playerSprite->setPosition(sf::Vector2f((*player_x - *cam_x) * scaleFactor,(*player_y - *cam_y) * scaleFactor));
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
        *velocity_x = -1.5;
        playerSprite->setScale(-scaleFactor,scaleFactor);
    }
    else if (rightPressed)
    {
        *velocity_x = 1.5;
        playerSprite->setScale(scaleFactor,scaleFactor);
    }
    else
    {
        *velocity_x = 0;
    }
    if (upPressed)
    {
        *velocity_y = -1.5;
    }
    else if (downPressed)
    {
        *velocity_y = 1.5;
    }
    else
    {
        *velocity_y = 0;
    }
}

static void drawHUD()
{
    sf::Text camXLabel(templateText);
    sf::Text camYLabel(templateText);
    sf::Text plrXLabel(templateText);
    sf::Text plrYLabel(templateText);
    camYLabel.setPosition(0, 27.5);
    plrXLabel.setPosition(pixelToTile(16),0);
    plrYLabel.setPosition(pixelToTile(16),27.5);
    std::string cxStr = std::to_string(*cam_x);
    std::string cyStr = std::to_string(*cam_y);
    std::string pxStr = std::to_string(*player_x);
    std::string pyStr = std::to_string(*player_y);
    camXLabel.setString("CX: " + cxStr);
    camYLabel.setString("CY: " + cyStr);
    plrXLabel.setString("PX: " + pxStr);
    plrYLabel.setString("PY: " + pyStr);
    window.draw(camXLabel);
    window.draw(camYLabel);
    window.draw(plrXLabel);
    window.draw(plrYLabel);
}

static void gameCleanup()
{
    delete cam_x;
    delete cam_y;
    delete isPaused;
    delete bgColor;
    delete lvlPxBG;
    delete lvlPxFG;
    delete playerSprite;
    delete playerTexture;
    delete player_x;
    delete player_y;
    delete velocity_x;
    delete velocity_y;
    delete maxSpeed;
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
        gameCleanup();
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

static void spawnPlayer()
{
    const std::string* playerTexturePath;
    if (player)
    {
        playerTexturePath = &stephSpr;
    }
    else
    {
        printerr(missingFuncErr);
    }
    playerTexture = new sf::Texture;
    playerTexture->loadFromFile(*playerTexturePath);
    playerSprite = new sf::Sprite(*playerTexture);
    playerSprite->setScale(sf::Vector2f(scaleFactor,scaleFactor));
    playerSprite->setOrigin(sf::Vector2f(playerTexture->getSize().x/2,playerTexture->getSize().y/2));
    player_x = new float(*cam_x);
    player_y = new float(*cam_x);
    velocity_x = new float(*cam_x);
    velocity_y = new float(*cam_x);
    maxSpeed = new float(*cam_x);
}

void gameInit()
{
    fadeRect.setFillColor(sf::Color::Black);
    cam_x = new float(0);
    cam_y = new float(*cam_x);
    isPaused = new bool(false);
    bgColor = new sf::Color(0x000000FF);
    lvlPxBG = new sf::Texture;
    lvlPxFG = new sf::Texture;
    lvlBG = new sf::RectangleShape(sf::Vector2f(window.getSize()));
    lvlFG = new sf::RectangleShape(*lvlBG);
    spawnPlayer();
    const types::u8* collisionArray = loadMap();
    while (window.isOpen())
    {
        sf::Event e;
        if (!*isPaused)
        {
            gameInputHdl_KB();
            camPos();
            updatePlayer();
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
        window.draw(*playerSprite);
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