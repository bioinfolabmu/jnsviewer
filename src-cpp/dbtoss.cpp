////////////////////////////////////////////////////////////
//  <Decleration>                                         //
//      It is a part of RNASViewer project.               //
//  <Date>                                                //
//      01/11/2014                                        //
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

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

    using namespace std;

    // circumference ratio
    #define PI 3.1415926

    // The bases of sequence in the data file
    vector<unsigned char> bases;

    // The dots & brackets in the date file.
    vector<unsigned char> dot_bracket;

    // The coordinate of every bases's(vertices's) lacation.
    vector<int> x_axis,y_axis;

    // The id of two paired bases in the export file.
    vector<int> pairings;

    // The "polygons" store the index of every bases(dot & brackets).
    vector<vector<int> > polygons;

    // The infor of each motif.
    vector<vector<int> > motifs;
    vector<unsigned char> motif_type;
    vector<vector<int> > motif_points;

    // Union Length(per union length)
    int length;


void release_system_resource()
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Release all of the vector variable.               //
    ////////////////////////////////////////////////////////////

    vector<unsigned char> ().swap( bases );
    vector<unsigned char> ().swap( dot_bracket );

    vector<vector<int> > ().swap( motifs );
    vector<unsigned char> ().swap( motif_type );
    vector<vector<int> > ().swap( motif_points );

    vector<int> ().swap( pairings );

    vector<int> ().swap( x_axis );
    vector<int> ().swap( y_axis );

}


void initialise_coordinate(  )
{
    ////////////////////////////////////////////////////////////////////////////////
    //  <Function>                                                                //
    //      Initialise the coordinate(x_axis,y_axis).                             //
    //  <Input>                                                                   //
    //      x_axis:                                                               //
    //      y_axis:                                                               //
    //  <Output>                                                                  //
    //      x_axis:                                                               //
    //      y_axis:                                                               //
    ////////////////////////////////////////////////////////////////////////////////

    int n;
    int end_n;
        end_n=bases.size();

    for ( n=0 ; n<end_n ; ++n )
    {
        x_axis.push_back(0);
        y_axis.push_back(0);
    }

    x_axis[end_n-1] = length/2;
    y_axis[end_n-1] = length/2;
    x_axis[0] = -length/2;
    y_axis[0] = length/2;

}


vector<string> split( string str, string pattern, bool not_empty)
{
    ////////////////////////////////////////////////////////////////////////////////
    //  <Function>                                                                //
    //      Split the main string into sever sub string                           //
    //  <Input>                                                                   //
    //      str: Main string.                                                     //
    //      pattern: Separator.                                                   //
    //      not_null: The empty strings would (not) allowed to return.            //
    //  <Output>                                                                  //
    //      result:                                                               //
    ////////////////////////////////////////////////////////////////////////////////

    int pos;
    vector<string> result;
        str += pattern;

    int size = str.size();

    for ( int i=0; i<size; ++i )
    {

        pos = str.find( pattern, i );
        if ( pos<size )
        {
            string s = str.substr( i, pos-i);

            if ( not_empty )
            {
                if ( s.size() >0 )
                {
                    result.push_back( s );
                }
            }
            else
            {
                result.push_back( s );
            }

            i = pos + pattern.size() - 1;
        }

    }

    return result;

    vector<string>().swap( result );

}


