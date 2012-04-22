/*
 * wxAuiToolBar.cpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */


#include "wxEvtHandler.hpp"

VALUE rb_cWXAuiToolBar;

#define _self wrap<wxAuiToolBar*>(self)

namespace RubyWX {
namespace AuiToolBar {

VALUE _alloc(VALUE self)
{
	return wrap(new wxAuiToolBar(),self);
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

void Init_WXAuiToolBar(VALUE rb_mWX)
{
	using namespace RubyWX::AuiToolBar;
	rb_cWXAuiToolBar = rb_define_class_under(rb_mWX,"AuiToolBar",rb_cWXToolBarBase);
	rb_define_alloc_func(rb_cWXAuiToolBar,_alloc);

	rb_define_method(rb_cWXAuiToolBar,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
}
