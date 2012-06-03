#include <Windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>

using namespace std;

HANDLE handle_out;
int work = 0; 

void clear_console()
{
	COORD coordScreen = { 0, 0 };    // home for the cursor 
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; 
	DWORD dwConSize;
	
	// Get the number of character cells in the current buffer. 
	if (!GetConsoleScreenBufferInfo(handle_out, &csbi))
	{
		return; 
	}

	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// Fill the entire screen with blanks.
	
	if( !FillConsoleOutputCharacter(handle_out,        // Handle to console screen buffer 
									(TCHAR) ' ',     // Character to write to the buffer
									dwConSize,       // Number of cells to write 
									coordScreen,     // Coordinates of first cell 
									&cCharsWritten ))// Receive number of characters written
	{
		return;
	}
	
	// Get the current text attribute.

	if( !GetConsoleScreenBufferInfo( handle_out, &csbi ))
	{
		return;
	}

	// Set the buffer's attributes accordingly.

	if( !FillConsoleOutputAttribute(handle_out,         // Handle to console screen buffer 
									csbi.wAttributes, // Character attributes to use
									dwConSize,        // Number of cells to set attribute 
									coordScreen,      // Coordinates of first cell 
									&cCharsWritten )) // Receive number of characters written
	{
		return;
	}

	// Put the cursor at its home coordinates.

	SetConsoleCursorPosition( handle_out, coordScreen );

}

int main(int argc, char** argv)
{	
	
	handle_out = GetStdHandle(STD_OUTPUT_HANDLE); 


	while (true)
	{
		clear_console();

		cout << work << "%" << endl; 
		
		++work; 
		Sleep(500); 
		
		if (work > 100)
		{
			break; 
		}
	} 
	
	return 0; 
}