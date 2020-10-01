#ifndef LEXICON_H	// include guard (to ensure that robot.h is not declared twice
#define LEXICON_H

#include <iostream>	// for console I/O streams
#include <fstream>	// for file I/O streams
#include <windows.h>	// for windows features of file list and console manipulations
#include <string.h>	// for string manipulations
#include <math.h>	// for standard math functions
#include <conio.h>	// for getch() function
using namespace std;

// Special Keys Defined //

const int KEY_BKSP = 8;
const int KEY_TAB = 9;
const int KEY_ENTER = 13;
const int KEY_ESC = 27;
const int KEY_SPC = 32;
const int KEY_DEL = 127;
const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_LEFT = 75;
const int KEY_RIGHT = 77;


// Function Prototypes //


// User Functions:

	int Conv_to_Int( char );
	char* Conv_to_Char( int );
	
	char* CutString( char*, int, int );
	void AppendString( char*, char* );

	char* HidePassKey( char, int );

	char* BrowseDisk( char*, int, char* );
	
	char* AsciiHeading( char* );
	
// Classes

class LIST {
	private:
		int line_count;
		COORD pos_start;
		COORD pos_end;
		char box[6];
		char list_data[200][1000];
		char level_data[200];
		int index;
		
		void ClearData();
		void WriteList( int );
	public:
		LIST();
		void AddListData( int, char* );
		void RadioList();
};

class CRYPTEX {
	private:
		char Alpha[26];
		char E_Alpha[26];
		char file[100000][1000];
		char Key[100];
		int n_lines;
		int caesar_c;
		int caesar_c_num;
		char savefile[400];
		
		void ClearData();
		void GeneratePassData();
	public:
		CRYPTEX();
		CRYPTEX( char* );
		CRYPTEX( char*, char* );
		CRYPTEX( char*, char*, char* );
		int LoadFile( char* );
		void SetMessage( char* );
		void SetPassKey( char* );
		int SetOutputLocation( char* );
		int OutputOnDisk( char* );
		
		void RunEncryption();
		void RunDecryption();
		
		char* ReturnAsString();
};

class PARAGRAPH {
	private:
		int margin;
		int center;
		int line_len;
		int console_len;
		char msg_data[100000];
		
		void ClearData();
		int CenterAlignLen( int );
	public:
		PARAGRAPH();
		void ParaData( char* );
		void SetMargin( int );
		void SetMaxLineLen( int );
		void CenterAlign( int );
		void WritePara();
		char* ReturnAsString();
		int ReturnLines();
};

class ANIMATION {
	private:
		char msg[100000];
		int time;
		
		void ClearData();
	public:
		ANIMATION();
		ANIMATION( char* );
		ANIMATION( int );
		ANIMATION( char*, int );
		
		void SetMessage( char* );
		void SetDuration( int );
		
		void TypeWriter();
		void Random();
};

class WINDOWS {
	private:
		HANDLE console;
		CONSOLE_SCREEN_BUFFER_INFO screen;
		CONSOLE_CURSOR_INFO cursor;
		COORD pos;
		
		void LoadData();
	public:
		WINDOWS();
		int CursorStatus();
		void CursorStatus( int );
		int ScreenLen();
		int getX();
		int getY();
		void gotoXY( int, int, int );
		void CleanSlate( int, int );
}CONSOLE;
	
	
// MAIN FUNCTIONS //

int Conv_to_Int( char* num_string ) {
	int len = strlen(num_string);
	int num = 0;
	
	for(int i = 0; i < len; i++ ) {
		num += int( num_string[i] - '0' ) * pow( 10, len - i - 1 );	// 213 = 2 * 10^2 + 20 * 10^1 + 3 * 10^0
	}
	
	return num;
}

char* Conv_to_Char( int number )
{
	int len = 0;
	
	while( floor( number / pow( 10, len ) ) > 0 ) {
		len++;
		// Get no. of positions
	}
	char number_char[len+1];
	
	int num_temp = 0;
	for( int i = 0; i < len; i++ ) {
		num_temp = number / pow( 10, len - i - 1 );	// All Previous terms (Mathematically) eliminated
		num_temp = num_temp % 10;	// All	Following terms (Mathematically) eliminated
		number_char[i] = char( num_temp + '0' );
	}
	number_char[len] = '\0';
	
	return &number_char[0];
}



char* CutString( char* input, int start, int end ) {
	char output[1000];
	for( int i = 0; i < 1000; i++ ) {
		output[i] = '\0';
	}
	
	for( int i = 0, j = start; j <= end; i++, j++ ) {
		output[i] = input[j];
	}
	
	return &output[0];
}

void AppendString( char* Destination_String, char* Source_String ) {
	int i = strlen( Destination_String );
	
	for(int j = 0; j < strlen( Source_String ); j++, i++) {
		Destination_String[i] = Source_String[j];
	}
	Destination_String[i] = '\0';
	return;
}



