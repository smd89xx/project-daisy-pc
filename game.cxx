#include "inc/includes.hxx"

sf::RectangleShape* lvlFG;
sf::RectangleShape* lvlBG;
sf::Texture* lvlPxBG;
sf::Texture* lvlPxFG;
types::u8 level = 0;
sf::Vector2f* cameraPositions;
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
const types::u8 collisionRectAmounts[] = {5};
std::thread* collisionThread;
structs::Directions* playerDirection;
enum cameraBoundaries {leftCamBnd = 144, rightCamBnd = 144, topCamBnd = 104, bottomCamBnd = 104};
enum tileTypes {tileBlank, tileSolid, tileSlopeE, tileSlopeW, tileSpringN, tileRing, tileSpringE, tileSpringW};

static void camPos()
{
    sf::Vector2f rectSize(lvlPxBG->getSize());
    rectSize.x *= scaleFactor;
    rectSize.y *= scaleFactor;
    sf::Vector2f playerScreenPos(playerPositions->x - cameraPositions->x, playerPositions->y - cameraPositions->y);
    sf::Vector2i bgCamPos(*cameraPositions);
    bgCamPos.x /= 2;
    bgCamPos.y /= 2;
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
    lvlBG->setTextureRect(sf::IntRect(bgCamPos,sf::Vector2i(window.getSize().x / scaleFactor, window.getSize().y / scaleFactor)));
    lvlFG->setTextureRect(sf::IntRect(sf::Vector2i(*cameraPositions),sf::Vector2i(window.getSize().x / scaleFactor, window.getSize().y / scaleFactor)));
}

static const sf::IntRect* loadMap()
{
    lvlFG = new sf::RectangleShape;
    lvlBG = new sf::RectangleShape;
    lvlPxBG = new sf::Texture;
    lvlPxFG = new sf::Texture;
    lvlBG->setSize(sf::Vector2f(window.getSize()));
    lvlFG->setSize(sf::Vector2f(window.getSize()));
    switch (level)
    {
    case 0:
    {
        lvlPxBG->loadFromFile(testLvlBG);
        lvlPxFG->loadFromFile(testLvlFG);
        lvlBG->setTexture(lvlPxBG);
        lvlFG->setTexture(lvlPxFG);
        playerPositions->x = 48;
        playerPositions->y = 160;
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

static void updatePlayer()
{
    playerPositions->x += playerVelocities->x;
    playerPositions->y += playerVelocities->y;
    sf::Vector2f rectSize(lvlPxBG->getSize());
    rectSize.x *= scaleFactor;
    rectSize.y *= scaleFactor;
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
        playerDirection->left = true;
        playerDirection->right = false;
    }
    else if (rightPressed)
    {
        playerVelocities->x = 1.5;
        playerSprite->setScale(scaleFactor,scaleFactor);
        playerDirection->left = false;
        playerDirection->right = true;
    }
    else
    {
        playerVelocities->x = 0;
        playerDirection->left = false;
        playerDirection->right = false;
    }
    if (upPressed)
    {
        playerVelocities->y = -1.5;
        playerDirection->up = true;
        playerDirection->down = false;
    }
    else if (downPressed)
    {
        playerVelocities->y = 1.5;
        playerDirection->up = false;
        playerDirection->down = true;
    }
    else
    {
        playerVelocities->y = 0;
        playerDirection->up = false;
        playerDirection->down = false;
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
    delete lvlBG;
    delete lvlPxBG;
    delete lvlFG;
    delete lvlPxFG;
    delete playerSprite;
    delete playerTexture;
    delete playerPositions;
    delete playerVelocities;
    delete maxSpeed;
    collisionThread->join();
    delete collisionThread;
    delete playerDirection;
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
    maxSpeed = new float(1.5);
    playerDirection = new structs::Directions;
}

static void chkCollision(const sf::IntRect* collisionArray)
{
    sf::IntRect playerHitbox;
    sf::Vector2i hitboxPosition;
    types::u8 collisionIndex = 0;
    while(!*isPaused)
    {
        if (collisionIndex == collisionRectAmounts[level])
        {
            collisionIndex = 0;
        }
        else
        {
            collisionIndex++;
        }
        hitboxPosition.x = playerPositions->x - playerSprite->getOrigin().x;
        hitboxPosition.y = playerPositions->y - playerSprite->getOrigin().y;
        playerHitbox = {hitboxPosition,sf::Vector2i(17,30)};
        bool isIntersecting = playerHitbox.intersects(collisionArray[collisionIndex]);
        if (isIntersecting)
        {
            if (playerDirection->down)
            {
                playerPositions->y = collisionArray[collisionIndex].top - playerSprite->getOrigin().y;
            }
            else if (playerDirection->up)
            {
                playerPositions->y = (collisionArray[collisionIndex].top + collisionArray[collisionIndex].height) + playerSprite->getOrigin().y;
            }
            if (playerDirection->right)
            {
                playerPositions->x = collisionArray[collisionIndex].left - playerSprite->getOrigin().x;
            }
            else if (playerDirection->left)
            {
                playerPositions->x = (collisionArray[collisionIndex].left + collisionArray[collisionIndex].width) + playerSprite->getOrigin().x;
            }
        }
    }
}

void gameInit()
{
    fadeRect.setFillColor(sf::Color::Black);
    cameraPositions = new sf::Vector2f(0,0);
    isPaused = new bool(false);
    spawnPlayer();
    const sf::IntRect* collisionArray = loadMap();
    collisionThread = new std::thread(chkCollision,collisionArray);
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        window.draw(*lvlBG);
        window.draw(*lvlFG);
        window.draw(*playerSprite);
        drawHUD();
        if (!*isPaused)
        {
            gameInputHdl_KB();
            camPos();
            updatePlayer();
            fadeMusic(false,volFadeSpeed,volMax);
            screenFade(volFadeSpeed,true,fadeDark);
        }
        else
        {
            fadeMusic(true,volFadeSpeed,50);
            screenFade(volFadeSpeed*3,false,0x7F);
        }
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