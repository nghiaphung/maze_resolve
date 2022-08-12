#ifndef _BMP_H_
#define _BMP_H_

struct bmp_file_header
{
    std::uint16_t file_type {0};
    std::uint32_t file_size {0};
    std::uint16_t rev1 {0};
    std::uint16_t rev2 {0};
    std::uint32_t pixel_offset {0}; //offset from file  header to pixel data in byte 
};

struct bmp_info_header
{
    std::uint32_t header_size{0};
    std::int32_t pixel_width{0};
    std::int32_t pixel_height{0};
    std::uint16_t color_plane_num{0};
    std::uint16_t bpp{0};
    std::uint32_t compress{0};
    std::uint32_t data_size{0};
    std::int32_t h_res{0};
    std::int32_t v_res{0};
    std::uint32_t color_num{0};
    std::uint32_t color_important{0}; 
};

struct bmp
{
    static const std::uint16_t bmp_file_type = 0x4D42; //0x4d42 is 'MB'

    bmp_file_header * file_header{nullptr};
    bmp_info_header * info_header{nullptr};
    FILE * fileptr{nullptr};
    std::uint8_t * image_data {nullptr};

    bmp(char* bmp_dir);
    ~bmp();
    bmp(const bmp &rhs) = delete;
    bmp(bmp &&rhs) = delete;
    bmp& operator=(const bmp &rhs) = delete;
    bmp& operator=(bmp &&rhs) = delete;

    int read_header();
    int read_image_data();
    void dump_header_info();
};


#endif