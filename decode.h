#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>

#include "types.h" // Contains user defined types

/*
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;     

    /* Secret File Info */
    char output_fname[50];       
    FILE *fptr_output;     
    char extn_secret_file[20];    
    long size_secret_file;   


} DecodeInfo;

/* Encoding function prototype */

/* Read and validate decode args */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Open files */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode magic string */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode extension size */
Status decode_secret_file_extn_size(int *size,DecodeInfo *decInfo);

/* Decode extension */
Status decode_secret_file_extn(char *file_extn, int extn_size, DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(long *size,DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode one byte from LSB */
Status decode_byte_from_lsb(char *data,char *image_buffer);

/* Decode integer from LSB */
Status decode_size_from_lsb(int *size,char *image_buffer);

#endif
