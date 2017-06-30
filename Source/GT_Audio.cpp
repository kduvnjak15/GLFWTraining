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

}


void GT_Audio::playSound(SOUND soundEnum)
{
    tempBuff_ = new sf::SoundBuffer();
    if (tempBuff_->loadFromFile("../Content/gunshot.wav"))
    {
        std::cout << "sebon"<< std::endl;
    }

    sound_.setBuffer( *tempBuff_ );
    sound_.setVolume(100);

    sound_.play();
}