void import_file(char* input_file)
{
    //////////////////////////////////////////////////////
    //  <Function>                                      //
    //      Import the data form the data file.         //
    //      There are 3 parts of data in file.          //
    //      1:filename;                                 //
    //      2:sequence;                                 //
    //      3:dot brackets                              //
    //  <Input>                                         //
    //      input_file: the name of import data file.   //
    //  <Output>                                        //
    //      bases:                                      //
    //      pair_signs:                                 //
    //////////////////////////////////////////////////////

    vector<string> strings;
    string separater;

    ifstream ifs(input_file);

    if ( !ifs )
    {
        cout<<"Error: can not open the input file.";
        cout<<endl;
    }
    else
    {

        string textline;
        int n;
        int end_n;

        // Ignore the 1st line ( Description of this sequence ).
        getline( ifs, textline , '\n' );

        // Get the bases from 2nd line.( the bases of this sequence )
        getline( ifs, textline , '\n' );

        end_n = textline.length();

        if ( textline[ end_n-1 ] =='\r' )
        {
            end_n = end_n - 1;
        }

        separater = " ";
        strings = split(textline, separater, true);

        end_n = strings.size();
        for ( n=0; n<end_n; ++n )
        {
            unsigned char ch;

            int m;
            int end_m;

            end_m = strings[n].size();

                for ( m=0; m<end_m; ++m )
                {
                    ch = strings[n][m];
                    ch = toupper(ch);

                    if ( ch=='A' || ch=='U' || ch=='G' || ch=='C' || ch=='T' || ch=='N' )
                    {
                        bases.push_back(ch);
                    }

                }

        }

        vector<int> ().swap( x_axis );

        // Get the pair signs(dot brackets) from 3th line.
        getline( ifs, textline , '\n' );

        end_n = textline.length();

        if ( textline[ end_n-1 ] =='\r' )
        {
            end_n = end_n - 1;
        }

        separater = " ";
        strings = split(textline, separater, true);

        end_n = strings.size();
        for ( n=0; n<end_n; ++n )
        {
            unsigned char ch;

            int m;
            int end_m;
                end_m = strings[n].size();

                if  (   ( strings[n][0]=='.' || strings[n][0]=='(' || strings[n][0]==')' )
                        && (strings[n][1]=='.' || strings[n][1]=='(' || strings[n][1]==')' )
                    )
                {
                    end_m = strings[n].size();

                    for ( m=0; m<end_m; ++m )
                    {
                        ch = strings[n][m];
                        dot_bracket.push_back(ch);
                    }

                }

        }

    }

    // Release system resource.
    ifs.close();
    vector<string>().swap( strings );

}


void insert_shadow()
{
    //////////////////////////////////////////////////////////////////////
    //  <Function>                                                      //
    //    Insert a show element("S") into bases in the last.         //
    //    In the mean time, insert a dot('.') into dot_bracket.         //
    //    To make the first element distance from the last one.         //
    //  <Input>                                                         //
    //      bases:                                                      //
    //      dat_bracket:                                                //
    //  <Output>                                                        //
    //      bases:                                                      //
    //      dat_bracket:                                                //
    //////////////////////////////////////////////////////////////////////

        dot_bracket.push_back('.');
        bases.push_back('S');

}


void group_vertexes(  )
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //  <Function>                                                                                    //
    //      According to the pair_signs(dots & brackets),                                             //
    //      Dived the bases(vertices) into several group(polygon).                                    //
    //  <Input>                                                                                       //
    //      pair_signs:                                                                               //
    //  <Output> polygons->motifs                                                                     //
    //      motifs: Push the index of bases(vertexes) into it in descending numerical order.          //
    //      (e.g.) <front>99, 98 97, 96, 9, 8, 0<back>.                                               //
    //      pair_id: Push the index of paired bases(vertexes) into it.                                //
    //  <Processing>                                                                                  //
    //      Use the STACK for pairing the brackets.                                                   //
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    vector<int> motif;
    vector<unsigned char> signs(dot_bracket);
    vector<int> indexs; 

    int s_index;
    int s_index_end;

    int n;
    int n_end;

    unsigned char ch;
    int i;

    s_index_end=signs.size()-1;

    for( s_index=0 ; s_index<=s_index_end ; ++s_index )
    {
        ch=signs[s_index];
        if ( s_index==s_index_end )
        {
            motif.push_back(s_index);

            n_end=indexs.size()-1;

            for ( n=n_end ; n>=0 ; --n )
            {
                motif.push_back( indexs.back() );
                indexs.pop_back();
            }

            // Store the motif into motifs
            motifs.push_back(motif);

            if ( ch ==')' )
            {
                pairings.push_back( motif.back() );
                pairings.push_back( motif.front() );
            }

            motif.clear();
        }
        else
        {
            if ( ch!=')' )
            {
                indexs.push_back(s_index);
            }
            else
            {
                motif.push_back(s_index);
                i=indexs.back();
                while(  signs[i]!='(' )
                {
                    motif.push_back( indexs.back() );
                    indexs.pop_back();
                    i=indexs.back();
                }
                motif.push_back( indexs.back() );

                signs[indexs.back()]='.';
                indexs.push_back(s_index);

                signs[indexs.back()]='.';

                motifs.push_back(motif);

                pairings.push_back( motif.back() );
                pairings.push_back( motif.front() );

                motif.clear();
            }
        }
    }

    // Release the system resource.
    vector<int> ().swap(motif);
    vector<unsigned char> ().swap(signs);
    vector<int> ().swap(indexs);

}