char* HidePassEntry( char hide_char, int max_length ) {
	CONSOLE.CursorStatus( 1 );
	
	char pwd[max_length + 1];
	for( int i = 0; i < max_length + 1; i++ ) {
		pwd[i] = '\0';
	}
	
	int counter = 0;
	char entry = '\0';
	do {
		entry = getch();
		switch( entry ) {
			case KEY_ENTER:
				pwd[counter] = '\0';
				cout << endl;
				break;
			case KEY_ESC:
				return "\0";
				break;
			case KEY_BKSP:
				if( counter ) {
					counter--;
					pwd[counter] = '\0';
					CONSOLE.gotoXY( -1, 0, 1 );
					cout << " ";
					CONSOLE.gotoXY( -1, 0, 1 );	
				}
				break;
			default:
				if( counter < max_length ) {
					pwd[counter] = entry;
					if( hide_char != '\0' ) {
						cout << hide_char;
					}
					else {
						cout << entry;
					}
					counter++;		
				}
				break;
		}
	}while( entry != KEY_ENTER );
	
	return &pwd[0];
}



char* BrowseDisk(char* location, int entry_type, char* file_extension = "*")
/*
entry_type:
	0: All
	1: Files
	2: Folders
*/
{
	int selection_int = 0;
	char selection_char[4] = "\0";
	COORD cursor_position;
	int cursor = CONSOLE.CursorStatus();
	int run_counter = 0;	// To store which iteration (numerically) of the function is the current one
	
	char path[260];
	for( int i = 0; i < 260; i++ )	// Clearing the variable path
	{
		path[i] = '\0';
	}
	AppendString( path, location );	// Original Variable/Input must not be changed
	
	char ext[10];
	if( file_extension[0] == '.' ) {
		strcpy( ext, CutString( file_extension, 1, strlen( file_extension ) ) );
	}
	else {
		strcpy( ext, file_extension );
	}
	
	SELECT_FILE_MAIN:
	
	int counter = 0;	// Variable to used as a recursive counter
	run_counter++;
	
	// Check if the path is relative
	if( path[1] != ':' )	// Since absolute path are of format [DRIVE LETTER]:\...., path[1] should hold ':' if it is an absolute path
	{
		char curr_location[260];
		GetCurrentDirectory( 260, curr_location );
		
		strcpy( path, curr_location );
		AppendString( path, "/" );
	}
	
	for(int i = 0; i < strlen( path ); i++)
	{
		if( path[i] == '\\')
		{
			path[i] = '/';	// Prefer '/' over '\'
		}
	}
	
	// Location Format: X/......XYZ/*
	switch(path[strlen(path) - 1])
	{
		case '*':
			if (path[strlen(path) - 2] == '/' || path[strlen(path) - 2] == '\\')
			{
				break;
			}
			break;
		case '/':
			AppendString( path, "*");
			break;
		case '\\':
			AppendString( path, "*");
			break;
		default:
			AppendString( path, "/*");
			break;
	}
	
	char file_list[500][260];
	for(int i=0; i<500; i++)
	{
		for(int j=0; j<260; j++)
		{
			file_list[i][j] = '\0';
		}
	}
	
	WIN32_FIND_DATA file;
	HANDLE curr_file = FindFirstFile( path, &file );
	if(curr_file != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(!(( file.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )||( file.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM )))	//Only show user files
			{
				switch( entry_type )
				{
					case 0:
						strcpy(file_list[counter], file.cFileName);
						counter++;
						break;
					case 1:
						if( file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
							strcpy( file_list[counter], file.cFileName );
							counter++;
						}
						else if ( strcmp( ext, "*" ) == 0 ) {
							strcpy( file_list[counter], file.cFileName );
							counter++;
						}
						else {
							for( int i = 0; i < strlen( file.cFileName ); i++ ) {
								if( file.cFileName[i] == '.' ) {
									if( strcmp( ext, CutString( file.cFileName, ( i + 1 ), strlen( file.cFileName ) ) ) == 0 ) {
										strcpy( file_list[counter], file.cFileName );
										counter++;
									}
									break;
								}
							}
						}
						break;
					case 2:
						if( file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )	//Only List Folders
						{
							strcpy(file_list[counter], file.cFileName);
							counter++;
						}
						break;
					default:
//						cout << paragraph( "\n\n:: HEADER FILE -------- Invlaid entry into parameter ::\n\n" );
//		
//						cout << paragraph( "## entry_type = [0, 1, 2] = FALSE\n" );
//		
//		
//						cout << paragraph( "Press any key to terminate...\n" );
						getch();
						exit( 1 );
						break;
				}
			}
		}while( FindNextFile( curr_file, &file ) );
		FindClose( curr_file );
		
		SHOW_FILE_LIST:
		
		if( run_counter == 1 )	// If it is the first run
		{
			// Marking the starting postion of the output displayed by the function
			cursor_position.Y = CONSOLE.getY();
		}
		
		// Clearing the previous output
		CONSOLE.CursorStatus( 0 );
		CONSOLE.CleanSlate( 0, cursor_position.Y );
		
//		cout << paragraph( "Current Location: " ) << endl;
		cout.write( path, strlen(path)-1 );	// Dont display the '*'
		cout << endl << endl;
		
		int c = 0;	// Temporary counter
		for(; c < counter; c++)
		{
			if( strcmp( file_list[c], "." ) == 0 )
			{
				if( entry_type != 1 )	// Folders not selectabe in file type
				{
					cout << c << ". << Use This Folder >>" << endl;
				}
			}
			else if( strcmp( file_list[c], ".." ) == 0 )
			{
				cout << c << ". << Go Up/Back a Folder >>" << endl;
			}
			else
			{
				cout << c << ". ";
				cout.write( file_list[c], strlen( file_list[c] ) );
				cout << endl;
			}
		}
		if( entry_type != 1 )	// If Folder can be selected
		{
			cout << c <<". << New Folder >>" << endl;
			c++;
		}
		cout << c <<". << Cancel >>" << endl;
		
		// Select desired file/folder
		CONSOLE.CursorStatus( 1 );;
//		cout << paragraph( "Enter the index of the folder/file to open: " );
		cin.getline(selection_char, 4);
		for( int i = 0; i < strlen( selection_char); i++ )
		{
			if(!( selection_char[i] >= '0' && selection_char[i] <= '9' ))	// Only number entry allowed
			{
				cout << endl<<"Invlaid! Please enter a number" << endl;
				cout << "Press any key to Retry!" << endl;
				getch();
				
				run_counter++;
				goto SHOW_FILE_LIST;
			}
		}
		CONSOLE.CursorStatus( 0 );
		
		selection_int = Conv_to_Int(selection_char);
		if( selection_int > counter + 1 )
		{
			cout <<endl << "Invalid index number!" << endl;
			cout << "Press any key to Retry!" << endl;
			getch();
			
			run_counter++;
			goto SHOW_FILE_LIST;
		}
		
		if( selection_int == 0 && entry_type == 1)	// Folders not selectable in file type
		{
			cout << endl << "Invalid index number!" << endl;
			cout << "Press any key to Retry!" << endl;
			getch();
			
			run_counter++;
			goto SHOW_FILE_LIST;
		}
		
		
		// Removing '*' and '/' from the path
		path[strlen( path ) - 1] = '\0';
		path[strlen( path ) - 1] = '\0';
		
		if( ( entry_type != 1 && selection_int == counter + 1 ) || ( entry_type == 1 && selection_int == counter ) )	// If user Cancels
		{
			return "\0";
		}
		else if( entry_type != 1 && selection_int == counter )	// If create new folder
		{
			char name[260];
			char invalid[] = {"/\\:*\"<>|"};
			
			NAME_ENTRY:
			CONSOLE.CursorStatus( 1 );;
			cout << "Enter name for New Folder: ";
			cin.getline( name, 260 );
			CONSOLE.CursorStatus( 0 );
			
			for(int i = 0; i < strlen( name ); i++)
			{
				for(int j = 0; j < strlen( invalid ); j++)
				{
					if( name[i] == invalid[j] )	// The characters in invalid array are not allowed in the folder name in windows
					{
						cout << endl << "The Folder name is not valid\nPlease Retry\n";
						goto NAME_ENTRY;
					}
				}
			}
			
			AppendString( path, "/" );
			AppendString( path, name );
			
			CreateDirectory( path, NULL );
			// If creation is not successful due to folder with same name already existing, the program shall simply open that folder
			
			goto SELECT_FILE_MAIN;
			return &path[0];
		}
		else if( strcmp( file_list[selection_int], ".") == 0)
		{
			return &path[0];
		}
		else if( strcmp( file_list[selection_int], "..") == 0)
		{
			// Getting rid of name of current directory from path
			for( int k = strlen( path ); k > 0; k--)
			{
				if( path[k] == '/' || path[k] == '\\')
				{
					goto SELECT_FILE_MAIN;
					return &path[0];
				}
				else
				{
					path[k] = '\0';
				}
			}
		}
		else
		{
			AppendString( path, "/");
			AppendString( path, file_list[selection_int] );
			
			goto SELECT_FILE_MAIN;
			return &path[0];
		}
	}
	else
	{
		// If the current path is not a Folder or invalid path
		
		
		//Removing '*' and '/' from the path
		path[strlen( path ) - 1] = '\0';
		path[strlen( path ) - 1] = '\0';
		
		fstream test;
		test.open( path, ios::in);
		
		if( test )
		{
			test.close();
			return &path[0];
		}
		else
		{
			cout << "\n\n:: HEADER FILE -------- Unexpected Handle ::\n\n";
		
		
			cout << "Press any key to terminate...\n";
			getch();
			exit(1);
		}
	}
	CONSOLE.CursorStatus( cursor );
	return &path[0];
}


