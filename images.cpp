#include "images.h"

void Images::LoadSuit(int rank) {
	int x = 60, y = 90;

		// suit images
		switch (rank) {
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
		}

		// resize and create textures for suit images
		ImageResize(&image, x, y);
		bigfiliTexture = LoadTextureFromImage(image);

		ImageResize(&image, x / 3, y / 3);
		filiTexture = LoadTextureFromImage(image);

		UnloadImage(image);
	}
void Images::LoadMatHiddenCardAndHome() {
		// mat and hidden card images for the table
		mat = LoadImage("Images/mat.jpg");
		ImageResize(&mat, 1200, 950);
		matTexture = LoadTextureFromImage(mat);
		UnloadImage(mat);

		mat = LoadImage("Images/card.png");
		ImageResize(&mat, 180, 250);
		hiddenCardTexture = LoadTextureFromImage(mat);
		UnloadImage(mat);

		mat = LoadImage("Images/home.png");
		ImageResize(&mat, 50, 50);
		mainMenuhomeTexture = LoadTextureFromImage(mat);
		UnloadImage(mat);
}


void Images::UnloadAll(){
	// Only unload textures that were actually loaded (texture.id != 0)
	if (filiTexture.id != 0) UnloadTexture(filiTexture);
	if (bigfiliTexture.id != 0) UnloadTexture(bigfiliTexture);
	if (matTexture.id != 0) UnloadTexture(matTexture);
	if (hiddenCardTexture.id != 0) UnloadTexture(hiddenCardTexture);
	if (mainMenuhomeTexture.id != 0) UnloadTexture(mainMenuhomeTexture);
}

Images::~Images(){
	UnloadAll();
}
