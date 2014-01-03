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

#define _self unwrap<RubyTaskBarIcon*>(self)


const wxIcon& RubyTaskBarIcon::GetIcon() const {return *mIcon;}
bool RubyTaskBarIcon::SetIcon(const wxIcon& icon,const wxString& tool)
{
	mIcon = &icon;
	if(icon.IsOk())
		return wxTaskBarIcon::SetIcon(icon,mTooltip);
	else
		RemoveIcon();
	return false;
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


/* Document-attr: menu
 * the menu of the TaskBar. WX::Menu
 */
/* Document-attr: icon
 * the icon of the TaskBar. WX::Bitmap
 */
/* Document-attr: tooltip
 * the tooltip of the TaskBar. String
 */

DLL_LOCAL void Init_WXTaskBar(VALUE rb_mWX)
{
#if wxUSE_TASKBARICON
#if 0
	rb_define_attr(rb_cWXTaskBar,"menu",1,1);
	rb_define_attr(rb_cWXTaskBar,"icon",1,1);
	rb_define_attr(rb_cWXTaskBar,"tooltip",1,1);
#endif
	using namespace RubyWX::TaskBar;

	rb_cWXTaskBar = rb_define_class_under(rb_mWX,"TaskBar",rb_cObject);
	rb_define_alloc_func(rb_cWXTaskBar,_alloc);

	rb_undef_method(rb_cWXTaskBar,"initialize_copy");
	rb_undef_method(rb_cWXTaskBar,"_load");
	rb_undef_method(rb_cWXTaskBar,"_dump");

	rb_include_module(rb_cWXTaskBar,rb_mWXEvtHandler);

	rb_define_attr_method(rb_cWXTaskBar,"menu",_getMenu,_setMenu);
	rb_define_attr_method(rb_cWXTaskBar,"icon",_getIcon,_setIcon);
	rb_define_attr_method(rb_cWXTaskBar,"tooltip",_getToolTip,_setToolTip);

	registerInfo<wxTaskBarIcon>(rb_cWXTaskBar);
	registerInfo<RubyTaskBarIcon>(rb_cWXTaskBar);

	registerEventType("taskbar_move",wxEVT_TASKBAR_MOVE);
	registerEventType("taskbar_left_down",wxEVT_TASKBAR_LEFT_DOWN);
	registerEventType("taskbar_left_up",wxEVT_TASKBAR_LEFT_UP);
	registerEventType("taskbar_right_down",wxEVT_TASKBAR_RIGHT_DOWN);
	registerEventType("taskbar_right_up",wxEVT_TASKBAR_RIGHT_UP);
	registerEventType("taskbar_left_dclick",wxEVT_TASKBAR_LEFT_DCLICK);
	registerEventType("taskbar_right_dclick",wxEVT_TASKBAR_RIGHT_DCLICK);
	registerEventType("taskbar_balloon_timeout",wxEVT_TASKBAR_BALLOON_TIMEOUT);
	registerEventType("taskbar_balloon_click",wxEVT_TASKBAR_BALLOON_CLICK);

#endif
}

