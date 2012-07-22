#include "VisEntityQuad.h"


VisEntityQuad::VisEntityQuad(void)
{
}


VisEntityQuad::~VisEntityQuad(void)
{
    if (vb)
        vb->Release();
    if (ib)
        ib->Release();
}

bool VisEntityQuad::Load(const char * filename, IDirect3DDevice9 * device) {
    HRESULT hr = device->CreateVertexBuffer(4*sizeof(CustomVertex), D3DUSAGE_WRITEONLY,
        TRIANGLE_FVF, D3DPOOL_DEFAULT, &vb, 0);

	if (FAILED(hr)) return false;

	// locations of ze vertices
	D3DXVECTOR3 arrayOfVertices[4];

	arrayOfVertices[0] = D3DXVECTOR3(100.f,200.f,0.0f); // -1 -1 0
	arrayOfVertices[1] = D3DXVECTOR3(100.f,100.f,0.0f); // -1 1 0
	arrayOfVertices[2] = D3DXVECTOR3(200.f,100.f,0.0f); // 1 1 0
	arrayOfVertices[3] = D3DXVECTOR3(200.f,200.f,0.0f); // 1 -1 0

	CustomVertex *vertex;
	hr = vb->Lock(0, 0, (void**)&vertex, 0);
	if(FAILED(hr)) return false;

	vertex[0].position = arrayOfVertices[0];
	vertex[0].rhw = 1.f;
	vertex[0].colour = D3DCOLOR_XRGB(255,0,0); //255 0 0

	vertex[1].position = arrayOfVertices[1];
	vertex[1].rhw = 1.f;
	vertex[1].colour = D3DCOLOR_XRGB(0,255,0); // 0 255 0

	vertex[2].position = arrayOfVertices[2];
	vertex[2].rhw = 1.f;
	vertex[2].colour = D3DCOLOR_XRGB(0,0,255); // 0 0 255

	vertex[3].position = arrayOfVertices[3];
	vertex[3].rhw = 1.f;
	vertex[3].colour = D3DCOLOR_XRGB(0,0,0); // 255 255 0

	vb->Unlock();

	device->CreateIndexBuffer(6*sizeof(WORD),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,
		&ib,NULL);

	WORD *indices = NULL;	
	ib->Lock( 0, 0, (void**)&indices, 0 );

	WORD indexStart = 0;
	int count = 0;

	indices[count]   = indexStart;
	indices[count+1] = indexStart+1;
	indices[count+2] = indexStart+3;
	indices[count+3] = indexStart+2; // was 3 imo
	indices[count+4] = indexStart+1; 
	indices[count+5] = indexStart+2;

	ib->Unlock();

    return true;
}

void VisEntityQuad::Render(IDirect3DDevice9 * device) {
    
    /*static float angle = 0.05f;
    static D3DXMATRIX rotation;
    D3DXMatrixRotationYawPitchRoll(&rotation, angle,angle/2, angle/3);
    static D3DXMATRIX translation;
    D3DXMatrixTranslation(&translation, -5,0,0);

    static D3DXMATRIX world;
    D3DXMatrixMultiply(&world, &rotation, &translation);
    device->SetTransform(D3DTS_WORLD, &world);*/

    device->SetRenderState( D3DRS_ZENABLE, TRUE );
	device->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

	device->BeginScene();

	device->SetTexture(0, NULL);
    device->SetStreamSource(0, vb, 0, sizeof(CustomVertex));
	device->SetFVF(TRIANGLE_FVF);
	device->SetIndices(ib);

	device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0,0,4,0,2);

	device->EndScene();

    //angle += angle;
}