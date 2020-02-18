// scan.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "scan.h"

#include "Subsys/CodeGeneration.h"
#include "System/RaptorConfig.h"
#include "System/RaptorErrorManager.h"

#include "ToolBox/Imaging.h"
#include "System/Image.h"


RAPTOR_NAMESPACE


extern bool extracttable(CImage &in, const std::string &out);
extern bool extractnum(CImage &in, const std::string &out);

//	Global variable to hold a set of results for a set of scan (typically a classroom)
std::vector<SCAN>	global_scans;
//	Global variable to hold an image for successive operations
CImage				*current_scan = NULL;


extern SCAN_API int scan_version(void)
{
	return 12345;
}

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

SCAN_API int open_scan(const char* scan)
{
	CRaptorErrorManager *mgr = Raptor::GetErrorManager();
	
	CImage image;
	const CVaArray<CImage::IImageOP*>& ops = {};
	if (image.loadImage(scan, ops))
	{
		if (!extracttable(image, "table.tga"))
		{
			mgr->generateRaptorError(CPersistence::CPersistenceClassID::GetClassId(),
									 CRaptorErrorManager::RAPTOR_ERROR,
									 "Impossible d'extraire la table des réponses");
			return 0;
		}
		else if (!extractnum(image, "num.tiff"))
		{
			mgr->generateRaptorError(CPersistence::CPersistenceClassID::GetClassId(),
									 CRaptorErrorManager::RAPTOR_ERROR,
									 "Impossible d'extraire l'identifiant");
			return 0;
		}
		else
			return 1;
	}
	else
	{
		mgr->generateRaptorError(CPersistence::CPersistenceClassID::GetClassId(),
								 CRaptorErrorManager::RAPTOR_FATAL,
								 "Impossible d'ouvrir l'image source");
		return 0;
	}
}


SCAN_API int open_doc(const char* doc)
{
	if (NULL == doc)
		return false;

	stringstream cmd;
	cmd << "F:\\QCM\\ghostscript\\gswin32c.exe -dNOPAUSE -dBATCH -sDEVICE=jpeg -sOutputFile=scan_p%d.jpg -r300x300 ";
	cmd << doc;
	cmd << std::ends;
	int ex = system(cmd.str().c_str());

	return ex;
}

SCAN_API int close_doc()
{
	for (size_t i = 0; i < global_scans.size(); i++)
	{
		SCAN& g_scan = global_scans[i];
		if (g_scan.answers != NULL)
			delete[] g_scan.answers;
	}

	if (NULL != current_scan)
		delete current_scan;

	global_scans.clear();

	int num_scan = 1;
	int rt = 0;
	while (0 == rt)
	{
		stringstream doc;
		doc << "scan_p" << num_scan++ << ".jpg" << ends;
		rt = rt | _unlink(doc.str().c_str());
	}

	rt = rt | _unlink("rectified.jpg");
	rt = rt | _unlink("resized.jpg");

	return (0 == rt);
}

