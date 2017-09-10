#pragma once

#include "map"
#include "SFML/Audio.hpp"

enum SOUND
{
    BELL,
    BOOM,
    MISSILE,
    SPRAY,
    LAND,
    ENGINE,
    OCEAN,
    EXPLOSION

};


class GT_Audio
{
public:
    GT_Audio();

    void playSound(SOUND soundEnum);

    sf::SoundBuffer* getSoundBuffMap(SOUND soundEnum) { return soundBuffMap_[soundEnum]; }

private:
    std::map<SOUND, sf::SoundBuffer*> soundBuffMap_;

    sf::Sound sound_;
    sf::SoundBuffer* tempBuff_;

};
