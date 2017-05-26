////////////////////////////////////////////////////////////
//  <Decleration>                                         //
//      It is a part of RNASViewer project.               //
//  <Date>                                                //
//      10/29/2013                                        //
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

/*
// Begin, GD libraries for png images.
#include "gd.h"
#include <gdfontt.h>
#include <gdfonts.h>
*/

// Begin, Linux system libraries.
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

    using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin, global variable.

    // Discription of the sequence.
    vector<string> discriptions;

    // A table of Secondinary Struture.
    vector< vector<string> > ss_table;

    // The bases of sequence in the data file
    vector<unsigned char> bases;
    // The dots & brackets in the date file.
    // They show the relationship(paired or not_paired) of bases in the DBN file.
    vector<unsigned char> dot_bracket;

    // End, global variable.
////////////////////////////////////////////////////////////////////////////////////////////////////

void release_system_resource()
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Release all of the vector variable.               //
    ////////////////////////////////////////////////////////////

    // Discription of the sequence.
    vector<string> ().swap( discriptions );

    // A table of Secondinary Struture.
    vector< vector<string> > ().swap( ss_table );

    // Sequence of base.
    vector<unsigned char> ().swap( bases );

    // Dot-bracket Notation
    vector<unsigned char> ().swap( dot_bracket );

} // void release_system_resource()

vector<string> split( string str, string pattern)
{
    ////////////////////////////////////////////////////////////////////////////////
    //  <Function>                                                                //
    //      Split the main string into sever sub string                           //
    //  <Input>                                                                   //
    //      str:                                                                  //
    //      pattern:                                                              //
    //  <Output>                                                                  //
    //      result: It is an array of string.                                     //
    ////////////////////////////////////////////////////////////////////////////////

    //std::string::size_type pos;
    int pos;
    vector<string> result;

    str = str + pattern;//
    int size;
        size = str.size();

    for ( int i=0; i<size; ++i )
    {
        //
        pos = str.find( pattern, i );
        if ( pos<size )
        {
            //
            string s = str.substr( i, pos-i);
            result.push_back( s );
            i = pos + pattern.size() - 1;
        } // if (pos<size)
    } // for ( int i=0; i<size; ++i )
    return result;

} // std::vector<std::string> split(std::string str, std::string pattern)

/*
//vector<string>
string filename_ex( string filename ) // For String
{
//    int len;
  //  int pos;
    string result;
    int n;
    int end_n;
        n = filename.length();
        end_n = n;

//    cout<<end_n;

    while ( filename[n] != '.' && n>0 )
    {
        n = n-1;
//        cout<<filename[n];
  //      cout<<" ";
    } // for ( n=end_n; n>=0; --n )

    n = n + 1;
    result = filename.substr( n, (end_n-n) );
    return( result );

} // string filename_ex( string filename )
*/

/*
string file_ex_char_start( char* filename ) // For Char*
{
    string result;
//        result = "AUGC";
    int len;
        len = strlen( filename );
    int n;
    //cout<<strlen( filename );
    //cout<<"testing";

    for ( n=(len-1); n>0; --n )
    {
        //
        cout<<n<<":";//<<".";
        cout<<filename[n]<<" ";
    } // for ( n=(len-1); n>0; --n )

    n = len-1;
    cout<<endl;
    cout<<"File_ex: ";

    while ( n>0 && filename[n]!='.' )
    {
        cout<<filename[n];
        n = n-1;
    } // while ( n>0 && filename[n]!='.' )

    cout<<endl;
    cout<<"Last n:"<<n;

    return result;
} // string file_ex_char_start( char* filename )
*/