/*

PARAGRAPH CLASS

*/


PARAGRAPH::PARAGRAPH() {
	ClearData();
	margin = 0;
	center = 0;
	line_len = CONSOLE.ScreenLen() - 4;
	console_len = CONSOLE.ScreenLen() - 4;
	return;
}

void PARAGRAPH::ClearData() {
	for( int i = 0; i < ( sizeof( msg_data ) / sizeof( char ) ); i++ ) {
		msg_data[i] = '\0';
	}
	return;
}

void PARAGRAPH::ParaData( char* input ) {
	AppendString( msg_data, input );
	return;
}

void PARAGRAPH::SetMargin( int len ) {
	margin = len;
	return;
}

void PARAGRAPH::SetMaxLineLen( int len ) {
	line_len = len;
	return;
}

void PARAGRAPH::CenterAlign( int set = 1 ) {
	if( set ) {
		center = 1;
	}
	else {
		center = 0;
	}
	return;
}

int PARAGRAPH::CenterAlignLen( int string_len )
{
	int len = string_len;
	int s_len = console_len;
	
	switch( len % 2)
	{
		case 0:
			len = ( len / 2 ) - 1;
			break;
		case 1:
			len = ( len - 1 ) / 2;
			break;
	}
	
	switch( s_len % 2 )
	{
		case 0:
			s_len = ( s_len / 2 ) + 1;
			break;
		case 1:
			s_len = ( s_len + 1 ) / 2;
			break;
	}
	
	return ( s_len - len );
}

