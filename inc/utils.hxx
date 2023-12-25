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
extern sf::Texture bitmapFont;
extern sf::Texture cursorTexture;
extern std::string* btnPrompts;
extern const std::string ds4Prompts[];
extern const std::string xb1Prompts[];
extern structs::SaveMData saveSlots[9];
extern types::u16 slotIndex;
extern const types::u16 maxSlots;
enum errorCodes
{
    genericErr,
    missingFuncErr,
    badLevelErr,
    missingAssetsErr,
    invalidScaleErr,
    invalidPlayerErr,
    invalidChecksum,
    invalidSRAMVersion,
};

enum joyButtons
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
    buttonView = 6,
    buttonMenu,
    buttonXB,
    buttonLS,
    buttonRS,
};

enum joyAxes
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

enum DPADStrIndxs
{
    dpadLeft = 13,
    dpadRight,
    dpadUp,
    dpadDown,
};

enum sramLocations
{
    addrChkSum,
    addrSaves = 4,
    addrScaling = 84,
    addrVerInf,
};

// Specifically, vendor IDs
enum controllerIDs
{
    ds4VID = 32903,
    xb1CCVID = 9414,
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
types::u16 readSRAM_u16(types::u16 startAddr);
void writeSRAM_u16(types::u16 startAddr, types::u16 value);
void drawBitmapFont(std::string text, sf::Vector2f position);
types::u32 RGB4toRGB8(types::u16 rgb4);
types::u8 startBtnID();