void class_motifs()
{
    ////////////////////////////////////////////////////////////////
    //  <Function>                                                //
    //      Divide polygons into diffrent tpye.                   //
    //      "S":stem                                              //
    //      "B":bulge                                             //
    //      "H":hairpin                                           //
    //      "I":interior loop                                     //
    //      "M":multibranch loop                                  //
    //  <Input>                                                   //
    //      motifs: vector<vector<int> > motifs;                  //
    //  <Output>                                                  //
    //      motif_type: vector<unsigned char> motif_type;         //
    //      motif_points: vector<vector<int> > motif_points;      //
    ////////////////////////////////////////////////////////////////

    int n;
    int end_n;
    int m;
    int end_m;

    vector<int> points;

    unsigned char type;

    type = 'N';

    end_n = motifs.size();
    for ( n=0; n<end_n; ++n )
    {

        // Clear the points.
        points.clear();

        if ( motifs[n].size() == 4 )
        {
            type = 'S';
            // process the 'S'.
        }
        else
        {
            // Store the front point into the points.
            points.push_back( motifs[n].front() );

            end_m = motifs[n].size() - 1;

            for ( m=0; m<end_m; ++m )
            {

                int step = abs ( motifs[n][m] - motifs[n][m+1] );

                if ( step>1 )
                {
                    // Store the points beside the gap into the points.
                    points.push_back(motifs[n][m]);
                    points.push_back(motifs[n][m+1]);
                }

            }

            // Store the back point into the points.
            points.push_back( motifs[n].back() );

            if ( ( points.front() == (dot_bracket.size()-1) ) && (points.back() == 0) )
            {

                switch ( points.size() )
                {
                    case 4:
                    { // 'H'
                        type = 'H';
                        // process the 'H'.

                        break;
                    }

                    case 6:
                    { // 'B' OR 'I'
                        int side_front;
                            side_front = points[0] - points[1];
                        int side_back;
                            side_back = points[4] - points[5];
                        int side_first;
                            side_first = side_front + side_back + 1;

                        int side_second;
                            side_second = points[2] - points[3];

                        if ( (side_first == 1) || (side_second == 1) )
                        { // 'B'
                            type = 'B';
                            // process the 'B'.
                        }
                        else
                        { // 'I'
                            type = 'I';
                            // process the 'I'.
                        }

                        break;
                    }

                    default:
                    {
                        type = 'M';
                        // process the 'M'.
                    }

                }

            }
            else
            {

                switch ( points.size() )
                {
                    case 2:
                    { // 'H'
                        type = 'H';
                        // process the 'H'.

                        break;
                    }

                    case 4:
                    { // 'B' & 'I'
                        int side_first;
                            side_first = points[0] - points[1];

                        int side_second;
                            side_second = points[2] - points[3];

                        if ( (side_first == 1) || (side_second == 1) )
                        { // 'B'
                            type = 'B';
                            // process the 'B'.
                        }
                        else
                        { // 'I'
                            type = 'I';
                            // process the 'I'.
                        }

                        break;
                    }

                    default:
                    { // 'M'
                        type = 'M';
                        // process the 'M'.
                    }

                }

            }

        }

        // Store(push_back) the type into motif_type.
        motif_type.push_back(type);

        // Store the points into motif_points.
        motif_points.push_back(points);

    }

    // Release the sysytem resource.
    vector<int> ().swap( points );

}


