#include "GT_Audio.h"

#include "SFML/Audio.hpp"
#include <iostream>


GT_Audio::GT_Audio()
{
    sf::SoundBuffer* sb = new sf::SoundBuffer();
    sb->loadFromFile("../Content/bell.wav");
    soundBuffMap_.insert(std::pair<SOUND, sf::SoundBuffer*>(BELL, sb));

    sb = new sf::SoundBuffer();
    sb->loadFromFile("../Content/gunshot.wav");
    soundBuffMap_.insert(std::pair<SOUND, sf::SoundBuffer*>(BOOM, sb));

    sb = new sf::SoundBuffer();
    sb->loadFromFile("../Content/missile.wav");
    soundBuffMap_.insert(std::pair<SOUND, sf::SoundBuffer*>(MISSILE, sb));

    sb = new sf::SoundBuffer();
    sb->loadFromFile("../Content/land.flac");
    soundBuffMap_.insert(std::pair<SOUND, sf::SoundBuffer*>(LAND, sb));

    sb = new sf::SoundBuffer();
    sb->loadFromFile("../Content/spray.wav");
    soundBuffMap_.insert(std::pair<SOUND, sf::SoundBuffer*>(SPRAY, sb));

}


void GT_Audio::playSound(SOUND soundEnum)
{
    tempBuff_ = soundBuffMap_[soundEnum];
    sound_.setBuffer( *tempBuff_ );
    sound_.play();
}
