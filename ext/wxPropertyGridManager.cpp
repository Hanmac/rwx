/*
 * wxPropertyGridManager.cpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */


#include "wxPropertyGridManager.hpp"
#include "wxPropertyGridInterface.hpp"

VALUE rb_cWXPropertyGridManager;

#if wxUSE_PROPGRID
#define _self wrap<wxPropertyGridManager*>(self)

namespace RubyWX {
namespace PropertyGridManager {

APP_PROTECT(wxPropertyGridManager)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	int style = wxPGMAN_DEFAULT_STYLE;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("style")))))
			style = NUM2INT(temp);
	}

	_self->Create(wrap<wxWindow*>(parent),wxID_ANY,wxDefaultPosition,wxDefaultSize,style);
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXPropertyGridManager(VALUE rb_mWX)
{
#if wxUSE_PROPGRID
	using namespace RubyWX::PropertyGridManager;
	rb_cWXPropertyGridManager = rb_define_class_under(rb_mWX,"PropertyGridManager",rb_cWXPanel);
	rb_define_alloc_func(rb_cWXPropertyGridManager,_alloc);

	rb_include_module(rb_cWXPropertyGridManager,rb_mWXPropertyGridInterface);

	rb_define_method(rb_cWXPropertyGridManager,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_const(rb_cWXPropertyGridManager,"DEFAULT_STYLE",INT2NUM(wxPGMAN_DEFAULT_STYLE));
	rb_define_const(rb_cWXPropertyGridManager,"TOOLBAR",INT2NUM(wxPG_TOOLBAR));

	rb_define_const(rb_cWXPropertyGridManager,"EX_MODE_BUTTONS",INT2NUM(wxPG_EX_MODE_BUTTONS));

	registerInfo<wxPropertyGridManager>(rb_cWXPropertyGridManager);
#endif

}
