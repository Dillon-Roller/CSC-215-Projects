/***************************************************************************//**
 * @file 
 *
 * @brief Functions that do the image operations
 ******************************************************************************/
#include "netPBM.h"

/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * Negates the image
 *
 * @param[in] file - contains all information about image, arrays are being 
 *                   accessed in this case.
 *
 * @returns nothing
 *
 ******************************************************************************/
void negate(image &file)
{
    int i, j;
    for (i = 0; i < file.rows; i++)
        for (j = 0; j < file.cols; j++)
        {
            file.redgray[i][j] = 255 - file.redgray[i][j];
            file.green[i][j] = 255 - file.green[i][j];
            file.blue[i][j] = 255 - file.blue[i][j];
        }
}

/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * Grayscales the image
 *
 * @param[in] file - contains all information about image, arrays are being 
 *                   accessed in this case.
 *
 * @returns nothing
 *
 ******************************************************************************/
void grayscale(image &file)
{
    int i, j;
    for (i = 0; i < file.rows; i++)
        for (j = 0; j < file.cols; j++)
        {
            file.redgray[i][j] = (pixel)(.3 * (int)file.redgray[i][j] +
                .6 * (int)file.green[i][j] + .1 * (int)file.blue[i][j]);
        }
}

/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * Brightens image based on numerical value inputted. Value is added to each
 * pixel in the image.
 *
 * @param[in] file - contains all information about image, arrays are being 
 *                   accessed in this case.
 * @param[in] value - value to be added to each pixel
 *
 * @returns nothing
 *
 ******************************************************************************/
void brighten(image &file, int value)
{
    int i, j;
    for (i = 0; i < file.rows; i++)
        for (j = 0; j < file.cols; j++)
        {
            //check if the value exceeds 255 or goes under 0
            if ((int)file.redgray[i][j] + value > 255)
                file.redgray[i][j] = (pixel)255;
            else if ((int)file.redgray[i][j] + value < 0)
                file.redgray[i][j] = (pixel)0;
            else
                file.redgray[i][j] += (pixel)value; //read into array

            if ((int)file.green[i][j] + value > 255)
                file.green[i][j] = (pixel)255;
            else if ((int)file.green[i][j] + value < 0)
                file.green[i][j] = (pixel)0;
            else
                file.green[i][j] += (pixel)value;

            if ((int)file.blue[i][j] + value > 255)
                file.blue[i][j] = (pixel)255;
            else if ((int)file.blue[i][j] + value < 0)
                file.blue[i][j] = (pixel)0;
            else
                file.blue[i][j] += (pixel)value;
        }
}

/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * Finds min and max pixel values inside the grayscaled image.
 *
 * @param[in] file - contains all information about image, arrays are being 
 *                   accessed in this case.
 * @param[in] min - stores min pixel value.
 * @param[in] max - stores max pixel value.
 *
 * @returns nothing
 *
 ******************************************************************************/
void getMinAndMax(image &file, int &min, int &max)
{
    int i, j;
    min = file.redgray[0][0]; //set min and max to the first element in array
    max = file.redgray[0][0];
    for (i = 0; i < file.rows; i++)
        for (j = 0; j < file.cols; j++)
        {
            if (file.redgray[i][j] < min)
                min = file.redgray[i][j];
            if (file.redgray[i][j] > max)
                max = file.redgray[i][j];
        }
}
/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * Contrasts the image: The original image is converted to grayscale and then
 * its grayscale values are scaled again based on a formula we were given.
 *
 * @param[in] file - contains all information about image, arrays are being 
 *                   accessed in this case.
 *
 * @returns nothing
 *
 ******************************************************************************/
void contrast(image &file)
{
    int i, j;
    int min, max;//local variables for this function
    double scale;
    
    grayscale(file);
    getMinAndMax(file, min, max);

    scale = 255.0 / (double)(max - min); //scale for 
    scale = floor(scale + 0.5);  //round

    for (i = 0; i < file.rows; i++)
        for (j = 0; j < file.cols; j++)
        {
            file.redgray[i][j] = (int)scale * (file.redgray[i][j] - min);
        }
}

/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * Sharpens the image. This is done by subtracting the adjacent pixel value of 
 * each color from 5 times the pixel value of the pixel you're at. It is then
 * stored into a new array which will replace the old array. The memory is then
 * cleaned up.
 *
 * @param[in] file - contains all information about image, arrays are being 
 *                   accessed in this case.
 *
 * @returns true - memory allocated
 * @returns false - memory allocation failed
 *
 ******************************************************************************/
