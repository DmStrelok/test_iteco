#include "ring_buffer.h"

#include <cstring>

namespace app {

//------------------------------------------------------------------------------
ring_buffer::packet::packet(std::unique_ptr<unsigned char[]> data, type size)
    : data(std::move(data)), size(size)
{}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ring_buffer::ring_buffer(const type size_)
    : size(size_)
{
    buffer.resize(size);
}
//------------------------------------------------------------------------------
ring_buffer::type ring_buffer::get_count()
{
    if (ind_in > ind_out) return ind_in - ind_out;
    else if (ind_in < ind_out) return size + ind_in - ind_out;
    else if (full) return size;
    else return 0;
}
//------------------------------------------------------------------------------
std::unique_ptr<ring_buffer::packet> ring_buffer::pop()
{
    std::lock_guard<std::mutex> lock{mutex};
    if (get_count() == 0) return nullptr;
    std::unique_ptr<packet> tmp = std::move(buffer[ind_out++]);
    if (ind_out >= size) ind_out = 0;
    if (ind_in == ind_out) full = false;
    return tmp;
}
//------------------------------------------------------------------------------
void ring_buffer::put(std::unique_ptr<unsigned char[]> data, type len)
{
    std::lock_guard<std::mutex> lock{mutex};
    if (full)
        if (++ind_out >= size) ind_out = 0;
    buffer[ind_in++] = std::make_unique<packet>(std::move(data), len);
    if (ind_in >= size) ind_in = 0;
    if (ind_in == ind_out) full = true;
}
//------------------------------------------------------------------------------

} // namespace app
