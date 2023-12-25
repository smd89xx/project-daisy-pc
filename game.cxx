#include "inc/includes.hxx"

sf::RectangleShape* lvlFG;
sf::RectangleShape* lvlBG;
sf::Texture* lvlPxBG;
sf::Texture* lvlPxFG;
sf::Vector2f* cameraPositions;
bool* isPaused;
const types::u8 pauseX = 18;
const types::u8 pauseY = 13;
const types::u8 pauseOptsAmnt = 3;
const structs::Option pauseMenu[] = 
{
    {pauseX,pauseY,"Return to Game"},
    {pauseX,pauseY+1.25,"Save and Exit"},
    {pauseX,pauseY+2.5,"Exit Without Saving"}
};
const types::u8 collisionRectAmounts[] = {5};
bool* runThread;
bool* isJumping;
enum cameraBoundaries {leftCamBnd = 144, rightCamBnd = 144, topCamBnd = 104, bottomCamBnd = 104};
enum tileTypes {tileBlank, tileSolid, tileSlopeE, tileSlopeW, tileSpringN, tileRing, tileSpringE, tileSpringW};
const sf::Vector2i playerHitboxes[] = {{0,0},{15,30}};
const sf::Vector2i playerSizes[] = {{32,32},{32,32}};
sf::SoundBuffer* sbGame;
sf::Sound* sndGame;
const structs::AnimMData playerAnimData[2][3] = 
{
    {
        {0,1,1},{1,2,15},{2,1,1},
    },
    {
        {0,1,1},{1,2,15},{2,1,1},
    }
};
sf::Texture* hudTexture;
sf::Texture* healthTexture;
const std::string playerNames[] = {"Lucy","Stephanie"};
structs::PlayerMData* playerObj;
std::thread* collisionThread;
types::u8* frameTimer;
types::u8* frameIndex;
types::u16* animIndex;
enum playerAnimIDs
{
    playerIdle,
    playerWalking,
    playerJumping,
};

