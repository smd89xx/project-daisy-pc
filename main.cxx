#include "inc/includes.hxx"

int main(int argc, char** argv)
{
    std::string vmajStr = std::to_string(versionMajor);
    std::string vminStr = std::to_string(versionMinor);
    std::string vrevStr = std::to_string(versionRevision);
    std::string vcStr = vmajStr + "." + vminStr + "." + vrevStr;
    std::string titleStr = programTitle + " " + releaseStageStringsL[releaseStage] + " " + vcStr + " " + buildTitle;
    window.setTitle(titleStr);
    sf::Image icon;
    if (!icon.loadFromFile(favicon))
    {
        printerr(missingAssetsErr);
    }
    window.setIcon(icon.getSize().x,icon.getSize().y,icon.getPixelsPtr());
    font.loadFromFile(blazeTTF);
    font.setSmooth(false);
    window.setFramerateLimit(60);
    fadeRect.setSize(sf::Vector2f(window.getSize()));
    fadeRect.setFillColor(sf::Color::Black);
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
    title();
    return 0;
}