#include "drawing.h"

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
    D3DRECT rect = {x, y, x + w, y + h};
    pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawLine(float x1, float y1, float x2, float y2, float thickness, D3DCOLOR color) {
    ID3DXLine* lineL;
    D3DXCreateLine(pDevice, &lineL);

    D3DXVECTOR2 line[2];
    line[0] = D3DXVECTOR2(x1, y1);
    line[1] = D3DXVECTOR2(x2, y2);
    lineL->SetWidth(thickness);
    lineL->Draw(line, 2, color);
    lineL->Release();
}
