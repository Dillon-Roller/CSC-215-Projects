/*************************************************************************//**
 * @file 
 *
 * @mainpage Program 3 - Git My Folders
 * 
 * @section course_section Course Information 
 *
 * @author Dillon Roller
 * 
 * @date 11/13/2017
 * 
 * @par Professor: 
 *         Roger Schrader
 * 
 * @par Course: 
 *         CSC215 - M001 -  1:00 pm - 2:50 pm
 * 
 * @par Location: 
 *         Minerals Building - 222
 *
 * @section program_section Program Information 
 * 
 * @details Since I have started programming in this class, most of my 
 * projects that have been created have been under git control, because 
 * we used github to upload our code to a git repository for submitting 
 * homework or to share code between our team mates for certain projects. 
 * 
 * Given a directory, this program will check if the folder is under 
 * git control or not, and depending on if it is, will output all 
 * visual studio type files to the appropriate xml file. This program 
 * then processes all folders inside this directory recursively. It does
 * this until there are no more folders to process, in which case it will 
 * drop out this directory and go onto the next folder.Folders that are not 
 * processed are: .git, .vs, x64, Debug, the self reference folder '.' and a 
 * reference to the previous directory '..' .Only files with certain extensions 
 * are to be processed. These extensions are: .sln, .cpp, .vcxproj, .h, and two
 * files that are named exactly ".gitignore" and ".gitattribute". Required 
 * lines and tags are outputted during the program to ensure that the xml is
 * formatted correctly. XML files can be viewed by opening with wordpad, but
 * it won't be formatted. To view it formatted, you can drag to a browser and
 * view it there. There are other ways to do it, but this is the easiest.
 *
 * @section compile_section Compiling and Usage 
 *
 * @par Compiling Instructions: 
 *      none
 * 
 * @par Usage: 
   @verbatim  
   c:\> prog3.exe [folder path to process] [Git file name] [Non-git file name]
   @endverbatim 
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 * 
 * @bug none
 * 
 * @todo none
 * 
 * @par Modifications and Development Timeline: 
 * <a href="https://gitlab.mcs.sdsmt.edu/7407475/csc215f17programs/commits/master
 * ">Click here for commit log. </a>
   @verbatim 
   Date          Modification 
   ------------  ------------------------------------------------------------- 
   Nov 06, 2017  Main completed, processFiles function almost done, outputXML
                 almost completed.
   Nov 07, 2017  Gitignore updated, outputXML function removed, isGit function
                 finished. Program commented, no bugs found yet.
   Nov 07, 2017  Fixed problem where subdirectories of a folder that had .git 
                 inside wasn't being outputted to git xml.
   Nov 12, 2017  Made extra functions to make code cleaner, program almost 
                 finished.
   Nov 12, 2017  Program finished
   @endverbatim
 *
 ****************************************************************************/
#include <iostream>
#include <direct.h>
#include <io.h>
#include <fstream>
#include <string>

using namespace std;

//prototypes
void processFiles( ofstream &gitxml, ofstream &nonGitxml, bool git );
bool isGit();
bool isValidFolder( string filename, _finddata_t a_file );
void outputFiles( string filename, bool git, ofstream &gitxml, 
    ofstream &nonGitxml );
void outputClosingTag( ofstream &gitxml, ofstream &nonGitxml, bool git );
void outputFolder( ofstream &gitxml, ofstream &nonGitxml, bool git,
    char *directory );

/*************************************************************************//**
 * @author Dillon Roller
 * 
 * @par Description: 
 * Controls the flow of the program. Checks that command line arguments are
 * valid and initializes variables. Handles the opening and closing of xml 
 * files and outputs required lines to each. It then changes directory to 
 * the folder that needs processing and recusively processes folders in that
 * folder. After all folders have been processed, it outputs closing tags for
 * the folders section. 
 * 
 * @param[in]   argc - Number of arguments entered through command line
 * @param[in]   argv - 2D c-style array containing arguments entered     
 * 
 * @returns 0 Program ran successful.
 * @returns 1 Invalid number of command line arguments  
 * @returns 2 Invalid file type
 * @returns 3 XML files failed to open
 * @returns 4 Unable to change into main folder to process (Doesn't exist)
 * 
 ****************************************************************************/
