/*
 * wxDialog.cpp
 *
 *  Created on: 12.02.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"
#include "wxSizer.hpp"

VALUE rb_cWXDialog;

#define _self wrap<wxDialog*>(self)

namespace RubyWX {
namespace Dialog {

VALUE _alloc(VALUE self)
{
	return wrap(new wxDialog,self);
}
macro_attr_with_func(EscapeId,wrapID,unwrapID)
macro_attr_with_func(ReturnCode,wrapID,unwrapID)
macro_attr_with_func(AffirmativeId,wrapID,unwrapID)


VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash,name;
	rb_scan_args(argc, argv, "12",&parent,&name,&hash);

	if(!_created) {
#if wxUSE_XRC
		if(rb_obj_is_kind_of(name,rb_cString)){
			if(!(wxXmlResource::Get()->LoadDialog(_self,wrap<wxWindow*>(parent),wrap<wxString>(name))))
				rb_raise(rb_eNameError,"Named Dialog '%s' is not found.",wrap<char*>(name));

		}
		else
#endif
		_self->Create(wrap<wxWindow*>(parent),wxID_ANY,wxEmptyString);
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

VALUE _ShowModal(VALUE self)
{
	return wrapID(_self->ShowModal());
}

VALUE _CreateButtonSizer(int argc,VALUE *argv,VALUE self)
{
	VALUE flags;
	int flag = 0;
	rb_scan_args(argc, argv, "*",&flags);
	for(int i= 0; i < RARRAY_LEN(flags);++i)
	{
		VALUE val = RARRAY_PTR(flags)[i];
		if(SYMBOL_P(val))
		{
			ID id = SYM2ID(val);
			if(id == rb_intern("ok"))
				flag |= wxOK;
			if(id == rb_intern("cancel"))
				flag |= wxCANCEL;
			if(id == rb_intern("yes"))
				flag |= wxYES;
			if(id == rb_intern("no"))
				flag |= wxNO;
			if(id == rb_intern("apply"))
				flag |= wxAPPLY;
			if(id == rb_intern("close"))
				flag |= wxCLOSE;
		}
	}
	return wrap(_self->CreateButtonSizer(flag));
}

}
}

void Init_WXDialog(VALUE rb_mWX)
{
	using namespace RubyWX::Dialog;
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);
	rb_define_alloc_func(rb_cWXDialog,_alloc);

	rb_define_method(rb_cWXDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXDialog,"show_modal",RUBY_METHOD_FUNC(_ShowModal),0);

	rb_define_method(rb_cWXDialog,"create_button_sizer",RUBY_METHOD_FUNC(_CreateButtonSizer),-1);

}
