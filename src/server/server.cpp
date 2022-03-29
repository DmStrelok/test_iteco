#include "settings.h"
#include "socket.h"
#include "utils.h"
#include "ring_buffer.h"
#include "thread_pool.h"

#include <cstring>

//------------------------------------------------------------------------------
int receive_packet(app::socket_server &sock, app::settings &sett, app::ring_buffer &rbuffer,
    app::condition_var &cv, unsigned char *buf, std::uint16_t &len)
{
    std::uint16_t bytes_read = sock.recvfrom(buf, len);
    if ((bytes_read < (sett.size_time + sett.size_data_min + sett.size_md5) * 2) ||
        bytes_read % 2 != 0) return -1;
    std::unique_ptr<unsigned char[]> data{new unsigned char[bytes_read]};
    std::memcpy(data.get(), buf, bytes_read);
    rbuffer.put(std::move(data), bytes_read);
    cv.notify_one();
    return bytes_read;
}
//------------------------------------------------------------------------------
void run(app::settings *sett, app::ring_buffer *rbuffer, app::condition_var *cv, std::function<bool()> is_stop)
{
    while(!is_stop())
    {
        std::unique_ptr<app::ring_buffer::packet> packet = rbuffer->pop();
        if (!packet)
            cv->wait_for(sett->processed_gap);
        else
        {
            std::unique_ptr<unsigned char[]> digest =
                app::utils::get_hash(*sett, packet->data.get(), packet->size - sett->size_md5 * 2);
            bool result = (std::memcmp(packet->data.get() + (packet->size - sett->size_md5 * 2),
                digest.get(), sett->size_md5 * 2) == 0);
            std::unique_ptr<std::int16_t[]> data =
                app::utils::net_to_data(packet->data.get(), packet->size - sett->size_md5 * 2);

            // Вывод данных
            // app::utils::print(data.get() + sett->size_time, packet->size / 2 - sett->size_time - sett->size_md5);

            std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch());
            std::cout << "Processed: " << packet->data[0] << " "
                << app::utils::get_time(ms) << (result ? " PASS" : " FAIL") << '\n' << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(sett->processed_gap));
        }
    }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int main()
{
    std::srand(std::time(nullptr));
    app::settings sett;

    // Открытие сокета
    app::socket_server sock{sett.ip_server, sett.port_server, sett.ip_client, sett.port_client};
    if (!sock.is_valid()) exit(1);

    // Прием сообщений
    std::uint16_t len{(sett.size_time + sett.size_data_max + sett.size_md5) * 2 + 1}; // Полный размер сообщения с данными
    std::unique_ptr<unsigned char[]> buf{new unsigned char[len]};

    app::thread_pool pool{sett.size_pool};
    app::ring_buffer rbuffer{sett.size_buffer};
    app::condition_var cv;
    for (std::uint16_t i{0}; i < sett.size_pool; i++)
        pool.add(run, sett, rbuffer, cv);

    while (true)
    {
        receive_packet(sock, sett, rbuffer, cv, buf.get(), len);
    }
    return 0;
}
//------------------------------------------------------------------------------
