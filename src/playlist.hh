#pragma once

#include "song.hh"

#include <vector>
#include <memory>
#include <string>

class Playlist
{
    public:
        Playlist();
        int add_song(std::shared_ptr<Song> const& song);
        int remove_song(std::string const& sname);

        std::shared_ptr<Song> cur_song();
        int next(bool forward, bool repeat);
        void next(bool forward);
        void randomize();
        void reset();

        std::ostream& dump(std::ostream& o) const;

    private:
        std::vector<std::shared_ptr<Song>> songs_;
        std::vector<std::size_t> idx_;
        std::size_t cur_song_;
};
