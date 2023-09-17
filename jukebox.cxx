#include "inc/includes.hxx"

const u8 sndAmnt = 7;
const u8 musX = 0;
const u8 musY = 0;
const u8 musTitleXDelta = 5;
const u8 musAlbumXDelta = 5;
const u8 musArtistXDelta = 6;
const structs::SndMData musHdr[] = 
{
    {testTrack,"Test Track","Game","TheWindowsPro98",true,false},
    {lvlClearTrack,"Act Complete","Game","TheWindowsPro98",false,false},
    {titleTrack,"Character Select","Sonic Advance","Tatsuyuki Maeda, Yutaka Minobe, Teruhiko Nakagawa",true,false},
    {lfTrack,"Angel Island Zone (Act 2)","Sonic the Hedgehog 3","Unknown Artist",true,false},
    {lsTrack,"Zone Select","Sonic Advance","Tatsuyuki Maeda, Yutaka Minobe, Teruhiko Nakagawa",true,false},
    {hoverSFX,"Menu Hover","Game","TheWindowsPro98",false,true},
    {confSFX,"Menu Select","Game","TheWindowsPro98",false,true}
};

void jukebox()
{
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
    songArtistLabel.setString("Artist:");
    songArtistLabel.setPosition(pixelToTile(musX),pixelToTile(musY+2));
    sf::SoundBuffer sb;
    sf::Sound snd;
    while(window.isOpen())
    {
        sf::Event e;
        songDrawable.setString(musHdr[*menuIndex].songTitle);
        songAlbumDrawable.setString(musHdr[*menuIndex].songAlbum);
        songArtistDrawable.setString(musHdr[*menuIndex].songArtist);
        screenFade(volFadeSpeed*3,true);
        window.clear();
        window.draw(songDrawable);
        window.draw(songAlbumDrawable);
        window.draw(songArtistDrawable);
        window.draw(songLabel);
        window.draw(songAlbumLabel);
        window.draw(songArtistLabel);
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
                if (!window.hasFocus() or fadeRect.getFillColor().a != 0)
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
                        music.openFromFile(musHdr[*menuIndex].songPath);
                        music.setLoop(musHdr[*menuIndex].loop);
                        music.play();
                    }
                    else
                    {
                        sb.loadFromFile(musHdr[*menuIndex].songPath);
                        snd.setBuffer(sb);
                        snd.setLoop(musHdr[*menuIndex].loop);
                        snd.play();
                    }
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Escape)
                {
                    sndCnf.play();
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