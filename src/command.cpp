#include "command.hh"

#include <iostream>


/** every command has an args parameter which is a string vector and the player
  * object */
COMMAND(new_playlist)
{
    if (args.size() == 0) {
        std::cerr << "usage: " << NEW_PLAYLIST_USAGE << std::endl;
        return;
    }

    for (std::string const& pname : args) {
        if (!player.new_playlist(pname))
            std::cout << "Playlist " << pname << " created." << std::endl;
        else
            std::cout << "Playlist " << pname << " already exists." << std::endl;
    }
}

COMMAND(add_song)
{
    if (args.size() < 2) {
        std::cerr << "usage: " << ADD_SONG_USAGE << std::endl;
        return;
    }

    std::string pname = args[0];
    if (!player.playlist_exists(pname)) {
        std::cerr << "Playlist " << pname << " does not exist." << std::endl;
        return;
    }
    for (std::size_t i = 1; i < args.size(); ++i) {
        if (!player.song_is_in_library(args[i])) {
            std::cout << "Song `" << args[i] << "' does not exist." << std::endl;
            continue;
        }
        if (player.add_to_playlist(pname, args[i])) {
            std::cout << "Song " << args[i] << " is already in playlist " << pname << std::endl;
            continue;
        }
        std::cout << "Added " << args[i] << " to playlist " << pname << std::endl;
    }
}

COMMAND(remove_song)
{
    if (args.size() < 2) {
        std::cerr << "usage: " << REMOVE_SONG_USAGE << std::endl;
        return;
    }

    std::string pname = args[0];
    if (!player.playlist_exists(pname)) {
        std::cerr << "Playlist " << pname << " does not exist." << std::endl;
        return;
    }
    for (std::size_t i = 1; i < args.size(); ++i) {
        if (player.remove_from_playlist(pname, args[i])) {
            std::cout << "Song " << args[i] << " is not in playlist "
                      << pname << std::endl;
            continue;
        }
        std::cout << "Removed " << args[i] << " from playlist " << pname << std::endl;
    }
}

COMMAND(show_playlist)
{
    if (args.size() == 0) {
        std::cerr << "usage: " << SHOW_PLAYLIST_USAGE << std::endl;
        return;
    }

    if (!player.playlist_exists(args[0])) {
        std::cout << "Playlist " << args[0] << " does not exist" << std::endl;
        return;
    }

    player.get_playlist(args[0]).dump(std::cout);
}

COMMAND(show_library)
{
    if (args.size() > 0 && args[0] == "-v")
        player.get_library().dump(std::cout);
    else
        player.get_library().short_dump(std::cout);
}

COMMAND(show_song)
{
    if (args.size() == 0)
        std::cout << "Arguments ignored." << std::endl;

    for (std::size_t i = 0; i < args.size(); ++i)
        if (!player.song_is_in_library(args[i]))
            std::cout << "Song `" << args[i] << "' is not in the library." 
                      << std::endl;
        else {
            player.show_song(args[i]);
            std::cout << std::endl;
        }
}

COMMAND(play)
{
    if (args.size() == 0) {
        if (player.toggle())
            std::cerr << "Player is not playing." << std::endl;
    }
    else
        if (player.play_song(args[0]))
            std::cerr << "`" << args[0] << "' is not in library" << std::endl;
}

COMMAND(pause)
{
    if (args.size() > 0)
        std::cout << "Arguments ignored." << std::endl;

    if (player.toggle())
        std::cerr << "Player is not playing." << std::endl;
}

COMMAND(stop)
{
    if (args.size() > 0)
        std::cout << "Arguments ignored." << std::endl;

    player.stop();
}

COMMAND(next)
{
    if (args.size() > 0)
        std::cout << "Arguments ignored." << std::endl;

    if (player.next(true) == 1)
        std::cerr << "No playlist playing." << std::endl;
}

COMMAND(prev)
{
    if (args.size() > 0)
        std::cout << "Arguments ignored." << std::endl;

    if (player.next(false))
        std::cerr << "No playlist playing." << std::endl;
}

COMMAND(repeat)
{
    if (args.size() > 0)
        std::cout << "Arguments ignored." << std::endl;

    player.toggle_repeat();
}

COMMAND(randomize)
{
    if (args.size() > 0)
        std::cout << "Arguments ignored." << std::endl;

    player.toggle_random();
}

COMMAND(help)
{
    std::cerr << "commands:" << std::endl
              << NEW_PLAYLIST_USAGE << std::endl
              << ADD_SONG_USAGE << std::endl
              << REMOVE_SONG_USAGE << std::endl
              << SHOW_PLAYLIST_USAGE << std::endl
              << "show_library [-v]    shows all song in library." << std::endl
              << "show_song            shows song information." << std::endl
              << "play [SNAME | PNAME] plays a song or playlist. No parameters"
              << " resumes playback." << std::endl
              << "pause        pauses playback." << std::endl
              << "stop         stops playback." << std::endl
              << "next/prev    play next/prev track in current playlist." << std::endl
              << "repeat       repeat playlist when it ends." << std::endl
              << "randomize    randomizes playlist playback." << std::endl
              << "help         prints this message." << std::endl;
}
