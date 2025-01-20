#ifndef IMG_READER_H
#define IMG_READER_H


/*
 * Fills the array passed as parameter with the dimensions of a png image.
 * First element is the width. Second is the height
 *
 * @param path The path of the image
 * @param dim The array to fill {width, height}
 *
 * @return 1 if valid,-1 if invalid filepath, -2 if array is null, 
 *         -3 if not a png, -4 if unable to find dimensions
*/
int get_png_dimensions(const char* path, unsigned int dim[2]);

#endif
