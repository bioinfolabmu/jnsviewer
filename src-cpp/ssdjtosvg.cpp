////////////////////////////////////////////////////////////
//  <Decleration>                                         //
//      It is a part of RNASViewer project.               //
//  <Date>                                                //
//      11/21/2013                                        //
//  <Author>                                              //
//      Xi Li                                             //
////////////////////////////////////////////////////////////

// Basic header file
#include <iostream>
#include <fstream>

#include <vector>
#include <string.h>
#include <sstream>

#include <math.h>
#include <algorithm>

// Begin, Linux system libraries.
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>


    using namespace std;

    // The bases of sequence in the data file
    vector<unsigned char> bases;

    // The dots & brackets in the date file.
    vector<unsigned char> dot_bracket;

    // The id of paired bases in the export file.
    vector<int> pairings;

    // The coordinate of every bases's(vertices's) lacation.
    vector<int> x_axis,y_axis;


void release_system_resource()
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Release the system resource.                      //
    ////////////////////////////////////////////////////////////

    // Begin,release the system resource.
    vector<unsigned char> ().swap(bases);
    vector<unsigned char> ().swap(dot_bracket);

    vector<int> ().swap(x_axis);
    vector<int> ().swap(y_axis);
    vector<int> ().swap(pairings);

}


std::vector<std::string> split(std::string str, std::string pattern)
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

    //std::string::size_type pos;
    int pos;
    std::vector<std::string> result;
    int size;

    str = str + pattern;
    size = str.size();

    for ( int i=0; i<size; ++i )
    {

        pos = str.find( pattern, i );
        if ( pos<size )
        {

            std::string s = str.substr( i, pos-i);
            result.push_back( s );
            i = pos + pattern.size() - 1;
        }

    }

    return result;

}


void process_str_sequence( string str_sequence )
{
    /////////////////////////////////////////////////////////////////////
    //  <Function>//
    //      //
    //  <Input>//
    //      str_sequence://
    //  <Output>//
    //      bases://
    /////////////////////////////////////////////////////////////////////

    int end_n;
        end_n = str_sequence.size();
    for ( int n=0; n<end_n; ++n )
    {

        unsigned char chr;
        string str;
            str = str_sequence.substr( n, n+1 );
        chr = str[0];

        bases.push_back( chr );

    }

}

void process_str_pair_signs( string str_pair_signs )
{
    //////////////////////////////////////////////////////////////////////
    //  <Function>                                         //
    //                                         //
    //  <Input>                                         //
    //      str_pair_signs://
    //  <Output>                                         //
    //      pair_signs://
    //////////////////////////////////////////////////////////////////////

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
    //////////////////////////////////////////////////////////////////////
    //  <Function>
    //
    //  <Input>
    //      str_coordinate://
    //  <Output>
    //      x_axis:
    //      y_axis:
    //////////////////////////////////////////////////////////////////////

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

    // Begin, Release the system resouce.
    vector<string> ().swap(xy);

}


void process_str_pairid( string str_pairid )
{
    //////////////////////////////////////////////////////////////////////
    //  <Function>                                         //
    //      //
    //  <Input>                                         //
    //      str_pairid://
    //  <Output>                                         //
    //      pair_id://
    //////////////////////////////////////////////////////////////////////

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
    //      bases:                                         //
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

        m_str = m_str.substr( 1, ( m_str.size() )-3 );

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

void export_svg( char* svg_file )
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Export a svg image.                               //
    //  <Input>                                               //
    //      bases:                                         //
    //      pair_signs:                                       //
    //      x_axis:                                           //
    //      y_axis:                                           //
    //      pairid:                                           //
    //  <Output>                                              //
    //      svg_file:                                         //
    ////////////////////////////////////////////////////////////
    ofstream ofsvg(svg_file);
    int n;
    int end_n;

    if( !ofsvg )
    {
        cout<<"Error: can't open the output file.";
        cout<<endl;
    }
    else
    {
        // File header information.
        ofsvg<<"<?xml version=\"1.0\" standalone=\"no\"?>";

        ofsvg<<endl;
        ofsvg<<endl;

        ofsvg<<"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" ";
        ofsvg<<"\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">";
        ofsvg<<endl;
        ofsvg<<endl;

        ofsvg<<"<svg width=\"100%\" height=\"100%\" version=\"1.1\" ";
        ofsvg<<"xmlns=\"http://www.w3.org/2000/svg\">";
        ofsvg<<endl;

        ////////// Draw polyline(bond) in index order. //////////
        end_n = dot_bracket.size()-1;
        for ( n=0; n<end_n; ++n )
        {
            ofsvg<<"<line x1=\""<<x_axis[n];
            ofsvg<<"\" y1=\""<<y_axis[n];
            ofsvg<<"\" x2=\""<<x_axis[n+1];
            ofsvg<<"\" y2=\""<<y_axis[n+1];
            ofsvg<<"\" ";
            ofsvg<<" style=\"stroke:rgb(00,00,99);stroke-width:2\"/>";

            ofsvg<<endl;
        }

        ////////// Draw line(bond) for pair. //////////
        end_n = pairings.size();
        for ( n=0; n<end_n; n=n+2 )
        {
            ofsvg<<"<line x1=\""<<x_axis[ pairings[n] ];
            ofsvg<<"\" y1=\""<<y_axis[ pairings[n] ];

            ofsvg<<"\" x2=\""<<x_axis[ pairings[n+1] ];
            ofsvg<<"\" y2=\""<<y_axis[ pairings[n+1] ];
            ofsvg<<"\" ";
            ofsvg<<" style=\"stroke:rgb(00,99,00);stroke-width:2\"/>";

            ofsvg<<endl;
        }

        ////////// Draw bases. //////////
        end_n = dot_bracket.size();
        for ( n=0; n<end_n; ++n )
        {
            ofsvg<<"<circle cx=\""<<x_axis[n];
            ofsvg<<"\" cy=\""<<y_axis[n];
            ofsvg<<"\" r=\""<<10;
            ofsvg<<"\" ";

            ofsvg<<" stroke=\"black\" stroke-width=\"0\" ";

            ofsvg<<"style=\"fill:white; opacity:1 \"";

            ofsvg<<"/>";
            ofsvg<<endl;

            // texts
            ofsvg<<"<text x=\""<<x_axis[n]-4;
            ofsvg<<"\" y=\""<<y_axis[n]+4;
            ofsvg<<"\" fill=\"black\">";
            ofsvg<<bases[n];
            ofsvg<<"</text>";

            ofsvg<<endl;
        }

        // End of file information.
        ofsvg<<"</svg>";

        // Release system resource.
        ofsvg.close();

    }

}


int main( int argc, char* argv[] )
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Import the data form file in JSON format.         //
    //      Export a svg image.                               //
    //  <Input>                                               //
    //      argv[1]: SSDJ file.                               //
    //  <Output>                                              //
    //      argv[2]: svg file.                                //
    ////////////////////////////////////////////////////////////

    // Import the data from SSDJ file.
    import_date( argv[1] );

    // Export the svg imager.
    export_svg( argv[2] );

    // Release system resource
    release_system_resource();

    return 0;

}

