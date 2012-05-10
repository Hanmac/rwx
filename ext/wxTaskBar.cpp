/*
 * wxTaskBar.cpp
 *
 *  Created on: 14.04.2012
 *      Author: hanmac
 */

#include "wxTaskBar.hpp"
#include "wxEvtHandler.hpp"
#include "wxBitmap.hpp"


VALUE rb_cWXTaskBar;
#if wxUSE_TASKBARICON

#define _self wrap<RubyTaskBarIcon*>(self)


const wxIcon& RubyTaskBarIcon::GetIcon() const {return *mIcon;}
void RubyTaskBarIcon::SetIcon(const wxIcon& icon)
{
	mIcon = &icon;
	if(icon.IsOk())
		wxTaskBarIcon::SetIcon(icon,mTooltip);
	else
		RemoveIcon();
}

wxString RubyTaskBarIcon::GetToolTip() const {return mTooltip;}
void RubyTaskBarIcon::SetToolTip(const wxString& tooltip)
{
	mTooltip = tooltip;
	if(mIcon->IsOk())
		wxTaskBarIcon::SetIcon(*mIcon,mTooltip);
}


namespace RubyWX {
namespace TaskBar {

APP_PROTECT(RubyTaskBarIcon)

macro_attr(Menu,wxMenu*)
macro_attr(Icon,wxIcon)
macro_attr(ToolTip,wxString)

}
}

#endif

DLL_LOCAL void Init_WXTaskBar(VALUE rb_mWX)
{
#if wxUSE_TASKBARICON
	using namespace RubyWX::TaskBar;

	rb_cWXTaskBar = rb_define_class_under(rb_mWX,"TaskBar",rb_cObject);
	rb_define_alloc_func(rb_cWXTaskBar,_alloc);

	rb_include_module(rb_cWXTaskBar,rb_mWXEvtHandler);

	rb_define_attr_method(rb_cWXTaskBar,"menu",_getMenu,_setMenu);
	rb_define_attr_method(rb_cWXTaskBar,"icon",_getIcon,_setIcon);
	rb_define_attr_method(rb_cWXTaskBar,"tooltip",_getToolTip,_setToolTip);

	registerType<wxTaskBarIcon>(rb_cWXTaskBar);
	registerType<RubyTaskBarIcon>(rb_cWXTaskBar);

	registerEventType("taskbar_move",wxEVT_TASKBAR_MOVE,rb_cWXEvent);
	registerEventType("taskbar_left_down",wxEVT_TASKBAR_LEFT_DOWN,rb_cWXEvent);
	registerEventType("taskbar_left_up",wxEVT_TASKBAR_LEFT_UP,rb_cWXEvent);
	registerEventType("taskbar_right_down",wxEVT_TASKBAR_RIGHT_DOWN,rb_cWXEvent);
	registerEventType("taskbar_right_up",wxEVT_TASKBAR_RIGHT_UP,rb_cWXEvent);
	registerEventType("taskbar_left_dclick",wxEVT_TASKBAR_LEFT_DCLICK,rb_cWXEvent);
	registerEventType("taskbar_right_dclick",wxEVT_TASKBAR_RIGHT_DCLICK,rb_cWXEvent);
	registerEventType("taskbar_balloon_timeout",wxEVT_TASKBAR_BALLOON_TIMEOUT,rb_cWXEvent);
	registerEventType("taskbar_balloon_click",wxEVT_TASKBAR_BALLOON_CLICK,rb_cWXEvent);

#endif
}

