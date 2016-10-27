/*
 * wxAuiPane.cpp
 *
 *  Created on: 15.04.2012
 *      Author: hanmac
 */

#include "wxAuiPane.hpp"
#include "wxBitmap.hpp"
#include "wxSize.hpp"
#include "wxPoint.hpp"

#define _self unwrap<wxAuiPaneInfo*>(self)

VALUE rb_cWXAuiPane;

template <>
VALUE wrap< wxAuiPaneInfo >(wxAuiPaneInfo *vinfo)
{
	return wrapTypedPtr(vinfo,rb_cWXAuiPane);
}

template <>
wxAuiPaneInfo unwrap< wxAuiPaneInfo >(const VALUE &vinfo)
{
	if(NIL_P(vinfo))
		return wxAuiPaneInfo();
	else if(RB_SYMBOL_P(vinfo))
	{
		ID id(RB_SYM2ID(vinfo));
		if(id == rb_intern("default"))
			return wxAuiPaneInfo().DefaultPane();
		else if(id == rb_intern("toolbar"))
			return wxAuiPaneInfo().ToolbarPane();
		else if(id == rb_intern("center") || id == rb_intern("centre"))
			return wxAuiPaneInfo().CenterPane();
		else
			return wxAuiPaneInfo();
	}
	else if(rb_obj_is_kind_of(vinfo,rb_cHash))
	{
		wxAuiPaneInfo info;

		set_obj_option(vinfo, "default", &wxAuiPaneInfo::DefaultPane, info);
		set_obj_option(vinfo, "toolbar", &wxAuiPaneInfo::ToolbarPane, info);

		set_obj_option(vinfo, "caption", &wxAuiPaneInfo::Caption, info);
		set_obj_option(vinfo, "name", &wxAuiPaneInfo::Caption, info);
		set_obj_option(vinfo, "icon", &wxAuiPaneInfo::Caption, info);
		//set_obj_option(vinfo, "direction", &wxAuiPaneInfo::Direction, info, unwrapenum< wxAuiManagerDock >);
		set_obj_option(vinfo, "layer", &wxAuiPaneInfo::Layer, info);
		set_obj_option(vinfo, "row", &wxAuiPaneInfo::Row, info);

		set_obj_option(vinfo, "gripper", &wxAuiPaneInfo::Gripper, info);
		set_obj_option(vinfo, "gripper_top", &wxAuiPaneInfo::GripperTop, info);
		set_obj_option(vinfo, "close_button", &wxAuiPaneInfo::CloseButton, info);
		set_obj_option(vinfo, "maximize_button", &wxAuiPaneInfo::MaximizeButton, info);
		set_obj_option(vinfo, "minimize_button", &wxAuiPaneInfo::MinimizeButton, info);
		set_obj_option(vinfo, "pin_button", &wxAuiPaneInfo::PinButton, info);

		return info;
	}else
		return *unwrapTypedPtr<wxAuiPaneInfo>(vinfo, rb_cWXAuiPane);
}

namespace RubyWX {
namespace AuiPane {

APP_PROTECT(wxAuiPaneInfo)

macro_attr_prop(caption,wxString)
macro_attr_prop(name,wxString)
macro_attr_prop(icon,wxIcon)

macro_attr_prop(floating_pos,wxPoint)
macro_attr_prop(floating_size,wxSize)

macro_attr_prop(min_size,wxSize)
macro_attr_prop(max_size,wxSize)
macro_attr_prop(best_size,wxSize)

macro_attr_prop(dock_layer,int)
macro_attr_prop(dock_row,int)
macro_attr_prop(dock_pos,int)

macro_attr_prop_enum(dock_direction,wxAuiManagerDock)


singlereturn(IsToolbar)
singlereturn(IsFloating)
singlereturn(IsMaximized)

macro_attr_bool2(Dockable,Dockable)
macro_attr_bool2(TopDockable,TopDockable)
macro_attr_bool2(BottomDockable,BottomDockable)
macro_attr_bool2(LeftDockable,LeftDockable)
macro_attr_bool2(RightDockable,RightDockable)

macro_attr_bool2(Movable,Movable)
macro_attr_bool2(Floatable,Floatable)
macro_attr_bool2(Resizable,Resizable)

macro_attr_bool2(Shown,Show)

macro_attr_bool3(Caption,CaptionVisible)
macro_attr_bool3(Gripper,Gripper)
macro_attr_bool3(GripperTop,GripperTop)
macro_attr_bool3(Border,PaneBorder)
macro_attr_bool3(CloseButton,CloseButton)
macro_attr_bool3(MaximizeButton,MaximizeButton)
macro_attr_bool3(MinimizeButton,MinimizeButton)
macro_attr_bool3(PinButton,PinButton)

DLL_LOCAL VALUE _setFloating(VALUE self, VALUE val)
{
	rb_check_frozen(self);
	if(RTEST(val))
		_self->Float();
	else
		_self->Dock();
	return val;
}

DLL_LOCAL VALUE _setMaximized(VALUE self, VALUE val)
{
	rb_check_frozen(self);
	if(RTEST(val))
		_self->Maximize();
	else
		_self->Restore();
	return val;
}


}
}


