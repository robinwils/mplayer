#pragma once

#include <memory>
#include <thread>
#include <mutex>

#include "library.hh"
#include "playlist.hh"
#include "playback.hh"


class Player
{
    public:
        enum State
        {
            STOPPED,
            PLAYING,
            PAUSED
        };

        Player();

        /* Helper methods */
        int add_dirs_to_library(char* const* dirs);
        int populate_library(int ac, char** av);
        int new_playlist(std::string const& pname);
        int add_to_playlist(std::string const& pname, std::string const& sname);
        int remove_from_playlist(std::string const& pname, std::string const& sname);
        int add_to_library(std::shared_ptr<Song> const& song);
        int show_song(std::string const& sname);

        /* Existency */
        bool song_is_in_library(std::string const& sname) const;
        bool playlist_exists(std::string const& pname);

        /* Getters */
        std::chrono::seconds get_cur_secs();
        State get_state() const;
        Playlist& get_playlist(std::string const& pname);
        std::shared_ptr<Song> get_playing_song() const;
        Library& get_library();

        /* Operations */
        int play_song(std::string const& sname);
        int toggle();
        void stop();
        void toggle_repeat();
        void toggle_random();
        int next(bool forward);
        std::string status_line();
        void inc_cur_secs();

        std::mutex mutex;

    private:
        typedef std::map<std::string, Playlist> PlaylistMap;
        Library library_;
        PlaylistMap playlists_;
        State cur_state_;
        std::shared_ptr<Song> playing_song_;
        std::string cur_playlist_;
        bool repeat_;
        bool random_;
        std::chrono::seconds cur_song_secs_;
        std::thread th_;
};
