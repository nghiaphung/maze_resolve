#ifndef _MAZE_H_
#define _MAZE_H_

struct maze_resolve
{
    static const std::uint32_t start_x{1};
    static const std::uint32_t start_y{1};
    std::uint32_t finish_x{0};
    std::uint32_t finish_y{0};
    std::uint32_t maze_width{0};
    std::uint32_t maze_height{0};
    char * maze_data{nullptr};
    std::uint32_t row_size{0};

    maze_resolve() = default;
    ~maze_resolve() {delete[] maze_data;};
    maze_resolve(const maze_resolve &rhs) = delete;
    maze_resolve(maze_resolve &&rhs) = delete;
    maze_resolve& operator=(const maze_resolve &rhs) = delete;
    maze_resolve& operator=(maze_resolve &&rhs) = delete;

    int maze_draw();
    int maze_parse_data(const std::uint32_t pix_width, const std::uint32_t pix_height, 
                    const std::uint8_t bpp, const std::uint8_t *image_data);
};

#endif