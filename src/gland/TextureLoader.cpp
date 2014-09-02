/***************************************************************************
*
* Copyright 2014 Bruno Ordozgoiti
* Distributed under GNU GPLv3
*
* This file is part of GLandbox.
*
* GLandbox is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* GLandbox is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with GLandbox. If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************/

/*
 * File TextureLoader.cpp
 * Author: DavidEGrayson, bruno
 */


#include <cstdlib>
#include <iostream>
#include "png.h"

#include "TextureLoader.h" 
namespace gland{

    TextureLoader::TextureLoader(){}

    TextureData TextureLoader::loadDataFromPng(const char * file_name){
        png_byte header[8];

        FILE *fp = fopen(file_name, "rb");
        if (fp == 0){
                perror(file_name);
            }
        // read the header
        fread(header, 1, 8, fp);
        if (png_sig_cmp(header, 0, 8)){
                fprintf(stderr, "error: %s is not a PNG.\n", file_name);
                fclose(fp);
            }

        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr){
                fprintf(stderr, "error: png_create_read_struct returned 0.\n");
                fclose(fp);
            }

        // create png info struct
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr){
                fprintf(stderr, "error: png_create_info_struct returned 0.\n");
                png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
                fclose(fp);
            }

        // create png info struct
        png_infop end_info = png_create_info_struct(png_ptr);
        if (!end_info){
                fprintf(stderr, "error: png_create_info_struct returned 0.\n");
                png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
                fclose(fp);
            }

        // the code in this if statement gets called if libpng encounters an error
        if (setjmp(png_jmpbuf(png_ptr))) {
            fprintf(stderr, "error from libpng\n");
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            fclose(fp);
        }

        // init png reading
        png_init_io(png_ptr, fp);

        // let libpng know you already read the first 8 bytes
        png_set_sig_bytes(png_ptr, 8);

        // read all the info up to the image data
        png_read_info(png_ptr, info_ptr);

        // variables to pass to get info
        int bit_depth, color_type;
        png_uint_32 width, height;

        // get info about png
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                     NULL, NULL, NULL);

        //printf("%s: %lux%lu %d\n", file_name, width, height, color_type);

        if (bit_depth != 8){
                fprintf(stderr, "%s: Unsupported bit depth %d. Must be 8.\n", file_name, bit_depth);
            }

        GLint format;
        switch(color_type){
            case PNG_COLOR_TYPE_RGB:
                format = GL_RGB;
                break;
            case PNG_COLOR_TYPE_RGB_ALPHA:
                format = GL_RGBA;
                break;
            default:
                fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name, color_type);
            }

        // Update the png info struct.
        png_read_update_info(png_ptr, info_ptr);

        // Row size in bytes.
        int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

        // glTexImage2d requires rows to be 4-byte aligned
        rowbytes += 3 - ((rowbytes-1) % 4);

        // Allocate the image_data as a big block, to be given to opengl
        png_byte * image_data = (png_byte *)malloc(rowbytes * height * sizeof(png_byte)+15);
        if (image_data == NULL){
                fprintf(stderr, "error: could not allocate memory for PNG image data\n");
                png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
                fclose(fp);
            }

        // row_pointers is for pointing to image_data for reading the png with libpng
        png_byte ** row_pointers = (png_byte **)malloc(height * sizeof(png_byte *));
        if (row_pointers == NULL){
                fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
                png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
                free(image_data);
                fclose(fp);
            }

        // set the individual row_pointers to point at the correct offsets of image_data
        for (unsigned int i = 0; i < height; i++){
                row_pointers[height - 1 - i] = image_data + i * rowbytes;
            }

        // read the png into image_data through row_pointers
        png_read_image(png_ptr, row_pointers);

        // clean up
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        //free(image_data);
        free(row_pointers);
        fclose(fp);

        TextureData tData(image_data, width, height, format);
        return tData;
    }


    GLuint TextureLoader::loadFromPng(const char * fileName){
        TextureData data = loadDataFromPng(fileName);
        // Generate the OpenGL texture object
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        int mipmapLevel = 9;
        //Loop for mipmaps
        for(int i = 0; i < mipmapLevel;i++){        
          glTexImage2D(GL_TEXTURE_2D, i, data.format, data.width, data.height, 0, data.format, GL_UNSIGNED_BYTE, data.data);
        }         
        
        glGenerateMipmap(GL_TEXTURE_2D);  
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        //Mipmapping levels to use
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmapLevel);
        free(data.data);
        return texture;
    }
}