string remove_excess_space( string str_source )
{
    //////////////////////////////////////////////////////////////////////////////////////////
    //  <Function>                                                                          //
    //      Remove the excess space in the start, the end & the middle of str_source.       //
    //      Then save the new string into str_goal.                                         //
    //      At the last, without space in the start & the end of str_goal.                  //
    //      And just ONE SPACE between severy parts in the middle of str_goal.              //
    //  <Input>                                                                             //
    //      str_source:                                                                     //
    //  <Output>                                                                            //
    //      str_goal:                                                                       //
    //////////////////////////////////////////////////////////////////////////////////////////
    string str_goal;
        //str_goal = "";
    string str;

    int flag_space;
//    bool flag_space;
        flag_space = 1;

    int n;
    int n_end;
    int m;

    n_end = str_source.length();
//    cout<<n_end;
  //  cout<<endl;

    for ( n=0; n<n_end; ++n )
    {
//        str_goal.append( " " ); // OK
        //
//        m = 0;
        if ( str_source[n] != ' ' )
        {
            //
            //str_goal[m] = str_source[n];
            str = str_source[n];
            str_goal.append( str );
//            cout<<str_source[n];
  //          m = m+1;
    //        cout<<str_goal[m];
      //      cout<<endl;
//            str_goal[m] = 0;
            //if ( flag_space == 1 )
//            {
                //
                flag_space = 0;
//            }
        }
        else
        { // str_source[n] == ' '
            //
            if ( flag_space == 0 )
            {
                //
                //str_goal[m] = str_source[n];
                //str = str_source[n];
                //str_goal.append( str );
                str_goal.append( " " );
//                cout<<str_source[n];
  //              m = m+1;
    //            cout<<str_goal[m];
//                str_goal[m] = 0;

                flag_space = 1;
            } // if ( flag_space == 0 )

        } // else

    } // for ( n=0; n<n_end; ++n )

//    cout<<endl;
  //  cout<<str_goal;
    //cout<<endl;
    m = str_goal.length()-1;
    if ( str_goal[m] == ' ' )
    {
        //str_goal[m] = '\0';//OK // Smoe time it does not work.
        //str_goal.erase( (n_end-1),1 );//OK // Smoe time it does not work.
        str_goal = str_goal.substr( 0 , m ); // OK
    } // if ( str_goal[m] == ' ' )
    return str_goal;

}  // string remove_excess_space( string str_source )

void import_file(char* input_file)
{
    //
    ifstream ifs(input_file);

    if ( !ifs )
    { // Error in open file.
        cout<<"Error: can not open the input file.";
        cout<<endl;
    }
    else
    { // file is opened.
        vector<string> results;

        string textline;
        string text;
//        int n;
        int end_n;

        // Read the first line. // Discription with sever parts of the sequence.
        // The first part is number of the sequence. The last part is the title of this struture.
        getline( ifs, textline , '\n' );

// Begin, ??????????

        end_n = textline.length();
//        cout<<"Length:"<<end_n<<endl;

        if ( textline[ end_n-1 ] =='\r' )
        { // A file of Windows.
//            cout<<"It is a "<<"\\r "<<"here."<<endl;
            // '\r' -> ' '(space)
            textline[ end_n-1 ] = ' ';//            textline[ end_n-1 ] = 0;
        //    cout<<end_n<<endl;
        } // if ( textline[ end_n-1 ] =='\r' )

//        end_n = textline.length();
  //      cout<<"Length:"<<end_n<<endl;

//        if ( textline[ end_n-1 ] =='\r' )
  //      { // A file of Linux.
    //        cout<<"It is a "<<"\\r "<<"here."<<endl;
      //      textline[ end_n-1 ] = ' ';
        //    cout<<end_n<<endl;
//        }
  //      else
    //    {
      //      cout<<"Here without "<<"\\r"<<"."<<endl;
        //}
// End, ??????????

//        cout<<textline;
//        cout<<endl;

        // Remove excess space form textline.
        text = remove_excess_space( textline );
//        cout<<"("<<text<<")";
  //      cout<<endl;
        discriptions = split( text, " " );

//        int n;
//        int end_n;
//            end_n = discriptions.size()-1;

//        for ( n=end_n; n>=0; --n )
//        {
 //           cout<<n<<":";
   //         cout<<discriptions[n];
     //       cout<<", ";
//        } // for ( n=end_n; n>=0; --n )
        //cout<<endl;

        // Read the other lines.
        while ( !ifs.eof() )
        {
            getline( ifs, textline , '\n' );
            end_n = textline.length();
//            cout<<"Length:"<<end_n<<endl;

            if ( textline[ end_n-1 ] =='\r' )
            { // A file of Windows.
                //cout<<"It is a "<<"\\r "<<"here."<<endl;
                // '\r' -> ' '(space)
                textline[ end_n-1 ] = ' ';//                textline[ end_n-1 ] = 0;
                //    cout<<end_n<<endl;
            } // if ( textline[ end_n-1 ] =='\r' )

//            cout<<"("<<textline<<")";
  //          cout<<endl;

            text = remove_excess_space( textline );
//            cout<<"{"<<text<<"}";
  //          cout<<endl;

            if ( text.length()>1 )
            {
//                cout<<"OK ";
                results = split( text, " " );

                ss_table.push_back( results );

//                end_n = results.size();
  //              for ( n=0; n<end_n; ++n )
    //            {
      //              cout<<n<<":";
        //            cout<<results[n]<<", ";
          //      } // for ( n=0; n<results.size(); ++n )
            //    cout<<endl;
            }// if ( text.length()>1 )
  //          else
    //        {
      //          cout<<"Ignore.";
        //        cout<<endl;
          //  } // else
        } // while ( !ifs.eof() )

//        cout<<"--------------------------------------------------";
  //      cout<<endl;

        // Release system resource.
        vector<string> swap( results );

     } // else // file is opened.

} // void import_file(char* input_file)

