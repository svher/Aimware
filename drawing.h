#pragma once

#include "includes.h"

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color);
void DrawLine(float x1, float y1, float x2, float y2, float thickness, D3DCOLOR color);
void DrawLine(Vec2 src, Vec2 dst, float thickness, D3DCOLOR color);