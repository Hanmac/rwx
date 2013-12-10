/*
 * wxPropertyGrid.cpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */

#include "wxPropertyGrid.hpp"
#include "wxPropertyGridInterface.hpp"

VALUE rb_cWXPropertyGrid;

#if wxUSE_PROPGRID
#define _self unwrap<wxPropertyGrid*>(self)

namespace RubyWX {
namespace PropertyGrid {

APP_PROTECT(wxPropertyGrid)

singlereturn(GetRoot)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	int style = wxPG_DEFAULT_STYLE;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!rb_obj_is_kind_of(hash,rb_cString))
	{
		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"style",style);
		}

		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,wxDefaultPosition,wxDefaultSize,style);
		_created = true;
	}

	rb_call_super(argc,argv);
	return self;
}


}
}
#endif
DLL_LOCAL void Init_WXPropertyGrid(VALUE rb_mWX)
{
#if wxUSE_PROPGRID
	using namespace RubyWX::PropertyGrid;
	rb_cWXPropertyGrid = rb_define_class_under(rb_mWX,"PropertyGrid",rb_cWXControl);
	rb_define_alloc_func(rb_cWXPropertyGrid,_alloc);

	rb_include_module(rb_cWXPropertyGrid,rb_mWXPropertyGridInterface);

	rb_define_method(rb_cWXPropertyGrid,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXPropertyGrid,"root",RUBY_METHOD_FUNC(_GetRoot),0);


	rb_define_const(rb_cWXPropertyGrid,"DEFAULT_STYLE",INT2NUM(wxPG_DEFAULT_STYLE));

	rb_define_const(rb_cWXPropertyGrid,"EX_HELP_AS_TOOLTIPS",INT2NUM(wxPG_EX_HELP_AS_TOOLTIPS));



	registerInfo<wxPropertyGrid>(rb_cWXPropertyGrid);
#endif

}