void process_motifs()
{
    ////////////////////////////////////////////////////////////////////////////////
    //  <Function>                                                                //
    //      According to the motif_tyoe, process every motif.                     //
    //  <Input>                                                                   //
    //      motifs:                                                               //
    //  <Output>                                                                  //
    //      polygons:                                                             //
    ////////////////////////////////////////////////////////////////////////////////

    int n;
    int end_n;
    int m;

    end_n = motifs.size();
    for ( n=0; n<end_n; ++n )
    {
        vector<int> polygon;

        switch ( motif_type[n] )
        {
            case 'S':
            {
                polygons.push_back( motifs[n] );

                break;
            }

            case 'H':
            {
                polygons.push_back( motifs[n] );

                break;
            }

            case 'B':
            {
                if ( motif_points[n].size() == 4 )
                {

                    int side_first;
                        side_first = ( motif_points[n][0] - motif_points[n][1] );
                    int side_second;
                        side_second = ( motif_points[n][2] - motif_points[n][3] );

                    if ( side_first > side_second )
                    {

                        for( m=motif_points[n][0]; m>=motif_points[n][1]; --m )
                        {
                            polygon.push_back( m );
                        }

                    }
                    else
                    {

                        for( m=motif_points[n][2]; m>=motif_points[n][3]; --m )
                        {
                            polygon.push_back( m );
                        }

                    }
                    polygons.push_back( polygon );
                    polygon.clear();

                    polygons.push_back( motif_points[n] );
                    polygon.clear();
                }
                else
                {

                    int side_first;
                    int side_front;
                    int side_back;
                        side_front = ( motif_points[n][0] - motif_points[n][1] );
                        side_back = ( motif_points[n][4] - motif_points[n][5] );
                        side_first = ( side_front + side_back ) + 1;

                    int side_second;
                        side_second = ( motif_points[n][2] - motif_points[n][3] );

                    if ( side_first > side_second )
                    {
                        // Order: 1)motif_points[n](processed),
                        // 2)side_first( 3.1)side_front 3.2)side_second ).

                        // motif_points[n](processed)
                        for ( m=1; m<=4; ++m )
                        { // P1, P2, P3, P4
                            polygon.push_back( motif_points[n][m] );
                        }
                        polygons.push_back( polygon );
                        polygon.clear();

                        for ( m=motif_points[n][0]; m>=motif_points[n][1]; --m )
                        {
                            polygon.push_back( m );
                        }

                        for ( m=motif_points[n][4]; m>=motif_points[n][5]; --m )
                        {
                            polygon.push_back( m );
                        }

                        polygons.push_back( polygon );
                        polygon.clear();
                    }
                    else
                    { // side_first < side_second // (side_first==1)
                        // Order: 1)side_second; 2)motif_points[n].
                        for ( m=motif_points[n][2]; m>=motif_points[n][3]; --m)
                        {
                            polygon.push_back( m );
                        }

                        polygons.push_back( polygon );
                        polygon.clear();

                        // motif_points[n](processed)
                        for ( m=1; m<=4; ++m )
                        { // P1, P2, P3, P4
                            polygon.push_back( motif_points[n][m] );
                        }
                        polygons.push_back( polygon );
                        polygon.clear();
                    }

                }

                break;
            }

            case 'I':
            {

                if ( motif_points[n].size() == 4 )
                {
                    for ( m=motif_points[n][0]; m>=motif_points[n][1]; --m )
                    {
                        polygon.push_back( m );
                    }
                    polygons.push_back( polygon );
                    polygon.clear();

                    for ( m=motif_points[n][2]; m>=motif_points[n][3]; --m )
                    {
                        polygon.push_back( m );
                    }
                    polygons.push_back( polygon );
                    polygon.clear();

                    polygons.push_back( motif_points[n] );
                    polygon.clear();
                }
                else
                { // ( motif_points[n].size() == 6 )
                    // Order: 1)side_second, 2)motif_points[n],
                    // 3)side_first( 3.1)side_front 3.2)side_second ).
                    for ( m=motif_points[n][2]; m>=motif_points[n][3]; --m)
                    {
                        //
                        polygon.push_back(m);
                    }
                    polygons.push_back( polygon );
                    polygon.clear();

                    for ( m=1; m<=4; ++m )
                    { // P1, P2, P3, P4
                        polygon.push_back( motif_points[n][m] );
                    }
                    polygons.push_back( polygon );
                    polygon.clear();

                    // side_first
                    for ( m=motif_points[n][0]; m>=motif_points[n][1]; --m )
                    {
                        polygon.push_back(m);
                    }

                    // side_second
                    for ( m=motif_points[n][4]; m>=motif_points[n][5]; --m )
                    {
                        polygon.push_back(m);
                    }
                    polygons.push_back( polygon );
                    polygon.clear();

                }

                break;
            }

            case 'M':
            {
                polygons.push_back( motifs[n] );

                break;
            }

            default :
            {
                cout<<"Error: wrong type of motif!";
            }

        }

        // Release the system resource.
        vector<int> ().swap( polygon );

    }

}


