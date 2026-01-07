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
#include "images.h"
#include "Audio.h"
namespace fs = std::filesystem;
constexpr int VIRTUAL_WIDTH = 1200;
constexpr int VIRTUAL_HEIGHT = 950; 

bool HasEnoughTimePassed(double& lastUpdateTime, double TimePassed);

extern bool ShouldWindowClose;
