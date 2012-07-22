#include "Visualisation.h"
#include "VisEntity.h"
#include "VisEntityQuad.h"

const int kFontSize = 14;

Visualisation::Visualisation(void) :
    d3d(nullptr),
    device(nullptr)
{
}

Visualisation::~Visualisation(void)
{
    if (!entities.empty()) {
        for (auto i = 0; i < entities.size(); ++i)
            delete entities[i];

        entities.clear();
    }

    font->Release();
    device->Release();
    d3d->Release();
}

bool Visualisation::Initialise(const int width, const int height, const HWND wnd) {
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp;
    
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = wnd;

    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
    d3dpp.BackBufferWidth = width;               // set the width of the buffer
    d3dpp.BackBufferHeight = height;

    if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd, 
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device))) return false;
   
   device->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

	// Setup basic render state
	device->SetRenderState( D3DRS_LIGHTING,         TRUE );
	device->SetRenderState( D3DRS_DITHERENABLE,     TRUE );
	device->SetRenderState( D3DRS_SPECULARENABLE,	  FALSE );
	device->SetRenderState( D3DRS_ZENABLE,          TRUE );
	device->SetRenderState( D3DRS_CULLMODE,         D3DCULL_CCW );
	device->SetRenderState( D3DRS_AMBIENT,          0x99999999 );
	device->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );

	// Setup states effecting texture rendering:
	device->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
	device->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

    D3DXMATRIX matProj;	
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 800.0f/600.0f, 1.0f, 5000.0f );
	device->SetTransform( D3DTS_PROJECTION, &matProj );

	// Lighting	
	device->SetRenderState( D3DRS_LIGHTING, TRUE );

	// Create a directional light
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type       = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;
	light.Diffuse.a  = 1.0f;
	light.Range      = 1000.0f;

	// Direction for our light - it must be normalized - pointing down and along z
	D3DXVECTOR3 vecDir;
	vecDir = D3DXVECTOR3(0.0f,-0.3f,0.5f);
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
	device->SetLight( 0, &light );
	device->LightEnable( 0, TRUE );

	// Plus some non directional ambient lighting
	device->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(80,80,80));
    D3DXCreateFont(device, kFontSize, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &font );

    return true;
}

IDirect3DDevice9 * Visualisation::GetDevice() const {
    return device;
}

void Visualisation::SetupCamera() {
    float aspectRatio = 800.0f / 600.0f;
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI/4, aspectRatio, 1.0f, 100.0f);
	device->SetTransform( D3DTS_PROJECTION, &projectionMatrix);

	// setup view, duh!
	D3DXVECTOR3 lookAt(9.0f, 0.0f, 0.0f);
	D3DXVECTOR3 eye(0.0f, 0.0f, -10.0f); // 0,0, -4
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &eye, &lookAt, &up);
	device->SetTransform(D3DTS_VIEW, &viewMatrix);
}

void Visualisation::ClearScreen() {
    device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
}

void Visualisation::BeginScene() {
    ClearScreen();
   // device->BeginScene();
    entities[0]->Render(device);
}

void Visualisation::EndScene() {
    //device->EndScene();
    device->Present(NULL, NULL, NULL, NULL);
}

bool Visualisation::AddEntity(const std::string& filename) {
    VisEntityQuad * tri = new VisEntityQuad;
    tri->Load(filename.c_str(), device);
    entities.push_back(tri);

    return true;
}