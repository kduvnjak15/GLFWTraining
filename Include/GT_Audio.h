#pragma once

#include "map"
#include "SFML/Audio.hpp"

enum SOUND
{
    BELL,
    BOOM
};


class GT_Audio
{
public:
    GT_Audio();

    void playSound(SOUND soundEnum);


private:
    std::map<SOUND, sf::SoundBuffer*> soundBuffMap_;

    sf::Sound sound_;
    sf::SoundBuffer* tempBuff_;

};