bool sharpen(image &file)
{
    int i, j;
    int r, g, b;

    if (!alloc2d(file.newred, file.rows, file.cols) ||
        !alloc2d(file.newgreen, file.rows, file.cols) ||
        !alloc2d(file.newblue, file.rows, file.cols))
    {
        return false;
    }

    for (i = 0; i < file.rows; i++) //fill with 0
    {
        for (j = 0; j < file.cols; j++)
        {
            file.newred[i][j] = (pixel)0;
            file.newgreen[i][j] = (pixel)0;
            file.newblue[i][j] = (pixel)0;
        }
    }
    //sharpen new array based on values in oldarray
    for (i = 1; i < file.rows - 1; i++) 
    {
        for (j = 1; j < file.cols - 1; j++)
        {
            
            r = 5 * file.redgray[i][j] - file.redgray[i][j - 1] -
                file.redgray[i + 1][j] - file.redgray[i][j + 1] -
                file.redgray[i - 1][j];

            g = 5 * file.green[i][j] - file.green[i][j - 1] -
                file.green[i + 1][j] - file.green[i][j + 1] -
                file.green[i - 1][j];

            b = 5 * file.blue[i][j] - file.blue[i][j - 1] -
                file.blue[i + 1][j] - file.blue[i][j + 1] -
                file.blue[i - 1][j];

            if (r > 255)
                r = 255;
            else if (r < 0)
                r = 0;
            file.newred[i][j] = (pixel)r;

            if (g > 255)
                g = 255;
            else if (g < 0)
                g = 0;
            file.newgreen[i][j] = (pixel)g;

            if (b > 255)
                b = 255;
            else if (b < 0)
                b = 0;
            file.newblue[i][j] = (pixel)b;
        }
    }
    //copy back to old array and freeup newarray
    for (i = 0; i < file.rows; i++) 
    {
        for (j = 0; j < file.cols; j++)
        {
            file.redgray[i][j] = file.newred[i][j];
            file.green[i][j] = file.newgreen[i][j];
            file.blue[i][j] = file.newblue[i][j];
        }
    }
    free2d(file.newred, file.rows);
    free2d(file.newgreen, file.rows);
    free2d(file.newblue, file.rows);
    return true;
}

/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * Smoothens the image. Does a manipulation similar to that of sharpen.
 *
 * @param[in] file - contains all information about image, arrays are being 
 *                   accessed in this case.
 *
 * @returns true - memory allocated
 * @returns false - memory allocation failed.
 *
 ******************************************************************************/
bool smooth(image &file)
{
    int i, j;
    double r, g, b;
    if (!alloc2d(file.newred, file.rows, file.cols) ||
        !alloc2d(file.newgreen, file.rows, file.cols) ||
        !alloc2d(file.newblue, file.rows, file.cols)) 
    {
        return false;
    }
    
    for (i = 0; i < file.rows; i++)
        for (j = 0; j < file.cols; j++)
        {
            file.newred[i][j] = (pixel)0;
            file.newblue[i][j] = (pixel)0;
            file.newgreen[i][j] = (pixel)0;
        }

    for (i = 1; i < file.rows - 1; i++)
        for (j = 1; j < file.cols - 1; j++)
        {
            r = ((double)file.redgray[i - 1][j - 1] + file.redgray[i - 1][j] +
                file.redgray[i - 1][j + 1] + file.redgray[i][j + 1] +
                file.redgray[i + 1][j + 1] + file.redgray[i + 1][j] +
                file.redgray[i + 1][j - 1] + file.redgray[i][j - 1]) / 9.0;

            g = ((double)file.green[i - 1][j - 1] + file.green[i - 1][j] +
                file.green[i - 1][j + 1] + file.green[i][j + 1] +
                file.green[i + 1][j + 1] + file.green[i + 1][j] +
                file.green[i + 1][j - 1] + file.green[i][j - 1]) / 9.0;

            b = ((double)file.blue[i - 1][j - 1] + file.blue[i - 1][j] +
                file.blue[i - 1][j + 1] + file.blue[i][j + 1] +
                file.blue[i + 1][j + 1] + file.blue[i + 1][j] +
                file.blue[i + 1][j - 1] + file.blue[i][j - 1]) / 9.0;
            //int cast to round
            r += 0.5;
            g += 0.5;
            b += 0.5;
            r = (int)r;
            g = (int)g;
            b = (int)b;

            file.newred[i][j] = (pixel)r;
            file.newgreen[i][j] = (pixel)g;
            file.newblue[i][j] = (pixel)b;
        }
    for (i = 0; i < file.rows; i++)
    {
        for (j = 0; j < file.cols; j++)
        {
            file.redgray[i][j] = file.newred[i][j];
            file.green[i][j] = file.newgreen[i][j];
            file.blue[i][j] = file.newblue[i][j];
        }
    }
    free2d(file.newred, file.rows);
    free2d(file.newgreen, file.rows);
    free2d(file.newblue, file.rows);
    return true;
}




