#include "inc/includes.hxx"

int main(int argc, char** argv)
{
    for (types::u16 i = 0; i < sizeof(saveRAM); i++)
    {
        saveRAM[i] = 0;
    }
    saveFile.open(saveFileLocation,std::ios::in);
    if (!saveFile)
    {
        saveRAM[addrDifficulty] = difficulty;
        saveRAM[addrLevel] = level;
        saveRAM[addrPlayer] = player;
        saveRAM[addrScaling] = scaleFactor;
        saveRAM[addrLives] = lives;
        writeSRAM_u32(addrScore,score);
        saveFile.close();
        saveFile.open(saveFileLocation,std::ios::out);
        saveFile.write(saveRAM,sizeof(saveRAM));
        saveFile.close();
        saveFile.open(saveFileLocation,std::ios::in);
    }
    saveFile.read(saveRAM,sizeof(saveRAM));
    saveFile.close();
    types::u32 ogChkSum = readSRAM_u32(addrChkSum); 
    char sramNoChkSum[32764];
    for (types::u16 i = 0; i < sizeof(sramNoChkSum); i++)
    {
        sramNoChkSum[i] = saveRAM[i+4];
    }
    types::u32 checksum = crc32(checksum,(types::u8*)sramNoChkSum,sizeof(sramNoChkSum));
    if (ogChkSum == 0)
    {
        ogChkSum = generateSaveChkSum();
    }
    if (ogChkSum != checksum)
    {
        printerr(invalidChecksum,"main()",true);
    }
    scaleFactor = saveRAM[addrScaling];
    player = saveRAM[addrPlayer];
    difficulty = saveRAM[addrDifficulty];
    std::string vmajStr = std::to_string(versionMajor);
    std::string vminStr = std::to_string(versionMinor);
    std::string vrevStr = std::to_string(versionRevision);
    std::string vcStr = vmajStr + "." + vminStr + "." + vrevStr;
    std::string titleStr = programTitle + " " + releaseStageStringsL[releaseStage] + " " + vcStr + " " + buildTitle;
    window.setTitle(titleStr);
    sf::Image icon;
    if (!icon.loadFromFile(favicon))
    {
        printerr(missingAssetsErr,"main()",true);
    }
    window.setIcon(icon.getSize().x,icon.getSize().y,icon.getPixelsPtr());
    font.loadFromFile(blazeTTF);
    font.setSmooth(false);
    window.setFramerateLimit(60);
    fadeRect.setSize(sf::Vector2f(window.getSize()));
    sbHvr.loadFromFile(hoverSFX);
    sndHvr.setBuffer(sbHvr);
    sbCnf.loadFromFile(confSFX);
    sndCnf.setBuffer(sbCnf);
    sbBack.loadFromFile(backSFX);
    sndBack.setBuffer(sbBack);
    templateText.setFont(font);
    templateText.setCharacterSize(fontSize * scaleFactor);
    templateText.setString("Default string.");
    templateText.setOutlineThickness((3.5 / 3) * scaleFactor);
    updScreenSize();
    title();
    return 0;
}