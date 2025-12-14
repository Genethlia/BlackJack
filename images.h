#pragma once
#include <raylib.h>

class Images {
public:
	Images(int rank);//Constructor that loads images based on rank
	~Images();
	Texture filiTexture, bigfiliTexture;//Textures for small and big suit images
	Texture matTexture;//Table mat texture
private:
	Image image;//Temporary image holder
	Image mat;//Table mat image
};
