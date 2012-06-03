#include "renderer_report.h"

#include <iostream>


HANDLE g_handle_out; 

void c_renderer_report::open_console_wnd()
{
	AllocConsole();

	g_handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long) g_handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;
}

void c_renderer_report::clear_console_wnd()
{
	COORD coordScreen = { 0, 0 };    // home for the cursor 
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; 
	DWORD dwConSize;

	// Get the number of character cells in the current buffer. 
	if (!GetConsoleScreenBufferInfo(g_handle_out, &csbi))
	{
		return; 
	}

	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// Fill the entire screen with blanks.

	if( !FillConsoleOutputCharacter(g_handle_out,        // Handle to console screen buffer 
		(TCHAR) ' ',     // Character to write to the buffer
		dwConSize,       // Number of cells to write 
		coordScreen,     // Coordinates of first cell 
		&cCharsWritten ))// Receive number of characters written
	{
		return;
	}

	// Get the current text attribute.

	if( !GetConsoleScreenBufferInfo(g_handle_out, &csbi ))
	{
		return;
	}

	// Set the buffer's attributes accordingly.

	if( !FillConsoleOutputAttribute(g_handle_out,         // Handle to console screen buffer 
		csbi.wAttributes, // Character attributes to use
		dwConSize,        // Number of cells to set attribute 
		coordScreen,      // Coordinates of first cell 
		&cCharsWritten )) // Receive number of characters written
	{
		return;
	}

	// Put the cursor at its home coordinates.

	SetConsoleCursorPosition(g_handle_out, coordScreen );
	
}

void c_renderer_report::update()
{
	m_work_done++; 
	int p = ((float)m_work_done / (float)m_total_work) * 100; 

	clear_console_wnd();
	
	std::cout << "Rendering: " << p << "%" << std::endl; 
}

void c_renderer_report::add_sample_stats(c_render_sample_stats& stats)
{
	m_sample_stats.push_back(stats); 
}