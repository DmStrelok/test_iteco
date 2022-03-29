#include "settings.h"
#include "socket.h"
#include "utils.h"

//------------------------------------------------------------------------------
void send_block(app::socket_client &sock, app::settings &sett)
{
    for (std::uint16_t i{0}; i < sett.size_block; i++)
    {
        std::uint16_t size;
        std::unique_ptr<std::int16_t[]> data = app::utils::gen_data(sett, size);

        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
        app::utils::add_number(data.get(), i);
        app::utils::add_time(data.get(), ms.count());

        // Вывод данных
        // app::utils::print(data.get() + sett.size_time, size - sett.size_time - sett.size_md5);

        app::utils::data_to_net(data.get(), size - sett.size_md5);
        app::utils::add_hash(sett, data.get(), size);

        sock.send(reinterpret_cast<unsigned char *>(data.get()), size * 2);
        std::cout << "Sent: " << i << " " << app::utils::get_time(ms) << '\n' << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(sett.packet_gap));
    }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int main()
{
    std::srand(std::time(nullptr));
    app::settings sett;

    // Открытие сокета
    app::socket_client sock{sett.ip_server, sett.port_server, sett.ip_client, sett.port_client};
    if (!sock.is_valid()) exit(1);

    // Отправка сообщений
    send_block(sock, sett);
    for (std::uint16_t i{1}; i < sett.amount_blocks; i++)
    {
        std::this_thread::sleep_for(std::chrono::seconds(sett.block_gap));
        send_block(sock, sett);
    }

    return 0;
}
//------------------------------------------------------------------------------
