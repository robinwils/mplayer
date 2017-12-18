#include "playback.hh"

#include "player.hh"

#include <thread>
#include <iostream>

Playback::Playback()
{
}

void Playback::operator()(Player *const player)
{
    std::chrono::seconds delay(1);

    for (; player->get_state() != Player::STOPPED; ) {
        auto t_start = std::chrono::high_resolution_clock::now();
        {
            std::lock_guard<std::mutex> guard(player->mutex);
            duration_ = player->get_playing_song()->get_duration();
        }

        // Loop that counts seconds to simulate track playback
        for (; player->get_state() != Player::STOPPED; ) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            auto t_now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t_now - t_start);
            if (delay <= elapsed) {
                t_start = t_now;
                std::lock_guard<std::mutex> guard(player->mutex);
                if (player->get_state() == Player::PLAYING) {
                    player->inc_cur_secs();
                }
            }
            std::lock_guard<std::mutex> guard(player->mutex);
            if (player->get_cur_secs() >= duration_)
                break;
        }
        std::lock_guard<std::mutex> guard(player->mutex);
        if (player->next(true)) {
            player->stop();
            break;
        }
    }
}
