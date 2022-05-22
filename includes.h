#pragma once

#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <cmath>
#include "MinHook.h"

const float PI = 3.14159265358;
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define TO_RAD(x) ((x)*PI/180)
#define W2S(x, y) hack->WorldToScreen(x, y)
#define DECL_ALIGN(x) __declspec(align(x))
#define ALIGN16 DECL_ALIGN(16)

#include "csgo.h"
#include "sdk.h"
#include "dx.h"
#include "vector.h"
#include "drawing.h"
#include "hook.h"
#include "hack.h"