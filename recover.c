#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<stdint.h>
typedef uint8_t BYTE;
#define Block_size 512
#define File_name_size 8
bool is_start_new_jpeg(BYTE buffer[]);
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        return 1;
    }
    FILE *infile = fopen(argv[1], "r");
    if(infile == NULL)
    {
        printf("invalid file name\n");
        return 1;
    }
    BYTE buffer[Block_size];
    int file_index = 0;
    bool have_found_firstjpg = false;
    FILE* outfile;
    while(fread(buffer, Block_size, 1, infile))
    {
        if(is_start_new_jpeg(buffer))
        {
            if(!have_found_firstjpg)
            {
                have_found_firstjpg = true;
            }
            else
            {
                fclose(outfile);
            }
            char filename[File_name_size];
            sprintf(filename, "%03i.jpeg", file_index++ );
            outfile = fopen(filename, "w");
            if(outfile == NULL)
                return 1;
            fwrite(buffer, Block_size, 1, outfile);
        }
        else if(have_found_firstjpg)
        {
            fwrite(buffer, Block_size, 1, outfile);
        }
    }
    fclose(outfile);
    fclose(infile);
}
bool is_start_new_jpeg(BYTE buffer[])
{
    return buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf) == 0xe;
}