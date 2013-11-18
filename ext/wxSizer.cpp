/*
 * wxSizer.cpp
 *
 *  Created on: 20.03.2012
 *      Author: hanmac
 */
#include "wxSizer.hpp"
#include "wxSizerItem.hpp"
#include "wxWindow.hpp"

#define _self unwrap<wxSizer*>(self)

VALUE rb_cWXSizer;

template <>
wxSizerFlags unwrap< wxSizerFlags >(const VALUE &hash)
{
	wxSizerFlags result;
	if(!rb_obj_is_kind_of(hash,rb_cHash))
		return result;
	VALUE val;

	if(RTEST(rb_hash_aref(hash,ID2SYM(rb_intern("expand")))))
		result.Expand();
	if(!NIL_P(val=rb_hash_aref(hash,ID2SYM(rb_intern("proportion")))))
			result.Proportion(NUM2INT(val));
	if(!NIL_P(val=rb_hash_aref(hash,ID2SYM(rb_intern("align")))))
	{
		if(SYMBOL_P(val))
		{
			if(SYM2ID(val) == rb_intern("left"))
				result.Left();
			if(SYM2ID(val) == rb_intern("right"))
				result.Right();
			if(SYM2ID(val) == rb_intern("bottom"))
				result.Bottom();
			if(SYM2ID(val) == rb_intern("top"))
				result.Top();
			if(SYM2ID(val) == rb_intern("center"))
				result.Center();

		}
	}

	return result;
}


template <>
wxSizer* unwrap<wxSizer*>(const VALUE &arg)
{
	return unwrapPtr<wxSizer>(arg,rb_cWXSizer);
}


namespace RubyWX {
namespace Sizer {


DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE args;
	rb_scan_args(argc, argv, "*",&args);
	//rb_call_super(argc,argv);
	if(rb_block_given_p())
		rb_yield(self);
	return self;
}


DLL_LOCAL VALUE _add(int argc,VALUE *argv,VALUE self)
{
	VALUE obj,hash;
	rb_scan_args(argc, argv, "11",&obj,&hash);

	if(rb_obj_is_kind_of(obj, rb_cWXWindow))
	{
		return wrap(_self->Add(unwrap<wxWindow*>(obj),unwrap<wxSizerFlags>(hash)));
	}else if(rb_obj_is_kind_of(obj, rb_cWXSizer))
	{
		return wrap(_self->Add(unwrap<wxSizer*>(obj),unwrap<wxSizerFlags>(hash)));
	}else {
		const wxSize &size = unwrap<wxSize>(obj);
		return wrap(_self->Add(size.GetWidth(),size.GetHeight(),unwrap<wxSizerFlags>(hash)));
	}
}


DLL_LOCAL VALUE _insert(int argc,VALUE *argv,VALUE self)
{
	VALUE index,obj,hash;
	rb_scan_args(argc, argv, "21",&index,&obj,&hash);

	if(rb_obj_is_kind_of(obj, rb_cWXWindow))
		return wrap(_self->Insert(NUM2INT(index),unwrap<wxWindow*>(obj),unwrap<wxSizerFlags>(hash)));
	else if(rb_obj_is_kind_of(obj, rb_cWXSizer))
		return wrap(_self->Insert(NUM2INT(index),unwrap<wxSizer*>(obj),unwrap<wxSizerFlags>(hash)));
	else {
		const wxSize &size = unwrap<wxSize>(obj);
		return wrap(_self->Insert(NUM2INT(index),size.GetWidth(),size.GetHeight(),unwrap<wxSizerFlags>(hash)));
	}
}


DLL_LOCAL VALUE _prepend(int argc,VALUE *argv,VALUE self)
{
	VALUE obj,hash;
	rb_scan_args(argc, argv, "11",&obj,&hash);

	if(rb_obj_is_kind_of(obj, rb_cWXWindow))
		return wrap(_self->Prepend(unwrap<wxWindow*>(obj),unwrap<wxSizerFlags>(hash)));
	else if(rb_obj_is_kind_of(obj, rb_cWXSizer))
		return wrap(_self->Prepend(unwrap<wxSizer*>(obj),unwrap<wxSizerFlags>(hash)));
	else {
		const wxSize &size = unwrap<wxSize>(obj);
		return wrap(_self->Prepend(size.GetWidth(),size.GetHeight(),unwrap<wxSizerFlags>(hash)));
	}
}


DLL_LOCAL VALUE _getItem(VALUE self,VALUE index)
{
	return wrap(_self->GetItem(NUM2UINT(index)));
}

DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_ENUMERATOR(self,0,NULL);
	size_t  count = _self->GetItemCount();
	for(size_t i = 0; i < count; ++i)
		rb_yield(wrap(_self->GetItem(i)));
	return self;
}


}
}

DLL_LOCAL void Init_WXSizer(VALUE rb_mWX)
{
	using namespace RubyWX::Sizer;
	rb_cWXSizer = rb_define_class_under(rb_mWX,"Sizer",rb_cObject);
	rb_undef_alloc_func(rb_cWXSizer);
	rb_define_method(rb_cWXSizer,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXSizer,"add",RUBY_METHOD_FUNC(_add),-1);
	rb_define_method(rb_cWXSizer,"insert",RUBY_METHOD_FUNC(_insert),-1);
	rb_define_method(rb_cWXSizer,"prepend",RUBY_METHOD_FUNC(_prepend),-1);

	rb_define_method(rb_cWXSizer,"[]",RUBY_METHOD_FUNC(_getItem),1);

	rb_define_method(rb_cWXSizer,"each",RUBY_METHOD_FUNC(_each),0);
	rb_include_module(rb_cWXSizer,rb_mEnumerable);

	registerInfo<wxSizer>(rb_cWXSizer);
}