void delete_shadow()
{
    ////////////////////////////////////////////////////////////////////////////////
    //  <Function>                                                                //
    //    Delete the shadow element('S') from bases.                           //
    //    In the meanwhile, Delete the last dot('.') from the pair_signs.         //
    //  <Input>                                                                   //
    //      bases:                                                             //
    //      dot_bracket:                                                          //
    //  <Output>                                                                  //
    //      bases:                                                             //
    //      dot_bracket:                                                          //
    ////////////////////////////////////////////////////////////////////////////////

    if ( bases.back()=='S' )
    {
        bases.pop_back();
        dot_bracket.pop_back();
    }

}


void export_file( char* output_file )
{
    //////////////////////////////////////////////////////////////////////////
    //  <Function>                                                          //
    //      Export the data to a file in json format.                       //
    //  <Input>                                                             //
    //      bases:                                                       //
    //      pair_signs:                                                     //
    //      x_axis:                                                         //
    //      y_axis:                                                         //
    //  <Output>                                                            //
    //      output_file: the name of export file with 5 parts.              //
    //      (1) bases                                                    //
    //      (2) pair_signs                                                  //
    //      (3) coordinates(x_axis,y_axis)                                  //
    //      (4) Color                                                       //
    //      (5) pairid(pair_id)                                             //
    //////////////////////////////////////////////////////////////////////////

    ofstream ofs(output_file);
    int n;
    int end_n;

    if( !ofs )
    { // Error in open file.
        cout<<"Error: can't open the output file.";
        cout<<endl;
        //return 1;
    }
    else
    {

        // Expor an begin sign.
        ofs<<"{"; // The begin of json string.

        //cout<<"Output_file is opened.";
        end_n=bases.size();

        // Export the bases to the output_file
        ofs<<"sequence:"<<"\"";
        for ( n=0 ; n<end_n ; ++n )
        {
            ofs<<bases[n];
            //ofs<<" ";
        }

        ofs<<"\"";

        // Begin, Export the pair_signs to the output_file
        ofs<<",";
        ofs<<"dot_bracket:"<<"\"";

        for ( n=0 ; n<end_n ; ++n )
        {
            ofs<<dot_bracket[n];
        }

        ofs<<"\"";

        // Export the coordinates of every vectors to the output_file
        ofs<<",";
        ofs<<"coordinate:"<<"\"";

        for ( n=0 ; n<end_n-1 ; ++n )
        {
            //cout<<x_axis[n]<<","<<y_axis[n]<<" ";
            ofs<<x_axis[n]<<","<<y_axis[n];
            ofs<<" ";
        }

        ofs<<x_axis[end_n-1]<<","<<y_axis[end_n-1];
        ofs<<"\"";

        // Export the color of every bases
        ofs<<",";
        ofs<<"color:"<<"\"";

        for ( n=0 ; n<end_n ; ++n )
        {
            string color;
            switch ( toupper(bases[n]) )
            {
                case 'A':
                { // 'A'/'a'
                    color = "#FF0000";
                    break;
                }

                case 'T':
                { // 'T'/'t'

                }

                case 'U':
                { // 'U'/'u'
                    color = "#FFFF00";
                    break;
                }

                case 'G':
                { // 'G'/'g'
                    color = "#00FFFF";
                    break;
                }

                case 'C':
                { // 'C'/'c'
                    color = "#28FF28";
                    break;
                }

                case 'N':
                { // 'N'/'n'
                    color = "#00FF00";
                    break;
                }

                default:
                { //
                    color = "#DDDDDD";
                }

            }

            ofs<<color;

            if ( n != (end_n-1) )
            {
                ofs<<" ";
            }

        }

        ofs<<"\"";

        // Export the pairedid
        ofs<<",";
        ofs<<"pairings:"<<"\"";

        end_n = pairings.size()/2;
        if ( end_n>0 )
        {
            for ( n=0 ; n<end_n-1 ; ++n )
            {
                ofs<<pairings[2*n]+1<<","<<pairings[2*n+1]+1;
                ofs<<" ";
            }

            ofs<<pairings[2*n]+1<<","<<pairings[2*n+1]+1;
        } // if ( end_n>0 )

        ofs<<"\"";

        // Export an end sign.
        ofs<<"}";

        // Release system resource.
        ofs.close();

    }

}


