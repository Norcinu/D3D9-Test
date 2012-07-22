#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <Windows.h>
#include <WindowsX.h>
#include <memory>
#include <cassert>
#include <vector>

class Visualisation;

class ApplicationFramework
{
public:
    ApplicationFramework(HINSTANCE instance_) : 
      wnd_(0), 
      instance(instance_), 
      selection(0),
      running(false)
    {
    }
	
    ~ApplicationFramework() 
    {
	    assert("Shutdown not performed." && wnd_ == 0);
        ::DestroyWindow(wnd_);
    }

public:
	bool Init();
	void Run();
	void Shutdown();

private:
    void  OnDraw();
    bool  OnEvent(UINT msg, WPARAM wParam, LPARAM lParam);
    //void  OpenFileDialog(const IOFileType ft);
   // void  OpenSaveDialog(const IOFileType ft);
    void  OnInit(HWND wnd, CREATESTRUCT * cs);
    void  SetControlFont(HWND font_control, int points, const char * font_name, 
	    bool is_bold = false);
    void  OnUpdate(float time);

private:
	static LRESULT WINAPI WndProc(HWND wnd, UINT msg, WPARAM wParam, 
        LPARAM lParam);

private:
	HWND wnd_;
	HINSTANCE instance;
	bool running;
    int selection;
    Visualisation * vis;
};

#endif