/*
void show_table()
{
        int y;
        int end_y;
            end_y = ss_table.size();
        for ( y=0; y<end_y; ++y )
        {
            //
            int x;
            int end_x;
                end_x = ss_table[y].size();
            for ( x=0; x<end_x; ++x )
            {
                //
                cout<<ss_table[y][x];
                cout<<", ";
            } // for ( x=0; x<end_x; ++x )
            cout<<endl;
        } // for ( y=0; y<end_y; ++y )
} // void show_table()
*/

void table_to_dbn()
{
    int col_index;
    int col_base;
    int col_pairing;

    int y;
    int end_y;
        end_y = ss_table.size();

    // Data file is CT or BPSEQ
        col_index = 0;
        col_base = 1;
        col_pairing = 4;
/*
    int columns;
        columns = ss_table[0].size();
    if ( columns == 6 )
    { // CT file
        col_pairing = 4;
    }
    else
    { // BPSEQ file
        col_pairing = 2;
    } // else // BPSEQ file
*/
    // Process every line of the ss_table.
    for ( y=0; y<end_y; ++y )
    {
//        cout<<ss_table[y][col_base][0]; // It is a char.
  //      cout<<ss_table[y][col_pairing]; // It is a string of int.
    //    cout<<",";

        // Get the Base.
        unsigned char base;
            base = ss_table[y][col_base][0];
        bases.push_back( base );

        // Get the Dot_bracket notation.
        string id_str;
            id_str = ss_table[y][col_index];
        int id_int;
            id_int = atoi( id_str.c_str() ); // string -> int.??????????
        string pairing_str;
            pairing_str = ss_table[y][col_pairing];
        int pairing_int;
            pairing_int = atoi( pairing_str.c_str() ); // String -> int.??????????

        unsigned char dbn;

        if ( pairing_str == "0" )
        {
            dbn = '.';
            dot_bracket.push_back( dbn );
        }
        else
        { // pairing !="0"
            if ( id_int < pairing_int )
            {
                dbn = '(';
                dot_bracket.push_back( dbn );
            }
            else
            { // id_int > pairing_int
                dbn = ')';
                dot_bracket.push_back( dbn );
            } // else // id_int > pairing_int
        } // else // pairing !="0"

    } // for ( y=0; y<end_y; ++y )

} // void table_to_dbn()

/*
void show_variable()
{
    //
    int n;
    int end_n;

    // Discription of SS.
    cout<<"Discriptions:";
    end_n = discriptions.size();
    for ( n=end_n-1; n>=0; --n )
    {
        cout<<discriptions[n];
    }
    cout<<endl;

    // Sequence.
    end_n = bases.size();
    cout<<"Sequence:";
    for ( n=0; n<end_n; ++n )
    {
        cout<<bases[n];
    }
    cout<<endl;

    // Dot_bracket notation.
    end_n = dot_bracket.size();
    cout<<"Dot_bracket notation:";
    for ( n=0; n<end_n; ++n )
    {
        cout<<dot_bracket[n];
    }
    cout<<endl;

} // void show_variable()
*/

