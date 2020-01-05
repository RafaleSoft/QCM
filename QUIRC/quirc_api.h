// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the QUIRC_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// QUIRC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef QUIRC_EXPORTS
#define QUIRC_API __declspec(dllexport)
#else
#define QUIRC_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	QUIRC_API int  quirc_resize_api(struct quirc *q, int w, int h);

	QUIRC_API unsigned char * quirc_begin_api(struct quirc *q, int *w, int *h);

	QUIRC_API const char *quirc_version_api(void);

	QUIRC_API struct quirc *quirc_new_api(void);

	QUIRC_API void quirc_destroy_api(struct quirc *q);

	QUIRC_API void quirc_end_api(struct quirc *q);

	QUIRC_API int quirc_count_api(const struct quirc *q);

	QUIRC_API void quirc_extract_api(const struct quirc *q, int index,struct quirc_code *code);

	QUIRC_API quirc_decode_error_t quirc_decode_api(const struct quirc_code *code,struct quirc_data *data);

	QUIRC_API const char *quirc_strerror_api(quirc_decode_error_t err);

#ifdef __cplusplus
}
#endif
