#include "inc/includes.hxx"

const types::u8 sndAmnt = 8;
const float musX = 0;
const float musY = 0;
const float musTitleXDelta = 5;
const float musAlbumXDelta = 5.5;
const float musArtistXDelta = 8;
const float musLoopXDelta = 11.65;
const float musTypeXDelta = 9.25;
const float musDurationXDelta = 11;
const float musTotalXDelta = 9.35;
const structs::SndMData musHdr[] = 
{
    {&testTrack,"Test Track","Game","TheWindowsPro98",true,false},
    {&lvlClearTrack,"Act Complete","Game","Jun Senoue",false,false},
    {&titleTrack,"Character Select","Sonic Advance","Tatsuyuki Maeda, Yutaka Minobe, Teruhiko Nakagawa",true,false},
    {&lfTrack,"Angel Island Zone (Act 2)","Sonic the Hedgehog 3","Unknown Artist",true,false},
    {&lsTrack,"Zone Select","Sonic Advance","Tatsuyuki Maeda, Yutaka Minobe, Teruhiko Nakagawa",true,false},
    {&hoverSFX,"Menu Hover","Game","TheWindowsPro98",false,true},
    {&confSFX,"Menu Select","Game","TheWindowsPro98",false,true},
    {&backSFX,"Menu Back","Game","TheWindowsPro98",false,true}
};

static void jukeboxBack()
{
    float volume = music.getVolume();
    while (window.isOpen())
    {
        if (volume == 0)
        {
            music.stop();
            music.setVolume(100);
            break;
        }
        sf::Event e;
        volume = music.getVolume();
        fadeMusic(true,volFadeSpeed,volMin);
        screenFade(volFadeSpeed,false,fadeDark);
        window.draw(fadeRect);
        window.display();
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
        }
    }
    prefsScreen();
}

void jukebox()
{
    std::string boolStrings[2] = {"No","Yes"};
    std::string musType[2] = {"Music","SFX"};
    std::string timeMetric = " seconds";
    sf::Music tmpMus;
    fadeRect.setFillColor(sf::Color::Black);
    sf::Text songLabel(templateText);
    songLabel.setPosition(pixelToTile(musX),pixelToTile(musY));
    sf::Text songAlbumLabel(templateText);
    songAlbumLabel.setPosition(pixelToTile(musX),pixelToTile(musY+1));
    sf::Text songArtistLabel(templateText);
    songArtistLabel.setPosition(pixelToTile(musX),pixelToTile(musY+2));
    sf::Text songLoopLabel(templateText);
    songLoopLabel.setPosition(pixelToTile(musX),pixelToTile(musY+4));
    sf::Text songTypeLabel(templateText);
    songTypeLabel.setPosition(pixelToTile(musX),pixelToTile(musY+5));
    sf::Text songTimeElapsed(templateText);
    songTimeElapsed.setPosition(pixelToTile(musX),pixelToTile(musY+7));
    sf::Text songTimeTotal(templateText);
    songTimeTotal.setPosition(pixelToTile(musX),pixelToTile(musY+8));
    sf::SoundBuffer sb;
    sf::Sound snd;
    while(window.isOpen())
    {
        sf::Event e;
        songLabel.setString("Title: " + musHdr[menuIndex].songTitle);
        songAlbumLabel.setString("Album: " + musHdr[menuIndex].songAlbum);
        songArtistLabel.setString("Artist(s): " + musHdr[menuIndex].songArtist);
        songLoopLabel.setString("Will Audio Loop: " + boolStrings[musHdr[menuIndex].loop]);
        songTypeLabel.setString("Audio Type: " + musType[musHdr[menuIndex].isSFX]);
        songTimeElapsed.setString("Elapsed Time: " + std::to_string(music.getPlayingOffset().asSeconds()) + timeMetric);
        songTimeTotal.setString("Total Time: " + std::to_string(music.getDuration().asSeconds()) + timeMetric);
        screenFade(volFadeSpeed,true,fadeLight);
        window.clear();
        window.draw(songLabel);
        window.draw(songAlbumLabel);
        window.draw(songArtistLabel);
        window.draw(songLoopLabel);
        window.draw(songTypeLabel);
        window.draw(songTimeElapsed);
        window.draw(songTimeTotal);
        window.draw(fadeRect);
        window.display();
        while(window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::Closed:
            {
                window.close();
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (!window.hasFocus() || fadeRect.getFillColor().a != 0)
                {
                    break;
                }
                if (e.key.scancode == sf::Keyboard::Scan::Left)
                {
                    sndHvr.play();
                    if (menuIndex == 0)
                    {
                        menuIndex = sndAmnt-1;
                    }
                    else
                    {
                        menuIndex--;
                    }
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Right)
                {
                    sndHvr.play();
                    if (menuIndex == sndAmnt-1)
                    {
                        menuIndex = 0;
                    }
                    else
                    {
                        menuIndex++;
                    }
                }
                if (e.key.scancode == sf::Keyboard::Scan::Enter)
                {
                    if (!musHdr[menuIndex].isSFX)
                    {
                        music.openFromFile(*musHdr[menuIndex].songPath);
                        music.setLoop(musHdr[menuIndex].loop);
                        music.play();
                    }
                    else
                    {
                        sb.loadFromFile(*musHdr[menuIndex].songPath);
                        snd.setBuffer(sb);
                        snd.setLoop(musHdr[menuIndex].loop);
                        snd.play();
                    }
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Space)
                {
                    if (music.getStatus() == sf::Music::Playing)
                    {
                        music.pause();
                    }
                    else
                    {
                        music.play();
                    }
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Escape)
                {
                    sndBack.play();
                    jukeboxBack();
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