int main( int argc, char* argv[] )
{
    //variables
    string dirpath,
        git,
        nonGit;
    bool gitbool;
    ofstream gitxml, nonGitxml;
    //check for proper number of command line arguments (4)
    if ( argc != 4 )
    {
        cout << "Invalid number of command line arguments" << endl;
        return 1;
    }
    dirpath = argv[1];
    git = argv[2];
    nonGit = argv[3];
    gitbool = false;
    //remove backslash if there is one
    if ( dirpath[0] == '\\' )
        dirpath.erase( 0, 1 );
    //checks for valid xml file
    if ( git.find( ".xml" ) == string::npos ||
        nonGit.find( ".xml" ) == string::npos )
    {
        cout << "Invalid file type..Ending" << endl;
        return 2;
    }
    //open both xml files
    gitxml.open( git );
    nonGitxml.open( nonGit );
    //check if files opened
    if ( !gitxml || !nonGitxml )
    {
        cout << "Xml files failed to open." << endl;
        return 3;
    }
    //change directory and check if it worked
    if ( _chdir( dirpath.c_str() ) != 0 )
    {
        cout << "Unable to change to the directory: " << argv[1] << endl;
        return 4;
    }
    //output required lines to both files
    gitxml << "<?xml version=\"1.0\"?>\n<folders>" << endl;
    nonGitxml << "<?xml version=\"1.0\"?>\n<folders>" << endl;
    //process files recursively
    processFiles( gitxml, nonGitxml, gitbool );
    //output folders closing tag and close files
    gitxml << "</folders>" << endl;
    nonGitxml << "</folders>" << endl;
    gitxml.close();
    nonGitxml.close();
    return 0;
}

/*************************************************************************//**
 * @author Dillon Roller
 * 
 * @par Description: 
 * Handles the outputting of folders and appropriate files to the correct
 * xml files. If the folder is under git control, all folders and files 
 * contained in this folder will be outputted to gitxml, otherwise it goes to
 * the Non-Git xml file. If a folder is encountered, this functions makes a 
 * recursive call to itself and processes that folder, until there are no more
 * folders to process, in which case it drops out and moves onto the next 
 * folder in the outer directory, and so on. 
 * 
 * @param[in]   gitxml - file containing all folders/files under git control
 * @param[in]   nonGitxml - file containing all folders/files not under
 *                          git control
 * @param[in]   git - bool containing whether or not the current directory 
 *                    being processed is under git control
 * 
 * @returns none
 * 
 ****************************************************************************/
void processFiles( ofstream &gitxml, ofstream &nonGitxml, bool git )
{
    //variables 
    intptr_t dir_handle;
    _finddata_t a_file;
    string filename;
    char *directory;
    //check if its git if it's not already git
    if ( !git )
        git = isGit();
    
    //dynamically allocate cstring containing directory path name
    directory = _getcwd( NULL, 0 );
    
    outputFolder( gitxml, nonGitxml, git, directory );

    //output vs files to appropriate xml file
    dir_handle = _findfirst( "*.*", &a_file );
    if( dir_handle == -1 ) 
        return;
    do
    {
        if ( !( a_file.attrib & _A_SUBDIR ) )
        {
            filename = ( string )a_file.name;
            outputFiles( filename, git, gitxml, nonGitxml );
        }
    } while( _findnext( dir_handle, &a_file ) == 0 );
    _findclose( dir_handle );

    //go through all directories
    dir_handle = _findfirst( "*.*", &a_file );
    do
    {
        filename = ( string )a_file.name;
        if ( isValidFolder( filename, a_file ) )
        {
            //change directory and process this directory
            if ( _chdir( filename.c_str() ) != 0 )
            {
                cout << "Error processing: " << directory 
                    << ". Skipping this directory" << endl;
                delete[] directory;
                return;
            }
            processFiles( gitxml, nonGitxml, git );
            //change directory back after processing inner directory
            _chdir( ".." );
        }
        
    } while( _findnext( dir_handle, &a_file ) == 0 );

    outputClosingTag( gitxml, nonGitxml, git );

    _findclose( dir_handle );

    delete[] directory; //delete allocated memory
    return;
}

/*************************************************************************//**
 * @author Dillon Roller
 * 
 * @par Description: 
 * Checks if the current directory is under git control. A folder is considered 
 * to be under git control if it contains the folder ".git" inside of it, or if
 * it is a folder inside a folder that contains ".git". 
 *
 * @returns git - true if it is under git, false if not
 * 
 ****************************************************************************/
bool isGit()
{
    intptr_t dir_handle;
    _finddata_t a_file;
    bool git;

    dir_handle = _findfirst( ".git", &a_file );
    if ( dir_handle == -1 || !( a_file.attrib & _A_SUBDIR ) )
        git = false;
    else
        git = true;
    _findclose( dir_handle );
    return git;
}

