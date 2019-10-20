// scantest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "scan.h"

int _tmain(int argc, _TCHAR* argv[])
{
	open_scan("scan_test.jpg");
	close_scan();

	return 0;
}