/* Document-attr: caption
 * string shown in the title of the pane
 */
/* Document-attr: name
 * name of the pane, for internal use, not shown.
 */
/* Document-attr: icon
 * WX::Bitmap: shown in the title of the pane
 */

/* Document-attr: layer
 * Integer (0 = innermost layer)
 */
/* Document-attr: row
 * Integer row number on the docking bar (0 = first row)
 */
/* Document-attr: pos
 * Integer position inside the row (0 = first position)
 */

/* Document-attr: min_size
 * WX::Size: the minimal size of the pane
 */
/* Document-attr: max_size
 * WX::Size: the maximal size of the pane
 */
/* Document-attr: best_size
 * WX::Size: the best size of the pane
 */

/* Document-attr: floating
 * boolean true if pane is currently floating
 */
/* Document-attr: maximized
 * boolean true if pane is currently maximized
 */

/* Document-attr: movable
 * boolean true if pane can be moved
 */
/* Document-attr: floatable
 * boolean true if pane can be floated
 */
/* Document-attr: resizable
 * boolean true if pane size can be changed
 */


/* Document-attr: gripper
 * boolean true if pane has gripper
 */
/* Document-attr: gripper_top
 * boolean true if pane has gripper on top
 */

/* Document-attr: border
 * boolean true if pane has border around
 */

/* Document-attr: close_button
 * boolean true if close button is shown
 */
/* Document-attr: maximize_button
 * boolean true if maximize button is shown
 */
/* Document-attr: minimize_button
 * boolean true if minimize button is shown
 */
/* Document-attr: pin_button
 * boolean true if pin button is shown
 */

