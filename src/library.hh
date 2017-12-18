#pragma once

#include <map>
#include <memory>
#include <string>

#include "song.hh"

class Library
{
    public:
        Library();

        int add_song(std::shared_ptr<Song> const& song);
        std::shared_ptr<Song>& get_song(std::string const& sname);

        bool song_exists(std::string const& sname) const;

        std::ostream& short_dump(std::ostream& o) const;
        std::ostream& dump(std::ostream& o) const;

    private:
        std::map<std::string, std::shared_ptr<Song>> songs_;
};
