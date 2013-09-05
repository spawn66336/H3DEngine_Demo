/********************************************************************
	created:	2012/12/05
	created:	5:12:2012   0:10
	filename: 	F:\Git\ClassicHippoAppFrameWork\HippoAppFrameWork\HippoWindow.h
	file path:	F:\Git\ClassicHippoAppFrameWork\HippoAppFrameWork
	file base:	HippoWindow
	file ext:	h
	author:		sssa2000
	
	purpose:	封装windows提供的窗体
*********************************************************************/
#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <functional>
#include "Win32MsgUtil.h"


class HippoWindow
{
public:



	HippoWindow(const char* name,unsigned int w,unsigned int h,unsigned int left,unsigned int top);
	~HippoWindow();

	HWND GetHwnd(){return m_wnd_handle;}
	unsigned int GetWidth(){return m_width;}
	unsigned int GetHeight(){return m_height;}

	
	CustomMsgProc& GetCustomMsgProc(){return custom_msg_proc;}
	
	void RegisterWndSizeEvent(WndSizeCallback& e);
	void RegisterKeyDownEvent(KeyCallback& e);
	void RegisterKeyUpEvent(KeyCallback& e);
	void RegisterMouseWheelEvent(MouseWheelCallback& e);
	void RegisterMouseLeftDownEvent(MouseKeyCallback& e);
	void RegisterMouseLeftUpEvent(MouseKeyCallback& e);
	void RegisterMouseRightDownEvent(MouseKeyCallback& e);
	void RegisterMouseRightUpEvent(MouseKeyCallback& e);
	void RegisterMouseMoveEvent(MouseKeyCallback& e);
	void RegisterDropFileEvent(DropFileCallback& e);
protected:
private:
	void CreateWnd();
	static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HippoWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	std::string m_name;
	HWND m_wnd_handle;
	//HINSTANCE m_hInst;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_pos_left;
	unsigned int m_pos_top;

private:
	CustomMsgProc custom_msg_proc;
	std::vector<WndSizeCallback> wnd_size_event;
	std::vector<KeyCallback> keydown_event;
	std::vector<KeyCallback> keyup_event;
	std::vector<MouseKeyCallback> mouse_leftdown_event;
	std::vector<MouseKeyCallback> mouse_leftup_event;
	std::vector<MouseKeyCallback> mouse_rightdown_event;
	std::vector<MouseKeyCallback> mouse_rightup_event;
	std::vector<MouseKeyCallback> mouse_move_event;
	std::vector<MouseWheelCallback> mouse_wheel_event;
	std::vector<DropFileCallback> dropfile_event;
};