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

//	Initialize the scan library
SCAN_API int init_scan(void);

//	Quit the scan library
SCAN_API int release_scan(void);


//	Open a jpg image scan of a single QCM
SCAN_API int open_scan(const char* scan);

//	Releases all the resources allocated by open_scan calls.
SCAN_API int close_scan();

//	Extract QCM answers from current (last) opened scan.
SCAN_API int extract_scan_fills(size_t num_lines, size_t num_columns);

//	Open a pdf document with multiple QCM scans (1 per page)
SCAN_API int open_doc(const char* doc);

//	Orthorectify the current opened scan
SCAN_API int rectify_scan(const char* scan);

//	Releases all the resources allocated by open_doc calls.
SCAN_API int close_doc();



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

SCAN_API int get_scan_answers(const SCAN* scan);
