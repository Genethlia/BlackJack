#pragma once
#include <raylib.h>

class Images {
public:
	Images(int rank);
	~Images();
	Texture filiTexture,bigfiliTexture;
private:
	Image image;

};
