#include "inc/includes.hxx"

sf::RenderWindow window(sf::VideoMode(1280, 720, 9),"");
sf::Font font;
sf::Music music;
const u8 fontSize = 24;
const u8 titleX = 6;
const u8 titleY = 22;
const u16 titleOptAmnt = 4;
u16* menuIndex;
u8* musIndex;
u8* sfxIndex;

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
        optionLabel.setOutlineThickness(3.5);
        window.draw(optionLabel);
        sf::Text selectedLabel;
        selectedLabel.setFont(font);
        selectedLabel.setString(option[*menuIndex].label);
        selectedLabel.setPosition(option[*menuIndex].x*fontSize,option[*menuIndex].y*fontSize);
        selectedLabel.setCharacterSize(fontSize);
        selectedLabel.setOutlineThickness(3.5);
        selectedLabel.setOutlineColor(sf::Color(0x00CC66FF));
        window.draw(selectedLabel);
    }
}


/// @brief Fades music in or out.
/// @param direction False = in, True = out
/// @param speed Smaller = slower
static void fadeMusic(bool direction, float speed)
{
    float volume = music.getVolume();
    if (direction)
    {
        volume -= speed;
        if (volume <= 0)
        {
            return;
        }
    }
    else
    {
        volume += speed;
        if (volume >= 100)
        {
            return;
        }
    }
    music.setVolume(volume);
}

static void title()
{
    std::string vmajStr = std::to_string(versionMajor);
    std::string vminStr = std::to_string(versionMinor);
    std::string vrevStr = std::to_string(versionRevision);
    std::string vcStr = vmajStr + "." + vminStr + "." + vrevStr;
    std::string versionCombined = "Version " + releaseStageStringsS[releaseStage] + vcStr;
    music.openFromFile(titleTrack);
    music.setLoop(true);
    music.play();
    sf::Text copyInfo,versionText;
    copyInfo.setString(L"©TheWindowsPro98 2023");
    copyInfo.setCharacterSize(fontSize);
    copyInfo.setFont(font);
    copyInfo.setPosition(0,696);
    versionText.setString(versionCombined);
    versionText.setFont(font);
    versionText.setCharacterSize(fontSize);
    versionText.setPosition(160,144);
    sf::Texture titleTexture;
    sf::Image titlePixels;
    titlePixels.loadFromFile(titleImg);
    titleTexture.loadFromFile(titleImg,sf::IntRect(0,0,960,144));
    titleTexture.update(titlePixels);
    titleTexture.setSmooth(true);
    sf::Sprite titleSprite;
    titleSprite.setTexture(titleTexture);
    titleSprite.setPosition(160,0);
    float volume = 100;
    sf::SoundBuffer sb;
    sb.loadFromFile(menuSFX);
    sf::Sound snd;
    snd.setBuffer(sb);
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
            switch (event.type)
            {
            case sf::Event::Closed:
            {
                window.close();
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (!window.hasFocus())
                {
                    break;
                }
                if (event.key.scancode == sf::Keyboard::Scan::Left)
                {
                    
                    if (*menuIndex == 0)
                    {
                        break;
                    }
                    else
                    {
                        snd.play();
                        *menuIndex -= 1;
                    }
                }
                else if (event.key.scancode == sf::Keyboard::Scan::Right)
                {
                    if (*menuIndex >= titleOptAmnt-1)
                    {
                        break;
                    }
                    else
                    {
                        snd.play();
                        *menuIndex += 1;
                    }
                }
                if (event.key.scancode == sf::Keyboard::Scan::Enter)
                {
                    snd.play();
                }
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
    menuIndex = new u16(0);
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