void generate_polygon( int n , vector<double> &polygon_x , vector<double> &polygon_y )
{
    //////////////////////////////////////////////////////////////////////////
    //  <Function>                                                          //
    //      Generate a polygon. Vertices 1 through n define this polygon.   //
    //      Return the coordinates.                                         //
    //  <Input>                                                             //
    //      n: This polygon with n sides.                                   //
    //  <Output>                                                            //
    //      polygon_x:                                                      //
    //      polygon_y:                                                      //
    //////////////////////////////////////////////////////////////////////////

    double r;
    double th;
    double x[n],y[n];
    int m;

    if ( n==4 )
    {
        x[0] = length/2;
        y[0] = x[0];

        x[1] =-x[0];
        y[1] = x[0];

        x[2] =-x[0];
        y[2] =-x[0];

        x[3] = x[0];
        y[3] =-x[0];
    }
    else
    {
        th=PI/n;
        r=(length/2) / ( sin(th) );

        x[0]=r;
        y[0]=0;
        for( m=1 ; m<n ; ++m )
        {
            th=m*(2*PI/n);
            x[m]=cos(th)*r;
            y[m]=sin(th)*r;
        }

    }

        for( m=0 ; m<n ; ++m )
        {
            polygon_x.push_back( x[m] );
            polygon_y.push_back( y[m] );
        }

}


void rotate( double xa , double ya , double th , double &xb,double &yb)
{
    ///////////////////////////////////////////////////////////////
    //  <Function>                                               //
    //    Point(xa,ya) rotate about Orign thita to point(xb,yb). //
    //  <input>                                                  //
    //    xa, ya:                                                //
    //    th:                                                    //
    //  <output>                                                 //
    //    xb, yb:                                                //
    ///////////////////////////////////////////////////////////////

    xb = ( xa*cos(th) ) - ( ya*sin(th) );
    yb = ( xa*sin(th) ) + ( ya*cos(th) );

}


void translate( double xa , double ya , double t_x , double t_y ,double &xb , double &yb )
{
    /////////////////////////////////////////////////////////////////////////
    //  <Function>                                                         //
    //      Move point A(xa,ya) to pointB(xb,yb) by a specified distance.  //
    //      The distance is described as (t_x,t_y).                        //
    //  <input>                                                            //
    //      xa, ya:                                                        //
    //      t_x:                                                           //
    //      t_y:                                                           //
    //  <output>                                                           //
    //      xb, yb:                                                        //
    /////////////////////////////////////////////////////////////////////////

    xb=xa+t_x;
    yb=ya+t_y;

}


