/*
 * wxMenu.cpp
 *
 *  Created on: 17.02.2012
 *      Author: hanmac
 */



#include "wxMenu.hpp"
#include "wxMenuItem.hpp"
#include "wxEvtHandler.hpp"
#include "wxApp.hpp"

#define _self wrap<wxMenu*>(self)

VALUE rb_cWXMenu;


namespace RubyWX {
namespace Menu {

macro_attr(Title,wxString)
macro_attr(Parent,wxMenu*)
macro_attr(EventHandler,wxEvtHandler*)

singlereturn(AppendSeparator)
singlereturn(PrependSeparator)



VALUE _alloc(VALUE self)
{
	if(ruby_app_inited)
		return wrap(new wxMenu);
	else
		rb_raise(rb_eArgError,"%s is not running.",rb_class2name(rb_cWXApp));
	return Qnil;

}

VALUE _each(VALUE self)
{
	int count = _self->GetMenuItemCount();
	for(int i = 0;i < count;++i)
		rb_yield(wrap(_self->FindItemByPosition(i)));
	return self;
}

}
}


void Init_WXMenu(VALUE rb_mWX)
{
	using namespace RubyWX::Menu;
	//rb_cWXEvtHandler = rb_define_class_under(rb_mWX,"EvtHandler",rb_cObject);
	rb_cWXMenu = rb_define_class_under(rb_mWX,"Menu",rb_cWXEvtHandler);
	rb_define_alloc_func(rb_cWXMenu,_alloc);

	rb_include_module(rb_cWXMenu,rb_mEnumerable);
}


