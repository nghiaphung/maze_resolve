#include <iostream>
#include "bmp.hpp"

using namespace std;

bmp::bmp(char* bmp_dir)
    : file_header{0}, info_header{0}
{
    cout << "Constructor\n";
    cout << "open file " << bmp_dir << " ...\n";
    fileptr = fopen(bmp_dir, "rb");
};

bmp::~bmp()
{
    cout << "Destructor\n";
    if (fileptr != nullptr)
    {
        fclose(fileptr);
    }
    delete file_header;
    delete info_header;
    delete image_data;
};

// bmp::bmp(const bmp &rhs)
// {
//     file_header = new bmp_file_header;
//     info_header = new bmp_info_header;
//     fileptr = new FILE;

//     *file_header = *(rhs.file_header);
//     *info_header = *(rhs.info_header);
//     // fileptr = rhs.fileptr;
// };

int bmp::read_header()
{
    bmp_file_header *file_header = new bmp_file_header;
    bmp_info_header *info_header = new bmp_info_header;
    uint8_t tmp[4];

    //we cannot read whole struct bmp_file_header in a single fread because of byte align
    fread(&file_header->file_type, sizeof(uint16_t), 1, this->fileptr);
    fread(&file_header->file_size, sizeof(uint32_t), 1, this->fileptr);
    fseek(this->fileptr, 10, SEEK_SET); // move file ptr to pixel_offset position
    fread(&file_header->pixel_offset, sizeof(uint32_t), 1, this->fileptr);


    int info_header_size{0};
    fread(&info_header_size, sizeof(uint32_t), 1, this->fileptr);

    fseek(this->fileptr, 14, SEEK_SET); //move file ptr to begin of info header
    if (info_header_size == 40) //we just support bitmapinfoheader type
    {
        fread(&info_header->header_size, sizeof(uint32_t), 1, this->fileptr);
        fread(&info_header->pixel_width, sizeof(int32_t), 1, this->fileptr);
        fread(&info_header->pixel_height, sizeof(int32_t), 1, this->fileptr);
        fread(&info_header->color_plane_num, sizeof(uint16_t), 1, this->fileptr);
        fread(&info_header->bpp, sizeof(uint16_t), 1, this->fileptr);
        fread(&info_header->compress, sizeof(uint32_t), 1, this->fileptr);
        fread(tmp, sizeof(uint32_t), 1, this->fileptr);
        fread(&info_header->h_res, sizeof(int32_t), 1, this->fileptr);
        fread(&info_header->v_res, sizeof(int32_t), 1, this->fileptr);
        fread(&info_header->color_num, sizeof(uint32_t), 1, this->fileptr);
    } else 
    {
        cout << "BMP info header is not in support type\n";
        delete file_header;
        delete info_header;
        return -1;
    }

    //Bug fix: swap data_size
    {
        for (int i = 0; i < 4; i++)
        {
            info_header->data_size = (info_header->data_size << 8) | (uint32_t)tmp[i];
        }
        //Bug: data_size is not correct in some bmp image, with unkown reason, so we will check it with size calculated

        //size of row in bytes(strides).
        //The size of each row is rounded up to a multiple of 4 bytes by padding
        //                  (bpp * Image_width)+31
        //Rowsize = floor [ ---------------------- * 4 ]
        //                              32
        uint32_t strides = info_header->bpp * info_header->pixel_width + 31;
        strides /= 32;
        strides *= 4;

        auto data_size_calculated = strides*info_header->pixel_height;
        if (info_header->data_size != data_size_calculated)
        {
            //we will trust data_size_calculated
            info_header->data_size = data_size_calculated;
        }
    }

    this->file_header = file_header;
    this->info_header = info_header;
    return 0;
};

int bmp::read_image_data()
{
    if ((this->file_header->pixel_offset == 0) && (this->info_header->data_size == 0))
    {
        cout << "No image info in header\n";
        return -1;
    }

    uint8_t * data = new uint8_t [this->info_header->data_size];
    if (data == nullptr)
    {
        cout << "null\n";
    }

    fseek(this->fileptr, this->file_header->pixel_offset, SEEK_SET);
    fread(data, this->info_header->data_size, 1, this->fileptr);

    this->image_data = data;

    return 0;
}

void bmp::dump_header_info()
{
    cout << "file_type = " << std::hex << this->file_header->file_type << "\n";
    cout << "file_size = " << std::dec << this->file_header->file_size << "\n";
    cout << "pixel_offset = " << this->file_header->pixel_offset << "\n";

    cout << "header_size = " << this->info_header->header_size << "\n";
    cout << "pixel_width = " << this->info_header->pixel_width << "\n";
    cout << "pixel_height = " << this->info_header->pixel_height << "\n";
    cout << "color_plane_num = " << this->info_header->color_plane_num << "\n";
    cout << "bpp = " << this->info_header->bpp << "\n";
    cout << "compress = " << this->info_header->compress << "\n";
    cout << "data_size = " << this->info_header->data_size << "\n";
    cout << "h_res = " << this->info_header->h_res << "\n";
    cout << "v_res = " << this->info_header->v_res << "\n";
    cout << "color_num = " << this->info_header->color_num << "\n";
}