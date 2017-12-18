#pragma once

#include "player.hh"

#include <vector>
#include <string>
#include <functional>
#include <map>

typedef std::function<void(std::vector<std::string> const&, Player&)> CmdHandler;
typedef std::map<std::string, CmdHandler> CmdMap;

#define NEW_PLAYLIST_USAGE "new_playlist PNAME [PNAME2...] creates a new" \
                           " playlist if it does not already exist."

#define ADD_SONG_USAGE "add_song PLAYLIST SNAME [SNAME2...] adds songs" \
                       " to a playlist."

#define REMOVE_SONG_USAGE "remove_song PLAYLIST SNAME [SNAME2...] removes songs" \
                          " from a playlist."

#define SHOW_PLAYLIST_USAGE "show_playlist PNAME [PNAME2...] displays the" \
                            " playlist content."

#define COMMAND(Name) void Name(std::vector<std::string> const& args, Player& player)
#define COMMAND_ENTRY(Name) {#Name, Name}

COMMAND(new_playlist);
COMMAND(add_song);
COMMAND(remove_song);
COMMAND(show_playlist);
COMMAND(show_library);
COMMAND(show_song);
COMMAND(play);
COMMAND(pause);
COMMAND(stop);
COMMAND(next);
COMMAND(prev);
COMMAND(repeat);
COMMAND(randomize);
COMMAND(help);
