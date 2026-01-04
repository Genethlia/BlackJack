#include "game.h"
using namespace std;


int main() {

	InitWindow(screenWidth, screenHeight, "BlackJack");
	SetTargetFPS(60);

	Game game = Game();


	while (!WindowShouldClose()&&!ShouldWindowClose) {
		BeginDrawing();

		game.Update();

		ClearBackground(lightGreen);
		game.Draw();

		EndDrawing();
	}

	game.SaveGame();
	CloseWindow();

	return 0;
}