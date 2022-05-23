#include "includes.h"

extern Hack* hack;
extern LPDIRECT3DDEVICE9 pDevice;

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
    D3DRECT rect = {x, y, x + w, y + h};
    pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawLine(float x1, float y1, float x2, float y2, float thickness, D3DCOLOR color) {

    if (!hack->LineL) {
        ID3DXLine* lineL;
        D3DXCreateLine(pDevice, &lineL);
        hack->LineL = lineL;
    }

    D3DXVECTOR2 line[2];
    line[0] = D3DXVECTOR2(x1, y1);
    line[1] = D3DXVECTOR2(x2, y2);
    hack->LineL->SetWidth(thickness);
    hack->LineL->Draw(line, 2, color);
}

void DrawLine(Vec2 src, Vec2 dst, float thickness, D3DCOLOR color) {
    DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawEspBox2D(Vec2 top, Vec2 bottom, float thickness, D3DCOLOR color) {
    float height = ABS(top.y - bottom.y);
    Vec2 tl{}, tr{}, bl{}, br{};
    tl.x = top.x - height / 4;
    tr.x =  top.x + height / 4;
    tl.y = tr.y = top.y;
    bl.x = bottom.x - height / 4;
    br.x = bottom.x + height / 4;
    bl.y = br.y = bottom.y;
    DrawLine(tl, tr, thickness, color);
    DrawLine(bl, br, thickness, color);
    DrawLine(tl, bl, thickness, color);
    DrawLine(tr, br, thickness, color);
}

void DrawEspBox3D(Vec3 top, Vec3 bot, float a, float width, float thickness, D3DCOLOR color) {
    float height = top.z - bot.z;
    Vec3 b1, b2, b3, b4, t1, t2, t3, t4;
    b1.z = b2.z = b3.z = b4.z = bot.z;
    b1.x = bot.x + cos(TO_RAD(a + 45)) * width;
    b1.y = bot.y + sin(TO_RAD(a + 45)) * width;
    b2.x = bot.x + cos(TO_RAD(a + 135)) * width;
    b2.y = bot.y + sin(TO_RAD(a + 135)) * width;
    b3.x = bot.x + cos(TO_RAD(a + 225)) * width;
    b3.y = bot.y + sin(TO_RAD(a + 225)) * width;
    b4.x = bot.x + cos(TO_RAD(a + 315)) * width;
    b4.y = bot.y + sin(TO_RAD(a + 315)) * width;

    t1.z = t2.z = t3.z = t4.z = top.z;
    t1.x = b1.x;
    t1.y = b1.y;
    t2.x = b2.x;
    t2.y = b2.y;
    t3.x = b3.x;
    t3.y = b3.y;
    t4.x = b4.x;
    t4.y = b4.y;

    Vec2 b1_2, b2_2, b3_2, b4_2, t1_2, t2_2, t3_2, t4_2;
    if (W2S(b1, b1_2) && W2S(b2, b2_2) && W2S(b3, b3_2) && W2S(b4, b4_2) && W2S(t1, t1_2) && W2S(t2, t2_2) && W2S(t3, t3_2) && W2S(t4, t4_2)) {
        DrawLine(t1_2, b1_2, thickness, color);
        DrawLine(t2_2, b2_2, thickness, color);
        DrawLine(t3_2, b3_2, thickness, color);
        DrawLine(t4_2, b4_2, thickness, color);

        DrawLine(t1_2, t2_2, thickness, color);
        DrawLine(t2_2, t3_2, thickness, color);
        DrawLine(t3_2, t4_2, thickness, color);
        DrawLine(t4_2, t1_2, thickness, color);

        DrawLine(b1_2, b2_2, thickness, color);
        DrawLine(b2_2, b3_2, thickness, color);
        DrawLine(b3_2, b4_2, thickness, color);
        DrawLine(b4_2, b1_2, thickness, color);
    }
}

void DrawTextWrapper(const char *text, int x, int y, D3DCOLOR color) {
    RECT rect;
    if (!hack->FontF) {
        D3DXCreateFont(pDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH || FF_DONTCARE, "Arial", &hack->FontF);
    }
    SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
    hack->FontF->DrawTextA(NULL, text , -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

    SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
    hack->FontF->DrawTextA(NULL, text , -1, &rect, DT_CENTER | DT_NOCLIP, color);
}
