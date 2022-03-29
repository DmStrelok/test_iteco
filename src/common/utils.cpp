#include "utils.h"

#include "mbedtls/md5.h"

#include <iostream>
#include <cstring>

namespace app {
namespace utils {

//------------------------------------------------------------------------------
// Генерация данных
std::unique_ptr<std::int16_t[]> gen_data(app::settings &sett, std::uint16_t &size)
{
    size = std::rand() % (sett.size_data_max - sett.size_data_min) + sett.size_data_min + sett.size_time + sett.size_md5;
    std::unique_ptr<std::int16_t[]> data{new std::int16_t[size]};
    for (std::uint16_t i{sett.size_time}; i < size - sett.size_md5; i++)
        data[i] = ((std::rand() % 0b11111111) << 8) + std::rand() % 0b11111111;
    return data;
}
//------------------------------------------------------------------------------
std::string get_time(std::chrono::milliseconds &ms)
{
    std::chrono::system_clock::time_point tp{ms};
    std::time_t now = std::chrono::system_clock::to_time_t(tp);
    std::string times(30, '\0');
    std::strftime(&times[0], times.size(), "%F %T.", std::localtime(&now));
    times.erase(times.find_last_of(".") + 1);
    times.append(std::to_string(ms.count() % 1000));
    return times;
}
//------------------------------------------------------------------------------
void add_number(std::int16_t *data, std::uint16_t &number)
{
    data[0] = number;
}
//------------------------------------------------------------------------------
void add_time(std::int16_t *data, std::int64_t &&time)
{
    std::memcpy(data + 1, &time, 8);
}
//------------------------------------------------------------------------------
std::unique_ptr<unsigned char[]> get_hash(app::settings &sett, unsigned char *data, std::uint16_t len)
{
    std::unique_ptr<unsigned char[]> digest{new unsigned char[sett.size_md5 * 2]};
    int ret = mbedtls_md5_ret(data, len, digest.get());
    if (ret != 0)
    {
        std::cout << "MD5 create error\n";
        throw std::string{"MD5 error"};
    }
    return digest;
}
//------------------------------------------------------------------------------
void add_hash(app::settings &sett, std::int16_t *data, std::uint16_t &size)
{
    std::unique_ptr<unsigned char[]> digest =
        get_hash(sett, reinterpret_cast<unsigned char *>(data), (size - sett.size_md5) * 2);
    std::memcpy(data + size - sett.size_md5, digest.get(), sett.size_md5 * 2);
}
//------------------------------------------------------------------------------
bool check_little_endian()
{
    static union
    {
        std::uint8_t  c[2];
        std::uint16_t i{0x0001};
    } check;
    static bool chk{check.c[0] == 0x01};
    return chk;
}
//------------------------------------------------------------------------------
void data_to_net(std::int16_t *data, std::uint16_t size)
{
    if (check_little_endian())
        for (std::uint16_t i{0}; i < size; i++)
            data[i] = ((data[i] >> 8) & 0xff) | (data[i] << 8);
}
//------------------------------------------------------------------------------
std::unique_ptr<std::int16_t[]> net_to_data(unsigned char *data, std::uint16_t size)
{
    std::unique_ptr<std::int16_t[]> result{new std::int16_t[size / 2]};
    if (check_little_endian())
    {
        for (std::uint16_t i{0}, j{0}; i < size; i += 2)
            result[j++] = (data[i] << 8) | data[i + 1];
    }
    else
    {
        for (std::uint16_t i{0}, j{0}; i < size; i += 2)
            result[j++] = data[i] | (data[i + 1] << 8);
    }
    return result;
}
//------------------------------------------------------------------------------
void print(std::int16_t *data, std::uint16_t size)
{
    std::cout << "Packet: " << data[0] << " Data: ";
    for (std::uint16_t i{0}; i < size; i++)
        std::cout << data[i] << ' ';
    std::cout << '\n' << std::flush;
}
//------------------------------------------------------------------------------

} // namespace utils
} // namespace app
