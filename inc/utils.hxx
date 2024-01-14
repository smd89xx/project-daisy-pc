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
extern std::string btnPrompts[17];
extern structs::SaveMData saveSlots[9];
extern types::u16 slotIndex;
extern const types::u16 maxSlots;
extern std::ifstream resList;
extern std::ifstream stringList;
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
    addrCtrlMap,
};

// Specifically, vendor IDs
enum controllerIDs
{
    ds4VID = 32903,
    xb1CCVID = 9414,
};

enum stringLines
{
    strInitAssetFunc = 1,
    strDefault,
    strChkChksum,
    strChkJPVendor,
    strMain,
    strVersionHdr,
    strCopyInfo,
    strStartTxt,
    strStartTxtKB,
    strGameTitle,
    strBuildNum,
    strRSShort,
    strRSLong = 19,
    strBSODMainTxt = 26,
    strBSODSubTxt = 28,
    strBSODInfoHdr = 30,
    strBSODStop,
    strBSODHexCode,
    strBSODFuncCall,
    strSTOPCodes,
    strEmpty = 42,
    strDPADDirs,
    strBtnPS = 47,
    strBtnXbox = 60,
    strBSODCChk = 73,
    strDiffs,
    strLvlNames = 78,
    strldLvlBGFunc = 80,
    strSelectSaveFunc,
    strDecisionPrompt,
    strExitPrompt,
    strPromptPlus,
    strNoSavePrompt,
    strPrefsPrompt,
    strPrefsSave,
    strPrefsNoSave,
    strHoverPrompt,
    strKeyEscape,
    strKey0,
    strKey1,
    strKey2,
    strKey3,
    strKey4,
    strKey5,
    strKey6,
    strKey7,
    strKey8,
    strKey9,
    strKey10,
    strKey11,
    strKey12,
    strKeyTilde,
    strKeyGrave,
    strKeyExclam,
    strKeyCAt,
    strKeyNumSign,
    strKeyDollarSign,
    strKeyPercent,
    strKeyCaret,
    strKeyAmpersand,
    strKeyAsterisk,
    strKeyLParam,
    strKeyRParam,
    strKeyHyphen,
    strKeyUnderscore,
    strKeyEqualSign,
    strKeyPlus,
    strKeyQ,
    strKeyW,
    strKeyE,
    strKeyR,
    strKeyT,
    strKeyY,
    strKeyU,
    strKeyI,
    strKeyO,
    strKeyP,
    strKeyA,
    strKeyS,
    strKeyD,
    strKeyF,
    strKeyG,
    strKeyH,
    strKeyJ,
    strKeyK,
    strKeyL,
    strKeyZ,
    strKeyX,
    strKeyC,
    strKeyV,
    strKeyB,
    strKeyN,
    strKeyM,
    strKeyLSqBracket,
    strKeyRSqBracket,
    strKeyLCurlBracket,
    strKeyRCurlBracket,
    strKeyBSlash,
    strKeyPipe,
    strKeySemicolon,
    strKeyColon,
    strKeySQuote,
    strKeyDQuote,
    strKeyComma,
    strKeyLessThan,
    strKeyPeriod,
    strKeyGreaterThan,
    strKeySlash,
    strKeyQuestion,
    strKeyLArrow,
    strKeyRArrow,
    strKeyUArrow,
    strKeyDArrow,
    strKeyLCtrl,
    strKeyLAlt,
    strKeySuper,
    strKeyLShift,
    strKeyTab,
    strKeyRCtrl,
    strKeyRAlt,
    strKeyRShift,
    strKeyBackspace,
    strKeyBCtrl,
    strKeyBAlt,
    strKeyBShift,
    strKeyEnter,
    strKeyPause,
    strKeyInsert,
    strKeyHome,
    strKeyPgUp,
    strKeyPgDown,
    strKeyEnd,
    strKeyDelete,
    strKeySpace,
    strSaveHdr,
    strDiffHdr,
    strLvlHdr,
    strNoSaveHdr,
    strMenuPrefsFunc,
    strSpace,
    strIncDec,
    strScaling,
    strNo,
    strYes,
    strMusic,
    strSfx,
    strSeconds,
    strSpeedDelt,
    strSDeltCap,
    strTogglePausePrompt,
    strStartSndPrompt,
    strTitleHdr,
    strAlbumHdr,
    strArtistHdr,
    strLoopHdr,
    strAudioTypeHdr,
    strElapsed,
    strTotal,
    strTimeSuffix,
    strPlrNames,
    strMapInitFunc = 214,
    strPauseMenuFunc,
    strXPos,
    strYPos,
    strWaterTimer,
    strUnpausePrompt,
    strVersionNum,
    strPlrHdr,
    strBSODFrown,
    strHexPrefix,
    strJukebox,
    strExit,
    strUnpause,
    strExitSave,
    strExitNoSave,
    strAlbumGame,
    strArtistMe,
    strUnknown,
    strAlbum,
    strArtist,
    strMenu,
    strHover,
    strSelect,
    strBack,
    strTestMus,
    strLvlClrMus,
    strChrSelectMus,
    strAIZ2Mus,
    strZoneSelectMus,
    strCrashSFX,
    strJumpSFX,
    strWarnBellSFX,
    strSplashSFX,
    strS3Album,
    strSAdvance1Album,
    strSAdvance1Artists,
    strS3Artist,
};

enum resLines
{
    resIcon = 1,
    resTestMus,
    resTitleLogo,
    resCursor,
    resGameTTF,
    resLvlClrMus,
    resHvrSFX,
    resTitleMus,
    resTestLvlMus,
    resDecisionSFX,
    resPrefsMus,
    resMDGameTTF,
    resBackSFX,
    resTestLvlBG,
    resTestLvlFG,
    resStephSpr,
    resMonoTTF,
    resBSODFrown,
    resBSODQR,
    resCrashSFX,
    resJumpSFX,
    resAnimTestSpr,
    resSaveFile,
    resHUDIcons,
    resHealthbarTextures,
    resBMPFont,
    resTitleBG,
    resWarningBellSFX,
    resSplashSFX,
    resWaveSpr,
};

void screenFade(float speed, bool direction, float fadeTarget);
void drawMenu(const structs::Option* option, types::u8 length);
void fadeMusic(bool direction, float speed, float targetVolume);
float pixelToTile(float pos);
void printerr(int error = genericErr, std::string file = "utils.hxx", bool fallback = false);
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
std::string getFileLine(types::u16 lineNum,std::ifstream* file = &stringList);