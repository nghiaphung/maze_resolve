#include <iostream>
#include "maze.hpp"

using namespace std;

int maze_resolve::maze_parse_data(const std::uint32_t pix_width, const std::uint32_t pix_height, 
                    const std::uint8_t bpp, const std::uint8_t *image_data)
{
    uint8_t byte_per_pix = bpp/8;
    uint32_t pos{0};
    char * data_maze = new char[pix_width*pix_height]; 

    //size of row in bytes(strides).
    //The size of each row is rounded up to a multiple of 4 bytes by padding
    //                  (bpp * Image_width)+31
    //Rowsize = floor [ ---------------------- * 4 ]
    //                              32
    uint32_t strides = bpp * pix_width + 31;
    strides /= 32;
    strides *= 4;

    // Parse image data to maze array
    // Detail: in image_data, each pixel has bpp/8 bytes per pixel, for now, we treat the image just has black and while color,
    // it means data pixel will be just 0 (black) and non-zero (white), and we just read the first byte to determine the color
    // black : wall
    // while : route
    for (auto i = 0; i < pix_height; i++)
    {
        for (auto j = 0; j< pix_width; j++)
        {
            pos = i*strides + j*byte_per_pix; //just check the first byte of a pixel, 1 row has strides bytes, not pix_width*byte_per_pixel bytes
            if (image_data[pos]) //white color -> route
            {
                data_maze[(pix_height -1 - i)*pix_width+j] = ' '; //we need to upside down the maze data, because pixel(0,0) is at the left bottom of image data
            } else //black color -> wall
            {
                data_maze[(pix_height -1 - i)*pix_width+j] = '#';
            }
        }
    }

    this->maze_data = data_maze;
    this->maze_width = pix_width;
    this->maze_height = pix_height;
    this->row_size = strides;

    return 0;
}

int maze_resolve::maze_draw()
{
    for (auto i = 0; i < maze_height; i++)
    {
        for (auto j = 0; j< maze_width; j++)
        {
            cout << maze_data[i*maze_width+j];
        }
        cout << "\n";
    }   
    return 0;
}