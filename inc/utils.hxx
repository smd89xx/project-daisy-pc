#pragma once

#include "includes.hxx"

extern sf::RenderWindow window;
extern sf::Font font;
extern sf::Music music;
extern sf::RectangleShape fadeRect;
extern const types::u8 fontSize;
extern sf::SoundBuffer sbHvr;
extern sf::Sound sndHvr;
extern sf::SoundBuffer sbCnf;
extern sf::Sound sndCnf;
extern sf::SoundBuffer sbBack;
extern sf::Sound sndBack;
extern sf::Text templateText;
extern const float volFadeSpeed;
extern const sf::Color playerColors[];
extern const types::u8 volMin;
extern const types::u8 volMax;
extern const types::u8 fadeDark;
extern const types::u8 fadeLight;
extern types::u8 scaleFactor;
extern sf::Event e;
extern const types::u8 maxScale;
extern std::fstream saveFile;
extern char saveRAM[32768];
enum errorCodes
{
    genericErr,
    missingFuncErr,
    badLevelErr,
    missingAssetsErr,
    invalidScaleErr,
    invalidPlayerErr,
    invalidChecksum,
};

enum ds4Buttons
{
    buttonCross,
    buttonCircle,
    buttonTriangle,
    buttonSquare,
    buttonL1,
    buttonR1,
    buttonL2,
    buttonR2,
    buttonShare,
    buttonOptions,
    buttonPS,
    buttonL3,
    buttonR3,
};

enum ds4Axes
{
    axisL3X,
    axisL3Y,
    axisL2,
    axisR3X,
    axisR3Y,
    axisR2,
    axisDPADX, // why in the hell
    axisDPADY, // are these mapped as axes?!
};

enum sramLocations
{
    addrChkSum,
    addrPlayer = 4,
    addrDifficulty,
    addrScaling,
    addrLevel,
    addrLives,
    addrScore,
    addrNext = 13,
};

void screenFade(float speed, bool direction, float fadeTarget);
void drawMenu(const structs::Option* option, types::u8 length);
void fadeMusic(bool direction, float speed, float targetVolume);
float pixelToTile(float pos);
void printerr(int error = genericErr, std::string file = "utils.cxx", bool fallback = false);
void updScreenSize();
types::u32 generateSaveChkSum();
void updSRAM();
types::u32 readSRAM_u32(types::u16 startAddr);
void writeSRAM_u32(types::u16 startAddr, types::u32 value);