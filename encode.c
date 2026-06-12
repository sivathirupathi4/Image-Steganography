#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    printf("Reading Width and Height\n");
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
    uint size;

    fseek(fptr, 0, SEEK_END);

    size = ftell(fptr);

    rewind(fptr);

    return size;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[],
                                     EncodeInfo *encInfo)
{
    char *secret_extn;

    /* Validate source bmp */

    if (strstr(argv[2], ".bmp") == NULL)
    {
        return e_failure;
    }
    else
    {
        encInfo->src_image_fname = argv[2];
    }

    /* Validate secret file */

    if (strstr(argv[3], ".") == NULL)
    {
        return e_failure;
    }
    else
    {
        encInfo->secret_fname = argv[3];

        /* Extract extension */

        secret_extn = strrchr(argv[3], '.');

        strcpy(encInfo->extn_secret_file,
               secret_extn);
    }
    printf("Extension = %s\n",
       encInfo->extn_secret_file);
    /* Validate output bmp */

    if (argv[4] == NULL)
    {
        encInfo->stego_image_fname = "stego.bmp";
    }
    else
    {
        if (strstr(argv[4], ".bmp") == NULL)
        {
            return e_failure;
        }
        else
        {
            encInfo->stego_image_fname = argv[4];
        }
    }

    printf("Validation Done\n");

    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    printf("checking open files validations\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    // step1 -> encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
    // step2 -> encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret)
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    // step3 -> check encInfo -> image_capacity > 16+32+32+32+(encInfo -> size_secret_file * 8)
                //yes -> return e_success
                //no -> return e_failure
    if(encInfo -> image_capacity > 16+32+32+32+(encInfo -> size_secret_file * 8)){
        return e_success;
    }
    else{
        return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    printf("Copying header source to destination\n");
    //step1 -> rewind file pointer
    rewind(fptr_src_image);
    //step2 -> read 54 bytes from src file
    char header[54];
    fread(header, 54, 1, fptr_src_image);
    //step3 -> write 54 bytes into dest file
    fwrite(header, 54, 1, fptr_dest_image);
    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    printf("Encoding magic string\n");
    // char buffer[8];
    char buffer[8];
    int i=0;
    //step1 -> read 8 bytes from src file
    while(magic_string[i]!='\0'){
        fread(buffer, 8, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i], buffer);
        fwrite(buffer, 8, 1, encInfo->fptr_stego_image);
        i++;
    }
    //step2 -> call encode_byte_to_lsb(magic_string[0], buffer)

    //step3 -> write the buffer into dest file

    //step4 -> repeat this for size of magic_string time
    return e_success;
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    printf("Encoding secret file extn size\n");
    // char buffer[32];
    char buffer[32];
    //step1 -> read 32 bytes from src file
    
        fread(buffer, 32, 1, encInfo->fptr_src_image);
        encode_size_to_lsb(size, buffer);
        fwrite(buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
    //step2 -> call encode_size_to_lsb(size, buffer)

    //step3 -> write the buffer into dest file
}

Status encode_secret_file_extn(const char *file_extn,
                               EncodeInfo *encInfo)
{
    char buffer[8];
    int i;

    for(i = 0; i < strlen(file_extn); i++)
    {
        fread(buffer, 8, 1,
              encInfo->fptr_src_image);

        encode_byte_to_lsb(file_extn[i],
                           buffer);

        fwrite(buffer, 8, 1,
               encInfo->fptr_stego_image);
    }

    return e_success;
}

Status encode_secret_file_size(long file_size,
                               EncodeInfo *encInfo)
{
    char buffer[32];

    printf("Encoding secret file size\n");

    fread(buffer, 32, 1,
          encInfo->fptr_src_image);

    encode_size_to_lsb(file_size,
                       buffer);

    fwrite(buffer, 32, 1,
           encInfo->fptr_stego_image);

    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("Encoding secret file data\n");
    // char buffer[8];
    char buffer[8];
    
    int ch;

    rewind(encInfo->fptr_secret);

    while ((ch = fgetc(encInfo->fptr_secret)) != EOF)
    {
        //step1 -> read 8 bytes from src file
        fread(buffer, 8, 1, encInfo->fptr_src_image);

        //step2 -> call encode_byte_to_lsb(magic_string[0], buffer)
        encode_byte_to_lsb(ch, buffer);

        //step3 -> write the buffer into dest file
        fwrite(buffer, 8, 1, encInfo->fptr_stego_image);
    }

    return e_success;

    //step4 -> repeat this for size of secret_data time
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    printf("Copying remaining inage data\n");
    //logic to copy reamaining data
    int ch;

    while ((ch = fgetc(fptr_src)) != EOF)
    {
        fputc(ch, fptr_dest);
    }

    return e_success;
    // return e_success
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    int i;

    for(i = 0; i < 8; i++)
    {
        image_buffer[i] =
            (image_buffer[i] & 0xFE) |
            ((data >> (7 - i)) & 1);
    }

    return e_success;
}

Status encode_size_to_lsb(int size, char *image_buffer)
{
    int i;

    for(i = 0; i < 32; i++)
    {
        image_buffer[i] =
            (image_buffer[i] & 0xFE) |
            ((size >> (31 - i)) & 1);
    }

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    // step1 -> check open_files(encInfo) returning e_success or not
        // yes -> print success msg goto next step
        // no -> return e_failure
    if(open_files(encInfo)!=e_success){
        return e_failure;
    }
    else{
        printf("Done\n");
    }
    // step2 -> check check_capacity(encInfo) is returning success or not
            // yes -> print success msg and goto next step
            // no -> print error msg and return e_failure
    if(check_capacity(encInfo)!=e_success){
        printf("Error: Your text size is higher..");
        return e_failure;
    }
    else{
        printf("Check capacity Done\n");
    }
    // step3 -> call copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)
    copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image);
        printf("Done\n");

    // step4 -> call encode_magic_string(MAGIC_STRING, encInfo)
    encode_magic_string(MAGIC_STRING, encInfo);
        printf("Done\n");
    
    // step5 -> call encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo)
    encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo);
        printf("Done\n");

    //step6 -> call encode_secret_file_extn(encInfo -> extn_secret_file, encInfo)
    encode_secret_file_extn(encInfo -> extn_secret_file, encInfo);
        printf("Done\n");

    //step7 -> Call encode_secret_file_size(encInfo -> size_secret_file, encInfo)
    encode_secret_file_size(encInfo -> size_secret_file, encInfo);
        printf("Done\n");

    //step8 -> call encode_secret_file_data(encInfo)
    encode_secret_file_data(encInfo);
       printf("done\n");

    //step9 -> call copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> stego_image_fname);
    copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image);
        printf("Done\n");
return e_success;
}
