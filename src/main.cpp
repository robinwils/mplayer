#include <string>
#include <iostream>
#include <sstream>
#include <fts.h>

int populate_library(int ac, char** av)
{
    char *dot[] = { ".", NULL };
    char **paths = ac > 1 ? av + 1 : dot;

    FTS *tree = fts_open(paths, FTS_NOCHDIR, 0);
    if (!tree) {
        perror("fts_open");
        return 1;
    }


    FTSENT *node;
    while ((node = fts_read(tree))) {
        if (node->fts_level > 0 && node->fts_name[0] == '.')
            fts_set(tree, node, FTS_SKIP);
        else if (node->fts_info & FTS_F) {
            printf("got file named %s at depth %d, "
                "accessible via %s from the current directory "
                "or via %s from the original starting directory\n",
                node->fts_name, node->fts_level,
                node->fts_accpath, node->fts_path);
            /* if fts_open is not given FTS_NOCHDIR,
             * fts may change the program's current working directory */
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

int main(int ac, char** av)
{
    populate_library(ac, av);

    bool should_exit = false;
    while (!should_exit) {
        std::string line;
        std::cout << "$> " << std::flush;
        std::getline(std::cin, line);
        std::cout << "entered: " << line << std::endl;
        if (line == "exit")
            should_exit = true;
    }

    return 0;
}
