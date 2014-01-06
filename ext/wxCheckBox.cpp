/*
 * wxCheckBox.cpp
 *
 *  Created on: 01.05.2012
 *      Author: hanmac
 */


#include "wxControl.hpp"

VALUE rb_cWXCheckBox;

#if wxUSE_CHECKBOX
#define _self unwrap<wxCheckBox*>(self)

namespace RubyWX {
namespace CheckBox {

macro_attr(Value,bool)

APP_PROTECT(wxCheckBox)

/*
 * call-seq:
 *   CheckBox.new(parent, [options])
 *
 * creates a new CheckBox widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	int style = 0;
	wxString label(wxEmptyString);

	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"style",style);
		set_hash_option(hash,"label",label);
	}

	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,label,wxDefaultPosition,wxDefaultSize,style);
	
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXCheckBox(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXCheckBox,"value",1,1);
#endif

#if wxUSE_CHECKBOX
	using namespace RubyWX::CheckBox;
	rb_cWXCheckBox = rb_define_class_under(rb_mWX,"CheckBox",rb_cWXControl);
	rb_define_alloc_func(rb_cWXCheckBox,_alloc);

	rb_define_method(rb_cWXCheckBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXCheckBox,"value",_getValue,_setValue);

	registerInfo<wxCheckBox>(rb_cWXCheckBox);
	registerEventType("checkbox",wxEVT_CHECKBOX);
#endif

}

