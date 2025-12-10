#include "game.h"
using namespace std;

Color tableGreen = { 34, 139, 34, 255 };

int main() {

	InitWindow(1000, 900, "BlackJack");
	SetTargetFPS(60);

	Game game = Game();


	while (!WindowShouldClose()) {
		BeginDrawing();

		if (!game.Drown) {
			game.Update(0.2);
		}

		ClearBackground(tableGreen);
		game.Draw();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}