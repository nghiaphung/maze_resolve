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
    // while : space
    for (auto i = 0; i < pix_height; i++)
    {
        for (auto j = 0; j< pix_width; j++)
        {
            pos = i*strides + j*byte_per_pix; //just check the first byte of a pixel, 1 row has strides bytes, not pix_width*byte_per_pixel bytes
            uint32_t space_idx = (pix_height -1 - i)*pix_width+j;
            if (image_data[pos]) //white color -> space
            {
                data_maze[space_idx] = ' '; //we need to upside down the maze data, because pixel(0,0) is at the left bottom of image data
                path_node_num++;
                space_id.push_back(space_idx);
            } else //black color -> wall
            {
                data_maze[space_idx] = '#';
            }
        }
    }

    this->maze_data = data_maze;
    this->maze_width = pix_width;
    this->maze_height = pix_height;
    this->row_size = strides;
    this->visited = new bool [pix_width*pix_height]{false};

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

static vector<uint32_t> get_neighbor(maze_resolve &obj, uint32_t space_id)
{
    vector<uint32_t>tmp{space_id + 1, space_id - 1, space_id + obj.maze_width,space_id - obj.maze_width};
    
    //check if space_id is at border
    if ((space_id / obj.maze_width) == 0)
    {
        //on the top border
        tmp.erase(tmp.begin()+3);
    }
    if ((space_id / obj.maze_width) == (obj.maze_height-1))
    {
        //on the bottom border
        tmp.erase(tmp.begin()+2);
    }
    if ((space_id % obj.maze_width) == 0)
    {
        //on the left border
        tmp.erase(tmp.begin()+1);
    }
    if ((space_id % obj.maze_width) == (obj.maze_width-1))
    {
        //on the right border
        tmp.erase(tmp.begin()+0);
    }

    //check if neighbor is wall or not
    for (auto i = tmp.begin(); i < tmp.end(); i++)
    {
        if (obj.maze_data[*i] == '#')
        {
            //Notice that the iterator is decremented after it is passed to erase()
            tmp.erase(i--);
        }
    }

    return tmp;
}

static int dfs(maze_resolve &obj, uint32_t space_idx, uint32_t des_idx)
{
    obj.visited[space_idx] = true;

    vector<uint32_t> neighbor_vec = get_neighbor(obj, space_idx);

    for (auto i:neighbor_vec)
    {
        if (i == des_idx)
        {
            return 0;
        }
        if (obj.visited[i] == false)
        {
            if (!dfs(obj, i, des_idx))
            {
                return 0;
            }
        }
    }
    return -1;

}

int maze_resolve::find_path(uint32_t des_idx)
{
    dfs(*this, start_x*maze_width+start_y, des_idx);
    return 0;
}