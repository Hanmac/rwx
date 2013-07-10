/*
 * wxCheckListBox.cpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#include "wxCheckListBox.hpp"

VALUE rb_cWXCheckListBox;

#if wxUSE_CHECKLISTBOX

namespace RubyWX {
namespace CheckListBox {
#define _self unwrap<wxCheckListBox*>(self)

APP_PROTECT(wxCheckListBox)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!_created)
	{
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;
}

}
}

#endif
void Init_WXCheckListBox(VALUE rb_mWX)
{
#if wxUSE_CHECKLISTBOX
	using namespace RubyWX::CheckListBox;
	rb_cWXCheckListBox = rb_define_class_under(rb_mWX,"CheckListBox",rb_cWXListBox);
	rb_define_alloc_func(rb_cWXCheckListBox,_alloc);

	rb_define_method(rb_cWXCheckListBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxCheckListBox>(rb_cWXCheckListBox);
#endif

}

