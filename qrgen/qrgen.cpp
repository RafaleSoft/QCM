// qrgen.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "qrgen.h"
#include "QR-Code-generator\c\qrcodegen.h"

#include "zlib.h"
#include "png.h"


QRGEN_API bool qrgen_getModule(const unsigned char qrcode[], int x, int y)
{
	return qrcodegen_getModule(qrcode, x, y);
}

QRGEN_API int qrgen_getSize(const unsigned char qrcode[])
{
	return qrcodegen_getSize(qrcode);
}

QRGEN_API bool qrgen_encodeText(const char *text,
								unsigned char tempBuffer[],
								unsigned char qrcode[],
								enum qrcodegen_Ecc ecl,
								int minVersion,
								int maxVersion,
								enum qrcodegen_Mask mask,
								bool boostEcl)
{
	return qrcodegen_encodeText(text, tempBuffer, qrcode,
								ecl, minVersion, maxVersion, 
								mask, boostEcl);

}

QRGEN_API int qrgen_version(void)
{
	return 54321;
}
