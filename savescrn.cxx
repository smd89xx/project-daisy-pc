#include "inc/includes.hxx"

const types::u8 saveX = 0;
const types::u8 saveY = 2;
const std::string diffStrs[] = {"Easy","Normal","Hard","Legend"};
const std::string lvlStrs[] = {"Testing Grounds","To be determined"};
const std::string blankString = "";
sf::Texture* saveBG;

enum saveDestinations
{
    destGame,
    destTitle,
    destPrefs,
};

static void exitEvent()
{
    updSRAM();
    window.close();
}

static void setLvlBG()
{
    std::string* file;
    switch (saveSlots[slotIndex].level)
    {
        case 0:
        {
            file = (std::string*)&testLvlBG;
            break;
        }
        default:
        {
            printerr(badLevelErr,"setLvlBG()");
            break;
        }
    }
    saveBG->loadFromFile(*file);
    saveBG->setRepeated(true);
}

static void moveCursor(bool direction)
{
    sndHvr.play();
    if (direction)
    {
        if (slotIndex >= maxSlots-1)
        {
            slotIndex = 0;
        }
        else
        {
            slotIndex++;
        }
    }
    else
    {
        if (slotIndex == 0)
        {
            slotIndex = maxSlots-1;
        }
        else
        {
            slotIndex--;
        }
    }
    setLvlBG();
}

static void selectSave(types::u8 destination)
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
    switch (destination)
    {
        case 0:
        {
            gameInit();
            break;
        }
        case 1:
        {
            title();
            break;
        }
        case 2:
        {
            prefsScreen();
            break;
        }
        default:
        {
            printerr(missingFuncErr,"selectSave(unsigned char)");
            break;
        }
    }
}

static void displayHints()
{
    bool joyConnected = sf::Joystick::isConnected(0);
    if (joyConnected)
    {
        drawBitmapFont(btnPrompts[buttonCross] + ": Decision",{0,25});
        drawBitmapFont(btnPrompts[buttonCircle] + ": Exit",{0,26});
        drawBitmapFont(btnPrompts[buttonR1] + " + " + btnPrompts[buttonCross] + ": No Save Mode",{0,27});
        drawBitmapFont(btnPrompts[buttonTriangle] + ": Preferences [Current Save]",{0,28});
        drawBitmapFont(btnPrompts[buttonR1] + " + " + btnPrompts[buttonTriangle] + ": Preferences [No Save]", {0,29});
    }
    else
    {
        drawBitmapFont("Enter: Decision",{0,25});
        drawBitmapFont("Escape: Exit",{0,26});
        drawBitmapFont("*Shift: No Save Mode",{0,27});
        drawBitmapFont("*CTRL: Preferences [Current Save]",{0,28});
        drawBitmapFont("*ALT: Preferences [No Save]", {0,29});
    }
}

