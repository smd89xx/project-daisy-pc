#include "inc/includes.hxx"

namespace structs
{
    Option::~Option(){}
    SndMData::~SndMData(){}
    Directions::~Directions(){}
    AnimMData::~AnimMData(){}
    /// @brief Writes save struct to the save RAM.
    /// @param index Index of first byte.
    void SaveMData::writeToSRAM(types::u16 index)
    {
        saveRAM[index+addrPlayer] = player;
        saveRAM[index+addrDifficulty] = difficulty;
        saveRAM[index+addrLevel] = level;
        saveRAM[index+addrLives] = lives;
        writeSRAM_u32(index+addrScore,score);
        writeSRAM_u16(index+addrHealth,health);
    }
    /// @brief Reads save struct from the save RAM.
    /// @param index Index of first byte.
    /// @return Save struct retrieved from the save RAM.
    SaveMData SaveMData::readFromSRAM(types::u16 index)
    {
        SaveMData save;
        save.player = saveRAM[index+addrPlayer];
        save.difficulty = saveRAM[index+addrDifficulty];
        save.level = saveRAM[index+addrLevel];
        save.lives = saveRAM[index+addrLives];
        save.score = readSRAM_u32(index+addrScore);
        save.health = readSRAM_u16(index+addrHealth);
        return save;
    }
    SaveMData::~SaveMData(){};
}