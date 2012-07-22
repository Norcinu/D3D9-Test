#ifndef VIS_ENTITY_TRIANGLE_H
#define VIS_ENTITY_TRIANGLE_H

#include "visentity.h"

#define TRIANGLE_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

class VisEntityQuad : public VisEntity
{
public:
    VisEntityQuad(void);
    ~VisEntityQuad(void);

    bool Load(const char * filename, IDirect3DDevice9 * device);
    void Render(IDirect3DDevice9 * device);
};

#endif
