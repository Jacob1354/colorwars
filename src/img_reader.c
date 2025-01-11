#include <stdio.h>
#include "../include/img_reader.h"


int check_png_signature(FILE* img);
unsigned int big_to_little_endian(unsigned char bytes[4]);



int get_png_dimensions(const char* path, unsigned int dim[2]) {
    FILE* img = fopen(path, "r");
    if (img == NULL) {
        printf("get_png_dimensions : invalid file path\n");
        return -1;
    } if (dim == NULL) {
        printf("get_png_dimensions : pointer to dimensions array is NULL\n");
        return -2;
    }
    if(check_png_signature(img) == -1) {
        printf("get_png_dimensions : not a png file\n");
        return -3;
    }
    fseek(img, PNG_CHUNK_TYPE_SIZE + PNG_CHUNK_LENGTH_SIZE, SEEK_CUR);
    unsigned char bytes[PNG_WIDTH_SIZE + PNG_HEIGHT_SIZE];
    if(fread(bytes, sizeof(char), PNG_WIDTH_SIZE + PNG_HEIGHT_SIZE, img) != 8)
        return -4;
    dim[0] = big_to_little_endian(&bytes[0]);
    dim[1] = big_to_little_endian(&bytes[4]);
    return 1;
}

unsigned int big_to_little_endian(unsigned char bytes[4]) {
    unsigned int x = 0x2001; //Checks if system isn't already big endian
    char* c = (char*) &x;
    if(*c == 0x01) {//Means system's is little-endian (needs conversion)
        return (unsigned int) bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3];
    }
    return (unsigned int) *bytes;
}

int check_png_signature(FILE* img) {
    char expected_sign[PNG_SIGNATURE_SIZE] = PNG_SIGNATURE;
    char actual_sign[PNG_SIGNATURE_SIZE];
    if(fread((char*) actual_sign, sizeof(char), PNG_SIGNATURE_SIZE, img) < PNG_SIGNATURE_SIZE)
       return -1; 
    int i;
    for(i = 0; i < PNG_SIGNATURE_SIZE; i++)
        if(expected_sign[i] != actual_sign[i])
            return -1;
    return 1;
}
