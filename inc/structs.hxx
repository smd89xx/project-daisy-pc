#pragma once

#include "includes.hxx"

namespace structs
{
    class Option
    {
        public:
            float x;
            float y;
            std::string label;
        ~Option();
    };
    class SndMData
    {
        public:
            const std::string* songPath;
            std::string songTitle;
            std::string songAlbum;
            std::string songArtist;
            bool loop;
            bool isSFX;
        ~SndMData();
    };
    class Directions
    {
        public:
            bool up;
            bool down;
            bool left;
            bool right;
        ~Directions();
    };
}