/*
 * wxListBox.cpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#include "wxListBox.hpp"
#include "wxItemContainer.hpp"

VALUE rb_cWXListBox;

#if wxUSE_LISTBOX

namespace RubyWX {
namespace ListBox {
#define _self unwrap<wxListBox*>(self)

APP_PROTECT(wxListBox)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
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

DLL_LOCAL VALUE _getSelections(VALUE self)
{
	wxArrayInt data;
	_self->GetSelections(data);
	return wrap(data);
}

}
}

#endif
DLL_LOCAL void Init_WXListBox(VALUE rb_mWX)
{
#if wxUSE_LISTBOX
	using namespace RubyWX::ListBox;
	rb_cWXListBox = rb_define_class_under(rb_mWX,"ListBox",rb_cWXControl);
	rb_define_alloc_func(rb_cWXListBox,_alloc);

	rb_define_method(rb_cWXListBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_include_module(rb_cWXListBox,rb_mWXItemContainer);

	rb_define_method(rb_cWXListBox,"selections",RUBY_METHOD_FUNC(_getSelections),0);

	registerEventType("listbox", wxEVT_LISTBOX,rb_cWXCommandEvent);
	registerEventType("listbox_dclick",  wxEVT_LISTBOX_DCLICK,rb_cWXCommandEvent);

	registerInfo<wxListBox>(rb_cWXListBox);
#endif

}


