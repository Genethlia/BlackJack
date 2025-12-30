#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <iostream>
#include <stack>
#include <queue>
#include <fstream>
#include <string>
#include <filesystem>
#include "color.h"
namespace fs = std::filesystem;

bool HasEnoughTimePassed(double& lastUpdateTime, double TimePassed);

extern bool ShouldWindowClose;
