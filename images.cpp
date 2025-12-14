#include "images.h"

Images::Images(int rank){
	int x=60, y=90;
	switch (rank){
	case 0:
		image = LoadImage("Images/diamond.png");
		break;
	case 1:
		image = LoadImage("Images/spade.png");
		x = 70;
		break;
	case 2:
		image = LoadImage("Images/hearts.png");
		x = 50;
		break;
	case 3:
		image = LoadImage("Images/club.png");
		x = 80;
		break;
	case 4:
		mat = LoadImage("Images/mat.jpg");
		ImageResize(&mat, 1000, 900);
		matTexture = LoadTextureFromImage(mat);
		UnloadImage(mat);
		break;
	default:
		break;
	}

	ImageResize(&image, x, y);
	bigfiliTexture = LoadTextureFromImage(image);
	ImageResize(&image, x/3, y/3);
	filiTexture = LoadTextureFromImage(image);
	UnloadImage(image);

}

Images::~Images(){
	UnloadTexture(filiTexture);
	UnloadTexture(bigfiliTexture);
	UnloadTexture(matTexture);
}
