#include "game.h"
const float TARGET_ASPECT = (float)VIRTUAL_WIDTH / VIRTUAL_HEIGHT;
using namespace std;


int main() {

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, "BlackJack");
	SetWindowMinSize(600, 380);
	SetWindowMaxSize(1500, 950);
	SetTargetFPS(60);

	Game game = Game();


	while (!WindowShouldClose()&&!ShouldWindowClose) {
		if (IsWindowResized()||IsWindowFullscreen()) {
			int currentWidth = GetScreenWidth();
			int currentHeight = GetScreenHeight();

			int targetHeight = (int)(currentWidth / TARGET_ASPECT);

			if (abs(currentHeight - targetHeight) > 1) {
				SetWindowSize(currentWidth, targetHeight);
			}
		}

		Camera2D camera= { 0 };
		camera.offset = { game.offset()};
		camera.target = { 0,0 };
		camera.zoom = game.scale;
		camera.rotation = 0.0f;

		BeginDrawing();

		game.Update();

		ClearBackground(lightGreen);

		BeginMode2D(camera);
		game.Draw();
		EndMode2D();

		EndDrawing();
	}

	game.SaveGame();
	CloseWindow();

	return 0;
}