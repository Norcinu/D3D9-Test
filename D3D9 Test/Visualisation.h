#ifndef VISUALISATION_H
#define VISUALISATION_H

#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include <string>
#include <vector>

#pragma comment(lib, "winmm.lib")

class VisEntity;

class Visualisation
{
public:
    Visualisation(void);
    ~Visualisation(void);

    bool Initialise(const int width, const int height, const HWND wnd);
    bool AddEntity(const std::string& filename);
    bool RemoveEntity(const int index);
    void SetupCamera();
    void ClearScreen();
    void BeginScene();
    void EndScene();

    IDirect3DDevice9 * GetDevice() const;

private:
    IDirect3D9 * d3d;
    IDirect3DDevice9 * device;
    ID3DXFont * font;

    int screen_width;
    int screen_height;

    std::vector<VisEntity*> entities;
};

#endif
