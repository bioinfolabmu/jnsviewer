////////////////////////////////////////////////////////////
//  <Decleration>                                         //
//      It is a part of RNASViewer project.               //
//  <Date>                                                //
//      11/22/2013                                        //
//  <Author>                                              //
//      Xi Li                                             //
////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#include <vector>
#include <string.h>
#include <sstream>

#include <math.h>
#include <algorithm>

// Begin, GD libraries for png images.
#include "gd.h"
#include <gdfontt.h>
#include <gdfonts.h>

// Begin, Linux system libraries.
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

    using namespace std;

    // The elements of sequence in the data file
    vector<unsigned char> elements;
    // The dots & brackets in the date file.
    vector<unsigned char> dot_bracket;

    // The id of paired elements in the export file.
    vector<int> pairings;

    // The coordinate of every elements's(vertices's) lacation.
    vector<int> x_axis,y_axis;

////////////////////////////////////////////////////////////////////////////////////////////////////

void release_system_resource()
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Release the system resource                       //
    ////////////////////////////////////////////////////////////

    vector<unsigned char> ().swap(elements);
    vector<unsigned char> ().swap(dot_bracket);
    vector<int> ().swap(x_axis);
    vector<int> ().swap(y_axis);
    vector<int> ().swap(pairings);

}


vector<std::string> split( string main_str, string pattern)
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Split a main string into a series of sub string.  //
    //  <Input>                                               //
    //      main_str:                                         //
    //      pattern:                                          //
    //  <Output>                                              //
    //      result:                                           //
    ////////////////////////////////////////////////////////////

    int pos;
    vector<std::string> result;
    int size;

    main_str = main_str + pattern;
    size = main_str.size();

    for ( int i=0; i<size; ++i )
    {
        //
        pos = main_str.find( pattern, i );
        if ( pos<size )
        {
            //
            string s = main_str.substr( i, pos-i);
            result.push_back( s );
            i = pos + pattern.size() - 1;
        }
    }
    return result;

}


void process_str_sequence( string str_sequence )
{
    //////////////////////////////////////////////////////////////////////
    //  <Function>                                                      //
    //      Get the bases from str_sequence.                            //
    //  <Input>                                                         //
    //      str_sequence: //
    //  <Output>                                                        //
    //      //
    //////////////////////////////////////////////////////////////////////

    int end_n;
        end_n = str_sequence.size();
    for ( int n=0; n<end_n; ++n )
    {
        unsigned char chr;
        string str;
            str = str_sequence.substr( n, n+1 );
        chr = str[0];

        elements.push_back( chr );
    }

}


void process_str_pair_signs( string str_pair_signs )
{
    //////////////////////////////////////////////////////////////////////
    //  <Function>
    //
    //  <Input>
    //      str_pair_signs: //
    //  <Output>
    //      //
    ////////////////////////////////////////////////////////////

    int end_n;
        end_n = str_pair_signs.size();
    for ( int n=0; n<end_n; ++n )
    {
        unsigned char chr;
        string str;
            str = str_pair_signs.substr( n, n+1 );
        chr = str[0];
        dot_bracket.push_back( chr );
    }

}


void process_str_coordinate( string str_coordinate )
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //                                            //
    //  <Input>                                               //
    //      std_coordinate:                                   //
    //  <Output>                                              //
    //      //
    ////////////////////////////////////////////////////////////

    vector<string> xy;
        xy = split( str_coordinate, " ");

    int end_n = xy.size();
    for ( int n=0; n<end_n; ++n )
    {
        int pos;
            pos = xy[n].find( "," );
        int end;
            end = xy[n].size();
        string xy_first;
            xy_first = xy[n].substr( 0, pos ) ;
        string xy_second;
            xy_second =  xy[n].substr( pos+1, end );

        int i;
            i = atoi( xy_first.c_str() );
        x_axis.push_back( i );
            i = atoi( xy_second.c_str() );
        y_axis.push_back( i );

    }

    vector<string> ().swap(xy);

}


void process_str_pairid( string str_pairid )
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Get the id info from pairing info.                //
    //  <Input>                                               //
    //      str_pairid:                                       //
    //  <Output>                                              //
    //      //
    ////////////////////////////////////////////////////////////

    vector<string> pairs;
        pairs = split( str_pairid, " " );

    int end_n;
        end_n = pairs.size();
    for ( int n=0; n<end_n; ++n )
    {

        int pos;
            pos = pairs[n].find( "," );
        int end;
            end = pairs[n].size();
        string id_first;
            id_first = pairs[n].substr( 0, pos ) ;
        string id_second;
            id_second =  pairs[n].substr( pos+1, end );

        int i;
            i = atoi( id_first.c_str() );
            i = i - 1;
        pairings.push_back( i );
            i = atoi( id_second.c_str() );
            i = i - 1;
        pairings.push_back( i );

    }

}

