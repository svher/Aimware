#pragma once

#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <cmath>

const float PI = 3.14159265358;
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define TORAD(x) ((x)*PI/180)
#define W2S(x, y) hack->WorldToScreen(x, y)

#include "csgo.h"
#include "dx.h"
#include "vector.h"
#include "drawing.h"
#include "hook.h"
#include "hack.h"