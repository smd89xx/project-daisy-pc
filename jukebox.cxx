#include "inc/includes.hxx"

const types::u8 sndAmnt = 10;
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
    {&lvlClearTrack,"Act Complete","Sonic the Hedgehog 3","Jun Senoue",false,false},
    {&titleTrack,"Character Select","Sonic Advance","Tatsuyuki Maeda, Yutaka Minobe, Teruhiko Nakagawa",true,false},
    {&lfTrack,"Angel Island Zone (Act 2)","Sonic the Hedgehog 3","Unknown Artist",true,false},
    {&lsTrack,"Zone Select","Sonic Advance","Tatsuyuki Maeda, Yutaka Minobe, Teruhiko Nakagawa",true,false},
    {&hoverSFX,"Menu Hover","Game","TheWindowsPro98",false,true},
    {&confSFX,"Menu Select","Game","TheWindowsPro98",false,true},
    {&backSFX,"Menu Back","Game","TheWindowsPro98",false,true},
    {&crashSFX,"Macintosh IIcx Crash Chime","Unknown Album","Unknown Artist",false,true},
    {&jumpSFX,"Jump","Game","TheWindowsPro98",false,true},
};
sf::SoundBuffer* sb;
sf::Sound* snd;
const std::string blankString = "";
const std::string boolStrings[] = {"No","Yes"};
const std::string musType[] = {"Music","SFX"};
const std::string timeMetric = " seconds";

static void jukeboxBack()
{
    fadeRect.setFillColor(sf::Color::White);
    float volume = music.getVolume();
    while (window.isOpen())
    {
        if (volume == 0)
        {
            music.stop();
            music.setVolume(100);
            break;
        }
        volume = music.getVolume();
        fadeMusic(true,volFadeSpeed,volMin);
        screenFade(volFadeSpeed,false,fadeLight);
        window.display();
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                updSRAM();
                window.close();
                return;
            }
        }
    }
    delete snd;
    delete sb;
    prefsScreen();
}

static void cursorMove(bool direction)
{
    if (!direction)
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
    else
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
}

static void initMusic()
{
    if (!musHdr[menuIndex].isSFX)
    {
        music.openFromFile(*musHdr[menuIndex].songPath);
        music.setLoop(musHdr[menuIndex].loop);
        music.play();
    }
    else
    {
        sb->loadFromFile(*musHdr[menuIndex].songPath);
        snd->setBuffer(*sb);
        snd->setLoop(musHdr[menuIndex].loop);
        snd->play();
    }
}

static void toggleMusicPause()
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

void jukebox()
{
    sb = new sf::SoundBuffer;
    snd = new sf::Sound;
    music.openFromFile(*musHdr[menuIndex].songPath);
    fadeRect.setFillColor(sf::Color::Black);
    std::stringstream songDurationStr;
    std::stringstream songElapsedStr;
    const types::u8 btnY = musY + 26;
    types::u32 color = RGB4toRGB8(0x0224);
    while(window.isOpen())
    {
        songDurationStr.str(blankString);
        songElapsedStr.str(blankString);
        songElapsedStr << std::fixed << std::showpoint << std::setprecision(3) << music.getPlayingOffset().asSeconds();
        songDurationStr << std::fixed << std::showpoint << std::setprecision(3) << music.getDuration().asSeconds();
        window.clear(sf::Color(color));
        drawBitmapFont("Title: " + musHdr[menuIndex].songTitle,{musX,musY});
        drawBitmapFont("Album: " + musHdr[menuIndex].songAlbum,{musX,musY+1});
        drawBitmapFont("Artist(s): " + musHdr[menuIndex].songArtist,{musX,musY+2});
        drawBitmapFont("Will Audio Loop: " + boolStrings[musHdr[menuIndex].loop],{musX,musY+4});
        drawBitmapFont("Audio Type: " + musType[musHdr[menuIndex].isSFX],{musX,musY+5});
        drawBitmapFont("Elapsed Time: " + songElapsedStr.str() + timeMetric,{musX,musY+7});
        drawBitmapFont("Total Time: " + songDurationStr.str() + timeMetric,{musX,musY+8});
        drawBitmapFont(btnPrompts[buttonSquare] + ": Pause music",{0,static_cast<float>(btnY)});
        drawBitmapFont(btnPrompts[buttonCross] + ": (Re)start music/SFX",{0,static_cast<float>(btnY+1)});
        drawBitmapFont(btnPrompts[buttonCircle] + ": Exit",{0,static_cast<float>(btnY+2)});
        drawBitmapFont(btnPrompts[dpadLeft] + "/" + btnPrompts[dpadRight] + ": Change selection",{0,static_cast<float>(btnY+3)});
        screenFade(volFadeSpeed,true,fadeDark);
        window.display();
        while(window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::Closed:
            {
                updSRAM();
                window.close();
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (e.key.scancode == sf::Keyboard::Scan::Left)
                {
                    cursorMove(false);
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Right)
                {
                    cursorMove(true);
                }
                if (e.key.scancode == sf::Keyboard::Scan::Enter)
                {
                    initMusic();
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Space)
                {
                    toggleMusicPause();
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Escape)
                {
                    sndBack.play();
                    jukeboxBack();
                }
                break;
            }
            case sf::Event::JoystickButtonPressed:
            {
                if (!window.hasFocus())
                {
                    break;
                }
                switch (e.joystickButton.button)
                {
                    case buttonCross:
                    {
                        initMusic();
                        break;
                    }
                    case buttonSquare:
                    {
                        toggleMusicPause();
                        break;
                    }
                    case buttonCircle:
                    {
                        sndBack.play();
                        jukeboxBack();
                        break;
                    }
                    break;
                }
                break;
            }
            case sf::Event::JoystickMoved:
            {
                if (!window.hasFocus())
                {
                    break;
                }
                switch (e.joystickMove.axis)
                {
                    case axisDPADX:
                    {
                        if (e.joystickMove.position == -100)
                        {
                            cursorMove(false);
                        }
                        else if (e.joystickMove.position == 100)
                        {
                            cursorMove(true);
                        }
                        break;
                    }
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