static void camPos()
{
    sf::Vector2f rectSize(lvlPxBG->getSize());
    rectSize.x *= scaleFactor;
    rectSize.y *= scaleFactor;
    sf::Vector2f playerScreenPos(playerObj->position.x - cameraPositions->x, playerObj->position.y - cameraPositions->y);
    sf::Vector2i bgCamPos(*cameraPositions);
    bgCamPos.x /= 2;
    bgCamPos.y /= 2;
    if (playerScreenPos.x > rightCamBnd * scaleFactor)
    {
        cameraPositions->x = (playerObj->position.x - rightCamBnd);
    }
    else if (playerScreenPos.x < leftCamBnd * scaleFactor)
    {
        cameraPositions->x = (playerObj->position.x - leftCamBnd);
    }
    if (playerScreenPos.y > bottomCamBnd * scaleFactor)
    {
        cameraPositions->y = (playerObj->position.y - bottomCamBnd);
    }
    else if (playerScreenPos.y < topCamBnd * scaleFactor)
    {
        cameraPositions->y = (playerObj->position.y - topCamBnd);
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
    switch (saveSlots[slotIndex].level)
    {
    case 0:
    {
        lvlPxBG->loadFromFile(testLvlBG);
        lvlPxFG->loadFromFile(testLvlFG);
        lvlBG->setTexture(lvlPxBG);
        lvlFG->setTexture(lvlPxFG);
        playerObj->position.x = 48;
        playerObj->position.y = 160;
        music.openFromFile(lfTrack);
        music.setLoop(true);
        music.play();
        return nullptr;
        break;
    }
    default:
    {
        printerr(badLevelErr,"loadMap()");
        return nullptr;
        break;
    }
    }
}

static void manageAnim()
{
    if (*frameTimer < playerAnimData[saveSlots[slotIndex].player][*animIndex].frameTime)
    {
        (*frameTimer)++;
    }
    else
    {
        *frameTimer = 0;
        if (*frameIndex < playerAnimData[saveSlots[slotIndex].player][*animIndex].frameAmount - 1)
        {
            (*frameIndex)++;
        }
        else
        {
            *frameIndex = 0;
        }
        
    }
    playerObj->sprite.setTextureRect({((*frameIndex)*playerSizes[saveSlots[slotIndex].player].x),((*animIndex) * playerSizes[saveSlots[slotIndex].player].y),playerSizes[saveSlots[slotIndex].player].x,playerSizes[saveSlots[slotIndex].player].y});
}

static void updatePlayer()
{
    playerObj->position.x += playerObj->velocity.x;
    playerObj->position.y += playerObj->velocity.y;
    sf::Vector2f rectSize(lvlPxBG->getSize());
    rectSize.x *= scaleFactor;
    rectSize.y *= scaleFactor;
    if (playerObj->position.x < playerObj->sprite.getOrigin().x)
    {
        playerObj->position.x = playerObj->sprite.getOrigin().x;
    }
    else if (playerObj->position.x > (rectSize.x - playerObj->texture.getSize().x) / scaleFactor)
    {
        playerObj->position.x = (rectSize.x - playerObj->texture.getSize().x) / scaleFactor;
    }
    if (playerObj->position.y > (rectSize.y) / scaleFactor)
    {
        playerObj->position.y = (rectSize.y) / scaleFactor;
    }
    playerObj->sprite.setPosition(sf::Vector2f((playerObj->position.x - cameraPositions->x) * scaleFactor,(playerObj->position.y - cameraPositions->y) * scaleFactor));
}

static void gameInputHdl()
{
    if (!window.hasFocus())
    {
        return;
    }
    types::s8 XPos = sf::Joystick::getAxisPosition(0,(sf::Joystick::Axis)axisDPADX);
    types::s8 YPos = sf::Joystick::getAxisPosition(0,(sf::Joystick::Axis)axisDPADY);
    bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) || (XPos == -100);
    bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) || (XPos == 100);
    bool upPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up) || (YPos == -100);
    bool downPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down) || (YPos == 100);
    if (leftPressed)
    {
        if (!*isJumping)
        {
            *animIndex = playerWalking;
        }
        playerObj->velocity.x = -1.5;
        playerObj->sprite.setScale(-scaleFactor,scaleFactor);
    }
    else if (rightPressed)
    {
        if (!*isJumping)
        {
            *animIndex = playerWalking;
        }
        playerObj->velocity.x = 1.5;
        playerObj->sprite.setScale(scaleFactor,scaleFactor);
    }
    else
    {
        if (!*isJumping)
        {
            *animIndex = playerIdle;
        }
        playerObj->velocity.x = 0;
    }
}

static void gameCleanup()
{
    delete cameraPositions;
    delete isPaused;
    delete lvlBG;
    delete lvlPxBG;
    delete lvlFG;
    delete lvlPxFG;
    *runThread = false;
    collisionThread->join();
    delete runThread;
    delete isJumping;
    delete sbGame;
    delete sndGame;
    delete healthTexture;
    delete hudTexture;
    delete playerObj;
    delete frameTimer;
    delete frameIndex;
    delete animIndex;
}

static void gameBack()
{
    menuIndex = 0;
    gameCleanup();
    title();
}

static void exitEvent()
{
    updSRAM();
    window.close();
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
        *isPaused = false;
        break;
    }
    case 1:
    {
        saveSlots[slotIndex].writeToSRAM(addrSaves+(slotIndex*10));
        gameBack();
        break;
    }
    case 2:
    {
        gameBack();
        break;
    }
    default:
    {
        printerr(missingFuncErr,"selectMenuPause()");
        break;
    }
    }
}

