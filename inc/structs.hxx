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
            std::string songPath;
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
    class AnimMData
    {
        public:
            types::u16 animIndex;
            types::u8 frameAmount;
            types::u8 frameTime; // In NTSC frames (60 frames = 1 second)
        ~AnimMData();
    };
    class SaveMData
    {
        public:
            bool player = true;
            types::u8 difficulty = 1;
            types::u8 level = 0;
            types::u8 lives = 5;
            types::u32 score = 0;
            types::u16 health = 760;
        void writeToSRAM(types::u16 index);
        SaveMData readFromSRAM(types::u16 index);
        ~SaveMData();

        private:
            enum sramLocations
            {
                addrPlayer,
                addrDifficulty,
                addrLevel,
                addrLives,
                addrScore,
                addrHealth = 8,
            };
    };
    class PlayerMData
    {
        public:
            sf::Texture texture;
            sf::Sprite sprite;
            sf::Vector2f position;
            sf::Vector2f velocity;
            float maxSpeed;
            float jumpStrength;
            Directions directions;
    };
}