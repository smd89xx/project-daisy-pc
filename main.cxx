#include "inc/includes.hxx"

static void initAssets()
{
    std::string titleStr = getFileLine(strGameTitle) + getFileLine(strSpace) + getFileLine(strRSLong+releaseStage) + getFileLine(strSpace) + getFileLine(strVersionNum) + getFileLine(strSpace) + getFileLine(strBuildNum);
    window.setTitle(titleStr);
    sf::Image icon;
    bool assetsChk = icon.loadFromFile(getFileLine(resIcon,&resList)) & font.loadFromFile(getFileLine(resMonoTTF,&resList)) & sbHvr.loadFromFile(getFileLine(resHvrSFX,&resList)) & sbCnf.loadFromFile(getFileLine(resDecisionSFX,&resList)) & sbBack.loadFromFile(getFileLine(resBackSFX,&resList)) & bitmapFont.loadFromFile(getFileLine(resBMPFont,&resList)) & cursorTexture.loadFromFile(getFileLine(resCursor,&resList));
    if (!assetsChk)
    {
        printerr(missingAssetsErr,getFileLine(strInitAssetFunc),true);
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
    templateText.setString(getFileLine(strDefault));
    templateText.setOutlineThickness((3.5 / 3) * scaleFactor);
}

static void initSRAM()
{
    for (types::u16 i = 0; i < sizeof(saveRAM); i++)
    {
        saveRAM[i] = 0;
    }
    saveFile.open(getFileLine(resSaveFile,&resList),std::ios::in);
    if (!saveFile)
    {
        for (types::u16 i = 0; i < maxSlots; i++)
        {
            saveSlots[i].writeToSRAM(addrSaves+(i*10));
        }
        saveRAM[addrScaling] = scaleFactor;
        saveRAM[addrVerInf] = saveVersion;
        saveFile.close();
        saveFile.open(getFileLine(resSaveFile,&resList),std::ios::out);
        saveFile.write(saveRAM,sizeof(saveRAM));
        saveFile.close();
        saveFile.open(getFileLine(resSaveFile,&resList),std::ios::in);
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
        printerr(invalidChecksum,getFileLine(strChkChksum),true);
    }
}

static void writeJoyStrings(types::u16 startLine)
{
    for (types::u8 i = 0; i < 13; i++)
    {
        btnPrompts[i] = getFileLine(startLine+i);
    }
    for (types::u8 i = 0; i < 4; i++)
    {
        btnPrompts[i+13] = getFileLine(strDPADDirs+i);
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
            writeJoyStrings(strBtnPS);
            break;
        }
        case xb1CCVID:
        {
            writeJoyStrings(strBtnXbox);
            break;
        }
        default:
        {
            if (joyConnected)
            {
                printerr(missingAssetsErr,getFileLine(strChkJPVendor));
            }
            else
            {
                writeJoyStrings(strBtnXbox);
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
        printerr(invalidSRAMVersion,getFileLine(strMain));
    }
    title();
    return 0;
}