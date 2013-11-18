/*
 * wxToolBar.cpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */


#include "wxToolBar.hpp"
#include "wxToolBarBase.hpp"

VALUE rb_cWXToolBar;

#define _self unwrap<wxToolBar*>(self)

namespace RubyWX {
namespace ToolBar {

APP_PROTECT(wxToolBar)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "12",&parent,&name,&hash);
	if(!rb_obj_is_kind_of(name,rb_cString)){
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;

}

}
}

DLL_LOCAL void Init_WXToolBar(VALUE rb_mWX)
{
	using namespace RubyWX::ToolBar;
	rb_cWXToolBar = rb_define_class_under(rb_mWX,"ToolBar",rb_cWXToolBarBase);
	rb_define_alloc_func(rb_cWXToolBar,_alloc);

	rb_define_method(rb_cWXToolBar,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxToolBar>(rb_cWXToolBar);
}


