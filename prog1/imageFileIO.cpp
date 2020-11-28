/***************************************************************************//**
 * @file 
 *
 * @brief Functions that handle the opening and closing of files
 ******************************************************************************/
#include "netPBM.h"
/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * Reads the header info of the '.ppm' file. This includes:\n
 *-Magic number(P3, P6, P2, P5) \n
 *-Comment \n
 *-Columns \n
 *-Rows \n
 *-Max number 
 *
 * @param[in] file - contains all information about image, arrays are being 
 *                   accessed in this case.
 * @param[in] fin - ifstream for input from image file.
 *
 * @returns true - no errors
 * @returns false - error opening file or invalid magic number
 *
 ******************************************************************************/
bool readHeaderInfo(ifstream &fin, image &file) 
{
    //open files in binary (can still use fstream)
    fin.open(file.name, ios::in | ios::binary); 

    if (!fin) 
    {
        cout << "File could not open." << endl;
        return false;
    }
    
    fin >> file.header;
    //handle invalid header number
    if (file.header != "P3" && file.header != "P6") 
    {
        cout << "Invalid magic number" << endl;
        return false;
    }

    fin.ignore();
    //check if there is a comment, if so, store it
    if (fin.peek() == '#')
        getline(fin, file.comment);

    //input rows cols and 
    fin >> file.cols >> file.rows >> file.max;
    fin.ignore();
    return true;
}

/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * Reads in the binary values of each pixel, one byte at a time, storing 
 * them into an array of pixels for that color.
 *
 * @param[in] file - contains all information about image, arrays are being 
 *                   accessed in this case.
 * @param[in] fin - ifstream for input from image.
 *
 * @returns nothing
 *
 ******************************************************************************/
void readBinaryRGB(ifstream &fin, image &file)
{
    int i, j;
    for (i = 0; i < file.rows; i++) 
    {
        for (j = 0; j < file.cols; j++)
        {
            fin.read((char*)&file.redgray[i][j], sizeof(pixel));
            fin.read((char*)&file.green[i][j], sizeof(pixel));
            fin.read((char*)&file.blue[i][j], sizeof(pixel));
        }
    }
    fin.close();
}

/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * Reads in the integers from an ASCII file, storing each pixel into its 
 * corresponding color array.
 *
 * @param[in] file - contains all information about image, arrays are being 
 *                   accessed in this case.
 * @param[in] fin - ifstream for input from image file.
 *
 * @returns nothing
 *
 ******************************************************************************/
void readAsciiRGB(ifstream &fin, image &file)
{
    int temp;
    int i, j;
    for (i = 0; i < file.rows; i++) 
    {
        for (j = 0; j < file.cols; j++)
        {
            fin >> temp;
            file.redgray[i][j] = (pixel)temp;
            fin >> temp;
            file.green[i][j] = (pixel)temp;
            fin >> temp;
            file.blue[i][j] = (pixel)temp;
        }
    }
    fin.close();
}

/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * Write out the pixel values to a file in ASCII. Also formats the 
 * header information based on input file. 
 *
 * @param[in] file - contains all information about image, arrays are being 
 *                   accessed in this case.
 * @param[in] fout - ofstream for output to image file
 * @param[in] outname - output file name.
 * @param[in] gray - bool that indicates whether or not it has been grayscaled
 *
 * @returns nothing
 *
 ******************************************************************************/
void writeAscii(ofstream &fout, image &file, string outname, bool gray)
{
    int i, j;
    if (gray) //makes output a .pgm file if its grayscaled
    {
        fout.open(outname + ".pgm");
        fout << "P2" << endl;
    }
    else //makes output a .ppm file if its not grayscaled
    {
        fout.open(outname + ".ppm");
        fout << "P3" << endl;
    }
    if (file.comment.size() != 0) //if there was a comment, write it out
        fout << file.comment << endl;
    
    fout << file.cols << ' ' << file.rows << endl
        << file.max << endl;

    for (i = 0; i < file.rows; i++) //write out ascii values to 
        for (j = 0; j < file.cols; j++)
        {
            if (gray)
                fout << (int)file.redgray[i][j] << '\n';
            else
            {
                fout << (int)file.redgray[i][j] << '\n'
                    << (int)file.green[i][j] << '\n'
                    << (int)file.blue[i][j] << '\n';
            }
        }
    fout.close();
    }

/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * Write out the pixel values to a file in binary. Also formats the 
 * header information based on input file. 
 *
 * @param[in] file - contains all information about image, arrays are being 
 *                   accessed in this case.
 * @param[in] fout - ofstream for output to image file
 * @param[in] outname - output file name.
 * @param[in] gray - bool that indicates whether or not it has been grayscaled
 *
 * @returns nothing
 *
 ******************************************************************************/
void writeBinary(ofstream &fout, image &file, string outname, bool gray)
{
    int i, j;
    if (gray)
    {
        fout.open(outname + ".pgm", ios::out | ios::trunc | ios::binary);
        fout << "P5" << endl;
    }
    else
    {
        fout.open(outname + ".ppm", ios::out | ios::trunc | ios::binary);
        fout << "P6" << endl;
    }
    if (file.comment.size() != 0)
        fout << file.comment << endl;
    
    fout << file.cols << ' ' << file.rows << endl
        << file.max << endl;

    for (i = 0; i < file.rows; i++)
        for (j = 0; j < file.cols; j++)
        {
            if (gray)
                fout.write((char*)&file.redgray[i][j], sizeof(pixel));
            else
            {
                fout.write((char*)&file.redgray[i][j], sizeof(pixel));
                fout.write((char*)&file.green[i][j], sizeof(pixel));
                fout.write((char*)&file.blue[i][j], sizeof(pixel));
            }
        }
    fout.close();
}
    

