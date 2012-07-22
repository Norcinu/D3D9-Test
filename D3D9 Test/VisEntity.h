#ifndef VIS_ENTITY_H
#define VIS_ENTITY_H

#include <d3d9.h>
#include <d3dx9.h>

struct CustomVertex 
{
    D3DXVECTOR3 position;
    float rhw;
    unsigned long colour;
};

struct CustomVertexTextured : public CustomVertex
{
   float u;
   float v;
};

class VisEntity
{
public:
    virtual ~VisEntity() {}
    virtual bool Load(const char * filename, IDirect3DDevice9 * device) = 0;
    virtual void Render(IDirect3DDevice9 * device) = 0;

protected:
    VisEntity() : vb(nullptr), ib(nullptr) {}

protected:
    IDirect3DVertexBuffer9 * vb;
    IDirect3DIndexBuffer9  * ib;
    D3DXVECTOR3 position;    
};

#endif