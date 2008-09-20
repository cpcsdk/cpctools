/**
 * Input : source image containing font
 * Output : image with all caracters in one line
 */
#include <Magick++.h>
#include <iostream>

/**
 * @author Krusty/Benediction
 * @date 25 january 2008
 * @licence GPL
 *
 * @todo Use command line args for size configuration
 * @todo Cleanup the code
 */

//Height of the resulting image
#define	    RESULT_HEIGHT	CHAR_HEIGHT


using namespace std;
using namespace Magick; 

Image align(Image source, int CHAR_HEIGHT =16, int CHAR_HEIGHT_SPACE = 0)
{
    //Initialisaitons
    int	source_x, source_y, dest_x, dest_y ;
    
    source_y = source_x = dest_y = dest_x = 0 ;
    try{
	//Reading of source image
	Geometry source_size = source.size() ;
	cout << "Source\n" ;
	cout << "------\n" ;
	cout << "Width  :\t" << source_size.width() << "\nHeight :\t" << source_size.height() << "\n\n" ;

	int NB_CHAR_LINES = source_size.height() / (CHAR_HEIGHT + CHAR_HEIGHT_SPACE);
	int RESULT_WIDTH = NB_CHAR_LINES * source_size.width() / source_size.height();

	//Size of destination image
	Image destination(Geometry(source_size.width()*NB_CHAR_LINES, CHAR_HEIGHT), "white");
	Geometry destination_size = destination.size() ;
	cout << "Destination\n" ;
	cout << "------\n" ;
	cout << "Width  :\t" << destination_size.width() << "\nHeight :\t" << destination_size.height() << "\n\n" ;


	//Vertical iteration of caracters
	do{
	    //begin to write at line 0
	    dest_y = 0 ;

	    //Horizontal iteration of pixels
	    for( source_x = 0; source_x < source_size.width() ; source_x++)
	    {
		//Vertical iteration fore each pixels
		for(dest_y = 0 ; dest_y < RESULT_HEIGHT ; dest_y++)
		{
		    Color couleur = source.pixelColor(source_x, source_y + dest_y) ;
		    destination.pixelColor(dest_x + source_x, dest_y, couleur) ;
		    
		}
	    }



	    //compute next line of reading
	    source_y += CHAR_HEIGHT + CHAR_HEIGHT_SPACE ;
	    dest_x += source_size.width() ;
	}while(source_y < source_size.height());

	return destination;
    }
    catch( Exception &error_ )
    {
      cout << "Caught exception: " << error_.what() << endl;
      return NULL;
    }

}
