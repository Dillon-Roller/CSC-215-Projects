/***************************************************************************//**
 * @file
 *
 * @brief Reads, writes, and performs operations on ASCII and Binary image files.
 *
 * @mainpage Program 1 - Image Manipulation on ASCII and Binary files.
 *
 * @section course_section Course Information
 *
 * @authors Dillon Roller
 *
 * @date October 5 2017
 *
 * @par Instructor:
 *         Dr. Schrader
 *
 * @par Course:
 *         CSC 215 - Section 1 - 2:00 pm
 *
 * @par Location:
 *         Mineral Industries Bulding 222
 *
 * @section program_section Program Information
 *
 * @details This program will read in '.ppm' image files and store their pixel
 * values into dynamically allocated arrays. Images read in can be in ASCII or
 * binary formal. You can then perform varous image operations and then output
 * an ASCII or binary image copy of the file. Only one image file can be outputted 
 * per run of the program.
 *
 * @section compile_section Compiling and Usage
 *
 * @par Compiling Instructions:
 *      Program is ran from command prompt and files are placed into same
 *      folder as the executable. Run from the command line in the following
 *      way:\n\n
 *      C:\>prog1.exe [option] -o[a or b] outputname inputname.ppm
 *
   @verbatim
   Option Code:     Option Name:
   -n               Negate
   -b #             Brighten  
   -p               Sharpen
   -s               Smooth
   -g               Grayscale
   -c               Contrast
   @endverbatim
 *
 * @par Usage:
   @verbatim
   c:\> prog1.exe
   d:\> c:\bin\prog1.exe
   @endverbatim
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 *
 * @bug  none
 *
 * @todo none
 *
 * @par Modifications and Development Timeline:
 *    <a href="https://gitlab.mcs.sdsmt.edu/7407475/csc215f17programs/commits/master">Click here for commit log. </a>
   @verbatim
   Date          Modification
   ------------  ---------------------------------------------------------------
   Sep 18, 2017  Setup header file skeleton
   Sep 19, 2017  Memory allocation done, imagefileIO almost completed, main
                 almost completed, netPBM.h updated
   Sep 20, 2017  ImageFileIO completed, main completed
   Sep 20, 2017  Finished everything except sharpen and smoothen functions,
                 TODO: doxygen
   Oct 4, 2017   Finished sharpen and smoothen functions and started doxygen
   Oct 5, 2017   Doxygen almost completed
   Oct 6, 2017   Program completed
   
   @endverbatim
 *
 ******************************************************************************/
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#ifndef __NETPBM__H__
#define __NETPBM__H__
/*!
 * @brief Pixel contains a value for a single pixel within an image.
 */
typedef unsigned char pixel;
/*!
 * @brief Holds information about an image
 */
struct image
{
    string name;                /*!< The name of the file*/
    string comment;             /*!< The comment in the file*/
    string header;              /*!< The magic number in the file*/
    int rows;                   /*!< The amount of rows for the image*/
    int cols;                   /*!< The amount of columns for the image*/
    int max;                    /*!< The max pixel value for the image*/
    pixel **redgray = nullptr;  /*!< Pointer to 2D array for red and gray values*/
    pixel **green = nullptr;    /*!< Pointer to 2D array for green values*/
    pixel **blue = nullptr;     /*!< Pointer to 2D array for blue values*/
    pixel **newred = nullptr;   /*!< Pointer to 2D array to hold new red values*/
    pixel **newgreen = nullptr; /*!< Pointer to 2D array to hold new green values*/
    pixel **newblue = nullptr;  /*!< Pointer to 2D array to hold new blue values*/
};

/*******************************************************************************
 *                         Function Prototypes
 ******************************************************************************/
bool readHeaderInfo(ifstream &fin, image &file);
bool alloc2d(pixel** &arr, int rows, int cols);
void free2d(pixel** &arr, int rows);
void readBinaryRGB(ifstream &fin, image &file);
void readAsciiRGB(ifstream &fin, image &file);
void negate(image &file);
void grayscale(image &file);
void writeAscii(ofstream &fout, image &file, string outname, bool gray);
void writeBinary(ofstream &fout, image &file, string outname, bool gray);
void brighten(image &file, int value);
void getMinAndMax(image &file, int &min, int &max);
void contrast(image &file);
bool sharpen(image &file);
bool smooth(image &file);
#endif