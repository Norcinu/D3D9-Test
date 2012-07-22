#include "Framework.h"
#include <iostream>
#include <ShObjIdl.h>
#include <string>
#include <functional>
#include "Visualisation.h"

namespace
{
    int window_height = 600;
    int window_width = 800;

    static std::string current_open_file = "";
    static COMDLG_FILTERSPEC extensions[] = 
    {
	    {L"Json Files (*.json)", L"*.json"},
	    {L"Text Files (*.txt)", L"*.txt"},
	    {L"XML Files (*.xml)", L"*.xml"}
    };

    static COMDLG_FILTERSPEC gfx_extensions[] = 
    {
        {L"Bitmap Files (*.bmp)", L"*.bmp"},
        {L"PNG Files (*.png)", L"*.png"},
        {L"Targa Files (*.tga)", L"*.tga"},
        {L"3D Object Files (*.obj)", L"*.obj"}
    };
}

struct GetSizeFunctor {
    void operator()(std::string& msg, size_t size) {      
       /* std::string conversion = utils::str::ToString<size_t>(size);
        msg.append(conversion.begin(), conversion.end());
        msg += "\n";
        OutputDebugString(msg.c_str());   */
    }
};

typedef struct GetSelectionFunctor {
    int operator()() {
        return 1;
    }
} GetSelection;

bool ApplicationFramework::Init()
{
    assert("ApplicationFramework::Init already performed." && wnd_ == 0);
    
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));

    LPCSTR class_name = "Direct3D 9 Test";
    LPCSTR window_text = "Direct 3D 9 Test - Steven Gleed";

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
   // wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE+1); 
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hIcon = 0;
    wc.hInstance = instance;
    wc.lpfnWndProc = ApplicationFramework::WndProc;
    wc.lpszClassName = class_name;
    wc.lpszMenuName = 0;
    wc.style = CS_OWNDC;

    if (0 == ::RegisterClass(&wc))
    {
        std::cerr << "ApplicationFramework::Init Failed; RegisterClass failed." << std::endl;
        return false;
    }

    int desktop_width = GetSystemMetrics(SM_CXSCREEN);
    int desktop_height = GetSystemMetrics(SM_CYSCREEN);

    wnd_ = ::CreateWindow(
	    class_name, 
        window_text,
		 WS_EX_TOPMOST | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW, 
		desktop_width / 8, 
		10, 
		window_width,
		window_height, 
		0, 0, 0, 0);

    if (wnd_ == 0)
    {
        std::cerr << "ApplicationFramework::Init Failed; CreateWindow failed." 
	        << std::endl;
		
        return false;
    }

    ::SetWindowLongPtr(wnd_, GWL_USERDATA, (LONG_PTR)this);

    // I do not like this!
    OnInit(wnd_, (CREATESTRUCT*)this);

    return true;
}

void ApplicationFramework::Run()
{
	assert( "ApplicationFramework::Init not performed" && wnd_ != 0 );

	// Make the window appear
	::ShowWindow(wnd_, SW_SHOW);
	//UpdateWindow(wnd_);

	// Establish a "message loop", continuously listening for and processing 
    // window events, updating and drawing the "scene"
	MSG msg;
	running = true;
	long start_time = ::timeGetTime();
	while (running)
	{
        while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE) != 0)
		{
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}

		OnUpdate( 0.001f * ( timeGetTime() - start_time ) );
		OnDraw();
	}
}

void ApplicationFramework::Shutdown()
{
    ::DestroyWindow(wnd_);
    wnd_ = 0;
    delete vis;
}

void ApplicationFramework::OnDraw() 
{
    if (vis->GetDevice() != nullptr) {
        vis->BeginScene();
        vis->EndScene();
    }

}

bool ApplicationFramework::OnEvent(UINT msg, WPARAM wParam, LPARAM lParam) 
{ 
   /* switch(LOWORD(wParam))
    {
    }*/

    return true; 
}

void ApplicationFramework::OnInit(HWND wnd, CREATESTRUCT * cs) 
{   
    vis = new Visualisation;
    vis->Initialise(window_width, window_height, wnd);
    vis->AddEntity("cunt.bmp");
}

void ApplicationFramework::OnUpdate(float time) 
{
}

LRESULT CALLBACK ApplicationFramework::WndProc(HWND wnd, UINT msg, WPARAM wParam,
	LPARAM lParam)
{
	ApplicationFramework* app = (ApplicationFramework *)GetWindowLongPtr(wnd, 
		GWL_USERDATA);

	if (app == nullptr)
		return DefWindowProc(wnd, msg, wParam, lParam);

	switch ( msg )
	{
    case WM_COMMAND:
        if (!app->OnEvent(msg, wParam, lParam))
            return ::DefWindowProc(wnd, msg, wParam, lParam);
        break;
	case WM_CLOSE:
		::ShowWindow( app->wnd_, SW_HIDE );
		app->running = false;
		break;
	case WM_CREATE:
		app->OnInit(wnd,reinterpret_cast<CREATESTRUCT*>(lParam));
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);    //signal end of application
		return 0; 
	}

	return ::DefWindowProc(wnd,msg,wParam,lParam);	
}




