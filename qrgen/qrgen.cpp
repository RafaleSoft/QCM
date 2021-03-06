// qrgen.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "QR-Code-generator/c/qrcodegen.h"
#include "qrgen.h"

static const char* version = QRGEN_VERSION;

QRGEN_API int qrgen_getModule(const unsigned char qrcode[], int x, int y)
{
	return (qrcodegen_getModule(qrcode, x, y) ? 1 : 0);
}

QRGEN_API int qrgen_getSize(const unsigned char qrcode[])
{
	return qrcodegen_getSize(qrcode);
}

QRGEN_API int qrgen_encodeText(const char *text,
								unsigned char tempBuffer[],
								unsigned char qrcode[],
								enum qrcodegen_Ecc ecl,
								int minVersion,
								int maxVersion,
								enum qrcodegen_Mask mask,
								int boostEcl)
{
	return (qrcodegen_encodeText(	text, tempBuffer, qrcode,
									ecl, minVersion, maxVersion,
									mask, (boostEcl > 0) ? true : false) ? 1 : 0);

}

QRGEN_API const char* qrgen_version(void)
{
	return version;
}
