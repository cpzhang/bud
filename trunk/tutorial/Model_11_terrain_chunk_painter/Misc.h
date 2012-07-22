#pragma once
#include "atlHeaders.h"
#include <string>
#include "FreeImage/Dist/FreeImage.h"
HBITMAP generateHBitMap(const std::string& fileName, int width, int height, bool selected);
bool skinButton(const std::string& fileName, CButton& b, bool selected = false);