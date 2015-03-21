/**
 * The aim of this tool is to catch the font
 * it's imilar to the fontcatcher utility from Antoine
 * There is a limitation :
 * All the characters must be on the same line, so use fontaligner before
 */

#include <Magick++.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <cstdlib>

#include "fontaligner.cpp"

/**
 * @author Krusty/Benediction
 * @date 25 january 2008
 * @licence GPL
 * @todo use command line arguments
 * @create classes and allow to work in all amstrad screen modes
 */
using namespace std;
using namespace Magick; 

Color lookup_colors[16];
int nb_colors = 0 ;


// Displays some help
void display_help()
{
    cout << "Font Catcher 0.2 - http://github.com/cpcsdk/ - Code by Krusty" << endl
	<<  "fontcatcher image destination charwidth fillspace" << endl
	<<  "cuts image into smaller images and encodes them for displaying as sprites/font characters on the Amstrad CPC computer" << endl
	<<  "Each caracter is charwidth pixels large. fillspace bytes are inserted between each char for memory alignment." << endl
	<<  "TODO: only works for mode 0" << endl
	<<  "Please report bugs !" << endl;
}

/**
 * Return the index of the color
 */
char getInkNumber(Color color)
{
    //search
    for (int i=0; i<nb_colors ; i++)
    {
	if (lookup_colors[i]==color)
	{
	    return i ;
	}
    }

    //not found
    lookup_colors[nb_colors++] = color ;
    assert(nb_colors <= 17);
    return nb_colors - 1 ;
}

char getByteInkLeft(char val)
{
    assert( val < 16) ; 
    char res = 0 ;

    if (val & 1) res += 1 << 7 ;
    if (val & (1 << 1 )) res += 1 << 3 ;
    if (val & (1 << 2 )) res += 1 << 5 ;
    if (val & (1 << 3 )) res += 1 << 1 ;

    return res ;
}


char getByteInkRight(char val)
{
    assert( val <16) ;
    char res = 0 ;

    if (val & 1) res += 1 << 6 ;
    if (val & (1 << 1 )) res += 1 << 2 ;
    if (val & (1 << 2 )) res += 1 << 4 ;
    if (val & (1 << 3 )) res += 1 << 0 ;

    return res ;
}


/**
 * Compute the byte to show in function of Colors
 * @param left color of the left pixel
 * @param right color of the right pixel
 * @returns byte tf cpc memory screen
 */
const char computeByte(Color left, Color right)
{
    char color_left = getInkNumber(left) ;
    char color_right = getInkNumber(right) ;


    return getByteInkLeft(color_left) | getByteInkRight(color_right) ; 
}




int main(int argc,char **argv)
{
    if(argc != 5)
    {
	display_help();
	exit(0);
    }

    //width of the char
    int block_width = atoi(argv[3]) ;

    int fill_space = atoi(argv[4]) ;

    try
    {
        //build the images
	Image toalign(argv[1]) ;
	Image source = align(toalign);
	Geometry size = source.size() ;
	ofstream destination(argv[2], std::ios_base::binary);

	int pos = 0 ;
	int x = 0 ;
	//Read the image horizontally and compute vertically
	while ( x<size.width() )
	{
	    //Read vertical block
	    for(int y=0; y<size.height() ; y++)
	    {	//Read the width of a char
		for (int delta=0 ; delta<block_width ; delta++)
		{
		    //Read the two pixels for the byte
		    Color left = source.pixelColor( x +2*delta + 0, y);
		    Color right = source.pixelColor( x + 2*delta + 1, y) ;

		    //Save result
		    char byte = computeByte(left, right) ;
		    destination.write(reinterpret_cast<char*>(&byte), sizeof(char)) ;
		}
	    }
	    for (int i=0 ; i< fill_space ; i++)
	    {
		char byte =0 ; 
		destination.write(reinterpret_cast<char*>(&byte), sizeof(char)) ;

	    }
	    x += 2*block_width ;

	}

	
	char byte = 255 ;

//	destination.write(reinterpret_cast<char*>(&byte), sizeof(char)) ;
//	destination.write(reinterpret_cast<char*>(&byte), sizeof(char)) ;
      destination.close();
    }
    catch( Exception &error_ )
    {
      cout << "Caught exception: " << error_.what() << endl;
      return 1;
    }


    cout << ">>" << nb_colors << " colors where used " ;

    return 0 ;
}
