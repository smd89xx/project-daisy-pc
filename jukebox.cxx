#include "inc/includes.hxx"

const types::u8 sndAmnt = 7;
const float musX = 0;
const float musY = 0;
const float musTitleXDelta = 5;
const float musAlbumXDelta = 5.5;
const float musArtistXDelta = 8;
const float musLoopXDelta = 11.65;
const float musTypeXDelta = 9.25;
const float musDurationXDelta = 12.5;
const structs::SndMData musHdr[] = 
{
    {&testTrack,"Test Track","Game","TheWindowsPro98",true,false},
    {&lvlClearTrack,"Act Complete","Game","TheWindowsPro98",false,false},
    {&titleTrack,"Character Select","Sonic Advance","Tatsuyuki Maeda, Yutaka Minobe, Teruhiko Nakagawa",true,false},
    {&lfTrack,"Angel Island Zone (Act 2)","Sonic the Hedgehog 3","Unknown Artist",true,false},
    {&lsTrack,"Zone Select","Sonic Advance","Tatsuyuki Maeda, Yutaka Minobe, Teruhiko Nakagawa",true,false},
    {&hoverSFX,"Menu Hover","Game","TheWindowsPro98",false,true},
    {&confSFX,"Menu Select","Game","TheWindowsPro98",false,true}
};

static void jukeboxBack()
{
    float volume;
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
        fadeMusic(true,volFadeSpeed);
        screenFade(volFadeSpeed,false);
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
    sf::Music tmpMus;
    fadeRect.setFillColor(sf::Color::Black);
    sf::Text songDrawable(templateText);
    songDrawable.setPosition(pixelToTile(musX+musTitleXDelta),pixelToTile(musY));
    sf::Text songAlbumDrawable(templateText);
    songAlbumDrawable.setPosition(pixelToTile(musX+musAlbumXDelta),pixelToTile(musY+1));
    sf::Text songArtistDrawable(templateText);
    songArtistDrawable.setPosition(pixelToTile(musX+musArtistXDelta),pixelToTile(musY+2));
    sf::Text songLabel(templateText);
    songLabel.setString("Title:");
    songLabel.setPosition(pixelToTile(musX),pixelToTile(musY));
    sf::Text songAlbumLabel(templateText);
    songAlbumLabel.setString("Album:");
    songAlbumLabel.setPosition(pixelToTile(musX),pixelToTile(musY+1));
    sf::Text songArtistLabel(templateText);
    songArtistLabel.setString("Artist(s):");
    songArtistLabel.setPosition(pixelToTile(musX),pixelToTile(musY+2));
    sf::Text songLoopDrawable(templateText);
    songLoopDrawable.setPosition(pixelToTile(musX+musLoopXDelta),pixelToTile(musY+4));
    sf::Text songLoopLabel(templateText);
    songLoopLabel.setString("Will Audio Loop:");
    songLoopLabel.setPosition(pixelToTile(musX),pixelToTile(musY+4));
    sf::Text songTypeDrawable(templateText);
    songTypeDrawable.setPosition(pixelToTile(musX+musTypeXDelta),pixelToTile(musY+5));
    sf::Text songTypeLabel(templateText);
    songTypeLabel.setString("Audio Type:");
    songTypeLabel.setPosition(pixelToTile(musX),pixelToTile(musY+5));
    sf::Text songDurationDrawable(templateText);
    songDurationDrawable.setPosition(pixelToTile(musX+musDurationXDelta),pixelToTile(musY+7));
    sf::Text songDurationLabel(templateText);
    songDurationLabel.setString("Audio Duration:");
    songDurationLabel.setPosition(pixelToTile(musX),pixelToTile(musY+7));
    sf::SoundBuffer sb;
    sf::Sound snd;
    while(window.isOpen())
    {
        sf::Event e;
        songDrawable.setString(musHdr[*menuIndex].songTitle);
        songAlbumDrawable.setString(musHdr[*menuIndex].songAlbum);
        songArtistDrawable.setString(musHdr[*menuIndex].songArtist);
        songLoopDrawable.setString(boolStrings[musHdr[*menuIndex].loop]);
        songTypeDrawable.setString(musType[musHdr[*menuIndex].isSFX]);
        songDurationDrawable.setString(std::to_string(music.getPlayingOffset().asSeconds()) + "/" + std::to_string(music.getDuration().asSeconds()) + " seconds");
        screenFade(volFadeSpeed*3,true);
        window.clear();
        window.draw(songDrawable);
        window.draw(songAlbumDrawable);
        window.draw(songArtistDrawable);
        window.draw(songLabel);
        window.draw(songAlbumLabel);
        window.draw(songArtistLabel);
        window.draw(songLoopDrawable);
        window.draw(songLoopLabel);
        window.draw(songTypeDrawable);
        window.draw(songTypeLabel);
        window.draw(songDurationDrawable);
        window.draw(songDurationLabel);
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
                    if (*menuIndex == 0)
                    {
                        *menuIndex = sndAmnt-1;
                    }
                    else
                    {
                        (*menuIndex)--;
                    }
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Right)
                {
                    sndHvr.play();
                    if (*menuIndex == sndAmnt-1)
                    {
                        *menuIndex = 0;
                    }
                    else
                    {
                        (*menuIndex)++;
                    }
                }
                if (e.key.scancode == sf::Keyboard::Scan::Enter)
                {
                    if (!musHdr[*menuIndex].isSFX)
                    {
                        music.openFromFile(*musHdr[*menuIndex].songPath);
                        music.setLoop(musHdr[*menuIndex].loop);
                        music.play();
                    }
                    else
                    {
                        sb.loadFromFile(*musHdr[*menuIndex].songPath);
                        snd.setBuffer(sb);
                        snd.setLoop(musHdr[*menuIndex].loop);
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
                    sndCnf.play();
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