void PARAGRAPH::WritePara() {
	COORD pos;
	pos.X = margin;
	pos.Y = CONSOLE.getY();
	CONSOLE.gotoXY( pos.X, pos.Y, 0 );
	
	int displacement = 0;
	if( margin + line_len > CONSOLE.ScreenLen() ) {
		displacement = CONSOLE.ScreenLen();
	}
	else {
		displacement = margin + line_len;
	}
	
	int temp = 0;
	char msg_temp[200];
	for( int i = 0; i <= strlen( msg_data ); i++, pos.X++ ) {
		if( pos.X == ( displacement ) || msg_data[i] == '\0' || msg_data[i] == '\n' ) {
			for( int j = 0; j < sizeof( msg_temp ); j++ ) {
				msg_temp[j] = '\0';
			}
			while( !( msg_data[i] == ' ' || msg_data[i] == '\n' || msg_data[i] == '\0' ) ) {
				i--;
			}
			strcpy( msg_temp, CutString( msg_data, temp, i - 1 ) );
			if( center ) {
				CONSOLE.gotoXY( CenterAlignLen( strlen( msg_temp ) ), pos.Y, 0 );
			}
			cout.write( msg_temp, strlen( msg_temp ) );
			temp = i + 1;
			pos.X = margin;
			pos.Y += 1;
			if( msg_data[i] != '\0' ) {
				CONSOLE.gotoXY( pos.X, pos.Y, 0 );
			}
		}
	}
	return;
}

char* PARAGRAPH::ReturnAsString() {
	char msg_rtrnd[100000];
	for( int i = 0; i < sizeof( msg_rtrnd ); i++ ) {
		msg_rtrnd[i] = '\0';
	}
	COORD pos;
	pos.X = CONSOLE.getX() + margin;
	pos.Y = CONSOLE.getY();
	for( int i = 0; i < margin; i++ ) {
		msg_rtrnd[i] = ' ';
	}
	
	int displacement = 0;
	if( margin + line_len > CONSOLE.ScreenLen() ) {
		displacement = CONSOLE.ScreenLen();
	}
	else {
		displacement = margin + line_len;
	}
	
	int temp = 0;
	char msg_temp[200];
	for( int i = 0; i <= strlen( msg_data ); i++, pos.X++ ) {
		if( pos.X == ( displacement ) || msg_data[i] == '\0' || msg_data[i] == '\n' ) {
			for( int j = 0; j < sizeof( msg_temp ); j++ ) {
				msg_temp[j] = '\0';
			}
			while( !( msg_data[i] == ' ' || msg_data[i] == '\n' || msg_data[i] == '\0' ) ) {
				i--;
			}
			strcpy( msg_temp, CutString( msg_data, temp, i - 1 ) );
			if( center ) {
				for( int j = 0; j < CenterAlignLen( strlen( msg_temp ) ); j++ ) {
					AppendString( msg_rtrnd, " " );
				}
			}
			AppendString( msg_rtrnd, msg_temp );
			temp = i + 1;
			pos.X = margin;
			pos.Y += 1;
			AppendString( msg_rtrnd, "\n" );
			for( int j = 0; j < pos.X; j++ ) {
				AppendString( msg_rtrnd, " " );
			}
		}
	}
	AppendString( msg_rtrnd, "\0" );
	return &msg_rtrnd[0];
}



/*

ANIMATION CLASS

*/


void ANIMATION::ClearData() {
	for( int i = 0; i < sizeof( msg ); i++ ) {
		msg[i] = '\0';
	}
	
	time = 0;
	
	return;
}


ANIMATION::ANIMATION() {
	ClearData();
	time = 2000;
	return;
}

ANIMATION::ANIMATION( char* message ) {
	ClearData();
	strcpy( msg, message );
	return;
}

ANIMATION::ANIMATION( int duration_in_ms ) {
	ClearData();
	time = duration_in_ms;
	return;
}

ANIMATION::ANIMATION( char* message, int duration_in_ms ) {
	ClearData();
	strcpy( msg, message );
	time = duration_in_ms;
	return;
}


void ANIMATION::SetMessage( char* message ) {
	strcpy( msg, message );
	return;
}

void ANIMATION::SetDuration( int duration_in_ms ) {
	time = duration_in_ms;
	return;
}



void ANIMATION::TypeWriter() {
	int len = 0;
	for( int i = 0; i < strlen( msg ); i++ ) {
		if( !(msg[i] == '\n' || msg[i] == ' ' ) ) {
			len++;
		}
	}
	
	int delay = time / len;
	
	for( int i = 0; i < strlen( msg ); i++ ) {
		if( !(msg[i] == '\n' || msg[i] == ' ' ) ) {
			Sleep( delay );
			cout << msg[i];
		}
		else {
			cout << msg[i];
		}
	}
	return;
}

