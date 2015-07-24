/*
 * wxToolBarTool.cpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */

#include "wxBitmap.hpp"

#include "wxToolBarBase.hpp"
#include "wxToolBarTool.hpp"

#if wxUSE_TOOLBAR

#define _self unwrap<wxToolBarToolBase*>(self)

VALUE rb_cWXToolBarTool;

//need that for register
struct wxToolBarTool;

namespace RubyWX {
namespace ToolBarTool {


#define macro_tool_attr_func(name, type, get, set, settool, con) \
DLL_LOCAL VALUE _get##name(VALUE self)\
{ \
	return wrap(_self->get());\
}\
\
DLL_LOCAL VALUE _set##name(VALUE self,VALUE other)\
{\
	rb_check_frozen(self);\
	if(con) { \
		wxToolBarBase *toolbar = _self->GetToolBar();\
		if(toolbar) {\
			int id = _self->GetId();\
			if(toolbar->FindById(id)) {\
				toolbar->settool(id, unwrap<type>(other));\
				return other;\
			}\
		}\
		_self->set(unwrap<type>(other));\
	}\
	return other;\
}

#define macro_tool_attr(name, type) macro_tool_attr_func(name, type, Get##name, Set##name, SetTool##name, true)
#define macro_tool_attr_bool(name, set) macro_tool_attr_func(name, bool, Is##name, set, set##Tool, true)
#define macro_tool_attr_bool_con(name, set, con) macro_tool_attr_func(name, bool, Is##name, set, set##Tool, _self->con())

macro_attr(Label, wxString)
macro_tool_attr(ShortHelp, wxString)
macro_tool_attr(LongHelp, wxString)

#if wxUSE_MENUS
macro_tool_attr_func(DropdownMenu,wxMenu*, GetDropdownMenu, SetDropdownMenu, SetDropdownMenu, true)
#endif

macro_tool_attr_bool(Enabled,Enable)
macro_tool_attr_bool_con(Toggled, Toggle, CanBeToggled)

macro_tool_attr_func(Toggleable, bool, CanBeToggled, SetToggle, SetToggle, true)

singlereturn(GetToolBar)
singlereturn(GetBitmap)
singlereturn(GetNormalBitmap)
singlereturn(GetDisabledBitmap)

DLL_LOCAL VALUE _SetNormalBitmap(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	wxBitmap bitmap = wrapBitmap(val,_self->GetId(),WRAP_BITMAP_RAISE,wxART_TOOLBAR);

	wxToolBarBase *toolbar = _self->GetToolBar();
	if(toolbar) {
		int id = _self->GetId();
		if(toolbar->FindById(id)) {
			toolbar->SetToolNormalBitmap(id, bitmap);
			return val;
		}
	}
	_self->SetNormalBitmap(bitmap);
	return val;
}

DLL_LOCAL VALUE _SetDisabledBitmap(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	wxBitmap bitmap = wrapBitmap(val,_self->GetId(),WRAP_BITMAP_NULL,wxART_TOOLBAR);

	wxToolBarBase *toolbar = _self->GetToolBar();
	if(toolbar) {
		int id = _self->GetId();
		if(toolbar->FindById(id)) {
			toolbar->SetToolDisabledBitmap(id, bitmap);
			return val;
		}
	}
	_self->SetDisabledBitmap(bitmap);
	return val;
}

singlereturn_if(GetControl, _self->IsControl())

DLL_LOCAL VALUE _alloc(VALUE self)
{

	return wrap(new wxToolBarToolBase);
}

}
}

#endif


/* Document-attr: toolbar
 * the toolbar that has this ToolBarTool or nil.
 */

/* Document-attr: bitmap
 * the current bitmap of this tool bar item.
 */


/* Document-attr: normal_bitmap
 * returns the default WX::Bitmap
 */
/* Document-attr: disabled_bitmap
 * returns the diabled WX::Bitmap
 */

/* Document-attr: label
 * returns the label of the tool bar item. Type is String
 */
/* Document-attr: short_help
 * returns the short help, used in status bar. Type is String
 */
/* Document-attr: long_help
 * returns the long help, used in tool tip. Type is String
 */

/* Document-attr: dropdown_menu
 * returns the dropdown menu WX::Menu
 */


/* Document-attr: enabled
 * returns if the menu item is enabled. bool
 */

/* Document-attr: toggled
 * returns if the menu item is toggled. bool
 */


DLL_LOCAL void Init_WXToolBarTool(VALUE rb_mWX)
{
#if 0
	rb_cWXToolBarBase = rb_define_class_under(rb_mWX,"ToolBarBase",rb_cWXControl);
#endif

#if wxUSE_TOOLBAR
	using namespace RubyWX::ToolBarTool;
	rb_cWXToolBarTool = rb_define_class_under(rb_cWXToolBarBase,"Tool",rb_cObject);

	//should that really have alloc?
	rb_define_alloc_func(rb_cWXToolBarTool,_alloc);

#if 0
	rb_define_attr(rb_cWXToolBarTool,"control",1,0);
	rb_define_attr(rb_cWXToolBarTool,"toolbar",1,0);
	rb_define_attr(rb_cWXToolBarTool,"bitmap",1,0);

	rb_define_attr(rb_cWXToolBarTool,"normal_bitmap",1,1);
	rb_define_attr(rb_cWXToolBarTool,"disabled_bitmap",1,1);

	rb_define_attr(rb_cWXToolBarTool,"label",1,1);
	rb_define_attr(rb_cWXToolBarTool,"short_help",1,1);
	rb_define_attr(rb_cWXToolBarTool,"long_help",1,1);

	rb_define_attr(rb_cWXToolBarTool,"enabled",1,1);
	rb_define_attr(rb_cWXToolBarTool,"toggled",1,1);

	rb_define_attr(rb_cWXToolBarTool,"dropdown_menu",1,1);
#endif

	rb_undef_method(rb_cWXToolBarTool,"initialize_copy");
	rb_undef_method(rb_cWXToolBarTool,"_load");
	rb_undef_method(rb_cWXToolBarTool,"_dump");

	rb_define_attr_method(rb_cWXToolBarTool,"control",_GetControl,0);

	rb_define_attr_method(rb_cWXToolBarTool,"toolbar",_GetToolBar,0);

	rb_define_attr_method(rb_cWXToolBarTool,"bitmap",_GetBitmap,0);

	rb_define_attr_method(rb_cWXToolBarTool,"normal_bitmap",_GetNormalBitmap,_SetNormalBitmap);
	rb_define_attr_method(rb_cWXToolBarTool,"disabled_bitmap",_GetDisabledBitmap,_SetDisabledBitmap);

	rb_define_attr_method(rb_cWXToolBarTool,"label",_getLabel,_setLabel);
	rb_define_attr_method(rb_cWXToolBarTool,"short_help",_getShortHelp,_setShortHelp);
	rb_define_attr_method(rb_cWXToolBarTool,"long_help",_getLongHelp,_setLongHelp);

	rb_define_attr_method(rb_cWXToolBarTool,"enabled",_getEnabled,_setEnabled);
	rb_define_attr_method(rb_cWXToolBarTool,"toggled",_getToggled,_setToggled);

#if wxUSE_MENUS
	rb_define_attr_method(rb_cWXToolBarTool,"dropdown_menu",_getDropdownMenu,_setDropdownMenu);
#endif

	registerInfo<wxToolBarToolBase>(rb_cWXToolBarTool);
//	registerInfo<wxToolBarTool>(rb_cWXToolBarTool);
#endif
}
