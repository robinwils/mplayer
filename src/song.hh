#pragma once

#include <string>
#include <chrono>

#define ZERO_PAD(Val) ((Val) < 10 ? "0" : "")

class Song
{
    public:
        Song();

        int from_file(std::string const& path);
        std::string filename() const;
        void set_filename(std::string const& name);

        std::chrono::seconds get_duration();

        std::ostream& dump(std::ostream& o) const;
        std::ostream& short_dump(std::ostream& o) const;

    private:
        std::string title_;
        std::string artist_;
        std::string album_;
        std::string codec_;
        std::string bitrate_;
        std::string duration_;
        std::string filename_;
        int n_;
        int year_;
};
