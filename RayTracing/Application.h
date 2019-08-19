#pragma once
#include <windows.h>
#include <gdiplus.h>
#include"RGBImage.h"
#include <string>
using std::wstring;
#pragma comment (lib,"Gdiplus.lib")

class Application {
public:
	Application(int width, int height);
	~Application();
	RGBImage GetImage(const wstring& fileName) const;
	void SetWindowsText(const wstring& text);
	bool Continue();
	void CopyInBuffer(RGBImage image);
private:
	HINSTANCE hInstance;
	HWND hwnd;
	RGBImage buffer;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
};