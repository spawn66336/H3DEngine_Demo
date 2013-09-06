#include "Win32MsgUtil.h"


 HippoMouseEvent Win32MsgUtil::GetMouseEventInfoFromMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
	HippoMouseEvent res;
	res.bMouseLeft=(wParam & MK_LBUTTON)==1;
	res.bMouseRight=(wParam & MK_RBUTTON)==1;
	res.bMouseMiddle=(wParam & MK_MBUTTON)==1;
	res.bKeyCtrl=(wParam & MK_CONTROL)==1;
	res.bKeyShift=(wParam & MK_SHIFT)==1;
	res.bKeyAlt=(wParam & MK_ALT)==1;
	res.bMouseMiddle=(wParam & MK_MBUTTON)==1;
	res.PosX=LOWORD(lParam);
	res.PosY=HIWORD(lParam);
	return res;
 }

 HippoWheelEvent Win32MsgUtil::GetWheelEventInfoFromMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
	 HippoWheelEvent res;
	 res.bKeyCtrl=(wParam & MK_CONTROL)==1;
	 res.bKeyShift=(wParam & MK_SHIFT)==1;
	 res.bKeyAlt=(wParam & MK_ALT)==1;
	 res.wheel_delta = ( short )HIWORD( wParam );
	 return res;
 }

 HippoSizeEvent Win32MsgUtil::GetHippoSizeEventFromMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
	 int w=LOWORD(lParam);
	 int h=HIWORD(lParam);
	 return HippoSizeEvent(w,h);
 }