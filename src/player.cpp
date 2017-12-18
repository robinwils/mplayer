# include "player.hh"

#include <fts.h>
#include <limits.h>
#include <stdlib.h>
#include <fnmatch.h>
#include <iostream>
#include <sstream>


Player::Player()
    : repeat_(false),
    random_(false)
{
    stop();
}

// recursively add all *.music files in given directories
int Player::add_dirs_to_library(char* const* dirs) {
    char rpath[PATH_MAX + 1] = { 0 };
    FTS *tree = fts_open(dirs, FTS_NOCHDIR, 0);

    if (!tree) {
        perror("fts_open");
        return 1;
    }

    FTSENT *node;
    while ((node = fts_read(tree))) {
        if (node->fts_level > 0 && node->fts_name[0] == '.')
            fts_set(tree, node, FTS_SKIP);
        else if ((node->fts_info & FTS_F) &&
                  !fnmatch("*.music", node->fts_name, FNM_PATHNAME)) {
            std::shared_ptr<Song> newsong = std::make_shared<Song>();

            realpath(node->fts_path, rpath);
            newsong->from_file(rpath);
            newsong->set_filename(node->fts_name);
            library_.add_song(newsong);
        }
    }

    if (errno) {
        perror("fts_read");
        return 1;
    }

    if (fts_close(tree)) {
        perror("fts_close");
        return 1;
    }

    return 0;
}

int Player::populate_library(int ac, char** av)
{
    char dot_str[] = ".";
    char *dot[] = { dot_str, NULL };

    return add_dirs_to_library(ac > 1 ? av + 1 : dot);
}

int Player::new_playlist(std::string const& pname)
{
    if (playlists_.find(pname) == playlists_.end()) {
        playlists_[pname] = Playlist();
        return 0;
    }

    return 1;
}

int Player::add_to_playlist(std::string const& pname, std::string const& sname)
{
    return playlists_[pname].add_song(library_.get_song(sname));
}

int Player::remove_from_playlist(std::string const& pname, std::string const& sname)
{
    return playlists_[pname].remove_song(sname);
}

bool Player::song_is_in_library(std::string const& sname) const
{
    return library_.song_exists(sname);
}

int Player::add_to_library(std::shared_ptr<Song> const& song)
{
    return library_.add_song(song);
}

bool Player::playlist_exists(std::string const& pname)
{
    return playlists_.count(pname) > 0;
}

Playlist& Player::get_playlist(std::string const& pname)
{
    return playlists_[pname];
}

Library& Player::get_library()
{
    return library_;
}

Player::State Player::get_state() const
{
    return cur_state_;
}

std::shared_ptr<Song> Player::get_playing_song() const
{
    return playing_song_;
}

void Player::inc_cur_secs()
{
    cur_song_secs_++;
}

std::chrono::seconds Player::get_cur_secs()
{
    return cur_song_secs_;
}

int Player::play_song(std::string const& sname)
{
    if (!song_is_in_library(sname) && !playlist_exists(sname))
        return 1;

    cur_state_ = PLAYING;

    if (!fnmatch("*.music", sname.c_str(), FNM_PATHNAME))
        playing_song_ = library_.get_song(sname);
    else {
        // other playlist currently playing
        if (cur_playlist_.size() > 0)
            return 0;
        cur_playlist_ = sname;
        if (random_)
            playlists_[sname].randomize();
        playing_song_ = playlists_[sname].cur_song();
    }

    Playback pb;

    // Thread should already be over by now
    if (th_.joinable())
        th_.join();

    th_ = std::thread(pb, this);

    return 0;
}

std::string Player::status_line()
{
    std::stringstream ss;

    switch (cur_state_) {
        case STOPPED:
            return "■  ----------";
        case PLAYING:
            ss << "▶  ";
            break;
        case PAUSED:
            ss << "|| ";
            break;
    }

    std::chrono::duration<int> s = playing_song_->get_duration();

    int totalmins = s.count() / 60;
    int totalsecs = s.count() % 60;
    std::lock_guard<std::mutex> guard(mutex);
    int curmins = cur_song_secs_.count() / 60;
    int cursecs = int(cur_song_secs_.count()) % 60;

    playing_song_->short_dump(ss);

    ss << " ";

    ss << ZERO_PAD(curmins) << curmins << ":"
       << ZERO_PAD(cursecs) << cursecs << " / "
       << ZERO_PAD(totalmins) << totalmins << ":"
       << ZERO_PAD(totalsecs) << totalsecs;

    return ss.str();
}

int Player::show_song(std::string const& sname)
{
    library_.get_song(sname)->dump(std::cout);

    return 0;
}

int Player::toggle()
{
    if (cur_state_ == STOPPED)
        return 1;

    cur_state_ = cur_state_ == PLAYING ? PAUSED : PLAYING;

    return 0;
}

void Player::stop()
{
    cur_state_ = STOPPED;

    cur_song_secs_ = std::chrono::seconds(0);
    playing_song_ = nullptr;
    if (cur_playlist_.size()) {
        playlists_[cur_playlist_].reset();
        cur_playlist_ = "";
    }
}

void Player::toggle_repeat()
{
    repeat_ = !repeat_;
}

void Player::toggle_random()
{
    random_ = !random_;
}

int Player::next(bool forward)
{
    if (cur_state_ == STOPPED)
        return 1;

    if (cur_playlist_.size() == 0)
        return 1;

    if (playlists_[cur_playlist_].next(forward, repeat_))
        return 1;

    playing_song_ = playlists_[cur_playlist_].cur_song();

    cur_song_secs_ = std::chrono::seconds(0);

    return 0;
}
