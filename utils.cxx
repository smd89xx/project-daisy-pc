#include "inc/includes.hxx"

sf::RenderWindow window(sf::VideoMode(1280, 720),"",sf::Style::Close);;
sf::Font font;
sf::Music music;
sf::RectangleShape fadeRect;
const types::u8 fontSize = 8;
sf::SoundBuffer sbHvr;
sf::Sound sndHvr;
sf::SoundBuffer sbCnf;
sf::Sound sndCnf;
sf::SoundBuffer sbBack;
sf::Sound sndBack;
sf::Text templateText;
const float volFadeSpeed = 1.75;
const float scrnFadeSpeed = 2;
const sf::Color playerColors[] = {sf::Color(0xAA22EEFF),sf::Color(0xEE0044FF)};
const types::u8 volMin = 0;
const types::u8 volMax = 100;
const types::u8 fadeDark = 0xFF;
const types::u8 fadeLight = 0x00;
types::u8 scaleFactor = 3;
const types::u8 maxScale = 5;
const sf::Vector2u screenSizes[] = {{426,240},{854,480},{1280,720},{1708,960},{2134,1200}};
sf::Event e;
const types::u8 bsodTextLines = 2;
const types::u8 bsodX = 6;
const types::u8 bsodY = 5;
const types::u8 bsodSubTxtLines[] = {2,4};
const char lineEnd = '\n';
const char space = ' ';
std::fstream saveFile;
char saveRAM[32768];
sf::Texture bitmapFont;
sf::Texture cursorTexture;
std::string btnPrompts[17];
structs::SaveMData saveSlots[9];
const types::u16 maxSlots = 8;
types::u16 slotIndex = 0;
const std::string resTxtLocation = "res/txt/res.txt";
const std::string stringLocation = "res/txt/strings.txt";
std::ifstream resList(resTxtLocation);
std::ifstream stringList(stringLocation);

/// @brief Fades the screen in or out.
/// @param speed Speed of the fade. Bigger value = faster fade.
/// @param direction Direction of the fade. True for in, false for out.
/// @param fadeTarget Alpha value (0 - 255) that the fade routine stops at.
void screenFade(float speed, bool direction, float fadeTarget)
{
    float alpha = fadeRect.getFillColor().r;
    if (!direction)
    {
        alpha -= speed/2;
        if (alpha <= fadeTarget)
        {
            alpha = fadeTarget;
        }
    }
    else
    {
        alpha += speed*3;
        if (alpha >= fadeTarget)
        {
            alpha = fadeTarget;
        }
    }
    fadeRect.setFillColor(sf::Color(alpha,alpha,alpha,0xFF));
    window.draw(fadeRect,sf::BlendMultiply);
}

/// @brief Renders an Option structure.
/// @param option Option structure.
/// @param length Amount of items inside the option structure.
void drawMenu(const structs::Option* option, types::u8 length)
{
    for (types::u8 i = 0; i < length; i++)
    {
        structs::Option o(option[i]);
        drawBitmapFont(o.label,{o.x,o.y});
        sf::Sprite cursor(cursorTexture);
        cursor.setScale(scaleFactor,scaleFactor);
        cursor.setPosition(pixelToTile(option[menuIndex].x - 1),pixelToTile(option[menuIndex].y));
        cursor.setColor(playerColors[saveSlots[slotIndex].player]);
        window.draw(cursor);
    }
}

/// @brief Fades music in or out.
/// @param direction False = in, True = out
/// @param speed Smaller = slower
/// @param targetVolume The volume that the function stops at.
void fadeMusic(bool direction, float speed, float targetVolume)
{
    float volume = music.getVolume();
    if (direction)
    {
        volume -= speed;
        if (volume <= targetVolume)
        {
            volume = targetVolume;
        }
    }
    else
    {
        volume += speed;
        if (volume >= targetVolume)
        {
            volume = targetVolume;
        }
    }
    music.setVolume(volume);
}

/// @brief Converts a pixel co-ordinate to a tile co-ordinate (font size).
/// @param pos Position (in tiles)
/// @return Position in tiles
float pixelToTile(float pos)
{
    return pos * (fontSize * scaleFactor);
}

