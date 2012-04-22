/*
 * wxToolBar.cpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */


#include "wxEvtHandler.hpp"

VALUE rb_cWXToolBar;

#define _self wrap<wxToolBar*>(self)

namespace RubyWX {
namespace ToolBar {

VALUE _alloc(VALUE self)
{
	return wrap(new wxToolBar(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(wrap<wxWindow*>(parent),wxID_ANY);
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

}
}

void Init_WXToolBar(VALUE rb_mWX)
{
	using namespace RubyWX::ToolBar;
	rb_cWXToolBar = rb_define_class_under(rb_mWX,"ToolBar",rb_cWXToolBarBase);
	rb_define_alloc_func(rb_cWXToolBar,_alloc);

	rb_define_method(rb_cWXToolBar,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
}


