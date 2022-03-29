#include "settings.h"

#include <iostream>
#include <unistd.h>
#include <thread>
#include <fstream>
#include <regex>

//------------------------------------------------------------------------------
int main()
{
    app::settings sett;
    if (fork() == 0)
    {
        std::system("build/src/server/server > test.txt");
        return 0;
    }
    std::system("build/src/client/client > /dev/null");
    std::this_thread::sleep_for(std::chrono::milliseconds(sett.processed_gap * sett.size_buffer + 100));
    std::system("kill -9 `ps aux | grep build/src/server/server | awk {'print $2'}`");

    int packet_max{sett.size_block * sett.amount_blocks};
    int packet_count{0};
    int packet_failed{0};

    std::string line;
    std::ifstream in("test.txt");
    if (in.is_open())
    {
        while (getline(in, line))
        {
            const std::regex regex{"Processed: (.+?) (.+?) (PASS|FAIL)"};
            std::smatch match;
            if (std::regex_match(line, match, regex))
            {
                packet_count++;
                if (match[3] == "FAIL") packet_failed++;
            }
        }
        std::cout << "All packets: " << packet_max << '\n';
        std::cout << "Received packets: " << packet_count << '\n';
        std::cout << "Lost packets: " << packet_max - packet_count << '\n';
        std::cout << "Failed packets: " << packet_failed << '\n';
        std::cout << "Percentage of received packets: " << (100.0 * packet_count / packet_max) << '\n';
        std::cout << "Percentage of lost packets: " << (100.0 * (packet_max - packet_count) / packet_max) << '\n';
        std::cout << "Percentage of failed packets: " << (100.0 * packet_failed / packet_max) << '\n';
        std::cout << "Percentage of successful packets: " << (100.0 * (packet_count - packet_failed) / packet_max) << '\n';
    }
    in.close();
    std::system("rm -rf test.txt");
    return 0;
}
//------------------------------------------------------------------------------
