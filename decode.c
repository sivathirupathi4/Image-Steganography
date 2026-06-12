#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"


Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    char *extn;

    /* Validate stego image */

    extn = strrchr(argv[2], '.');

    if (extn == NULL || strcmp(extn, ".bmp") != 0)
    {
        return e_failure;
    }

    decInfo->stego_image_fname = argv[2];

    /* Optional output file */

    if (argv[3] == NULL)
    {
        strcpy(decInfo->output_fname, "stego.txt");
    }
    else
    {
        strcpy(decInfo->output_fname, argv[3]);

        /* Remove old extension */

        char *dot = strrchr(decInfo->output_fname, '.');

        if (dot != NULL)
        {
            *dot = '\0';
        }

        /* Add .txt */

        strcat(decInfo->output_fname, ".txt");
    }

    printf("Read and Validation Done\n");

    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    /* Open stego image file */

    decInfo->fptr_stego_image =
        fopen(decInfo->stego_image_fname, "rb");

    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");

        fprintf(stderr,
                "ERROR: Unable to open file %s\n",
                decInfo->stego_image_fname);

        return e_failure;
    }

    /* Open output file */

    decInfo->fptr_output =
        fopen(decInfo->output_fname, "w");

    if (decInfo->fptr_output == NULL)
    {
        perror("fopen");

        fprintf(stderr,
                "ERROR: Unable to open file %s\n",
                decInfo->output_fname);

        return e_failure;
    }

    printf("Open files successful\n");

    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo){
    printf("Decoding magic string\n");
    char buffer[8];
    char magic[3];
    int i;

    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    for (i = 0; i < 2; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);

        decode_byte_from_lsb(&magic[i], buffer);
    }

    magic[2] = '\0';

    if (strcmp(magic, MAGIC_STRING) == 0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status decode_secret_file_extn_size(int *size,
                                    DecodeInfo *decInfo)
{
    printf("Decoding secret file extn size\n");
    char buffer[32];

    fread(buffer, 32, 1, decInfo->fptr_stego_image);

    decode_size_from_lsb(size, buffer);

    return e_success;
}

Status decode_secret_file_extn(char *file_extn, int extn_size,DecodeInfo *decInfo)
{
    char buffer[8];
    int i;
    for (i = 0; i < extn_size; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);

        decode_byte_from_lsb(&file_extn[i], buffer);
    }

    file_extn[extn_size] = '\0';
    

    return e_success;
}

Status decode_secret_file_size(long *size, DecodeInfo *decInfo)
{
    char buffer[32];
    int temp;

    fread(buffer, 32, 1, decInfo->fptr_stego_image);

    decode_size_from_lsb(&temp, buffer);

    *size = temp;

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];
    char ch;
    long i;

    for (i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);

        decode_byte_from_lsb(&ch, buffer);

        fputc(ch, decInfo->fptr_output);
    }

    return e_success;
}


Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    int i;
    *data = 0;

    for(i = 0; i < 8; i++)
    {
        *data = *data << 1;
        *data = *data | (image_buffer[i] & 1);
    }

    return e_success;
}

Status decode_size_from_lsb(int *size, char *image_buffer)
{
    int i;

    *size = 0;

    for(i = 0; i < 32; i++)
    {
        *size = *size << 1;
        *size = *size | (image_buffer[i] & 1);
    }

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    int extn_size;

    if (open_decode_files(decInfo) != e_success)
    {
        return e_failure;
    }

    printf("Files opened successfully\n");

    if (decode_magic_string(decInfo) != e_success)
    {
        printf("Magic string mismatch\n");
        return e_failure;
    }

    printf("Magic string decoded\n");

    decode_secret_file_extn_size(&extn_size, decInfo);

    printf("Extension size decoded\n");

    decode_secret_file_extn(decInfo->extn_secret_file,extn_size,decInfo);

    printf("Extension decoded: %s\n",decInfo->extn_secret_file);

    decode_secret_file_size(&decInfo->size_secret_file,decInfo);
    printf("Secret file size decoded\n");

    decode_secret_file_data(decInfo);
    printf("Secret file size = %ld\n",
       decInfo->size_secret_file);

    printf("Secret file data decoded\n");

    return e_success;
}