#pragma once

#include <chrono>

class Player;

class Playback
{
    public:
        Playback();

        void operator()(Player *const player);

    private:
        std::chrono::seconds duration_;
};
