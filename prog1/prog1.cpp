/***************************************************************************//**
 * @file 
 *
 * @brief 'main' function (controls flow of program)
 ******************************************************************************/
#include "netPBM.h"
/***************************************************************************//**
 * @author Dillon Roller
 *
 * @par Description:
 * This is the starting point to the program.  It will get the command 
 * arguments from the user, input image file contents and call functions 
 * corresponding to those chosen by the user. 
 *
 * @param[in] argc - the number of arguments from the command prompt.
 * @param[in] argv - a 2d array of characters containing the arguments.
 *
 * @returns 1 - failed to open file
 * @returns 2 - failed to allocated memory
 * @returns 3 - invalid command line
 *
 ******************************************************************************/
int main(int argc, char **argv) {
    image inFile; //variables
    string outname;
    ifstream fin;
    ofstream fout;
    int i;
    bool gray = false;
    if (argc == 1) {
        cout << "Usage - prog1.exe [option] -o[a or b] outputname"
            << "inputname.ppm\nEnding program..." << endl;
        return 1;
    }

    inFile.name = argv[argc - 1];
    outname = argv[argc - 2];
    if (!readHeaderInfo(fin, inFile)) //ends if couldnt read file
        return 1;
    //if allocation fails for any array, free up memory and end
    if (!alloc2d(inFile.redgray, inFile.rows, inFile.cols) || 
        !alloc2d(inFile.green, inFile.rows, inFile.cols) ||
        !alloc2d(inFile.blue, inFile.rows, inFile.cols)) 
    {
        cout << "Memory error" << endl;
        return 2;
    }
    if(inFile.header == "P6") //check number and call appropriate func
        readBinaryRGB(fin, inFile);
    if (inFile.header == "P3")
        readAsciiRGB(fin, inFile);
    if (argc < 4 || argc > 6) {//error check number of arguments
        cout << "Not enough arguments...Ending program" << endl;
        return 3;
    }
    for (i = 1; i < argc - 2; i++) { //loop through command arguments
        if (argv[i][0] == '-' && argv[i][1] == 'o'){
            if (argv[i][2] == 'a')
                writeAscii(fout, inFile, outname, gray); //write ascii
            else if (argv[i][2] == 'b')
                writeBinary(fout, inFile, outname, gray);//write binary
            else {
                cout << "Invalid operation: " << argv[i][1] << argv[i][2]
                    << endl;
                return 3;
            }
        } 
        else if (argv[i][1] == 'n')//negate
            negate(inFile);
        else if (argv[i][1] == 'b') {//brighten
            brighten(inFile, stoi(argv[i + 1]));
            i++;
        } 
        else if (argv[i][1] == 'p') {//sharpen
            if (!sharpen(inFile))
                return 2;
        } 
        else if (argv[i][1] == 's') {//smooth
            if (!smooth(inFile))
                return 2;
            }
        else if (argv[i][1] == 'g') {//grayscale
            grayscale(inFile);
            gray = true;
        }
        else if (argv[i][1] == 'c'){//contrast
            contrast(inFile);
            gray = true;
        }
        else {
            cout << "Invalid operation: " << argv[i][1] << endl;
            return 3;
        }
    }
    return 0;
}