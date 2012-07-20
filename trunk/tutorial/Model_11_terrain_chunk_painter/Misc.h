#pragma once
#include "FreeImage/Source/FreeImage.h"
HBITMAP generateHBitMap(const std::string& fileName, int width, int height)
{
	HBITMAP h = 0;
	//
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	// check the file signature and deduce its format
	// (the second argument is currently not used by FreeImage)
	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	if(fif == FIF_UNKNOWN) 
	{
		// no signature ?
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	}
	// check that the plugin has reading capabilities ...
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) 
	{
		// ok, let's load the file
		FIBITMAP *dib = FreeImage_Load(fif, fileName.c_str(), 0);
		// ok, we can save the file
		{
			std::string temp("c:/temp.bmp");
			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
			// try to guess the file format from the file extension
			fif = FreeImage_GetFIFFromFilename(temp.c_str());
			if(fif != FIF_UNKNOWN ) 
			{
				// check that the plugin has sufficient writing and export capabilities ...
				WORD bpp = FreeImage_GetBPP(dib);
				if(FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp)) 
				{
					FIBITMAP* newDIB = FreeImage_Rescale(dib, width, height, FILTER_BOX); 
					// ok, we can save the file
					bool success = FreeImage_Save(fif, newDIB, temp.c_str(), BMP_DEFAULT);

					// free the dib
					FreeImage_Unload(newDIB);
					h =  (HBITMAP)LoadImage(NULL, 
						temp.c_str(), 
						IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
					// unless an abnormal bug, we are done !
				}
			}
			DeleteFile(temp.c_str());
		}

		// free the dib
		FreeImage_Unload(dib);
	}
	return h;
}
void skinButton(const std::string& fileName, CButton& b)
{
	//
	DeleteObject(b.GetBitmap());
	//
	RECT s;
	b.GetClientRect(&s);
	b.SetBitmap(generateHBitMap(fileName.c_str(), s.right - s.left, s.bottom - s.top));
}