void ANIMATION::Random() {
	char message[100000];
	for( int i = 0; i < sizeof( message ); i++ ) {
		message[i] = '\0';
	}
	strcpy( message, msg );
	
	int len = 0;
	for( int i = 0; i < strlen( msg ); i++ ) {
		if( !(msg[i] == '\n' || msg[i] == ' ' ) ) {
			len++;
		}
	}
	
	int delay = time / len;
	
	CONSOLE.CursorStatus( 0 );
	
	srand( len );
	COORD pos;
	pos.X = CONSOLE.getX();
	pos.Y = CONSOLE.getY();
	
	int x = 0, y = 0, y_max = 0;
	int i = 0;
	int counter = 0;
	
	while( counter != len ) {
		i = rand() % strlen( msg );
		if( message[i] != '\0' && message[i] != '\n' && message[i] != ' ' ) {
			Sleep( delay );
			for( int j = 0; j < i; j++ ) {
				if( message[j] == '\n' ) {
					y++;
				}
			}
			if( y_max < y ) {
				y_max = y;
			}
			for( int j = i; j > 0; j-- ) {
				if( message[j - 1] != '\n' ) {
					x++;
				}
				else {
					break;
				}
			}
			
			CONSOLE.gotoXY( x, y, 1 );
			
			cout << message[i];
			x = 0; y = 0;
			message[i] = '\0';
			counter++;
			CONSOLE.gotoXY( pos.X, pos.Y, 0 );
		}
	}
	
	CONSOLE.gotoXY( 0, CONSOLE.getY() + y_max + 1, 0 );
	CONSOLE.CursorStatus( 1 );
	
	return;
}


/*

WINDOWS CONSOLE CLASS

*/

void WINDOWS::LoadData() {
	console = GetStdHandle( STD_OUTPUT_HANDLE );
	GetConsoleScreenBufferInfo( console, &screen );
	GetConsoleCursorInfo( console, &cursor );
	return;
}


WINDOWS::WINDOWS() {
	LoadData();
	pos.X = 0;
	pos.Y = 0;
}

int WINDOWS::CursorStatus() {
	LoadData();
	return (int)cursor.bVisible;
}

void WINDOWS::CursorStatus( int SetTo ) {
	LoadData();
	cursor.bVisible = SetTo;
	SetConsoleCursorInfo( console, &cursor );
	return;
}


int WINDOWS::ScreenLen() {
	LoadData();
	return screen.dwMaximumWindowSize.X;
}


int WINDOWS::getX() {
	LoadData();
	return screen.dwCursorPosition.X;
}


int WINDOWS::getY() {
	LoadData();
	return screen.dwCursorPosition.Y;
}


void WINDOWS::gotoXY( int X_coord, int Y_coord, int relative_movement = 0 ) {
	LoadData();
	COORD pos_temp;
	if( relative_movement ) {
		pos_temp.X = getX() + X_coord;
		pos_temp.Y = getY() + Y_coord;
	}
	else {
		pos_temp.X = X_coord;
		pos_temp.Y = Y_coord;
	}
	SetConsoleCursorPosition( console, pos_temp );
	return;
}


void WINDOWS::CleanSlate( int X_coord, int Y_coord ) {
	LoadData();
	
	int counter = 0;
	COORD pos_orig;
	pos_orig.X = getX();
	pos_orig.Y = getY();
	
	pos.X = X_coord;
	pos.Y = Y_coord;
	
	if( pos.Y == pos_orig.Y ) {
		pos.X = X_coord;
		while( pos.X != pos_orig.X ) {
			counter++;
			pos.X++;
		}
	}
	else {
		for( int i = X_coord; i < ScreenLen(); i++ ) {
			counter++;
		}
	}
	
	pos.X = 0;
	pos.Y += 1;
	
	while( pos_orig.Y - pos.Y  >= 1 ) {
		for( int i = 0; i < ScreenLen(); i++ ) {
			counter++;
		}
		pos.Y++;
	}
	
	while( pos.X != pos_orig.X ) {
		counter++;
		pos.X++;
	}
	
	char output_clear[counter];
	
	for( int i = 0; i < ( sizeof( output_clear ) / sizeof( char ) ); i++ ) {
		output_clear[i] = ' ';
	}
	
	gotoXY( X_coord, Y_coord, 0 );
	
	cout.write( output_clear, sizeof( output_clear ) );
	
	gotoXY( X_coord, Y_coord, 0 );

	return;
}


/*

CRYPTEX CLASS

*/

void CRYPTEX::ClearData() {
	for( int i = 0; i < sizeof( file ) / sizeof( file[0] ); i++ ) {
		for( int j = 0; j < sizeof( file[0] ); j++ ) {
			file[i][j] = '\0';
		}
	}
	for( int i = 0; i < sizeof( Key ); i++ ) {
		Key[i] = '\0';
	}
	for( int i = 0; i < sizeof( savefile ); i++ ) {
		savefile[i] = '\0';
	}
	n_lines = 0;
	caesar_c = 0;
	caesar_c_num = 0;
	
	for( int i = 0; i < sizeof( Alpha ); i++ ) {
		Alpha[i] = 'A' + i;
	}
	for( int i = 0; i < sizeof( E_Alpha ); i++ ) {
		E_Alpha[i] = '\0';
	}
	return;
}

CRYPTEX::CRYPTEX() {
	ClearData();
	return;
}

CRYPTEX::CRYPTEX( char* message ) {
	ClearData();
	SetMessage( message );
	return;
}

CRYPTEX::CRYPTEX( char* message, char* PassKey ) {
	ClearData();
	SetMessage( message );
	SetPassKey( PassKey );
	return;
}

CRYPTEX::CRYPTEX( char* message, char* PassKey, char* output ) {
	ClearData();
	SetMessage( message );
	SetPassKey( PassKey );
	SetOutputLocation( output );
	return;
}


