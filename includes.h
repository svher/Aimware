#pragma once

inline constexpr float PI = 3.14159265358;
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define TO_RAD(x) ((x)*PI/180)
#define W2S(x, y) hack->WorldToScreen(x, y)
#define DECL_ALIGN(x) __declspec(align(x))
#define ALIGN16 DECL_ALIGN(16)

#include <cstddef>
#include "csgo.h"
#include "sdk/enginetrace.h"
#include "sdk/usercmd.h"
#include "sdk/in_buttons.h"
#include "sdk/consts.h"
#include "sdk/client_class.h"
#include "sdk/cliententitylist.h"

#include "netvar.h"
#include "hash.h"
#include "drawing.h"
#include "gui.h"
#include "memory.h"
#include "dx.h"
#include "vector.h"
#include "hook.h"
#include "hack.h"