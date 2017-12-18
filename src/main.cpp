#include <string>
#include <iostream>
#include <sstream>
#include <iterator>

#include "command.hh"

int main(int ac, char** av)
{
    Player player;
    CmdMap commands =
    {
        COMMAND_ENTRY(new_playlist),
        COMMAND_ENTRY(add_song),
        COMMAND_ENTRY(remove_song),
        COMMAND_ENTRY(show_playlist),
        COMMAND_ENTRY(show_library),
        COMMAND_ENTRY(show_song),
        COMMAND_ENTRY(play),
        COMMAND_ENTRY(pause),
        COMMAND_ENTRY(stop),
        COMMAND_ENTRY(next),
        COMMAND_ENTRY(prev),
        COMMAND_ENTRY(repeat),
        COMMAND_ENTRY(randomize),
        COMMAND_ENTRY(help)
    };

    player.populate_library(ac, av);

    bool should_exit = false;
    while (!should_exit) {
        std::string line;
        CmdMap::const_iterator cmd_it;

        std::cout << player.status_line() << std::endl;
        std::cout << "$> " << std::flush;
        if (!std::getline(std::cin, line))
            break;

        std::stringstream ss(line);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::string cmd = *begin++;
        std::vector<std::string> cmd_args(begin, end);

        if (cmd == "exit")
            should_exit = true;
        else if ((cmd_it = commands.find(cmd)) != commands.end())
            cmd_it->second(cmd_args, player);
        else if (line.size() > 0)
            std::cerr << "unknown command: `" << line << "'" << std::endl;
    }

    return 0;
}
