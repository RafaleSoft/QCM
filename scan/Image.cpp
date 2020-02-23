/***************************************************************************/
/*                                                                         */
/*  Image.cpp                                                              */
/*                                                                         */
/*    Raptor OpenGL & Vulkan realtime 3D Engine SDK.                       */
/*                                                                         */
/*  Copyright 1998-2019 by                                                 */
/*  Fabrice FERRAND.                                                       */
/*                                                                         */
/*  This file is part of the Raptor project, and may only be used,         */
/*  modified, and distributed under the terms of the Raptor project        */
/*  license, LICENSE.  By continuing to use, modify, or distribute         */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/

#include "stdafx.h"

#if !defined(AFX_IMAGE_H__F545D0D5_5F10_4EFA_BE3B_3F3D34D4DBF3__INCLUDED_)
	#include "Image.h"
#endif

#include <map>
std::map<std::string, CImage::IImageIO*>	imageKindIO;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImage::CImage()
	:m_name(""), m_width(0), m_height(0), m_layers(1),
	m_pixelType(CGL_COLOR24_ALPHA), pixels(NULL)
{
}

CImage::~CImage()
{
	releasePixels();
}

CImage* CImage::createSubImage(size_t x, size_t y, size_t w, size_t h)
{
	if ((x > getWidth()) || (y > getHeight()))
		return NULL;

	if (x + w > getWidth())
		return NULL;
	if (y + h > getHeight())
		return NULL;

	if (NULL == pixels)
		return NULL;

	CImage *res = new CImage();
	if (res->allocatePixels(w, h, m_pixelType))
	{
		size_t y_end = y + h;
		size_t y_start = y;
		size_t x_start = x;
		size_t x_end = x + w;

		uint8_t *pixels2 = res->getPixels();
		if (NULL != pixels2)
		{
			uint8_t *src = getPixels();
			for (size_t v = y_start, j = 0; v < y_end; v++)
				for (size_t o = (w * v + x_start) * 4; o < (w * v + x_end) * 4; o++, j++)
					pixels2[j] = src[o];
		}
		else
		{
			float *pixels3 = res->getFloatPixels();
			if (NULL != pixels3)
			{
				float *src = getFloatPixels();
				for (size_t v = y_start, j = 0; v < y_end; v++)
					for (size_t o = (w * v + x_start) * 4; o < (w * v + x_end) * 4; o++, j++)
						pixels3[j] = src[o];
			}
		}
	}
		
	return res;
}

uint8_t* CImage::getPixels(uint32_t layer) const
{
	uint8_t* ret = NULL;
	if (layer >= m_layers)
		return ret;

	switch (m_pixelType)
	{
		case CGL_COLOR24:
		case CGL_COLOR24_ALPHA:
			ret = (uint8_t*)pixels + layer*(sizeof(uint32_t) * m_width*m_height);
			break;
		default:
			ret = NULL;
			break;
	}

	return ret;
}


float* CImage::getFloatPixels(uint32_t layer) const
{
	float* ret = NULL;
	if (layer >= m_layers)
		return ret;

	switch (m_pixelType)
	{
		case CGL_COLOR_FLOAT16:
		case CGL_COLOR_FLOAT16_ALPHA:
		case CGL_COLOR_FLOAT32:
		case CGL_COLOR_FLOAT32_ALPHA:
			ret = (float*)pixels + layer*(sizeof(float) * m_width*m_height);
			break;
		default:
			ret = NULL;
			break;
	}

	return ret;
}

bool CImage::loadImage(const std::string &filename,
					   const std::vector<CImage::IImageOP*>& ops)
{
	if (filename.empty())
		return false;

	bool ret = true;

	CImage::IImageIO *imager = CImage::getImageKindIO(filename);
	if (NULL != imager)
	{
		if (imager->loadImageFile(filename, this))
		{
			m_name = filename;
			for (size_t i = 0; i < ops.size();i++)
			{
				CImage::IImageOP *op = ops[i];
				if (NULL != op)
					op->apply(this);
			}
		}
		else	// load from file failed
		{
			/*
			CRaptorMessages::MessageArgument arg;
			arg.arg_sz = filename.data();
			vector<CRaptorMessages::MessageArgument> args;
			args.push_back(arg);
			Raptor::GetErrorManager()->generateRaptorError(CTextureFactory::CTextureFactoryClassID::GetClassId(),
														   CRaptorErrorManager::RAPTOR_WARNING,
														   CRaptorMessages::ID_TEXTURE_MISS, 
														   __FILE__, __LINE__, args);
														   */
			ret = false;
		}
	}
	else
		ret = false;

	return ret;
}

void CImage::releasePixels(void)
{
	if (pixels != NULL)
	{
		switch (m_pixelType)
		{
			case CGL_COLOR24:
			case CGL_COLOR24_ALPHA:
			{
				unsigned char *ppixels = (unsigned char*)pixels;
				delete[] ppixels;
				break;
			}
			case CGL_COLOR_FLOAT16:
			case CGL_COLOR_FLOAT16_ALPHA:
			case CGL_COLOR_FLOAT32:
			case CGL_COLOR_FLOAT32_ALPHA:
			{
				float *ppixels = (float*)pixels;
				delete[] ppixels;
				break;
			}
		}
	}

	pixels = NULL;
}

bool CImage::allocatePixels(uint32_t width, uint32_t height, uint32_t layers, PIXEL_TYPE type)
{
	m_layers = layers;

	return allocatePixels(width, height, type);
}

bool CImage::allocatePixels(uint32_t width, uint32_t height, PIXEL_TYPE type)
{
	//   reserve default space for at least rgba type, even if it larger than necessary.
	uint32_t size = 4 * width * height * m_layers;

	if (size > 0)
	{
		// always clear memory, even is type is identical:
		// the texture may have been resized.
		releasePixels();

		switch (type)
		{
			case CGL_COLOR24:
			case CGL_COLOR24_ALPHA:
			{
				pixels = new unsigned char [size];
				break;
			}
			case CGL_COLOR_FLOAT16:
			case CGL_COLOR_FLOAT16_ALPHA:
			case CGL_COLOR_FLOAT32:
			case CGL_COLOR_FLOAT32_ALPHA:
			{
				pixels = new float [size];
				break;
			}
		}
	}
	else
		return false;

	m_pixelType = type;
	m_width = width;
	m_height = height;

	return true;
}

void CImage::setImageKindIO(IImageIO *imager)
{
	std::vector<std::string> extensionKind = imager->getImageKind();
	for (size_t j=0;j<extensionKind.size();j++)
	{
		std::string ext;
		for (unsigned int i=0;i<extensionKind[j].size();i++)
			ext += toupper(extensionKind[j][i]);

		imageKindIO.insert(std::map<std::string, IImageIO*>::value_type(ext, imager));
	}
}

CImage::IImageIO* const CImage::getImageKindIO(const std::string &extension)
{
	//	extract the right image loader
    std::string ext = extension;
	std::string::size_type pos = ext.rfind('.');
    if (pos < ext.size())
        ext = ext.substr(pos+1);

    pos = 0;
	for (pos=0;pos<ext.size();pos++)
        ext[pos] = toupper(ext[pos]);

	std::map<std::string,IImageIO*>::const_iterator itr = imageKindIO.find(ext);
	if (imageKindIO.end() != itr)
		return (*itr).second;
	else 
		return NULL;
}
