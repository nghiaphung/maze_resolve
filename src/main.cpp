#include <iostream>
#include "bmp.hpp"
#include "maze.hpp"

using namespace std;

int main(int argc, char**argv)
{
	std::cout << "hello\n";
	maze_resolve maze;
	
	//check bmp file arg
	if (argv[1] == nullptr)
	{
		cout << "Please input bmp file\n";
		return 0;
	}

	bmp maze_bmp(argv[1]);

	if (maze_bmp.fileptr == nullptr)
	{
		cout << "Unable to open bmp file\n";
		return 0;
	}

	if (maze_bmp.read_header() == 0)
		return 0;

	// maze_bmp.dump_header_info();
	maze_bmp.read_image_data();

	maze.maze_parse_data(maze_bmp.info_header->pixel_width, maze_bmp.info_header->pixel_height, 
	      maze_bmp.info_header->bpp, maze_bmp.image_data);
	
	if (!maze.find_path(1521))
	{
		cout << "No path found\n";
		return 0;
	}
	// maze.maze_draw();
	maze.print_path();

	return 0;

}
