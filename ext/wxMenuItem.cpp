/*
 * wxMenuItem.cpp
 *
 *  Created on: 18.02.2012
 *      Author: hanmac
 */

#include "wxWindow.hpp"
#include "wxApp.hpp"
#include "wxMenu.hpp"
#include "wxMenuItem.hpp"
#include "wxBitmap.hpp"

#define _self unwrap<wxMenuItem*>(self)

VALUE rb_cWXMenuItem;


namespace RubyWX {
namespace MenuItem {

APP_PROTECT(wxMenuItem)

macro_attr(Menu,wxMenu*)
macro_attr(ItemLabel,wxString)
macro_attr_with_func(Id,wrapID,unwrapID)
//macro_attr(Kind,wxItemKind)
macro_attr(SubMenu,wxMenu*)

macro_attr(Help,wxString)

singlereturn(GetBitmap)

VALUE _SetBitmap(VALUE self,VALUE val)
{
	_self->SetBitmap(wrapBitmap(val,_self->GetId(),true,wxART_MENU));
	return val;
}

}
}


DLL_LOCAL void Init_WXMenuItem(VALUE rb_mWX)
{
	using namespace RubyWX::MenuItem;
	//rb_cWXMenu = rb_define_class_under(rb_mWX,"Menu",rb_cObject);
	rb_cWXMenuItem = rb_define_class_under(rb_cWXMenu,"Item",rb_cObject);
	rb_define_alloc_func(rb_cWXMenuItem,_alloc);

	rb_define_attr_method(rb_cWXMenuItem,"menu",_getMenu,_setMenu);
	rb_define_attr_method(rb_cWXMenuItem,"itemLabel",_getItemLabel,_setItemLabel);
	rb_define_attr_method(rb_cWXMenuItem,"id",_getId,_setId);

	rb_define_attr_method(rb_cWXMenuItem,"help",_getHelp,_setHelp);

	rb_define_attr_method(rb_cWXMenuItem,"bitmap",_GetBitmap,_SetBitmap);

	registerInfo<wxMenuItem>(rb_cWXMenuItem);
}
