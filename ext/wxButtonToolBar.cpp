/*
 * wxButtonToolBar.cpp
 *
 *  Created on: 06.12.2013
 *      Author: hanmac
 */


#include "wxButtonToolBar.hpp"
#include "wxToolBarBase.hpp"

VALUE rb_cWXButtonToolBar;

#if defined(__WXMAC__) && wxUSE_TOOLBAR && wxUSE_BMPBUTTON
#include <wx/generic/buttonbar.h>
#define _self unwrap<wxButtonToolBar*>(self)

namespace RubyWX {
namespace ButtonToolBar {

APP_PROTECT(wxButtonToolBar)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!rb_obj_is_kind_of(name,rb_cString)){
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		
	}
	rb_call_super(argc,argv);
	return self;

}

}
}

#endif

DLL_LOCAL void Init_WXButtonToolBar(VALUE rb_mWX)
{
#if defined(__WXMAC__) && wxUSE_TOOLBAR && wxUSE_BMPBUTTON
	using namespace RubyWX::ButtonToolBar;
	rb_cWXButtonToolBar = rb_define_class_under(rb_mWX,"ButtonToolBar",rb_cWXToolBarBase);
	rb_define_alloc_func(rb_cWXButtonToolBar,_alloc);

	rb_define_method(rb_cWXButtonToolBar,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxButtonToolBar>(rb_cWXButtonToolBar);
#endif
}


