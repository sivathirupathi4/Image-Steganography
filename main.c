/* 
 * Name: Sivaraman T
 * Date: 26/05/2026
 * Project: Steganography
 * 
 * Description: 
 * Developed a BMP Image Steganography project in C to securely hide and retrieve secret text data 
 * inside image files using Least Significant Bit (LSB) encoding technique. Implemented both encoding
 * and decoding modules with file handling, bitwise operations, and command-line argument processing.
 */
#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

OperationType check_operation_type(char *);

int main(int argc, char *argv[])
{
    //step1 -> call check_operation_type(argv[1]);
            // print msg
        //-e -> encode
        // declare structure variable EncodeInfo encInfo;
        // check read_and_validate_encode_args(argv, &encInfo) is e_success or not
                // no -> print error msg and return e_failure;
                // yes -> check do_encoding(&encInfo) returning e_success or not
                            // no -> print error msg and stop 
                            // yes -> print success msg and stop
    
    if(check_operation_type(argv[1])==e_encode){
        EncodeInfo encInfo;
        if(read_and_validate_encode_args(argv, &encInfo)!=e_success){
            printf("Invalid Input");
            return e_failure;
        }
        else{
            if(do_encoding(&encInfo)!=e_success){
                printf("ERROR: not done encoding");
                return 0;
            }
            else{
                printf("Encoding done successfully");
                return 0;
            }
        }
    }
    else if(check_operation_type(argv[1])==e_decode){
        DecodeInfo decInfo;
        if(read_and_validate_decode_args(argv, &decInfo)!=e_success){
            printf("Read and validate failed");
            return e_failure;
        }
        else{
            if(do_decoding(&decInfo)!=e_success){
                printf("ERROR: not done encoding");
                return 0;
            }
            else{
                printf("decoding done successfully");
                return 0;
            }
        }
    }
    else{
        printf("Unsupported Operation");
    }
}

OperationType check_operation_type(char *symbol)
{
    //step1 -> check it is -e or -d
         //-e ->return e_encode
            //-d -> return e_decode

            //return e_unsupported
    if(strcmp(symbol,"-e")==0){
        return e_encode;
    }
    else if(strcmp(symbol,"-d")==0){
        return e_decode;
    }
    else{
        return e_unsupported;
    }
}
