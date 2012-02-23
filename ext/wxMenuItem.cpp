/*
 * wxMenuItem.cpp
 *
 *  Created on: 18.02.2012
 *      Author: hanmac
 */

#include "wxMenu.hpp"
#include "wxMenuItem.hpp"
#include "wxApp.hpp"

#define _self wrap<wxMenuItem*>(self)

VALUE rb_cWXMenuItem;


namespace RubyWX {
namespace MenuItem {

VALUE _alloc(VALUE self)
{

	return wrap(new wxMenuItem);
}

macro_attr(Menu,wxMenu*)
macro_attr(ItemLabel,wxString)
macro_attr(Id,int)
macro_attr(Kind,wxItemKind)
macro_attr(SubMenu,wxMenu*)

macro_attr(Help,wxString)
}
}


void Init_WXMenuItem(VALUE rb_mWX)
{
	using namespace RubyWX::MenuItem;
	//rb_cWXMenu = rb_define_class_under(rb_mWX,"Menu",rb_cObject);
	rb_cWXMenuItem = rb_define_class_under(rb_cWXMenu,"Item",rb_cObject);
	rb_define_alloc_func(rb_cWXMenuItem,_alloc);

	rb_define_attr_method(rb_cWXMenuItem,"itemLabel",_getItemLabel,_setItemLabel);
	rb_define_attr_method(rb_cWXMenuItem,"id",_getId,_setId);

	rb_define_attr_method(rb_cWXMenuItem,"help",_getHelp,_setHelp);
}


