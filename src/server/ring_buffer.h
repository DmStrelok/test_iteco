#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include <cstdint>
#include <memory>
#include <vector>
#include <mutex>

namespace app {

//------------------------------------------------------------------------------
class ring_buffer
{
public:
    struct packet;

private:
    using type = std::uint16_t;
    type size;
    std::vector<std::unique_ptr<packet>> buffer;
    type ind_in{0};
    type ind_out{0};
    bool full{false};
    std::mutex mutex;

public:
    ring_buffer(const type);
    ring_buffer(const ring_buffer &) = delete;
    ring_buffer(ring_buffer &&) = delete;
    ring_buffer &operator=(const ring_buffer &) = delete;
    ring_buffer &operator=(ring_buffer &&) = delete;

    type get_count();
    std::unique_ptr<packet> pop();
    void put(std::unique_ptr<unsigned char[]>, type);
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
struct ring_buffer::packet
{
    packet(std::unique_ptr<unsigned char[]>, type);
    std::unique_ptr<unsigned char[]> data;
    type size;
};
//------------------------------------------------------------------------------

} // namespace app
#endif // __RING_BUFFER_H__
