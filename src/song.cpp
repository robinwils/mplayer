#include "song.hh"

#include <fstream>
#include <iostream>

Song::Song()
{
}

std::string Song::filename() const
{
    return filename_;
}

void Song::set_filename(std::string const& name)
{
    filename_ = name;
}

int Song::from_file(std::string const& path)
{
    std::ifstream infile(path);
    std::string line;

    if (!infile.is_open())
        return -1;

    while (std::getline(infile, line)) {
        std::size_t found = line.find("=");
        if (found == std::string::npos)
            continue;
        std::pair<std::string, std::string> mdata;
        mdata.first = line.substr(0, found);
        mdata.second = line.substr(found + 1, line.size());
        if (mdata.first == "title")
            title_ = mdata.second;
        else if (mdata.first == "artist")
            artist_ = mdata.second;
        else if (mdata.first == "track")
            n_ = std::stoi(mdata.second);
        else if (mdata.first == "album")
            album_ = mdata.second;
        else if (mdata.first == "codec")
            codec_ = mdata.second;
        else if (mdata.first == "duration")
            duration_ = mdata.second;
        else if (mdata.first == "bitrate")
            bitrate_ = mdata.second;
        else if (mdata.first == "date")
            year_ = std::stoi(mdata.second);
    }

    return 0;
}

std::chrono::seconds Song::get_duration()
{
    if (duration_.size() != 11)
        return std::chrono::duration<int>(0);

    int hours = std::stoi(duration_.substr(0, 1));
    int minutes = std::stoi(duration_.substr(3, 4));
    int seconds = std::stoi(duration_.substr(6, 7));

    std::chrono::seconds res = std::chrono::hours(hours)
                            + std::chrono::minutes(minutes)
                            + std::chrono::seconds(seconds);

    return res;
}

std::ostream& Song::dump(std::ostream& o) const
{
    o << "title: " << title_ << std::endl
      << "artist: " << artist_ << std::endl
      << "track: " << n_ << std::endl
      << "album: " << album_ << std::endl
      << "codec: " << codec_ << std::endl
      << "duration: " << duration_ << std::endl
      << "bitrate: " << bitrate_ << std::endl
      << "date: " << year_ << std::endl
      << "filename: " << filename_ << std::endl;

    return o;
}

std::ostream& Song::short_dump(std::ostream& o) const
{
    o << artist_ << " " << title_;

    return o;
}
