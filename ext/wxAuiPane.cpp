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
	return wrapPtr(vinfo,rb_cWXAuiPane);
}

template <>
wxAuiPaneInfo unwrap< wxAuiPaneInfo >(const VALUE &vinfo)
{
	if(NIL_P(vinfo))
		return wxAuiPaneInfo();
	else if(SYMBOL_P(vinfo))
	{
		ID id(SYM2ID(vinfo));
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
		VALUE temp;
		if(!NIL_P(temp=rb_hash_aref(vinfo,ID2SYM(rb_intern("caption")))))
			info.caption = unwrap<wxString>(temp);
		if(!NIL_P(temp=rb_hash_aref(vinfo,ID2SYM(rb_intern("name")))))
			info.name = unwrap<wxString>(temp);
		if(!NIL_P(temp=rb_hash_aref(vinfo,ID2SYM(rb_intern("icon")))))
			info.icon = unwrap<wxBitmap>(temp);
		if(!NIL_P(temp=rb_hash_aref(vinfo,ID2SYM(rb_intern("direction")))))
			info.Direction(unwrapenum< wxAuiManagerDock >(temp));
		if(!NIL_P(temp=rb_hash_aref(vinfo,ID2SYM(rb_intern("layer")))))
			info.Layer(NUM2INT(temp));
		return info;
	}else
		return *unwrapPtr<wxAuiPaneInfo>(vinfo, rb_cWXAuiPane);
}

namespace RubyWX {
namespace AuiPane {

APP_PROTECT(wxAuiPaneInfo)

macro_attr_prop(caption,wxString)
macro_attr_prop(name,wxString)
macro_attr_prop(icon,wxIcon)

macro_attr_prop(floating_pos,wxPoint)
macro_attr_prop(floating_size,wxSize)

macro_attr_prop(dock_layer,int)

macro_attr_prop_enum(dock_direction,wxAuiManagerDock)


singlereturn(IsToolbar)
}
}

DLL_LOCAL void Init_WXAuiPane(VALUE rb_mWX)
{
#if wxUSE_AUI
	using namespace RubyWX::AuiPane;
	rb_cWXAuiPane = rb_define_class_under(rb_mWX,"AuiPane",rb_cObject);
	rb_define_alloc_func(rb_cWXAuiPane,_alloc);
	//rb_define_method(rb_cWXAuiManager,"[]=",RUBY_METHOD_FUNC(_set),2);

	rb_define_attr_method(rb_cWXAuiPane,"caption",_get_caption,_set_caption);
	rb_define_attr_method(rb_cWXAuiPane,"name",_get_name,_set_name);
	rb_define_attr_method(rb_cWXAuiPane,"icon",_get_icon,_set_icon);
	rb_define_attr_method(rb_cWXAuiPane,"direction",_get_dock_direction,_set_dock_direction);

	rb_define_attr_method(rb_cWXAuiPane,"floating_pos",_get_floating_pos,_set_floating_pos);
	rb_define_attr_method(rb_cWXAuiPane,"floating_size",_get_floating_size,_set_floating_size);

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
