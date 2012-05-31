#include "renderer_report.h"

#include <iostream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>

void c_renderer_report::open_console_wnd()
{
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;
	
	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in; 	
}

void c_renderer_report::update()
{
	m_work_done++; 
	int p = ((float)m_work_done / (float)m_total_work) * 100; 

	std::cout << p << std::endl; 
}