void saveScreen()
{
    for (types::u8 i = 0; i < maxSlots; i++)
    {
        saveSlots[i] = saveSlots[i].readFromSRAM(addrSaves+(i*10));
    }
    saveBG = new sf::Texture;
    setLvlBG();
    fadeRect.setFillColor(sf::Color::Black);
    music.openFromFile(lsTrack);
    music.setLoop(true);
    music.play();
    types::u32 color = RGB4toRGB8(0x086C);
    sf::Texture plrIcon;
    plrIcon.loadFromFile(hudIcons);
    sf::Texture barTextures;
    barTextures.loadFromFile(barImg);
    sf::RectangleShape iconDrawable(sf::Vector2f({16,16}));
    iconDrawable.setTexture(&plrIcon);
    iconDrawable.setScale(scaleFactor,scaleFactor);
    std::stringstream scoreStr;
    std::stringstream livesStr;
    sf::RectangleShape healthbar(sf::Vector2f(152,8));
    healthbar.setScale(scaleFactor,scaleFactor);
    healthbar.setTexture(&barTextures);
    healthbar.setTextureRect({0,0,8,8});
    healthbar.setPosition(sf::Vector2f(pixelToTile(2),pixelToTile(1)));
    sf::RectangleShape healthMeter(healthbar);
    healthMeter.setTextureRect({16,0,8,8});
    sf::RectangleShape healthCap({8,8});
    healthCap.setScale(scaleFactor,scaleFactor);
    healthCap.setTexture(&barTextures);
    healthCap.setTextureRect({8,0,8,8});
    healthCap.setPosition(pixelToTile(21),pixelToTile(1));
    sf::RectangleShape lvlBG;
    lvlBG.setTexture(saveBG);
    lvlBG.setSize(sf::Vector2f(window.getSize()));
    float scroll;
    while (window.isOpen())
    {
        healthMeter.setFillColor(playerColors[saveSlots[slotIndex].player]);
        healthMeter.setSize(sf::Vector2f(saveSlots[slotIndex].health/5,8));
        livesStr.str(blankString);
        scoreStr.str(blankString);
        livesStr << "|" << std::setfill('0') << std::setw(3) << (types::u16)saveSlots[slotIndex].lives;
        scoreStr << "{" << std::setfill('0') << std::setw(8) << saveSlots[slotIndex].score;
        iconDrawable.setTextureRect({16*(saveSlots[slotIndex].player),0,16,16});
        scroll += 0.3334;
        lvlBG.setTextureRect({(types::u16)scroll,0,426,240});
        window.clear(sf::Color(color));
        window.draw(lvlBG);
        drawBitmapFont("Save " + std::to_string(slotIndex) + ":",{saveX,saveY});
        drawBitmapFont(playerNames[saveSlots[slotIndex].player],{2,0});
        drawBitmapFont("Difficulty: " + diffStrs[saveSlots[slotIndex].difficulty],{saveX+2,saveY+1});
        drawBitmapFont("Level: " + lvlStrs[saveSlots[slotIndex].level],{saveX+2,saveY+2});
        drawBitmapFont(livesStr.str(),{21.5,1});
        drawBitmapFont(scoreStr.str(),{12,0});
        displayHints();
        drawBitmapFont("No Save:",{saveX,saveY+4});
        drawBitmapFont("Difficulty: " + diffStrs[saveSlots[maxSlots].difficulty],{saveX+2,saveY+5});
        drawBitmapFont("Level: " + lvlStrs[saveSlots[maxSlots].level],{saveX+2,saveY+6});
        drawBitmapFont("Player: " + playerNames[saveSlots[maxSlots].player],{saveX+2,saveY+7});
        window.draw(iconDrawable);
        window.draw(healthbar);
        window.draw(healthMeter);
        window.draw(healthCap);
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
                        selectSave(destGame);
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::Escape)
                    {
                        sndBack.play();
                        selectSave(destTitle);
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::LShift || e.key.scancode == sf::Keyboard::Scan::RShift)
                    {
                        slotIndex = 8;
                        sndCnf.play();
                        selectSave(destGame);
                    }
                    if (e.key.scancode == sf::Keyboard::Scan::LControl || e.key.scancode == sf::Keyboard::Scan::RControl)
                    {
                        sndCnf.play();
                        selectSave(destPrefs);
                    }
                    else if (e.key.scancode == sf::Keyboard::Scan::LAlt || e.key.scancode == sf::Keyboard::Scan::RAlt)
                    {
                        slotIndex = 8;
                        sndCnf.play();
                        selectSave(destPrefs);
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
                            if (sf::Joystick::isButtonPressed(0,buttonR1))
                            {
                                slotIndex = 8;
                            }
                            sndCnf.play();
                            selectSave(destGame);
                            break;
                        }
                        case buttonCircle:
                        {
                            sndBack.play();
                            selectSave(destTitle);
                            break;
                        }
                        case buttonTriangle:
                        {
                            if (sf::Joystick::isButtonPressed(0,buttonR1))
                            {
                                slotIndex = 8;
                            }
                            sndCnf.play();
                            selectSave(destPrefs);
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
                    switch (e.joystickMove.axis)
                    {
                        case axisDPADX:
                        {
                            if (e.joystickMove.position == -100)
                            {
                                moveCursor(false);
                            }
                            else if (e.joystickMove.position == 100)
                            {
                                moveCursor(true);
                            }
                            break;
                        }
                        default:
                        {
                            break;
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