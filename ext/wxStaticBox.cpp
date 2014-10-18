/*
 * wxStaticBox.cpp
 *
 *  Created on: 24.04.2012
 *      Author: hanmac
 */

#include "wxStaticBox.hpp"
#include "wxSizer.hpp"

VALUE rb_cWXStaticBox;

#if wxUSE_STATBOX
#define _self unwrap<wxStaticBox*>(self)

namespace RubyWX {
namespace StaticBox {

APP_PROTECT(wxStaticBox)

/*
 * call-seq:
 *   StaticBox.new(parent, name, [options])
 *   StaticBox.new(parent, [options])
 *
 * creates a new StaticBox widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		wxString label(wxEmptyString);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"label",label);
		}

		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id,label);
		}

	}

	rb_call_super(argc,argv);
	return self;
}

DLL_LOCAL VALUE _GetContainingSizer(VALUE self)
{
	wxSizer *result = _self->GetContainingSizer();
	if(!result)
		result = new wxStaticBoxSizer(_self,wxHORIZONTAL);
	return wrap(result);
}

}
}
#endif
DLL_LOCAL void Init_WXStaticBox(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif
#if wxUSE_STATBOX
	using namespace RubyWX::StaticBox;
	rb_cWXStaticBox = rb_define_class_under(rb_mWX,"StaticBox",rb_cWXControl);
	rb_define_alloc_func(rb_cWXStaticBox,_alloc);

	rb_define_method(rb_cWXStaticBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);


	rb_define_method(rb_cWXStaticBox,"containing_sizer",RUBY_METHOD_FUNC(_GetContainingSizer),0);// :nodoc:

	registerInfo<wxStaticBox>(rb_cWXStaticBox);
#endif

}




