/*
 * wxMenuBar.cpp
 *
 *  Created on: 24.02.2012
 *      Author: hanmac
 */

#include "wxApp.hpp"

#include "wxMenu.hpp"
#include "wxWindow.hpp"

VALUE rb_cWXMenuBar;

#define _self wrap<wxMenuBar*>(self)

namespace RubyWX {
namespace MenuBar {

VALUE _alloc(VALUE self)
{
	if(ruby_app_inited)
		return wrap(new wxMenuBar,self);
	else
		rb_raise(rb_eArgError,"%s is not running.",rb_class2name(rb_cWXApp));
	return Qnil;

}

VALUE _each(VALUE self)
{
	RETURN_ENUMERATOR(self,0,NULL);
	int count = _self->GetMenuCount();
	for(int i = 0;i < count;++i)
		rb_yield(wrap(_self->GetMenu(i)));
	return self;
}


VALUE _appendShift(VALUE self,VALUE menu)
{
	wxMenu *m = wrap<wxMenu*>(menu);
	if(m->GetTitle() == wxEmptyString)
		rb_raise(rb_eTypeError,"menu must have a title to be append.");
	_self->Append(m,m->GetTitle());
	return self;
}



VALUE _append(VALUE self,VALUE menu)
{
	if(rb_obj_is_kind_of(menu,rb_cWXMenu))
		return _appendShift(self,menu);
	wxMenu *m = new wxMenu;
	if(rb_block_given_p())
		rb_yield(wrap(m));

	_self->Append(m,wrap<wxString>(menu));
	return self;
}


singlereturn(GetFrame);

}
}

void Init_WXMenuBar(VALUE rb_mWX)
{
	using namespace RubyWX::MenuBar;
	rb_cWXMenuBar = rb_define_class_under(rb_cWXMenu,"Bar",rb_cWXWindow);
	rb_define_alloc_func(rb_cWXMenuBar,_alloc);


	rb_define_method(rb_cWXMenuBar,"each_menu",RUBY_METHOD_FUNC(_each),0);

	rb_define_method(rb_cWXMenuBar,"<<",RUBY_METHOD_FUNC(_appendShift),1);

	rb_define_method(rb_cWXMenuBar,"append",RUBY_METHOD_FUNC(_append),1);

	rb_define_method(rb_cWXMenuBar,"frame",RUBY_METHOD_FUNC(_GetFrame),0);
}
