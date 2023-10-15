#include "inc/includes.hxx"

sf::RectangleShape* lvlFG;
sf::RectangleShape* lvlBG;
sf::Texture* lvlPxBG;
sf::Texture* lvlPxFG;
sf::Color* bgColor;
types::u8 level = 0;
sf::Vector2f* cameraPositions;
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
sf::Vector2f* playerPositions;
sf::Vector2f* playerVelocities;
float* maxSpeed;

enum cameraBoundaries {leftCamBnd = 144, rightCamBnd = 144, topCamBnd = 104, bottomCamBnd = 104};

static void camPos()
{
    sf::Vector2f rectSize(lvlBG->getSize());
    sf::Vector2f playerScreenPos(playerPositions->x - cameraPositions->x, playerPositions->y - cameraPositions->y);
    if (playerScreenPos.x > rightCamBnd * scaleFactor)
    {
        cameraPositions->x = (playerPositions->x - rightCamBnd);
    }
    else if (playerScreenPos.x < leftCamBnd * scaleFactor)
    {
        cameraPositions->x = (playerPositions->x - leftCamBnd);
    }
    if (playerScreenPos.y > bottomCamBnd * scaleFactor)
    {
        cameraPositions->y = (playerPositions->y - bottomCamBnd);
    }
    else if (playerScreenPos.y < topCamBnd * scaleFactor)
    {
        cameraPositions->y = (playerPositions->y - topCamBnd);
    }
    if (cameraPositions->x < 0)
    {
        cameraPositions->x = 0;
    }
    else if (cameraPositions->x > (rectSize.x - window.getSize().x) / scaleFactor)
    {
        cameraPositions->x = (rectSize.x - window.getSize().x) / scaleFactor;
    }
    if (cameraPositions->y < 0)
    {
        cameraPositions->y = 0;
    }
    else if (cameraPositions->y > (rectSize.y - window.getSize().y) / scaleFactor)
    {
        cameraPositions->y = (rectSize.y - window.getSize().y) / scaleFactor;
    }
    lvlFG->setPosition(sf::Vector2f(-cameraPositions->x * scaleFactor,-cameraPositions->y * scaleFactor));
    lvlBG->setPosition(sf::Vector2f((-cameraPositions->x * scaleFactor)/2,(-cameraPositions->y * scaleFactor)/2));
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
        playerPositions->x = 48;
        playerPositions->y = 160;
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
    playerPositions->x += playerVelocities->x;
    playerPositions->y += playerVelocities->y;
    sf::Vector2f rectSize(lvlBG->getSize());
    if (playerPositions->x < playerSprite->getOrigin().x)
    {
        playerPositions->x = playerSprite->getOrigin().x;
    }
    else if (playerPositions->x > (rectSize.x - playerTexture->getSize().x) / scaleFactor)
    {
        playerPositions->x = (rectSize.x - playerTexture->getSize().x) / scaleFactor;
    }
    if (playerPositions->y < playerSprite->getOrigin().y)
    {
        playerPositions->y = playerSprite->getOrigin().y;
    }
    else if (playerPositions->y > (rectSize.y) / scaleFactor)
    {
        playerPositions->y = (rectSize.y) / scaleFactor;
    }
    playerSprite->setPosition(sf::Vector2f((playerPositions->x - cameraPositions->x) * scaleFactor,(playerPositions->y - cameraPositions->y) * scaleFactor));
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
        playerVelocities->x = -1.5;
        playerSprite->setScale(-scaleFactor,scaleFactor);
    }
    else if (rightPressed)
    {
        playerVelocities->x = 1.5;
        playerSprite->setScale(scaleFactor,scaleFactor);
    }
    else
    {
        playerVelocities->x = 0;
    }
    if (upPressed)
    {
        playerVelocities->y = -1.5;
    }
    else if (downPressed)
    {
        playerVelocities->y = 1.5;
    }
    else
    {
        playerVelocities->y = 0;
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
    std::string cxStr = std::to_string(cameraPositions->x);
    std::string cyStr = std::to_string(cameraPositions->y);
    std::string pxStr = std::to_string(playerPositions->x);
    std::string pyStr = std::to_string(playerPositions->y);
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
    delete cameraPositions;
    delete isPaused;
    delete bgColor;
    delete lvlPxBG;
    delete lvlPxFG;
    delete playerSprite;
    delete playerTexture;
    delete playerPositions;
    delete playerVelocities;
    delete maxSpeed;
}

static void selectMenuPause()
{
    if (menuIndex >= 1)
    {
        fadeRect.setFillColor(sf::Color::White);
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
    playerPositions = new sf::Vector2f(*cameraPositions);
    playerVelocities = new sf::Vector2f(*cameraPositions);
    maxSpeed = new float(cameraPositions->x);
}

static void chkCollision(const types::u8* collisionArray)
{

}

void gameInit()
{
    fadeRect.setFillColor(sf::Color::Black);
    cameraPositions = new sf::Vector2f(0,0);
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
        window.clear(*bgColor);
        window.draw(*lvlBG);
        window.draw(*lvlFG);
        window.draw(*playerSprite);
        if (!*isPaused)
        {
            gameInputHdl_KB();
            camPos();
            updatePlayer();
            fadeMusic(false,volFadeSpeed,volMax);
            screenFade(volFadeSpeed,true,fadeDark);
            chkCollision(collisionArray);
        }
        else
        {
            fadeMusic(true,volFadeSpeed,50);
            screenFade(volFadeSpeed*3,false,0x7F);
        }
        drawHUD();
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