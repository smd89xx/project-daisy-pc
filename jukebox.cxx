#include "inc/includes.hxx"

const types::u8 sndAmnt = 12;
const float musX = 0;
const float musY = 0;
const structs::SndMData musHdr[] = 
{
    {getFileLine(resTestMus,&resList),getFileLine(strTestMus),getFileLine(strAlbumGame),getFileLine(strArtistMe),true,false},
    {getFileLine(resLvlClrMus,&resList),getFileLine(strLvlClrMus),getFileLine(strS3Album),getFileLine(strS3Artist),false,false},
    {getFileLine(resTitleMus,&resList),getFileLine(strChrSelectMus),getFileLine(strSAdvance1Album),getFileLine(strSAdvance1Artists),true,false},
    {getFileLine(resTestLvlMus,&resList),getFileLine(strAIZ2Mus),getFileLine(strS3Album),getFileLine(strUnknown) + getFileLine(strArtist),true,false},
    {getFileLine(resPrefsMus,&resList),getFileLine(strZoneSelectMus),getFileLine(strSAdvance1Album),getFileLine(strSAdvance1Artists),true,false},
    {getFileLine(resHvrSFX,&resList),getFileLine(strMenu) + getFileLine(strHover),getFileLine(strAlbumGame),getFileLine(strArtistMe),false,true},
    {getFileLine(resDecisionSFX,&resList),getFileLine(strMenu) + getFileLine(strSelect),getFileLine(strAlbumGame),getFileLine(strArtistMe),false,true},
    {getFileLine(resBackSFX,&resList),getFileLine(strMenu) + getFileLine(strBack),getFileLine(strAlbumGame),getFileLine(strArtistMe),false,true},
    {getFileLine(resCrashSFX,&resList),getFileLine(strCrashSFX),getFileLine(strUnknown) + getFileLine(strAlbum),getFileLine(strUnknown) + getFileLine(strArtist),false,true},
    {getFileLine(resJumpSFX,&resList),getFileLine(strJumpSFX),getFileLine(strAlbumGame),getFileLine(strArtistMe),false,true},
    {getFileLine(resWarningBellSFX,&resList),getFileLine(strWarnBellSFX),getFileLine(strAlbumGame),getFileLine(strArtistMe),false,true},
    {getFileLine(resSplashSFX,&resList),getFileLine(strSplashSFX),getFileLine(strAlbumGame),getFileLine(strArtistMe),false,true},
};
sf::SoundBuffer* sb;
sf::Sound* snd;

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
    music.setPitch(1);
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
        music.openFromFile(musHdr[menuIndex].songPath);
        music.setLoop(musHdr[menuIndex].loop);
        music.play();
    }
    else
    {
        sb->loadFromFile(musHdr[menuIndex].songPath);
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

static void drawBtnHints()
{
    bool joyConnected = sf::Joystick::isConnected(0);
    float btnY = musY + 25;
    types::s32 pitch = music.getPitch() * 100;
    std::stringstream speedStr;
    if (joyConnected)
    {
        speedStr << btnPrompts[buttonL1] << getFileLine(strKeySlash) << btnPrompts[buttonR1] << getFileLine(strIncDec) << getFileLine(strSpeedDelt) << pitch << getFileLine(strSDeltCap);
        drawBitmapFont(btnPrompts[buttonSquare] + getFileLine(strTogglePausePrompt),{0,btnY});
        drawBitmapFont(btnPrompts[buttonCross] + getFileLine(strStartSndPrompt),{0,btnY+1});
        drawBitmapFont(btnPrompts[buttonCircle] + getFileLine(strExitPrompt),{0,btnY+2});
        drawBitmapFont(btnPrompts[dpadLeft] + getFileLine(strKeySlash) + btnPrompts[dpadRight] + getFileLine(strHoverPrompt),{0,btnY+3});
        drawBitmapFont(speedStr.str(),{0,btnY+4});
    }
    else
    {
        speedStr << getFileLine(strKeyBCtrl) << getFileLine(strIncDec) << getFileLine(strSpeedDelt) << pitch << getFileLine(strSDeltCap);
        drawBitmapFont(getFileLine(strKeySpace) + getFileLine(strTogglePausePrompt),{0,btnY});
        drawBitmapFont(getFileLine(strKeyEnter) + getFileLine(strStartSndPrompt),{0,btnY+1});
        drawBitmapFont(getFileLine(strKeyEscape) + getFileLine(strExitPrompt),{0,btnY+2});
        drawBitmapFont(getFileLine(strKeyLArrow) + getFileLine(strKeySlash) + getFileLine(strKeyRArrow) + getFileLine(strHoverPrompt),{0,btnY+3});
        drawBitmapFont(speedStr.str(),{0,btnY+4});
    }
}

static void changeSpeed(bool direction)
{
    float speed = music.getPitch();
    if (!direction)
    {
        speed -= 0.05f;
    }
    else
    {
        speed += 0.05f;
    }
    music.setPitch(speed);
    snd->setPitch(speed);
}

void jukebox()
{
    sb = new sf::SoundBuffer;
    snd = new sf::Sound;
    music.openFromFile(musHdr[menuIndex].songPath);
    fadeRect.setFillColor(sf::Color::Black);
    std::stringstream songDurationStr;
    std::stringstream songElapsedStr;
    types::u32 color = RGB4toRGB8(0x0224);
    while(window.isOpen())
    {
        songDurationStr.str(getFileLine(strEmpty));
        songElapsedStr.str(getFileLine(strEmpty));
        songElapsedStr << std::fixed << std::showpoint << std::setprecision(3) << music.getPlayingOffset().asSeconds();
        songDurationStr << std::fixed << std::showpoint << std::setprecision(3) << music.getDuration().asSeconds();
        window.clear(sf::Color(color));
        drawBitmapFont(getFileLine(strTitleHdr) + musHdr[menuIndex].songTitle,{musX,musY});
        drawBitmapFont(getFileLine(strAlbumHdr) + musHdr[menuIndex].songAlbum,{musX,musY+1});
        drawBitmapFont(getFileLine(strArtistHdr) + musHdr[menuIndex].songArtist,{musX,musY+2});
        drawBitmapFont(getFileLine(strLoopHdr) + getFileLine(strNo+musHdr[menuIndex].loop),{musX,musY+4});
        drawBitmapFont(getFileLine(strAudioTypeHdr) + getFileLine(strMusic+musHdr[menuIndex].isSFX),{musX,musY+5});
        drawBitmapFont(getFileLine(strElapsed) + getFileLine(strTimeSuffix) + songElapsedStr.str() + getFileLine(strSeconds),{musX,musY+7});
        drawBitmapFont(getFileLine(strTotal) + getFileLine(strTimeSuffix) + songDurationStr.str() + getFileLine(strSeconds),{musX,musY+8});
        drawBtnHints();
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
                if (e.key.scancode == sf::Keyboard::Scan::LControl)
                {
                    changeSpeed(false);
                }
                else if (e.key.scancode == sf::Keyboard::Scan::RControl)
                {
                    changeSpeed(true);
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
                    case buttonL1:
                    {
                        changeSpeed(false);
                        break;
                    }
                    case buttonR1:
                    {
                        changeSpeed(true);
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