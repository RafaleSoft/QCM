// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the QRGEN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// QRGEN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef WIN32
	#ifdef QRGEN_EXPORTS
		#define QRGEN_API __declspec(dllexport)
	#else
		#define QRGEN_API __declspec(dllimport)
	#endif
#else
	#define QRGEN_API
#endif


#ifdef __cplusplus
extern "C" 
{
#endif

QRGEN_API bool qrgen_getModule(const unsigned char qrcode[], int x, int y);

QRGEN_API int qrgen_getSize(const unsigned char qrcode[]);

QRGEN_API bool qrgen_encodeText(const char *text, 
								unsigned char tempBuffer[], 
								unsigned char qrcode[],
								enum qrcodegen_Ecc ecl, 
								int minVersion, 
								int maxVersion, 
								enum qrcodegen_Mask mask, 
								bool boostEcl);

QRGEN_API int qrgen_version(void);

#ifdef __cplusplus
}
#endif