void import_date( char* file_json )
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Import date from file in JSON format.             //
    //  <Input>                                               //
    //      file_json:                                        //
    //  <Output>                                              //
    //      elements:                                         //
    //      dot_bracket:                                      //
    //      x_axis, y_axis:                                   //
    //      pairing:                                          //
    ////////////////////////////////////////////////////////////

    ifstream ifs( file_json );

    if ( !ifs )
    {
        cout<<"Error: can not open the input file.";
        cout<<endl;
    }
    else
    {

        string textline;
        string m_str;
            m_str = "";

        // Read the data from the data file.
        while ( !ifs.eof() )
        {
            getline( ifs, textline , '\n' );

            int size;
                size = textline.size();
            if ( textline[ size-1 ] =='\r' )
            {
                textline = textline.substr( 0, size-1 );
            }

            m_str = m_str + textline;
        }

        m_str = m_str.substr( 1, ( m_str.size() )-3 ); // '{' & '"}'

        string pat = "\",";
        vector<string> re;
            re = split(m_str,pat);

        int end_n;
            end_n = re.size();

        for ( int n=0; n<end_n; ++n )
        {

            int start;
                start = re[n].find(":\"");

            int end;
                end = re[n].size();

            string h_str;
                h_str = re[n].substr( 0, start);

            string b_str;
                b_str = re[n].substr( start+2, end);

            if ( h_str == "sequence" )
            {

                process_str_sequence( b_str );

            }
            else if ( h_str == "dot_bracket" )
                {

                    process_str_pair_signs( b_str );

                }
                else if ( h_str == "coordinate" )
                    {

                        process_str_coordinate( b_str );

                    }
                    else if ( h_str == "pairings" )
                        {

                            process_str_pairid( b_str );

                        }
                        else
                        {

                            cout<<"Error!";
                            cout<<endl;

                        }

        }

    }

}


void SavePng( char *filename , gdImagePtr im)
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Save a PNG image as a PNG file.                   //
    //  <Input>                                               //
    //      im:                                               //
    //  <Output>                                              //
    //      filename:                                         //
    ////////////////////////////////////////////////////////////
	FILE *out;
	int size;
	char *data;
	out = fopen( filename , "wb");
	if (!out)
	{
        // Error
	}
	data = (char *) gdImagePngPtr(im, &size);

	if (!data)
	{
        // Error
	}

	if ( (int)fwrite(data, 1, size, out) != size)
	{
        // Error
	}

	if (fclose(out) != 0)
	{
        // Error
	}

	gdFree(data);
} // void mySavePng( )

void export_png( char* png_file )
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Draw a png image & save it as a png file.         //
    //  <Input>                                               //
    //      elements, dot_bracket, x_axis, y_axis, pairings.  //
    //  <Output>                                              //
    //      png_file:                                         //
    ////////////////////////////////////////////////////////////

    int top_margins,bottom_margins,left_margins,right_margins;
    int max_x,max_y;
    int mapend,mapheight;

    int scale;
        scale = 1;

    top_margins = bottom_margins = left_margins = right_margins = 15*scale;
    max_x = *max_element( x_axis.begin(), x_axis.end() ) ;
    mapend = (left_margins+right_margins+max_x)*scale;
    max_y = *max_element( y_axis.begin(),y_axis.end() ) ;
    mapheight = (top_margins+bottom_margins+max_y)*scale;

    gdImagePtr im;
    im=gdImageCreate(mapend,mapheight);
        int white = gdImageColorAllocate(im,255,255,255);
		int black=gdImageColorAllocate(im,0,0,0);
   		int red = gdImageColorAllocate(im,255,0,0);
		int green=gdImageColorAllocate(im,0,255,0);
		int blue=gdImageColorAllocate(im,0,0,255);

    int n;
    int end_n;

    //  Draw line in order
    end_n=elements.size();
    for ( n=1 ; n<end_n ; ++n )
    {
        gdImageLine( im, x_axis[n-1]*scale, y_axis[n-1]*scale
                    , x_axis[n]*scale, y_axis[n]*scale, blue );
    }

    //  draw line for peair
    int front,back;

    end_n = ( pairings.size() )/2;
    for ( n=0 ; n<end_n ; ++n )
    {

        front = pairings[2*n];
        back = pairings[2*n+1];
            gdImageLine( im, x_axis[ front ]*scale, y_axis[ front ]*scale
                        , x_axis[ back ]*scale, y_axis[ back ]*scale, green );
    }

    //Ouput elements & subscript
    end_n=elements.size()-1;
    for ( n=end_n ; n>=0 ; --n )
    {
        // Draw the circles.
        gdImageFilledEllipse( im, x_axis[n]*scale, y_axis[n]*scale, 18*scale, 18*scale, white );

        unsigned char *p;
        string s;
        s=elements[n];
        const char* c;
        c = s.c_str();
        p=(unsigned char*)c;

        gdImageString( im, gdFontGetSmall()
                      , (x_axis[n]-2)*scale, (y_axis[n]-6)*scale, p , black );

        if ( ((n+1)%10 == 0) || (n == 0) || (n == end_n)  )
        {
            ostringstream oss;
            string ss;
            const char *src;
            oss<<(n+1);
            ss=oss.str();
            src = ss.c_str();
            char *dest;
            dest=new char[512];
            strcpy(dest,src);
            unsigned char *p_no;
            p_no=(unsigned char *)dest;

            gdImageString( im, gdFontTiny
                          , (x_axis[n]+4)*scale, (y_axis[n]-2)*scale, p_no , black );
        }

    }

    // Export the Image
	SavePng( png_file,im);

	gdImageDestroy(im);

}


int main( int argc, char* argv[] )
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Import the data form file in SSDJ format.         //
    //      Export a png imager.                              //
    //  <Input>                                               //
    //      argv[1]: SSDJ file.                               //
    //  <Output>                                              //
    //      argv[2]: PNG File.                                //
    ////////////////////////////////////////////////////////////

    // Import the data from a file in JSON file.
    import_date( argv[1] );


    // Export the PNG imager.
    export_png( argv[2] );

    // Release the system resource
    release_system_resource();

    return 0;

}