int CRYPTEX::LoadFile( char* location ) {
	for( int i = 0; i < sizeof( file ) / sizeof( file[0] ); i++ ) {
		for( int j = 0; j < sizeof( file[0] ); j++ ) {
			file[i][j] = '\0';
		}
	}
	n_lines = 0;
	
	fstream fin;
	fin.open( location, ios::in );
	
	char line[200];
	if( fin ) {
		while( fin.getline( line, 200 ) ) {
			AppendString( file[n_lines], line );
			n_lines++;
		}
		fin.close();
		return 1;
	}
	else {
		fin.close();
		return 0;
	}
}

void CRYPTEX::SetMessage( char* message ) {
	for( int i = 0; i < sizeof( file ) / sizeof( file[0] ); i++ ) {
		for( int j = 0; j < sizeof( file[0] ); j++ ) {
			file[i][j] = '\0';
		}
	}
	n_lines = 0;
	
	int j = 0;
	for( int i = 0; i < sizeof( message ); i++ ) {
		if( message[i] != '\n' ) {
			file[n_lines][i - j] = message[i];
		}
		else {
			n_lines++;
			j = i;
		}
	}
	return;
}


void CRYPTEX::SetPassKey( char* key ) {
	strcpy( Key, key );
	GeneratePassData();
	
	return;
}

void CRYPTEX::GeneratePassData() {
	char PassKey[100];
	for( int i = 0; i < sizeof( PassKey ); i++ ) {
		PassKey[i] = '\0';
	}
	strcpy( PassKey, Key );
	char Temp[100];
	for( int i = 0; i < strlen( PassKey ); i++ ) {
		if( isdigit( PassKey[i] ) ) {
			caesar_c += PassKey[i] - '0';
		}
	}
	caesar_c_num = caesar_c % 10;
	if( !( caesar_c_num ) ) {
		caesar_c_num = 1;
	}
	
	caesar_c %= 26;
	if( !( caesar_c ) ) {
		caesar_c = 1;
	}
	
	for( int i = 0; i < sizeof( Temp ); i++ ) {
		Temp[i] = '\0';
	}
	for( int i = 0, j = 0; i < strlen( PassKey ); i++ ) {
		if( isalpha( PassKey[i] ) ) {
			Temp[j] = PassKey[i];
			j++;
		}
	}
	for( int i = 0; i < sizeof( PassKey ); i++ ) {
		PassKey[i] = '\0';
	}
	strcpy( PassKey, Temp );
	
	for( int i = 0; i < strlen( PassKey ); i++ ) {
		if( isupper( PassKey[i] ) ) {
			PassKey[i] = tolower( PassKey[i] );
		}
	}
	
	int flag = 0;
	int counter = 0;
	for( int i = 0; i < sizeof( Temp ); i++ ) {
		Temp[i] = '\0';
	}
	for( int i = 0; i < strlen( PassKey ); i++ ) {
		flag = 0;
		for( int j = 0; j < strlen( Temp ); j++ ) {
			if( Temp[j] == PassKey[i] ) {
				flag = 1;
			}
		}
		if( !flag ) {
			Temp[counter] = PassKey[i];
			counter++;
		}
	}
	for( int i = 0; i < sizeof( PassKey ); i++ ) {
		PassKey[i] = '\0';
	}
	strcpy( PassKey, Temp );
	
	for( int i = 0; i < strlen( PassKey ); i++ ) {
		E_Alpha[i] = PassKey[i];
	}
	
	flag = 0, counter = strlen( E_Alpha );
	for( int i = 0; i < sizeof( Alpha ); i++ ) {
		for( int j = 0; j < strlen( E_Alpha ); j++ ) {
			if( E_Alpha[j] == Alpha[i] ) {
				flag = 1;
			}
		}
		if( !flag ) {
			E_Alpha[counter] = Alpha[i];
			counter++;
		}
	}
	
	return;
}

int CRYPTEX::SetOutputLocation( char* location ) {
	fstream fout;
	fout.open( location, ios::out | ios::trunc );
	if( fout ) {
		strcpy( savefile, location );
		fout.close();
		return 1;
	}
	else {
		fout.close();
		return 0;
	}
}


void CRYPTEX::RunEncryption() {
	int scrambler = 0;
	for( int i = 0; i < n_lines; i++ ) {
		for( int j = 0; j < strlen( file[i] ); j++ ) {
			scrambler = i + j;
			if( isalpha( file[i][j] ) ) {
				scrambler %= 26;
				
				if( isalpha( file[i][j] + caesar_c ) ) {
					file[i][j] += caesar_c;
				}
				else {
					file[i][j] += ( caesar_c - 26 );
				}
				
				if( islower( file[i][j] ) ) {
					for( int k = 0; k < sizeof( Alpha ); i++ ) {
						if( file[i][j] == Alpha[k] ) {
							file[i][j] = E_Alpha[k];
						}
					}
				}
				else {
					for( int k = 0; k < sizeof( Alpha ); i++ ) {
						if( file[i][j] == toupper( Alpha[k] ) ) {
							file[i][j] = toupper( E_Alpha[k] );
						}
					}
				}
				
				if( isalpha( file[i][j] + scrambler ) ) {
					file[i][j] += scrambler;
				}
				else {
					file[i][j] += ( scrambler - 26 );
				}
			}
			else if( isdigit( file[i][j] ) ) {
				scrambler %= 10;
				
				if( isdigit( file[i][j] + caesar_c_num ) ) {
					file[i][j] += caesar_c_num;
				}
				else {
					file[i][j] += ( caesar_c_num - 10 );
				}
				
				if( isdigit( file[i][j] + scrambler ) ) {
					file[i][j] += scrambler;
				}
				else {
					file[i][j] += ( scrambler - 26 );
				}
			}
		}
	}
	
	if( strlen( savefile ) ) {
		OutputOnDisk( savefile );
	}
	return;
}