static void resetGame(int error)
{
    switch (error)
    {
    case invalidSRAMVersion:
    {
        if ((saveRAM[addrVerInf] == 1) && saveVersion == 2)
        {
            bool player = saveRAM[4];
            types::u8 difficulty = saveRAM[5];
            types::u8 level = saveRAM[7];
            types::u8 lives = saveRAM[8];
            types::u32 score = readSRAM_u32(9);
            saveSlots[0].difficulty = difficulty;
            saveSlots[0].level = level;
            saveSlots[0].lives = lives;
            saveSlots[0].player = player;
            saveSlots[0].score = score;
            for (types::u8 i = 0; i < maxSlots; i++)
            {
                saveSlots[i].writeToSRAM(4+(i*8));
            }
            updSRAM();
        }
        switch (saveRAM[addrVerInf])
        {
            default:
            {
                saveRAM[addrVerInf] = saveVersion;
                updSRAM();
                break;
            }
        }

        break;
    }
    case invalidChecksum:
    {
        updSRAM();
        break;
    }
    case invalidScaleErr:
    {
        scaleFactor = 1;
        updScreenSize();
        break;
    }
    case invalidPlayerErr:
    {
        saveSlots[slotIndex].player = true;
        break;
    }
    default:
    {
        title();
        break;
    }
    }
    title();
}

static void fallback_printerr(int error, std::string file)
{
    using namespace std;
    cout << getFileLine(strBSODFrown) << lineEnd << lineEnd;
    for (types::u8 i = 0; i < bsodTextLines; i++)
    {
        cout << getFileLine(strBSODMainTxt+i,&stringList) << lineEnd;
    }
    cout << lineEnd;
    for (types::u8 i = 0; i < bsodSubTxtLines[0]; i++)
    {
        cout << getFileLine(strBSODSubTxt+i,&stringList) << lineEnd;
    }
    cout << lineEnd;
    cout << getFileLine(strBSODInfoHdr,&stringList) << lineEnd;
    cout << getFileLine(strBSODStop,&stringList) << space << getFileLine(strSTOPCodes+error,&stringList) << lineEnd;
    cout << getFileLine(strBSODHexCode,&stringList) << space << hex << getFileLine(strHexPrefix) << setfill('0') << setw(types::varSizes::var32Bit*2) << error << lineEnd;
    cout << getFileLine(strBSODFuncCall,&stringList) << space << file << endl;
    if (error == invalidChecksum)
    {
        string sumPrompt = getFileLine(strBSODCChk,&stringList); 
        cout << lineEnd << sumPrompt << lineEnd;
        types::u32 checksum = generateSaveChkSum();
        cout << getFileLine(strHexPrefix) << hex << setfill('0') << setw(types::varSizes::var32Bit*2) << uppercase << checksum << endl;
    }
    exit(1);
}

