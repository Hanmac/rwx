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

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash,name;
	rb_scan_args(argc, argv, "12",&parent,&name,&hash);

	if(!_created) {
#if wxUSE_XRC
		if(rb_obj_is_kind_of(name,rb_cString)){
			wxXmlResource::Get()->LoadPanel(_self,unwrap<wxWindow*>(parent),unwrap<wxString>(name));
			rb_raise(rb_eNameError,"Named Panel '%s' is not found.",unwrap<char*>(name));
		}
		else
#endif
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}
	if(rb_obj_is_kind_of(name,rb_cString)){
		VALUE args[] = {parent,hash};

		rb_call_super(2,args);
	}else {
		rb_call_super(argc,argv);
	}

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