void CRYPTEX::RunDecryption() {
	int scrambler = 0;
	for( int i = 0; i < n_lines; i++ ) {
		for( int j = 0; j < strlen( file[i] ); j++ ) {
			scrambler = i + j;
			if( isalpha( file[i][j] ) ) {
				scrambler %= 26;
				
				if( isalpha( file[i][j] - scrambler ) ) {
					file[i][j] -= scrambler;
				}
				else {
					file[i][j] -= ( scrambler - 26 );
				}
				
				if( islower( file[i][j] ) ) {
					for( int k = 0; k < sizeof( E_Alpha ); i++ ) {
						if( file[i][j] == E_Alpha[k] ) {
							file[i][j] = Alpha[k];
						}
					}
				}
				else {
					for( int k = 0; k < sizeof( E_Alpha ); i++ ) {
						if( file[i][j] == toupper( E_Alpha[k] ) ) {
							file[i][j] = toupper( Alpha[k] );
						}
					}
				}
				
				if( isalpha( file[i][j] - caesar_c ) ) {
					file[i][j] -= caesar_c;
				}
				else {
					file[i][j] -= ( caesar_c - 26 );
				}
			}
			else if( isdigit( file[i][j] ) ) {
				scrambler %= 10;
				
				if( isdigit( file[i][j] - scrambler ) ) {
					file[i][j] -= scrambler;
				}
				else {
					file[i][j] -= ( scrambler - 26 );
				}
				
				if( isdigit( file[i][j] - caesar_c_num ) ) {
					file[i][j] -= caesar_c_num;
				}
				else {
					file[i][j] -= ( caesar_c_num - 10 );
				}
			}
		}
	}
	
	if( strlen( savefile ) ) {
		OutputOnDisk( savefile );
	}
	return;
}

int CRYPTEX::OutputOnDisk( char* location ) {
	fstream fout;
	fout.open( location, ios::out | ios::trunc );
	
	if( fout ) {
		for( int i = 0; i < n_lines; i++ ) {
			fout.write( file[i], strlen( file[i] ) );
			fout << endl;
		}
		fout.close();
		return 1;
	}
	else {
		fout.close();
		return 0;
	}
}


char* CRYPTEX::ReturnAsString() {
	char file_temp[2000000];
	for( int i = 0; i < sizeof( file_temp ); i++ ) {
		file_temp[i] = '\0';
	}
	
	for( int i = 0; i < n_lines; i++ ) {
		AppendString( file_temp, file[i] );
		AppendString( file_temp, "\n" );
	}
	return &file_temp[0];
}


/*

CLASS LIST

*/

LIST::LIST() {
	for( int i = 0; i < sizeof( list_data ) / sizeof( list_data[0] ); i++ ) {
		for( int j = 0; j < sizeof( list_data[0] ); j++ ) {
			list_data[i][j] = '\0';
		}
	}
	for( int i = 0; i < sizeof( level_data ) / sizeof( int ); i++ ) {
		level_data[i] = 1;
	}
	for( int i = 0; i < sizeof( box ); i++ ) {
		box[i] = '\0';
	}
	strcpy( box, "[   ]" );
	index = 0;
	pos_start.X = 0;
	pos_start.Y = 0;
	pos_end.X = 0;
	pos_end.Y = 0;
	line_count = 1;
	return;
}

void LIST::AddListData( int level, char* ListItem ) {
	strcpy( list_data[index], ListItem );
	level_data[index] = level;
	index++;
	return;
}


void LIST::WriteList( int level ) {
	pos_start.X = CONSOLE.getX();
	pos_start.Y = CONSOLE.getY();
	
	PARAGRAPH list_item[index];
	int margin = 0;
	margin = ( level )*strlen( box ) + 1;
	
	for( int i = 0; i < index; i++ ) {
		if( level_data[i] == level ) {
			list_item[i].SetMargin( margin );
			list_item[i].SetMaxLineLen( 50 );
			list_item[i].ParaData( list_data[i] );
			
			CONSOLE.gotoXY( margin - strlen( box ) - 1, CONSOLE.getY(), 0 );
			cout.write( box, strlen( box ) );
			cout << " ";
			list_item[i].WritePara();
			pos_end.Y = CONSOLE.getY();
			cout << endl;
		}
	}
	CONSOLE.gotoXY( ( margin / 2 ) - 1, pos_start.Y, 0 );
	return;
}

void LIST::RadioList() {
	int index_temp = 0;
	int level = 1;
	
	pos_start.Y = CONSOLE.getY();
	WriteList( 1 );
	
	char selection = '\0';
	do{
		line_count = 1;
		selection = getch();
		switch( selection ) {
			case KEY_UP:
				if( CONSOLE.getY() > pos_start.Y ) {
					for( int i = 0; i < strlen( list_data[index_temp - 1] ); i++ ) {
						if( list_data[index_temp - 1][i] == '\n' ) {
							line_count++;
						}
					}
					CONSOLE.gotoXY ( 0, -line_count, 1 );
					do {
						index_temp--;
					}while( level_data[index_temp] != level );
					cout << index_temp;
				}
				break;
			case KEY_DOWN:
				if( CONSOLE.getY() < pos_end.Y ) {
					for( int i = 0; i < strlen( list_data[index_temp] ); i++ ) {
						if( list_data[index_temp][i] == '\n' ) {
							line_count++;
						}
					}
					CONSOLE.gotoXY( 0, line_count, 1 );
					index_temp++;
				 	do{
						index_temp++;
					}while( level_data[index_temp] != level );
					cout << index_temp;
				}
				break;
		}
	}while( selection != KEY_ENTER );
	return;
}