static void spawnPlayer()
{
    const std::string* playerTexturePath;
    if (saveSlots[slotIndex].player)
    {
        playerTexturePath = &placeholderSpr;
    }
    else
    {
        playerTexturePath = &placeholderSpr;
    }
    playerObj = new structs::PlayerMData;
    playerObj->texture.loadFromFile(*playerTexturePath);
    playerObj->sprite.setTexture(playerObj->texture);
    playerObj->sprite.setTextureRect({0,0,32,32});
    playerObj->sprite.setScale(sf::Vector2f(scaleFactor,scaleFactor));
    playerObj->sprite.setOrigin(sf::Vector2f(playerObj->sprite.getTextureRect().width/2,playerObj->sprite.getTextureRect().height/2));
    isJumping = new bool(false);
    playerObj->jumpStrength = 9.8;
}

static void chkCollision(const sf::IntRect* collisionArray)
{
    while(*runThread)
    {
        
    }
}

static void cursorMove(bool direction)
{
    if (!direction)
    {
        sndHvr.play();
        if (menuIndex <= 0)
        {
            menuIndex = pauseOptsAmnt - 1;
        }
        else if (menuIndex == 2 && slotIndex == maxSlots)
        {
            menuIndex = 0;
        }
        else
        {
            menuIndex--;
        }
    }
    else
    {
        sndHvr.play();
        if (menuIndex >= pauseOptsAmnt - 1)
        {
            menuIndex = 0;
        }
        else if (menuIndex == 0 && slotIndex == maxSlots)
        {
            menuIndex += 2;
        }
        else
        {
            menuIndex++;
        }
    }
}

static void jump()
{
    if (*isJumping)
    {
        return;
    }
    *animIndex = playerJumping;
    sbGame->loadFromFile(jumpSFX);
    sndGame->setBuffer(*sbGame);
    sndGame->play();
    playerObj->position.y -= 5;
    playerObj->velocity.y -= playerObj->jumpStrength;
    *isJumping = true;
}

static void drawHUD()
{
    sf::RectangleShape hudIcon{sf::Vector2f(hudTexture->getSize())};
    hudIcon.setScale(scaleFactor,scaleFactor);
    hudIcon.setTexture(hudTexture);
    drawBitmapFont(playerNames[saveSlots[slotIndex].player],{2,0});
    std::stringstream scoreStr;
    scoreStr << "{" << std::setfill('0') << std::setw(8) << saveSlots[slotIndex].score;
    drawBitmapFont(scoreStr.str(),{12,0});
    sf::RectangleShape healthbar(sf::Vector2f(152,8));
    healthbar.setScale(scaleFactor,scaleFactor);
    healthbar.setTexture(healthTexture);
    healthbar.setTextureRect({0,0,8,8});
    healthbar.setPosition(sf::Vector2f(pixelToTile(2),pixelToTile(1)));
    sf::RectangleShape healthMeter(healthbar);
    healthMeter.setTextureRect({16,0,8,8});
    healthMeter.setFillColor(playerColors[saveSlots[slotIndex].player]);
    healthMeter.setSize(sf::Vector2f(saveSlots[slotIndex].health/5,8));
    sf::RectangleShape healthCap({8,8});
    healthCap.setScale(scaleFactor,scaleFactor);
    healthCap.setTexture(healthTexture);
    healthCap.setTextureRect({8,0,8,8});
    healthCap.setPosition(pixelToTile(21),pixelToTile(1));
    std::stringstream livesStr;
    livesStr << "|" << std::setfill('0') << std::setw(3) << (types::u16)saveSlots[slotIndex].lives;
    drawBitmapFont(livesStr.str(),{21.5,1});
    std::stringstream pxStr;
    pxStr << "PX: " << std::showpoint << std::fixed << std::setprecision(3) << playerObj->position.x;
    drawBitmapFont(pxStr.str(),{0,2});
    std::stringstream pyStr;
    pyStr << "PY: " << std::showpoint << std::fixed << std::setprecision(3) << playerObj->position.y;
    drawBitmapFont(pyStr.str(),{0,3});
    std::stringstream cxStr;
    cxStr << "PX: " << std::showpoint << std::fixed << std::setprecision(3) << cameraPositions->x;
    drawBitmapFont(cxStr.str(),{0,4});
    std::stringstream cyStr;
    cyStr << "PY: " << std::showpoint << std::fixed << std::setprecision(3) << cameraPositions->y;
    drawBitmapFont(cyStr.str(),{0,5});
    window.draw(hudIcon);
    window.draw(healthbar);
    window.draw(healthMeter);
    window.draw(healthCap);
}

