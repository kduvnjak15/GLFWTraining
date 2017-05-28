
#pragma once
#include <iostream>

class Audio
{
public:
    virtual ~Audio() {}
    virtual void playSound(int soundID) = 0;
    virtual void stopSound(int soundID) = 0;
    virtual void stopAllSounds() = 0;
};

class ConsoleAudio : public Audio
{
public:
    virtual void playSound(int soundID)
    {
        std::cout << "playSound "<< std::endl;
    }

    virtual void stopSound(int soundID)
    {
        std::cout << "stopSound" << std::endl;
    }

    virtual void stopAllSounds()
    {
        std::cout << "stopped all Sounds" << std::endl;
    }
};

class Locator
{
public:
    static Audio* getAudio()
    {
        return service_;
    }

    static void provide(Audio* service)
    {
        service_ = service;
    }

private:
    static Audio* service_;
};