/*

ASCII HEADING

*/


char* AsciiHeading( char* message ) {
	int m = 0;
	int x = 0;
	char message_refined[200];
	char var[5000];
	char msg[20][100];
	
	for( int i = 0; i < 5000; i++ ) {
		var[i] = '\0';
	}
	
	for( int i = 0; i < 200; i++ ) {
		message_refined[i] = '\0';
	}
	
	for( int i = 0; i < 20; i++ ) {
		for( int j = 0; j < 100; j++ ) {
			msg[i][j] = '\0';
		}
	}

	for( int i = 0, j = 0; i < strlen( message ); i++, j++ ) {
		if( islower( message[i] ) ) {
			message_refined[j] = toupper( message[i] );
		}
		else if( isalpha( message[i] ) ) {
			message_refined[j] = message[i];
		}
		else if( message[i] == ' ' || message[i] == '\n' ) {
			message_refined[j] = message[i];
		}
		else {
			j--;
		}
	}
	
	int counter = 0;
	for( int i = 0, pos = 0, letter_count = 0; i <= strlen( message_refined ); i++ ) {
		if( ( message_refined[i] == '\n' || message_refined[i] == '\0' ) ) {
			strcpy( msg[counter], CutString( message_refined, pos, ( i - 1 ) ) );
			pos = i + 1;
			counter++;
			letter_count = 0;
		}
		else {
			letter_count++;
		}
		
		if( letter_count * 12 >= CONSOLE.ScreenLen() ) {
			int j = i;
			while( message_refined[j] != ' ' && j > pos ) {
				j--;
			}
			if( j > pos ) {
				i = j;
			}
			strcpy( msg[counter], CutString( message_refined, pos, i ) );
			pos = i + 1;
			counter++;
			letter_count = 0;
		}
	}
	
	/*
	for( int i = 0; i < counter; i++ ) {
		cout.write( msg[i], strlen( msg[i] ) ); cout << endl;
	}
	getch();
	*/

	
	for( int j = 0; j < counter; j++ ) {
		x = 0;
		while( x < 6 ) {
			for( int i = 0; i < strlen( msg[j] ); i++ ) {
				switch( msg[j][i] ) {
					case ' ':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 2:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
						}						
						break;
					case 'A':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 2:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = '/'; m++;
								var[m] = '_'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '\\'; m++;
								break;
						}
						break;
					case 'B':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = ')'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = '<'; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = ')'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								break;
						}
						break;
					case 'C':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
						}
						break;
					case 'D':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								break;
						}
						break;
					case 'E':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
						}
						break;
					case 'F':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
						}
						break;
					case 'G':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 5:
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
						}
						break;
					case 'H':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
						}
						break;
					case 'I':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
							case 2:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
						}
						break;
					case 'J':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 2:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 3:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 5:
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								break;
						}
						break;
					case 'K':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '\''; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '<'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '.'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '\\'; m++;
								break;
						}
						break;
					case 'L':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
						}
						break;
					case 'M':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '\\'; m++;
								var[m] = '/'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
						}
						break;
					case 'N':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '.'; m++;
								var[m] = ' '; m++;
								var[m] = '`'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
						}
						break;
					case 'O':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 5:
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								break;
						}
						break;
					case 'P':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ')'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
						}
						break;
					case 'Q':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 5:
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '\\'; m++;
								break;
						}
						break;
					case 'R':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ')'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '\\'; m++;
								break;
						}
						break;
					case 'S':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '('; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ')'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								break;
						}
						break;
					case 'T':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
							case 2:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
						}
						break;
					case 'U':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 5:
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								break;
						}
						break;
					case 'V':
						switch( x ) {
							case 0:
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								break;
							case 1:
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								break;
							case 2:
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
						}
						break;
					case 'W':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 2:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 3:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 4:
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								break;
							case 5:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
						}
						break;
					case 'X':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								break;
							case 1:
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								break;
							case 2:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = 'V'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '>'; m++;
								var[m] = ' '; m++;
								var[m] = '<'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '.'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = '_'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '\\'; m++;
								break;
						}
						break;
					case 'Y':
						switch( x ) {
							case 0:
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								break;
							case 1:
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								break;
							case 2:
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = '_'; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '\\'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
						}
						break;
					case 'Z':
						switch( x ) {
							case 0:
								var[m] = ' '; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								break;
							case 1:
								var[m] = '|'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								break;
							case 2:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								break;
							case 3:
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = ' '; m++;
								break;
							case 4:
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = ' '; m++;
								var[m] = '/'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = ' '; m++;
								break;
							case 5:
								var[m] = '/'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '_'; m++;
								var[m] = '|'; m++;
								break;
						}
						break;

				}
				var[m] = ' '; m++;
			}
			var[m] = '\n'; m++;
			x++;
		}
		var[m] = '\n'; m++;
	}
	var[m] = '\0';
	return &var[0];
}


#endif LEXICON_H