static void initHUD()
{
    hudTexture = new sf::Texture;
    healthTexture = new sf::Texture;
    sf::IntRect iconTextureRect = {16*saveSlots[slotIndex].player,0,16,16};
    hudTexture->loadFromFile(hudIcons,iconTextureRect);
    healthTexture->loadFromFile(barImg);
}

static void drawBtnHints()
{
    bool joyConnected = sf::Joystick::isConnected(0);
    types::u8 startButton = startBtnID();
    if (joyConnected)
    {
        drawBitmapFont(btnPrompts[dpadUp] + "/" + btnPrompts[dpadDown] + ": Change selection",{0,29});
        drawBitmapFont(btnPrompts[startButton] + ": Unpause",{0,28});
        drawBitmapFont(btnPrompts[buttonCross] + ": Decision",{0,27});
    }
    else
    {
        drawBitmapFont("Up/Down: Change selection",{0,29});
        drawBitmapFont("Escape: Unpause",{0,28});
        drawBitmapFont("Enter: Decision",{0,27});
    }
}

void gameInit()
{
    fadeRect.setFillColor(sf::Color::Black);
    cameraPositions = new sf::Vector2f(0,0);
    isPaused = new bool(false);
    runThread = new bool(true);
    sbGame = new sf::SoundBuffer;
    sndGame = new sf::Sound;
    frameTimer = new types::u8(0);
    frameIndex = new types::u8(0);
    animIndex = new types::u16(0);
    spawnPlayer();
    const sf::IntRect* collisionArray = loadMap();
    initHUD();
    collisionThread = new std::thread(chkCollision,collisionArray);
    types::u8 startButton = startBtnID();
    while (window.isOpen())
    {
        manageAnim();
        window.clear(sf::Color::Black);
        window.draw(*lvlBG);
        window.draw(*lvlFG);
        window.draw(playerObj->sprite);
        drawHUD();
        if (!*isPaused)
        {
            gameInputHdl();
            camPos();
            updatePlayer();
            fadeMusic(false,volFadeSpeed,volMax);
            screenFade(volFadeSpeed,true,fadeDark);
        }
        else
        {
            fadeMusic(true,volFadeSpeed,50);
            screenFade(volFadeSpeed*3,false,0x7F);
            drawMenu(pauseMenu,pauseOptsAmnt);
            drawBtnHints();
        }
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
                        cursorMove(false);
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::Down)
                    {
                        cursorMove(true);
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
            case sf::Event::JoystickButtonPressed:
            {
                if (!window.hasFocus())
                {
                    break;
                }
                if (!*isPaused)
                {
                    if (e.joystickButton.button == startButton)
                    {
                        sndCnf.play();
                        *isPaused = true;
                    }
                    if (e.joystickButton.button == buttonCross)
                    {
                        jump();
                    }
                }
                else
                {
                    if (e.joystickButton.button == buttonCross)
                    {
                        sndCnf.play();
                        selectMenuPause();
                    }
                    else if (e.joystickButton.button == buttonOptions)
                    {
                        sndBack.play();
                        menuIndex = 0;
                        selectMenuPause();
                    }
                }
                break;
            }
            case sf::Event::JoystickMoved:
            {
                if (!window.hasFocus() || !*isPaused)
                {
                    break;
                }
                if (e.joystickMove.axis == axisDPADY)
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