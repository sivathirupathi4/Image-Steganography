#Image Steganography in C
##📌 Overview

This project implements Image Steganography using the Least Significant Bit (LSB) technique in C. It allows users to hide a secret text file inside a BMP image and later retrieve the hidden information without visibly altering the image.

##🚀 Features
Encode a secret text file into a BMP image
Decode and recover hidden data from the stego image
Validation of input files and formats
Capacity checking before encoding
Supports command-line arguments
Binary file handling and bit-level manipulation

##🛠️ Technologies Used
C Programming
File Handling
Bitwise Operators
Structures
Command Line Arguments
Modular Programming

##📂 Project Structure
.
├── main.c
├── encode.c
├── decode.c
├── common.h
├── encode.h
├── decode.h
└── README.md

##⚙️ How It Works
Encoding Process
Read the source BMP image.
Validate image capacity.
Encode metadata (file extension and file size).
Encode secret file data into image pixels using LSB.
Generate a new stego image.
Decoding Process
Read the stego image.
Extract metadata.
Recover the hidden file.
Write the decoded content to an output file.

##▶️ Compilation
gcc *.c -o stego

##🔐 Encoding
./stego -e beautiful.bmp secret.txt output.bmp

##Example
./stego -e beautiful.bmp secret.txt stego.bmp

##🔓 Decoding
./stego -d stego.bmp output.txt

##Example
./stego -d stego.bmp decoded.txt

##💡 Concepts Learned
Binary File Operations
Image Data Processing
Bitwise Manipulation
Data Encoding and Decoding
Memory Management
Modular Software Design
Error Handling and Validation

##🎯 Learning Outcome

This project provided hands-on experience with real-world applications of C programming, particularly in file handling, bitwise operations, and data security concepts. It strengthened my understanding of low-level programming and modular software development.

##📸 Sample Execution
###Encoding
./stego -e beautiful.bmp secret.txt output.bmp

Validation Done
Checking file permissions
Reading image dimensions
Checking capacity
Encoding successful

###Decoding
./stego -d output.bmp decoded.txt

Decoding successful
Secret data recovered
👨‍💻 Author

Sivaraman T
