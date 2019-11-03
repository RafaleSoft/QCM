// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SCAN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SCAN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef SCAN_EXPORTS
#define SCAN_API __declspec(dllexport)
#else
#define SCAN_API __declspec(dllimport)
#endif

#include <stdint.h>


#define  DEFAULT_NUM_COLUMNS 5


SCAN_API bool open_scan(const char* scan);

SCAN_API bool close_scan();

SCAN_API bool extract_scan_fills(size_t num_lines, size_t num_columns = DEFAULT_NUM_COLUMNS);


typedef struct QCM_Answer_t
{
	uint32_t	question_id;
	uint8_t		question_value;
} QCM_Answer;

typedef struct SCAN_t
{
	uint32_t	scan_id;
	size_t		num_answers;
	QCM_Answer	*answers;
} SCAN;

SCAN_API bool get_scan_answers(const SCAN* scan);
