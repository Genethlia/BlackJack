#include "Source.h"
using namespace std;

bool HasEnoughTimePassed(double& lastUpdateTime, double TimePassed) {
	bool HasEnoughTimePassed = false;
	double CurrentTime = GetTime();
	if (CurrentTime - lastUpdateTime > TimePassed) {
		HasEnoughTimePassed = true;
		lastUpdateTime = CurrentTime;
	}
	return HasEnoughTimePassed;
}
