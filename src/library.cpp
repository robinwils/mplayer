#include "library.hh"
#include <iostream>

Library::Library()
{
}

int Library::add_song(std::shared_ptr<Song> const& song)
{
    if (songs_.find(song->filename()) == songs_.end()) {
        songs_[song->filename()] = song;
        return 0;
    }

    return 1;
}

std::shared_ptr<Song>& Library::get_song(std::string const& sname)
{
    return songs_.at(sname);
}

bool Library::song_exists(std::string const& sname) const
{
    return songs_.count(sname) > 0;
}

std::ostream& Library::short_dump(std::ostream& o) const
{
    for (auto const& p : songs_) {
        p.second->short_dump(o);
        o << std::endl << std::endl;
    }

    return o;
}

std::ostream& Library::dump(std::ostream& o) const
{
    for (auto const& p : songs_) {
        p.second->dump(o);
        o << std::endl;
    }

    return o;
}
