// scan.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "process.h"
#include "scan.h"

#include "Subsys/CodeGeneration.h"
#include "System/RaptorConfig.h"
#include "System/RaptorErrorManager.h"

#include "ToolBox/Imaging.h"
#include "System/Image.h"


RAPTOR_NAMESPACE



std::vector<SCAN> global_scans;



SCAN_API int init_scan(void)
{
	CRaptorConfig config;
	config.m_bRelocation = true;
	config.m_uiPolygons = 10000;
	config.m_uiVertices = 40000;
	config.m_logFile = "Scan.log";
	bool b = Raptor::glInitRaptor(config);

	if (b)
		b = CImaging::installImagers();

	return ((b) ? 1 : 0);
}

SCAN_API int release_scan(void)
{
	int c = close_scan();

	bool b = Raptor::glQuitRaptor();

	return ((1 == c) && b ? 1 : 0);
}


const uint8_t TRESH = 192;

bool normalize(CImage &in, const std::string &out)
{
	uint8_t *pixels = in.getPixels();
	if (NULL != pixels)
	{
		size_t w = in.getWidth();
		size_t h = in.getHeight();

		uint8_t *normalized = new uint8_t[w*h];
		memset(normalized, 0, w*h);

		for (size_t i = 0, j = 0; i < w*h * 4; i += 4, j++)
		{
			uint8_t r = pixels[i];
			uint8_t g = pixels[i + 1];
			uint8_t b = pixels[i + 2];

			uint8_t &n = normalized[j];

			if ((r >= TRESH) || (g >= TRESH) || (b >= TRESH))
				n = 255;
			else
				n = 0;
		}

		CImage tmp;
		tmp.allocatePixels(w, h);
		uint8_t *pixels2 = tmp.getPixels();
		if (NULL != pixels2)
		{
			for (size_t i = 0, j = 0; i < w*h * 4; i += 4, j++)
			{
				uint8_t n = normalized[j];

				pixels2[i] = n;
				pixels2[i + 1] = n;
				pixels2[i + 2] = n;
				pixels2[i + 3] = 255;
			}
		}

		CImage::IImageIO *io = in.getImageKindIO("tga");
		io->storeImageFile(out, &tmp);

		delete[] normalized;

		return true;
	}
	else
		return false;
}


SCAN_API int get_scan_answers(const SCAN* scan)
{
	if (NULL == scan)
		return 0;

	size_t nb_scans = global_scans.size();
	size_t index = nb_scans;
	for (size_t i = 0; (i < nb_scans) && (index == nb_scans); i++)
		if (global_scans[i].scan_id == scan->scan_id)
			index = i;

	if (index == global_scans.size())
	{
		std::stringstream msg;
		msg << "Questionnaire incorrect : scan_id ";
		msg << scan->scan_id << " introuvable";
		msg << ends;

		CRaptorErrorManager *mgr = Raptor::GetErrorManager();
		mgr->generateRaptorError(CPersistence::CPersistenceClassID::GetClassId(),
								 CRaptorErrorManager::RAPTOR_ERROR, msg.str());
		return 0;
	}

	SCAN& g_scan = global_scans[index];
	for (size_t i = 0; i < g_scan.num_answers; i++)
		if (i < scan->num_answers)
			scan->answers[i] = g_scan.answers[i];

	return 1;
}

