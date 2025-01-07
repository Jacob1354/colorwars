#ifndef IMG_READER_H
#define IMG_READER_H


//PNG consts
#define PNG_SIGNATURE {137, 80, 78, 71, 13, 10, 26, 10}
#define PNG_SIGNATURE_SIZE 8
#define PNG_WIDTH_SIZE 4
#define PNG_HEIGHT_SIZE 4
#define PNG_CHUNK_TYPE_SIZE 4
#define PNG_CHUNK_LENGTH_SIZE 4

/*
 * Fills the array passed as parameter with the dimensions of a png image.
 * First element is the width. Second is the height
 *
 * @param path The path of the image
 * @param dim The array to fill {width, height}
 *
 * @return -1 if invalid filepath, -2 if array is null, -3 if not a png, -4 if unable to find dimensions
*/
int get_png_dimensions(const char* path, unsigned int dim[2]);

#endif
