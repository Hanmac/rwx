/*
 * wxPanel.cpp
 *
 *  Created on: 06.04.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"

VALUE rb_cWXPanel;

#define _self unwrap<wxPanel*>(self)

namespace RubyWX {
namespace Panel {

APP_PROTECT(wxPanel)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "12",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)){
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;
}

}
}

DLL_LOCAL void Init_WXPanel(VALUE rb_mWX)
{
	using namespace RubyWX::Panel;
	rb_cWXPanel = rb_define_class_under(rb_mWX,"Panel",rb_cWXWindow);
	rb_define_alloc_func(rb_cWXPanel,_alloc);

	rb_define_method(rb_cWXPanel,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxPanel>(rb_cWXPanel);
}





