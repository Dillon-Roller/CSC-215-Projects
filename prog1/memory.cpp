/***************************************************************************//**
 * @file 
 *
 * @brief Handles the allocation and freeing up of memory
 ******************************************************************************/
#include "netPBM.h"
/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * This function is responsible for the dynamic allocation of memory for 
 * arrays. 
 *
 * @param[in] arr - pointer to allocate memory 
 * @param[in] rows - number of rows in picture
 * @param[in] cols - number of cols in picture
 *
 * @returns false - memory error
 *
 ******************************************************************************/
bool alloc2d( pixel** &arr, int rows, int cols ) 
{
    int i;
    arr = new (nothrow) pixel*[rows];
    if (arr == nullptr)
        return false;
    
    for (i = 0; i < rows; i++)
    {
        arr[i] = new (nothrow) pixel[cols];
        if (arr[i] == nullptr)
        {
            free2d(arr, i);
            return false;
        }
    }
    return true;
}

/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * This function frees up dynamically allocated arrays after they are done 
 * being used.
 *
 * @param[in] arr - pointer to dynamically allocated array
 * @param[in] rows - number of 1D arrays to free up in 2D array
 *
 * @returns nothing
 *
 ******************************************************************************/
void free2d(pixel** &arr, int rows)
{
    int i;
    if (arr == nullptr)
        return;

    for (i = 0; i < rows; i++)
        delete[] arr[i];
    delete[] arr;
}