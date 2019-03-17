// quirc.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "quirc/lib/quirc.h"
#include "quirc_api.h"

QUIRC_API int quirc_resize_api(struct quirc *q, int w, int h)
{
	return quirc_resize(q, w, h);
}

QUIRC_API unsigned char * quirc_begin_api(struct quirc *q, int *w, int *h)
{
	return quirc_begin(q, w, h);
}

QUIRC_API const char *quirc_version_api(void)
{
	return quirc_version();
}

QUIRC_API struct quirc *quirc_new_api(void)
{
	return quirc_new();
}

QUIRC_API void quirc_destroy_api(struct quirc *q)
{
	quirc_destroy(q);
}

QUIRC_API void quirc_end_api(struct quirc *q)
{
	quirc_end(q);
}
QUIRC_API int quirc_count_api(const struct quirc *q)
{
	return quirc_count(q);
}

QUIRC_API void quirc_extract_api(const struct quirc *q, int index, struct quirc_code *code)
{
	quirc_extract(q, index, code);
}

QUIRC_API quirc_decode_error_t quirc_decode_api(const struct quirc_code *code, struct quirc_data *data)
{
	return quirc_decode(code, data);
}

QUIRC_API const char *quirc_strerror_api(quirc_decode_error_t err)
{
	return quirc_strerror(err);
}

// This is the constructor of a class that has been exported.s
// see quirc.h for the class definition
//Cquirc::Cquirc()
//{
//	return;
//}