double angle_xaxis_point( double xp , double yp )
{
    //////////////////////////////////////////////////
    //  <Function>                                  //
    //      The angle from x-axis to point(x,y).    //
    //  <Input>                                     //
    //      xp, yp:                                 //
    //  <Output>                                    //
    //      th:                                     //
    //////////////////////////////////////////////////

    double r;
    r=sqrt( xp*xp + yp*yp);
    double th;

    th=(acos(xp/r));

    if (yp<0)
    {
        th=(2*PI)-( acos(xp/r) );
    }

    return(th);
}


double angle_line1_line2( double x10,double y10 , double x11,double y11 ,double x20,double y20 ,double x21,double y21 )
{
    ///////////////////////////////////////////////////////////////////////
    //  <Function>                                                       //
    //      The angle (in radians) between line1 and line2.              //
    //      The line1 passes through ponit1 (x1,y1) and point2 (x2,y2).  //
    //      The line2 passes through ponit3 (x3,y3) and point4 (x4,y4).  //
    //  <Input>                                                          //
    //      x10, y10, x11, y11:                                          //
    //      x20, y20, x21, y21:                                          //
    //  <Output>                                                         //
    //      th: The angle of line1 & line2                               //
    ///////////////////////////////////////////////////////////////////////

    double th;
    double th1,th2;

    translate( x11,y11 , -x10,-y10 , x11,y11 );
    th1=angle_xaxis_point(x11,y11);

    translate( x21,y21 , -x20,-y20 , x21,y21 );
    th2=angle_xaxis_point(x21,y21);

    th=th2-th1;
    return(th);
}


void combine( vector<int> polygon )
{
    //////////////////////////////////////////////////////////////////////
    //  <Function>                                                      //
    //      polygon's vertices sort by ascending.                       //
    //  <Input>                                                         //
    //      polygon: It records the indexs of a set of vertexes.        //
    //  <Output>                                                        //
    //      x_axis: The x_axis of the vertexes.                         //
    //      y_axis: The y_axis of the vertexes.                         //
    //////////////////////////////////////////////////////////////////////

    double x01,y01 , x02,y02;
    double th;
    double x_polygon_front,y_polygon_front;
    double x_polygon_back,y_polygon_back;
    int front,back;
    int n_p;

    vector<double> new_polygon_x;
    vector<double> new_polygon_y;
    double t_x,t_y;
    int m;

    front=polygon.front();
    back=polygon.back();

    x01=x_axis[front];
    y01=y_axis[front];

    x02=x_axis[back];
    y02=y_axis[back];

    n_p=polygon.size();

    // Generate a new polygon.
    generate_polygon( n_p , new_polygon_x , new_polygon_y );

    x_polygon_front=new_polygon_x.front();
    y_polygon_front=new_polygon_y.front();
    x_polygon_back=new_polygon_x.back();
    y_polygon_back=new_polygon_y.back();

    th=angle_line1_line2( x01,y01 , x02,y02 , x_polygon_front,y_polygon_front ,
                          x_polygon_back,y_polygon_back );

    if ( th!=0 )
    {
        for ( m=0 ; m<n_p ; ++m )
        {
            rotate(new_polygon_x[m],new_polygon_y[m] , -th ,
                    new_polygon_x[m],new_polygon_y[m] );
        }
    }

    t_x=x01-new_polygon_x[0];
    t_y=y01-new_polygon_y[0];

    if ( t_x!=0 && t_y!=0 )
    {

        for ( m=0 ; m<n_p ; ++m )
        { // Translate every vertices.
            translate( new_polygon_x[m],new_polygon_y[m] ,
                       t_x , t_y , new_polygon_x[m],new_polygon_y[m] );
        }

    }

    for ( m=0 ; m<n_p ; ++m )
    {
        // Round x_axis
        if (new_polygon_x[m]>0)
        {
            x_axis[ polygon[m] ]=(int)(new_polygon_x[m] + 0.5);
        }
        else
        {
            x_axis[ polygon[m] ]=(int)(new_polygon_x[m] - 0.5);
        }

        // Round y_axis
        if (new_polygon_y[m]>0)
        {
            y_axis[ polygon[m] ]=(int)(new_polygon_y[m] + 0.5);
        }
        else
        {
            y_axis[ polygon[m] ]=(int)(new_polygon_y[m] - 0.5);
        }

    }

}