void export_file( char* output_file )
{
    ofstream ofs(output_file);

    if( !ofs )
    { // Error in open file.
        cout<<"Error: can't open the output file.";
        cout<<endl;
    }
    else
    { // File is opened.
        // Begin, Expor an begin sign.
//        cout<<">"; // The begin of json string.
        ofs<<">"; // The begin of json string.

        int n;
        int end_n;

        // Discription of SS.
        end_n = discriptions.size();
        for ( n=end_n-1; n>0; --n )
        {
//            cout<<discriptions[n];
//            cout<<' ';
            ofs<<discriptions[n];
            ofs<<' ';
        }
//        cout<<discriptions[0];
//        cout<<endl;
        ofs<<discriptions[0];
        ofs<<endl;

        // Sequence.
        //cout<<"Sequence:";
        end_n = bases.size();
        for ( n=0; n<end_n; ++n )
        {
//            cout<<bases[n];
            ofs<<bases[n];
        }
//        cout<<endl;
        ofs<<endl;

        // Dot_bracket notation.
        //cout<<"Dot_bracket notation:";
        end_n = dot_bracket.size();
        for ( n=0; n<end_n; ++n )
        {
//            cout<<dot_bracket[n];
            ofs<<dot_bracket[n];
        }
//        cout<<endl;
        ofs<<endl;

    } // else // File is opened.

} // void export_file( char* output_file )

/*
void test_import_file()
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Testing import data from specifies file.          //
    ////////////////////////////////////////////////////////////

    // Begin, Import testing.
    char* input_filename;
    string ifile;

    ifile="./output_files/test.ct"; // OK
//    ifile="./output_files/test_ct.ct"; // OK
//    ifile="./output_files/test_bpseq.bpseq";

    input_filename=(char*)ifile.c_str();
    import_file( input_filename );
    // End, Import testing.

} // void test_import_file()

void test_export_file()
{
    ////////////////////////////////////////////////////////////
    //  <Function>                                            //
    //      Testing export data to specifies file.            //
    ////////////////////////////////////////////////////////////

    // Begin, Export data to file testing.
    char* output_filename;

    string ofile;
    ofile = "./output_files/test1.dbn";

    output_filename=(char*)ofile.c_str();
    export_file(output_filename);
    // End, Export data to file testing.

} // void test_export_file()
*/

int main( int argc, char* argv[] )
{
    //////////////////////////////////////////////////////////////////////////////////////////
    //  <Function>                                                                          //
    //      Inport data of Secondary Struture from a file in CT format.                     //
    //      According to the data info, work out the dot-bracket notation.                  //
    //      Export the Secondary Structure Dataset into a file in DBN format.               //
    //  <Input>                                                                             //
    //      argv[1]: The filename of Input(CT).                                             //
    //  <Output>                                                                            //
    //      argv[2]:  The filename of Output(DBN).                                          //
    //////////////////////////////////////////////////////////////////////////////////////////

	//test input file exist
	if(argc != 3) {
		cout << "Usage: cttodbn <input_ct_file> <output_dbn_file>" << endl;
		return 1;
	}
	ifstream in(argv[1]);
	if(!in) {
		cout << "Can't open file " << argv[1] << endl;
		exit(1);
	}
	in.close();

//    string arg(argv[1]);
//    if ( arg=="--help" )
//    { // Ask for help. Show the help information.
//        help_info();
//    }
//    else
//    { // Not ask for help.

        // Import data from file.
//        test_import_file();
        import_file( argv[1] );

//        show_table();

        table_to_dbn();

//        show_variable();

        // Export data to file.
//        test_export_file();
        export_file( argv[2] );

        // Release the system resource.
        release_system_resource();

//    } // else // Not ask for help.

    return 0;

} // int main( int argc, char* argv[] )