/*************************************************************************//**
 * @author Dillon Roller
 * 
 * @par Description: 
 * Checks if the folder/file is a valid folder to be processed. Folders to not 
 * be processed are: .git, .vs, x64, Debug, the self reference folder '.' and
 * a reference to the previous directory '..'
 * 
 * @param[in]   filename - string containing name of the file
 * @param[in]   a_file - file handle containing information about file
 * 
 * @returns true - folder is valid
 * @returns false - folder/file is not valid
 * 
 ****************************************************************************/
bool isValidFolder( string filename, _finddata_t a_file )
{
    if ( a_file.attrib & _A_SUBDIR ) //checks if it a folder
    {
        if ( filename != ".git" && filename != ".vs" && filename != "x64" &&
            filename != "Debug" && filename != "." && filename != ".." )
        {
            return true;
        }
    }
    return false;
}

/*************************************************************************//**
 * @author Dillon Roller
 * 
 * @par Description: 
 * Goes through current directory and outputs files with appropriate extension
 * to correct xml file. These extensions are: .sln, .cpp, .vcxproj, .h, and 
 * two files that are named exactly ".gitignore" and ".gitattribute".
 * 
 * @param[in]   filename - string containing name of file
 * @param[in]   git - bool containing whether or not directory is under git
 *                    control
 * @param[in]   gitxml - file containing names of files and folders under git
 *                       control
 * @param[in]   nonGitxml - file containing names of files and folders under
 *                          git control
 * 
 * @returns none
 * 
 ****************************************************************************/
void outputFiles( string filename, bool git, ofstream &gitxml, ofstream &nonGitxml )
{
    int pos;
    bool valid = false;
    string before;
    string after;
    //output files with these extension and name only
    if ( filename.size() > 4 && ( filename.substr( filename.size() - 4 ) 
        == ".sln" || filename.substr( filename.size() - 4 ) == ".cpp" ) )
        valid = true;
    if ( filename.size() > 8 && filename.substr( filename.size() - 8 )
        == ".vcxproj" )
        valid = true;
    if ( filename.size() > 2 && filename.substr( filename.size() - 2 ) == ".h" 
        )
        valid = true;
    if ( filename == ".gitignore" || filename == ".gitattributes" )
        valid = true;

    if ( valid )
    {
        //replaces all & symbols in file name with "amp;"
        pos = 0;
        while ( ( pos = filename.find( "&", pos ) ) != string::npos )
        {
            before = filename.substr( 0, pos );
            after = filename.substr( pos + 1 );
            before += "&amp;" + after;
            pos += 5;
            filename = before;
        }
        //replaces all ' symbols in file name with "&apos;"
        pos = 0;
        while ( ( pos = filename.find( "'", pos ) ) != string::npos )
        {
            before = filename.substr( 0, pos ); 
            after = filename.substr( pos + 1 ); 
            before += "&apos;" + after; 
            pos += 6;
            filename = before;
        }
        //outputs files with appropriate attribute name
        if( git )
            gitxml << "<file name=\"" << filename << "\"/>" << endl;
        else
            nonGitxml << "<file name=\"" << filename << "\"/>" << endl;
    }
}

/*************************************************************************//**
 * @author Dillon Roller
 * 
 * @par Description: 
 * Outputs folder name to correct xml file, depending on if it is git or not, 
 * before it is processed.
 * 
 * @param[in]   gitxml - file that contains folder/file names that are under 
 *                       git control
 * @param[in]   nonGitxml - file that contains folder/file names that are not
 *                          under git control
 * @param[in]   git - bool that contains whether or not the current directory
 *                    is under git
 * @param[in]   directory - pointer to cstyle array containing the name of 
 *                          the folder that is about to be processed
 * @returns none
 * 
 ****************************************************************************/
void outputFolder( ofstream &gitxml, ofstream &nonGitxml, bool git, 
    char *directory )
{
    if ( git )
        gitxml << "<folder name=\"" << directory << "\">" << endl;
    else
        nonGitxml << "<folder name=\"" << directory << "\">" << endl;
}

/*************************************************************************//**
 * @author Dillon Roller
 * 
 * @par Description: 
 * Outputs closing tag after each folder is finished processing
 * 
 * @param[in]   gitxml - file that contains folder/file names that are under 
 *                       git control
 * @param[in]   nonGitxml - file that contains folder/file names that are not
 *                          under git control
 * @param[in]   git - bool that contains whether or not the current directory
 *                    is under git
 * 
 * @returns none
 * 
 ****************************************************************************/
void outputClosingTag( ofstream &gitxml, ofstream &nonGitxml, bool git )
{
    if( git )
        gitxml << "</folder>" << endl;
    else 
        nonGitxml << "</folder>" << endl;
}