void adjust_coordinates( )
{
    //////////////////////////////////////////////////////////////////
    //  <Function>                                                  //
    //      Adjust the coordinate(x_axis & y_axis) of the graph.    //
    //      After then, every x_axis and y_axis will >= 0.          //
    //  <Input>                                                     //
    //      x_axis:                                                 //
    //      y_axis:                                                 //
    //  <Output>                                                    //
    //      x_axis:                                                 //
    //      y_axis:                                                 //
    //////////////////////////////////////////////////////////////////

    int min_x,min_y;//max_x,max_y;
    int n,end_n;
    int t_x,t_y;

    min_x=*min_element( x_axis.begin() , x_axis.end() );
    min_y=*min_element( y_axis.begin() , y_axis.end() );

    t_x=-min_x+30;
    t_y=-min_y+30;

    end_n=x_axis.size();

    for( n=0 ; n<end_n ; ++n )
    {
        x_axis[n]=x_axis[n]+t_x;
        y_axis[n]=y_axis[n]+t_y;
    }

}


void merge_polygons( )
{
    //////////////////////////////////////////////////////
    //  <Function>                                      //
    //      Mergen all the polygons into a Big Graph.   //
    //  <Input>                                         //
    //      x_axis                                      //
    //      y_axis                                      //
    //  <Output>                                        //
    //      x_axis                                      //
    //      y_axis                                      //
    //////////////////////////////////////////////////////

    vector<int> x_axis;
    vector<int> y_axis;

    int n,n_end;
    n_end=polygons.size()-1 ;

    for ( n=n_end ; n>=0 ; --n )
    {
        combine(polygons[n]);
    }

}


void help_info()
{
    //////////////////////////////////////////////////////////////////////////////////////////
    //  <Function>                                                                          //
    //      Show the help information.                                                      //
    //////////////////////////////////////////////////////////////////////////////////////////

    cout<<"Usage: dbntossdj ifile ofile"<<endl;
    cout<<"Inport sequence & dot_bracket from input file."<<endl;
    cout<<"According to the dot_bracket notation, work out the Secondary Structure."<<endl;
    cout<<"Export the Secondary Structure Dataset in Json format into output file."<<endl;
    cout<<endl;

}


int main( int argc, char* argv[] )
{
    //////////////////////////////////////////////////////////////////////////////////////////
    //  <Function>                                                                          //
    //      DBN->SSDJ                                                                       //
    //      Inport sequence & dot_bracket from data file(DBN).                              //
    //      According to the dot_bracket notation, work out the Secondary Structure.        //
    //      Export the Secondary Structure Dataset in Json format into data file(SSDJ).     //
    //  <Input>                                                                             //
    //      argv[1]: The filename of Input.                                                 //
    //  <Output>                                                                            //
    //      argv[2]:  The filename of Output.                                               //
    //////////////////////////////////////////////////////////////////////////////////////////

    string arg(argv[1]);

    if ( ( arg=="--help" ) || ( arg=="-help" ))
    {
        help_info();
    }
    else
    {

        // Import data from file.
        import_file( argv[1] );

        // Insert "Shadow"
        insert_shadow();

        // Group vertexes into many polygon
        group_vertexes();

        // distinguish_motif(); & process them in diffrent way.
        class_motifs();
        process_motifs();

        // Define the Union Length
        length=30;

        // Initialise the coordinate of x_axis & y_axis.
        initialise_coordinate();

        // Merge polygons
        merge_polygons( );

        // Adjust coordinates.
        adjust_coordinates();

        // Delete "Shadow"
        delete_shadow();

        // Export data to file.
        export_file( argv[2] );

        // Release the system resource.
        release_system_resource();

    }

    return 0;

}

