#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

#include "settings.h"
#include "socket.h"

#include <memory>
#include <thread>

namespace app {
namespace utils {

//------------------------------------------------------------------------------
// Генерация данных
std::unique_ptr<std::int16_t[]> gen_data(app::settings &, std::uint16_t &);
//------------------------------------------------------------------------------
std::string get_time(std::chrono::milliseconds &);
//------------------------------------------------------------------------------
void add_number(std::int16_t *, std::uint16_t &);
//------------------------------------------------------------------------------
void add_time(std::int16_t *, std::int64_t &&);
//------------------------------------------------------------------------------
std::unique_ptr<unsigned char[]> get_hash(app::settings &, unsigned char *, std::uint16_t);
//------------------------------------------------------------------------------
void add_hash(app::settings &sett, std::int16_t *, std::uint16_t &);
//------------------------------------------------------------------------------
bool check_little_endian();
//------------------------------------------------------------------------------
void data_to_net(std::int16_t *, std::uint16_t);
//------------------------------------------------------------------------------
std::unique_ptr<std::int16_t[]> net_to_data(unsigned char *, std::uint16_t);
//------------------------------------------------------------------------------
void print(std::int16_t *, std::uint16_t);
//------------------------------------------------------------------------------

} // namespace utils
} // namespace app
#endif // __COMMON_UTILS_H__
