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
        fprintf(stderr,"Assets not found.");
        exit(1);
    }
    window.setIcon(16,16,icon.getPixelsPtr());
    font.loadFromFile(blazeTTF);
    font.setSmooth(false);
    window.setFramerateLimit(60);
    menuIndex = new types::u8(0);
    fadeRect.setSize(sf::Vector2f(1280,720));
    fadeRect.setFillColor(sf::Color::Black);
    sbHvr.loadFromFile(hoverSFX);
    sndHvr.setBuffer(sbHvr);
    sbCnf.loadFromFile(confSFX);
    sndCnf.setBuffer(sbCnf);
    templateText.setFont(font);
    templateText.setCharacterSize(fontSize);
    templateText.setString("Default string.");
    title();
    return 0;
}