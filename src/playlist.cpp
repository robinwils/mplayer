#include "playlist.hh"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <limits>

Playlist::Playlist()
    : cur_song_(0)
{
}

int Playlist::add_song(std::shared_ptr<Song> const& song)
{
    std::vector<std::shared_ptr<Song>>::iterator it;

    if (std::find_if(songs_.begin(), songs_.end(),
                    [&song](std::shared_ptr<Song> const& s) {
                        return song->filename() == s->filename();
                    }) != songs_.end())
        return 1;

    songs_.push_back(song);
    idx_.push_back(idx_.size());

    return 0;
}

int Playlist::remove_song(std::string const& sname)
{
    std::vector<std::shared_ptr<Song>>::iterator it;

    it = std::find_if(std::begin(songs_), std::end(songs_),
                    [&sname](std::shared_ptr<Song> const& song) {
                        return song->filename() == sname;
                    });

    if (it == songs_.end())
        return 1;
    idx_.erase(std::remove(idx_.begin(), idx_.end(), idx_.size() - 1),
               idx_.end());
    songs_.erase(it);

    return 0;
}

std::shared_ptr<Song> Playlist::cur_song()
{
    return songs_[idx_[cur_song_]];
}

int Playlist::next(bool forward, bool repeat)
{
    std::size_t next = forward ? cur_song_ + 1 : cur_song_ - 1;

    std::cout << "next: " << next << std::endl;

    if (!repeat && next >= songs_.size()) {
        return -1;
    }
    else
        cur_song_ = next % songs_.size();

    if (next == std::numeric_limits<std::size_t>::max())
        cur_song_ = songs_.size() - 1;

    return 0;
}

void Playlist::randomize()
{
    std::random_shuffle(idx_.begin(), idx_.end());
}

void Playlist::reset()
{
    cur_song_ = 0;
}

std::ostream& Playlist::dump(std::ostream& o) const
{
    int i = 1;

    for (std::shared_ptr<Song> const& song : songs_) {
        o << ZERO_PAD(i) << i << ". ";
        song->short_dump(o);
        o << std::endl;
        ++i;
    }

    return o;
}
