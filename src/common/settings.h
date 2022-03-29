#ifndef __COMMON_SETTINGS_H__
#define __COMMON_SETTINGS_H__

#include <cstdint>

namespace app {

//------------------------------------------------------------------------------
struct settings
{
    const char *ip_server = "127.0.0.1";        // IP сервера
    const char *ip_client = "127.0.0.1";        // IP клиента
    const std::uint16_t port_server{10000};     // Порт сервера
    const std::uint16_t port_client{11000};     // Порт клиента
    const std::uint16_t size_time{5};           // Место под номер пакета и время
    const std::uint16_t size_md5{8};            // Место под хеш MD5
    const std::uint16_t size_data_min{600};     // Размер данных в пакете минимум
    const std::uint16_t size_data_max{1500};    // Размер данных в пакете максимум
    const std::uint16_t size_block{1000};       // Количество пакетов в блоке
    const std::uint16_t amount_blocks{2};       // Количество блоков
    const std::uint16_t packet_gap{10};         // Интервал между пакетами, мс
    const std::uint16_t block_gap{10};          // Интервал между блоками, с
    const std::uint16_t processed_gap{15};      // Время задержки обработчика, мс
    const std::uint16_t size_buffer{16};        // Размер кольцевого буффера
    const std::uint16_t size_pool{1};           // Размер пула потоков обработчиков
};
//------------------------------------------------------------------------------

} // namespace app
#endif // __COMMON_SETTINGS_H__