/// @brief Displays an error screen.
/// @param error Error code.
/// @param file Origin of error.
/// @param fallback Determines whether it should use the graphics window or stdout. Note that crashing to stdout is unrecoverable; the entire game process is destroyed.
void printerr(int error, std::string file, bool fallback)
{
    sf::Texture bsodQRPx;
    sf::Texture bsodFrownPx;
    sf::SoundBuffer sbSad;
    bool assetsFailed = bsodFrownPx.loadFromFile(getFileLine(resBSODFrown,&resList)) & bsodQRPx.loadFromFile(getFileLine(resBSODQR,&resList)) & sbSad.loadFromFile(getFileLine(resCrashSFX,&resList));
    if (!assetsFailed || fallback)
    {
        fallback_printerr(error,file);
    }
    music.stop();
    sf::Sound sndSad;
    sndSad.setBuffer(sbSad);
    sndSad.play();
    font.setSmooth(true);
    sf::Text bsodDrawableText[bsodTextLines];
    sf::RectangleShape bsodFrownDrawable(sf::Vector2f(bsodFrownPx.getSize()));
    bsodFrownDrawable.setTexture(&bsodFrownPx);
    bsodFrownDrawable.setScale(sf::Vector2f(scaleFactor,scaleFactor));
    bsodFrownDrawable.setPosition(sf::Vector2f(pixelToTile(bsodX),pixelToTile(bsodY)));
    sf::RectangleShape bsodQRDrawable(sf::Vector2f(bsodQRPx.getSize()));
    bsodQRDrawable.setTexture(&bsodQRPx);
    bsodQRDrawable.setScale(sf::Vector2f(scaleFactor*1.5,scaleFactor*1.5));
    bsodQRDrawable.setPosition(sf::Vector2f(pixelToTile(bsodX),pixelToTile(bsodY+12)));
    const types::u8 bsodSubTxtLinesCmb = 6;
    /*for (types::u8 i = 0; i < sizeof(bsodSubTxtLines); i++)
    {
        bsodSubTxtLinesCmb += bsodSubTxtLines[i];
    }*/
    sf::Text bsodDrawableSubTxt[bsodSubTxtLinesCmb];
    for (types::u8 i = 0; i < bsodTextLines; i++)
    {
        bsodDrawableText[i] = templateText;
        bsodDrawableText[i].setString(getFileLine(strBSODMainTxt+i,&stringList));
        bsodDrawableText[i].setPosition(sf::Vector2f(pixelToTile(bsodX),pixelToTile(bsodY+8+i)));
        bsodDrawableText[i].setStyle(sf::Text::Style::Bold);
    }
    for (types::u8 i = 0; i < bsodSubTxtLines[0]; i++)
    {
        bsodDrawableSubTxt[i] = templateText;
        bsodDrawableSubTxt[i].setString(getFileLine(strBSODSubTxt+i,&stringList));
        bsodDrawableSubTxt[i].setPosition(sf::Vector2f(pixelToTile(bsodX+6.75),pixelToTile(bsodY+12+i)));
        bsodDrawableSubTxt[i].setStyle(sf::Text::Style::Bold);
        bsodDrawableSubTxt[i].setScale(0.65,0.65);
    }
    for (types::u8 i = 0; i < bsodSubTxtLines[1]; i++)
    {
        bsodDrawableSubTxt[i+2] = templateText;
        bsodDrawableSubTxt[i+2].setString(getFileLine(strBSODInfoHdr+i,&stringList));
        bsodDrawableSubTxt[i+2].setPosition(sf::Vector2f(pixelToTile(bsodX+6.75),pixelToTile(bsodY+15+i)));
        bsodDrawableSubTxt[i+2].setStyle(sf::Text::Style::Bold);
        bsodDrawableSubTxt[i+2].setScale(0.55,0.55);
    }
    sf::Text bsodDrawableStopText(bsodDrawableSubTxt[3]);
    bsodDrawableStopText.setString(getFileLine(strSTOPCodes+error,&stringList));
    bsodDrawableStopText.setPosition(pixelToTile(bsodX+13.75),pixelToTile(bsodY+16));
    std::stringstream hexStr;
    hexStr << "0x" << std::setfill('0') << std::setw(types::varSizes::var32Bit*2) << std::hex << error;
    sf::Text bsodDrawableStopHex(bsodDrawableStopText);
    bsodDrawableStopHex.setString(hexStr.str());
    bsodDrawableStopHex.setPosition(pixelToTile(bsodX+14.25),pixelToTile(bsodY+17));
    sf::Text bsodDrawablePtr(bsodDrawableStopHex);
    bsodDrawablePtr.setString(file);
    bsodDrawablePtr.setPosition(pixelToTile(bsodX+11.35),pixelToTile(bsodY+18));
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        window.draw(bsodFrownDrawable);
        window.draw(bsodQRDrawable);
        for (types::u8 i = 0; i < bsodTextLines; i++)
        {
            window.draw(bsodDrawableText[i]);
        }
        for (types::u8 i = 0; i < bsodSubTxtLines[0]; i++)
        {
            window.draw(bsodDrawableSubTxt[i]);
        }
        for (types::u8 i = 0; i < bsodSubTxtLines[1]; i++)
        {
            window.draw(bsodDrawableSubTxt[i+2]);
        }
        window.draw(bsodDrawableStopText);
        window.draw(bsodDrawableStopHex);
        window.draw(bsodDrawablePtr);
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
                    sndSad.stop();
                    resetGame(error);
                    break;
                }
                case sf::Event::JoystickButtonPressed:
                {
                    if (!window.hasFocus())
                    {
                        break;
                    }
                    sndSad.stop();
                    resetGame(error);
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

/// @brief Changes screen and viewport scale based on the 'scaleFactor' variable.
void updScreenSize()
{
    if (scaleFactor - 1 >= maxScale)
    {
        printerr(invalidScaleErr);
    }
    sf::Vector2u baseSize = screenSizes[scaleFactor-1];
    sf::View view;
    view.setSize(sf::Vector2f(baseSize));
    view.setCenter(baseSize.x / 2,baseSize.y / 2);
    window.setSize(baseSize);
    window.setView(view);
    fadeRect.setSize(sf::Vector2f(baseSize));
    templateText.setCharacterSize(fontSize * scaleFactor);
    templateText.setOutlineThickness((3.5 / 3) * scaleFactor);
}

/// @brief Generates a checksum for the save file.
/// @return Checksum for the entire savefile (minus the first 4 bytes, since that is where the checksum is stored.)
types::u32 generateSaveChkSum()
{
    char sramNoChkSum[32764];
    for (types::u16 i = 0; i < sizeof(sramNoChkSum); i++)
    {
        sramNoChkSum[i] = saveRAM[i+4];
    }
    types::u32 checksum = CRC::Calculate(sramNoChkSum,sizeof(sramNoChkSum),CRC::CRC_32());
    writeSRAM_u32(addrChkSum,checksum);
    return checksum;
}

/// @brief Writes all the values currently in the save RAM to the save file.
void updSRAM()
{
    types::u32 checksum = generateSaveChkSum();
    saveFile.open(getFileLine(resSaveFile,&resList),std::ios::out);
    saveFile.write(saveRAM,sizeof(saveRAM));
    saveFile.close();
}

/// @brief Reads a 32-bit value from the save RAM.
/// @param startAddr First byte to read from.
/// @return A 32-bit number from the specified location in the save RAM.
types::u32 readSRAM_u32(types::u16 startAddr)
{
    types::u32 dword = 0;
    types::u8* byte = (types::u8*)&dword;
    byte[0] = saveRAM[startAddr+3];
    byte[1] = saveRAM[startAddr+2];
    byte[2] = saveRAM[startAddr+1];
    byte[3] = saveRAM[startAddr];
    return dword;
}

/// @brief Writes a 32-bit value to the save RAM.
/// @param startAddr First byte to write to.
/// @param value The 32-bit value to write.
void writeSRAM_u32(types::u16 startAddr, types::u32 value)
{
    types::u8* valueByte = (types::u8*)&value;
    saveRAM[startAddr+3] = valueByte[0];
    saveRAM[startAddr+2] = valueByte[1];
    saveRAM[startAddr+1] = valueByte[2];
    saveRAM[startAddr] = valueByte[3];
}

/// @brief Reads a 16-bit value from the save RAM.
/// @param startAddr First byte to read from. 
/// @return A 16-bit value from the specified location in the save RAM.
types::u16 readSRAM_u16(types::u16 startAddr)
{
    types::u16 word = 0;
    types::u8* byte = (types::u8*)&word;
    byte[0] = saveRAM[startAddr+1];
    byte[1] = saveRAM[startAddr];
    return word;
}

/// @brief Writes a 16-bit value to the save RAM.
/// @param startAddr First byte to write to.
/// @param value The 16-bit value to write.
void writeSRAM_u16(types::u16 startAddr, types::u16 value)
{
    types::u8* valueByte = (types::u8*)&value;
    saveRAM[startAddr+1] = valueByte[0];
    saveRAM[startAddr] = valueByte[1];
}

/// @brief Draws text using a bitmap font instead of a TrueType font.
/// @param text String to draw.
/// @param position Position of string.
void drawBitmapFont(std::string text, sf::Vector2f position)
{
    position.x *= scaleFactor;
    position.y *= scaleFactor;
    for (types::u16 i = 0; i < text.length(); i++)
    {
        sf::Sprite character(bitmapFont);
        character.setScale(scaleFactor,scaleFactor);
        character.setPosition(pixelToTile((position.x/scaleFactor)+i),pixelToTile(position.y/scaleFactor));
        character.setTextureRect({8*(text.c_str()[i] - 0x20),0,8,8});
        window.draw(character);
    }
}

/// @brief Converts a 4bpc color to an 8bpc color.
/// @param rgb3 Color to convert.
/// @return Color in 8bpc format
types::u32 RGB4toRGB8(types::u16 rgb4)
{
    types::u32 rgb8 = 0x000000FF;
    types::u8 r,g,b;
    r = rgb4 >> 8;
    g = rgb4 >> 4;
    b = (types::u8)rgb4;
    r = r << 4;
    g = g << 4;
    b = b << 4;
    types::u8* channels = (types::u8*)&rgb8;
    channels[3] = r + (r >> 4);
    channels[2] = g + (g >> 4);
    channels[1] = b + (b >> 4);
    channels[0] = 0xFF;
    return rgb8;
}

/// @brief Determines which button on your controller pertains to [an equivalent of] the start button.
/// @return Button ID of the start button.
types::u8 startBtnID()
{

    types::u8 startButton;
    types::u32 vid = sf::Joystick::getIdentification(0).vendorId;
    bool joyConnected = sf::Joystick::isConnected(0);
    if (!joyConnected)
    {
        startButton = 0;
    }
    else if (vid == xb1CCVID)
    {
        startButton = buttonMenu;
    }
    else if (vid == ds4VID)
    {
        startButton = buttonOptions;
    }
    return startButton;
}

/// @brief Gets a string from a specific line in a text file.
/// @param lineNum Desired line number.
/// @param file Source file.
/// @return String containing text line.
std::string getFileLine(types::u16 lineNum,std::ifstream* file)
{
    std::string line;
    for (types::u16 i = 0; i < lineNum; i++)
    {
        getline(*file,line);
    }
    file->seekg(0);
    return line;
}