#include "inc/includes.hxx"

const char dot = '.';
const char space = ' ';

static void initAssets()
{
    std::string vmajStr = std::to_string(versionMajor);
    std::string vminStr = std::to_string(versionMinor);
    std::string vrevStr = std::to_string(versionRevision);
    std::string vcStr = vmajStr + dot + vminStr + dot + vrevStr;
    std::string titleStr = programTitle + space + releaseStageStringsL[releaseStage] + space + vcStr + space + buildTitle;
    window.setTitle(titleStr);
    sf::Image icon;
    bool assetsChk = icon.loadFromFile(favicon) & font.loadFromFile(scpTTF) & sbHvr.loadFromFile(hoverSFX) & sbCnf.loadFromFile(confSFX) & sbBack.loadFromFile(backSFX) & bitmapFont.loadFromFile(pixelFont) & cursorTexture.loadFromFile(cursorImg);
    if (!assetsChk)
    {
        printerr(missingAssetsErr,"initAssets()",true);
    }
    window.setIcon(icon.getSize().x,icon.getSize().y,icon.getPixelsPtr());
    font.setSmooth(false);
    window.setFramerateLimit(60);
    fadeRect.setSize(sf::Vector2f(window.getSize()));
    sndHvr.setBuffer(sbHvr);
    sndCnf.setBuffer(sbCnf);
    sndBack.setBuffer(sbBack);
    templateText.setFont(font);
    templateText.setCharacterSize(fontSize * scaleFactor);
    templateText.setString("Default string.");
    templateText.setOutlineThickness((3.5 / 3) * scaleFactor);
}

static void initSRAM()
{
    for (types::u16 i = 0; i < sizeof(saveRAM); i++)
    {
        saveRAM[i] = 0;
    }
    saveFile.open(saveFileLocation,std::ios::in);
    if (!saveFile)
    {
        for (types::u16 i = 0; i < maxSlots; i++)
        {
            saveSlots[i].writeToSRAM(addrSaves+(i*10));
        }
        saveRAM[addrScaling] = scaleFactor;
        saveRAM[addrVerInf] = saveVersion;
        saveFile.close();
        saveFile.open(saveFileLocation,std::ios::out);
        saveFile.write(saveRAM,sizeof(saveRAM));
        saveFile.close();
        saveFile.open(saveFileLocation,std::ios::in);
    }
    saveFile.read(saveRAM,sizeof(saveRAM));
    saveFile.close();
}

static void chkChecksum()
{
    types::u32 ogChkSum = readSRAM_u32(addrChkSum); 
    char sramNoChkSum[32764];
    for (types::u16 i = 0; i < sizeof(sramNoChkSum); i++)
    {
        sramNoChkSum[i] = saveRAM[i+4];
    }
    types::u32 checksum = CRC::Calculate(sramNoChkSum, sizeof(sramNoChkSum), CRC::CRC_32());
    if (ogChkSum == 0)
    {
        ogChkSum = generateSaveChkSum();
    }
    if (ogChkSum != checksum)
    {
        printerr(invalidChecksum,"chkChecksum()",true);
    }
}

static void chkJoypadVendor()
{
    sf::Joystick joy;
    types::u32 joyVendor = joy.getIdentification(0).vendorId;
    bool joyConnected = joy.isConnected(0);
    switch (joyVendor)
    {
        case ds4VID:
        {
            btnPrompts = (std::string*)ds4Prompts;
            break;
        }
        case xb1CCVID:
        {
            btnPrompts = (std::string*)xb1Prompts;
            break;
        }
        default:
        {
            if (joyConnected)
            {
                printerr(missingAssetsErr,"chkJoypadVendor()");
            }
            else
            {
                btnPrompts = (std::string*)ds4Prompts;
            }
            break;
        }
    }
}

int main(int argc, char** argv)
{
    initSRAM();
    chkChecksum();
    initAssets();
    chkJoypadVendor();
    types::u8 saveVersion_SRAM = saveRAM[addrVerInf];
    scaleFactor = saveRAM[addrScaling];
    updScreenSize();
    if (saveVersion_SRAM != saveVersion)
    {
        printerr(invalidSRAMVersion,"main(int,char**)");
    }
    title();
    return 0;
}