DLL_LOCAL void Init_WXAuiPane(VALUE rb_mWX)
{

#if wxUSE_AUI
	using namespace RubyWX::AuiPane;
	rb_cWXAuiPane = rb_define_class_under(rb_mWX,"AuiPane",rb_cObject);
	rb_define_alloc_func(rb_cWXAuiPane,_alloc);
	//rb_define_method(rb_cWXAuiManager,"[]=",RUBY_METHOD_FUNC(_set),2);

#if 0
	rb_define_attr(rb_cWXAuiPane,"caption",1,1);
	rb_define_attr(rb_cWXAuiPane,"name",1,1);
	rb_define_attr(rb_cWXAuiPane,"icon",1,1);
	rb_define_attr(rb_cWXAuiPane,"direction",1,1);

	rb_define_attr(rb_cWXAuiPane,"layer",1,1);
	rb_define_attr(rb_cWXAuiPane,"row",1,1);
	rb_define_attr(rb_cWXAuiPane,"pos",1,1);

	rb_define_attr(rb_cWXAuiPane,"floating_pos",1,1);
	rb_define_attr(rb_cWXAuiPane,"floating_size",1,1);

	rb_define_attr(rb_cWXAuiPane,"min_size",1,1);
	rb_define_attr(rb_cWXAuiPane,"max_size",1,1);
	rb_define_attr(rb_cWXAuiPane,"best_size",1,1);

	rb_define_attr(rb_cWXAuiPane,"dockable",1,1);
	rb_define_attr(rb_cWXAuiPane,"top_dockable",1,1);
	rb_define_attr(rb_cWXAuiPane,"bottom_dockable",1,1);
	rb_define_attr(rb_cWXAuiPane,"left_dockable",1,1);
	rb_define_attr(rb_cWXAuiPane,"right_dockable",1,1);

	rb_define_attr(rb_cWXAuiPane,"floating",1,1);
	rb_define_attr(rb_cWXAuiPane,"maximized",1,1);

	rb_define_attr(rb_cWXAuiPane,"floatable",1,1);
	rb_define_attr(rb_cWXAuiPane,"movable",1,1);
	rb_define_attr(rb_cWXAuiPane,"resizable",1,1);

	rb_define_attr(rb_cWXAuiPane,"caption_visible",1,1);
	rb_define_attr(rb_cWXAuiPane,"gripper",1,1);
	rb_define_attr(rb_cWXAuiPane,"gripper_top",1,1);
	rb_define_attr(rb_cWXAuiPane,"border",1,1);
	rb_define_attr(rb_cWXAuiPane,"close_button",1,1);
	rb_define_attr(rb_cWXAuiPane,"maximize_button",1,1);
	rb_define_attr(rb_cWXAuiPane,"minimize_button",1,1);
	rb_define_attr(rb_cWXAuiPane,"pin_button",1,1);

#endif

	rb_define_attr_method(rb_cWXAuiPane,"caption",_get_caption,_set_caption);
	rb_define_attr_method(rb_cWXAuiPane,"name",_get_name,_set_name);
	rb_define_attr_method(rb_cWXAuiPane,"icon",_get_icon,_set_icon);
	rb_define_attr_method(rb_cWXAuiPane,"direction",_get_dock_direction,_set_dock_direction);

	rb_define_attr_method(rb_cWXAuiPane,"layer",_get_dock_layer,_set_dock_layer);
	rb_define_attr_method(rb_cWXAuiPane,"row",_get_dock_row,_set_dock_row);
	rb_define_attr_method(rb_cWXAuiPane,"pos",_get_dock_pos,_set_dock_pos);

	rb_define_attr_method(rb_cWXAuiPane,"floating_pos",_get_floating_pos,_set_floating_pos);
	rb_define_attr_method(rb_cWXAuiPane,"floating_size",_get_floating_size,_set_floating_size);

	rb_define_attr_method(rb_cWXAuiPane,"min_size",_get_min_size,_set_min_size);
	rb_define_attr_method(rb_cWXAuiPane,"max_size",_get_max_size,_set_max_size);
	rb_define_attr_method(rb_cWXAuiPane,"best_size",_get_best_size,_set_best_size);

	rb_define_attr_method(rb_cWXAuiPane,"floating",_IsFloating,_setFloating);
	rb_define_attr_method(rb_cWXAuiPane,"maximized",_IsMaximized,_setMaximized);

	rb_define_attr_method(rb_cWXAuiPane,"dockable",_getDockable,_setDockable);
	rb_define_attr_method(rb_cWXAuiPane,"top_dockable",_getTopDockable,_setDockable);
	rb_define_attr_method(rb_cWXAuiPane,"bottom_dockable",_getBottomDockable,_setBottomDockable);
	rb_define_attr_method(rb_cWXAuiPane,"left_dockable",_getLeftDockable,_setLeftDockable);
	rb_define_attr_method(rb_cWXAuiPane,"right_dockable",_getRightDockable,_setRightDockable);

	rb_define_attr_method(rb_cWXAuiPane,"floatable",_getFloatable,_setFloatable);
	rb_define_attr_method(rb_cWXAuiPane,"movable",_getMovable,_setMovable);
	rb_define_attr_method(rb_cWXAuiPane,"resizable",_getResizable,_setResizable);

	rb_define_attr_method(rb_cWXAuiPane,"caption_visible",_getCaption,_setCaption);
	rb_define_attr_method(rb_cWXAuiPane,"gripper",_getGripper,_setGripper);
	rb_define_attr_method(rb_cWXAuiPane,"gripper_top",_getGripperTop,_setGripperTop);
	rb_define_attr_method(rb_cWXAuiPane,"border",_getBorder,_setBorder);
	rb_define_attr_method(rb_cWXAuiPane,"close_button",_getCloseButton,_setCloseButton);
	rb_define_attr_method(rb_cWXAuiPane,"maximize_button",_getMaximizeButton,_setMaximizeButton);
	rb_define_attr_method(rb_cWXAuiPane,"minimize_button",_getMinimizeButton,_setMinimizeButton);
	rb_define_attr_method(rb_cWXAuiPane,"pin_button",_getPinButton,_setPinButton);

	rb_define_method(rb_cWXAuiPane,"toolbar?",RUBY_METHOD_FUNC(_IsToolbar),0);

	registerType<wxAuiPaneInfo>(rb_cWXAuiPane);

	registerEnum<wxAuiManagerDock>("WX::AuiManagerDock")
		->add(wxAUI_DOCK_NONE,"none")
		->add(wxAUI_DOCK_TOP,"top")
		->add(wxAUI_DOCK_RIGHT,"right")
		->add(wxAUI_DOCK_BOTTOM,"bottom")
		->add(wxAUI_DOCK_LEFT,"left")
		->add(wxAUI_DOCK_CENTER,"center");

#endif

}
