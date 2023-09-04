#include "inc/includes.hxx"

sf::RenderWindow window(sf::VideoMode(1280, 720, 9),"");
sf::Font font;
const u8 fontSize = 24;
const u8 titleX = 6;
const u8 titleY = 22;

const Option titleMenu[] = 
{
    {titleX,titleY,"Start Game"},
    {titleX+10,titleY,"Continue Game"},
    {titleX+22,titleY,"Statistics"},
    {titleX+31,titleY,"Preferences"},
};

static void drawMenu(const Option* option, u16 length)
{
    for (u16 i = 0; i < length; i++)
    {
        Option o = option[i];
        sf::Text optionLabel;
        optionLabel.setFont(font);
        optionLabel.setString(o.label);
        optionLabel.setPosition(o.x*fontSize,o.y*fontSize);
        optionLabel.setCharacterSize(fontSize);
        window.draw(optionLabel);
    }
}

static void title()
{
    std::string vmajStr = std::to_string(versionMajor);
    std::string vminStr = std::to_string(versionMinor);
    std::string vrevStr = std::to_string(versionRevision);
    std::string vcStr = vmajStr + "." + vminStr + "." + vrevStr;
    std::string versionCombined = "Version " + releaseStageStringsS[releaseStage] + vcStr;
    sf::Music music;
    music.openFromFile(testTrack);
    music.setLoop(true);
    music.play();
    sf::Text copyInfo,versionText;
    copyInfo.setString(L"©TheWindowsPro98 2023");
    copyInfo.setCharacterSize(fontSize);
    copyInfo.setFont(font);
    copyInfo.setPosition(0,690);
    versionText.setString(versionCombined);
    versionText.setFont(font);
    versionText.setCharacterSize(fontSize);
    versionText.setPosition(160,144);
    sf::Texture titleTexture;
    sf::Image titlePixels;
    titlePixels.loadFromFile(titleImg);
    titleTexture.loadFromFile(titleImg,sf::IntRect(0,0,960,144));
    titleTexture.update(titlePixels);
    sf::Sprite titleSprite;
    titleSprite.setTexture(titleTexture);
    titleSprite.setPosition(160,0);
    while (window.isOpen())
    {
        sf::Event event;
        window.clear(sf::Color::Black);
        window.draw(copyInfo);
        window.draw(versionText);
        window.draw(titleSprite);
        drawMenu(titleMenu,4);
        window.display();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
    }
}

int main(int argc, char** argv)
{
    std::string vmajStr = std::to_string(versionMajor);
    std::string vminStr = std::to_string(versionMinor);
    std::string vrevStr = std::to_string(versionRevision);
    std::string vcStr = vmajStr + "." + vminStr + "." + vrevStr;
    std::string titleStr = programTitle + " " + releaseStageStringsL[releaseStage] + " " + vcStr + " " + buildTitle;
    window.setTitle(titleStr);
    sf::Image icon;
    icon.loadFromFile(favicon);
    window.setIcon(16,16,icon.getPixelsPtr());
    font.loadFromFile(blazeTTF);
    window.setFramerateLimit(60);
    title();
    while (window.isOpen())
    {
        sf::Event event;
        window.clear(sf::Color::Black);
        window.display();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
    }
    return 0;
}