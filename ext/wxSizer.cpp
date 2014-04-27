/*
 * wxSizer.cpp
 *
 *  Created on: 20.03.2012
 *      Author: hanmac
 */
#include "wxSizer.hpp"
#include "wxSizerItem.hpp"
#include "wxWindow.hpp"
#include "wxPoint.hpp"

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
	if(RTEST(rb_hash_aref(hash,ID2SYM(rb_intern("border")))))
			result.Border();
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

macro_attr(MinSize,wxSize)
singlereturn(GetSize)
singlereturn(GetPosition)

singlefunc(Layout)


DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE args;
	rb_scan_args(argc, argv, "*",&args);
	//rb_call_super(argc,argv);
	if(rb_block_given_p())
		rb_yield(self);
	return self;
}

/*
 * call-seq:
 *   add(window, [options])
 *   add(sizer, [options])
 *   add(size, [options])
 *
 * adds a new WX::Sizer::Item
 * ===Arguments
 * * window is a WX::Window
 * * sizer is a WX::Sizer
 * * size is a WX::Size
 *
 * *options: Hash with possible options to set:
 *   * expand true/false says if the element should expand to the whole size
 *   * proportion Integer
 * ===Return value
 * WX::Sizer::Item
 */
DLL_LOCAL VALUE _add(int argc,VALUE *argv,VALUE self)
{
	VALUE obj,hash;
	rb_scan_args(argc, argv, "11",&obj,&hash);

	wxSizerFlags flags(unwrap<wxSizerFlags>(hash));
	if(rb_obj_is_kind_of(obj, rb_cWXWindow))
	{
		return wrap(_self->Add(unwrap<wxWindow*>(obj),flags));
	}else if(rb_obj_is_kind_of(obj, rb_cWXSizer))
	{
		return wrap(_self->Add(unwrap<wxSizer*>(obj),flags));
	}else {
		const wxSize &size = unwrap<wxSize>(obj);
		return wrap(_self->Add(size.GetWidth(),size.GetHeight(),flags));
	}
}


/*
 * call-seq:
 *   add_spacer(size)
 *
 * adds a new spacer WX::Sizer::Item
 * ===Arguments
 * * size is a Integer
 * ===Return value
 * WX::Sizer::Item
 */
DLL_LOCAL VALUE _add_spacer(VALUE self,VALUE size)
{
	return wrap(_self->AddSpacer(NUM2INT(size)));
}

/*
 * call-seq:
 *   add_stretch_spacer([prop])
 *
 * adds a new stretch spacer WX::Sizer::Item
 * ===Arguments
 * * prop is a Integer
 * ===Return value
 * WX::Sizer::Item
 */
DLL_LOCAL VALUE _add_stretch_spacer(int argc,VALUE *argv,VALUE self)
{
	VALUE prop;
	rb_scan_args(argc, argv, "01",&prop);

	return wrap(_self->AddStretchSpacer(NIL_P(prop) ? 1 : NUM2INT(prop)));
}


/*
 * call-seq:
 *   insert(pos, window, [options])
 *   insert(pos, sizer, [options])
 *   insert(pos, size, [options])
 *
 * inserts a new WX::Sizer::Item into the given position.
 * ===Arguments
 * * pos is Integer
 * * window is a WX::Window
 * * sizer is a WX::Sizer
 * * size is a WX::Size
 *
 * *options: Hash with possible options to set:
 *   * expand true/false says if the element should expand to the whole size
 *   * proportion Integer
 * ===Return value
 * WX::Sizer::Item
 */
DLL_LOCAL VALUE _insert(int argc,VALUE *argv,VALUE self)
{
	VALUE index,obj,hash;
	rb_scan_args(argc, argv, "21",&index,&obj,&hash);

	wxSizerFlags flags(unwrap<wxSizerFlags>(hash));

	if(rb_obj_is_kind_of(obj, rb_cWXWindow))
		return wrap(_self->Insert(NUM2INT(index),unwrap<wxWindow*>(obj),flags));
	else if(rb_obj_is_kind_of(obj, rb_cWXSizer))
		return wrap(_self->Insert(NUM2INT(index),unwrap<wxSizer*>(obj),flags));
	else {
		const wxSize &size = unwrap<wxSize>(obj);
		return wrap(_self->Insert(NUM2INT(index),size.GetWidth(),size.GetHeight(),flags));
	}
}

/*
 * call-seq:
 *   insert_spacer(pos, size)
 *
 * inserts a new spacer WX::Sizer::Item into the given position.
 * ===Arguments
 * * pos is Integer
 * * size is a Integer
 * ===Return value
 * WX::Sizer::Item
 */
DLL_LOCAL VALUE _insert_spacer(VALUE self,VALUE idx,VALUE size)
{
	return wrap(_self->InsertSpacer(NUM2UINT(idx),NUM2INT(size)));
}

/*
 * call-seq:
 *   insert_stretch_spacer(pos,[prop])
 *
 * inserts a new stretch spacer WX::Sizer::Item
 * ===Arguments
 * * pos is Integer
 * * prop is a Integer
 * ===Return value
 * WX::Sizer::Item
 */
DLL_LOCAL VALUE _insert_stretch_spacer(int argc,VALUE *argv,VALUE self)
{
	VALUE idx,prop;
	rb_scan_args(argc, argv, "11",&idx,&prop);

	return wrap(_self->InsertStretchSpacer(NUM2UINT(idx),NIL_P(prop) ? 1 : NUM2INT(prop)));
}


/*
 * call-seq:
 *   prepend(window, [options])
 *   prepend(sizer, [options])
 *   prepend(size, [options])
 *
 * prepends a new WX::Sizer::Item
 * ===Arguments
 * * window is a WX::Window
 * * sizer is a WX::Sizer
 * * size is a WX::Size
 *
 * *options: Hash with possible options to set:
 *   * expand true/false says if the element should expand to the whole size
 *   * proportion Integer
 * ===Return value
 * WX::Sizer::Item
 */
DLL_LOCAL VALUE _prepend(int argc,VALUE *argv,VALUE self)
{
	VALUE obj,hash;
	rb_scan_args(argc, argv, "11",&obj,&hash);

	wxSizerFlags flags(unwrap<wxSizerFlags>(hash));

	if(rb_obj_is_kind_of(obj, rb_cWXWindow))
		return wrap(_self->Prepend(unwrap<wxWindow*>(obj),flags));
	else if(rb_obj_is_kind_of(obj, rb_cWXSizer))
		return wrap(_self->Prepend(unwrap<wxSizer*>(obj),flags));
	else {
		const wxSize &size = unwrap<wxSize>(obj);
		return wrap(_self->Prepend(size.GetWidth(),size.GetHeight(),flags));
	}
}

/*
 * call-seq:
 *   prepend_spacer(size)
 *
 * prepend a new spacer WX::Sizer::Item
 * ===Arguments
 * * size is a Integer
 * ===Return value
 * WX::Sizer::Item
 */
DLL_LOCAL VALUE _prepend_spacer(VALUE self,VALUE size)
{
	return wrap(_self->PrependSpacer(NUM2INT(size)));
}

/*
 * call-seq:
 *   prepend_stretch_spacer([prop])
 *
 * prepends a new stretch spacer WX::Sizer::Item
 * ===Arguments
 * * prop is a Integer
 * ===Return value
 * WX::Sizer::Item
 */
DLL_LOCAL VALUE _prepend_stretch_spacer(int argc,VALUE *argv,VALUE self)
{
	VALUE prop;
	rb_scan_args(argc, argv, "01",&prop);

	return wrap(_self->PrependStretchSpacer(NIL_P(prop) ? 1 : NUM2INT(prop)));
}



DLL_LOCAL VALUE _getItem(VALUE self,VALUE index)
{
	int cidx = NUM2INT(index);
	if(check_index(cidx,_self->GetItemCount()))
		return wrap(_self->GetItem(cidx));
	return Qnil;
}

DLL_LOCAL VALUE _each_size(VALUE self)
{
	return UINT2NUM(_self->GetItemCount());
}


DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_size));
	std::size_t  count(_self->GetItemCount());
	for(std::size_t i = 0; i < count; ++i)
		rb_yield(wrap(_self->GetItem(i)));
	return self;
}


DLL_LOCAL VALUE _remove(VALUE self,VALUE index)
{
	rb_check_frozen(self);
	int cidx = NUM2INT(index);
	if(check_index(cidx,_self->GetItemCount()))
		return wrap(_self->Remove(cidx));
	return Qnil;
}


}
}

DLL_LOCAL void Init_WXSizer(VALUE rb_mWX)
{
	using namespace RubyWX::Sizer;
	rb_cWXSizer = rb_define_class_under(rb_mWX,"Sizer",rb_cObject);
	rb_undef_alloc_func(rb_cWXSizer);

	rb_undef_method(rb_cWXSizer,"initialize_copy");
	rb_undef_method(rb_cWXSizer,"_load");
	rb_undef_method(rb_cWXSizer,"_dump");

	rb_define_method(rb_cWXSizer,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXSizer,"size",RUBY_METHOD_FUNC(_GetSize),0);
	rb_define_method(rb_cWXSizer,"position",RUBY_METHOD_FUNC(_GetPosition),0);

	rb_define_method(rb_cWXSizer,"layout",RUBY_METHOD_FUNC(_Layout),0);

	rb_define_method(rb_cWXSizer,"add",RUBY_METHOD_FUNC(_add),-1);
	rb_define_method(rb_cWXSizer,"insert",RUBY_METHOD_FUNC(_insert),-1);
	rb_define_method(rb_cWXSizer,"prepend",RUBY_METHOD_FUNC(_prepend),-1);

	rb_define_method(rb_cWXSizer,"add_spacer",RUBY_METHOD_FUNC(_add_spacer),1);
	rb_define_method(rb_cWXSizer,"insert_spacer",RUBY_METHOD_FUNC(_insert_spacer),2);
	rb_define_method(rb_cWXSizer,"prepend_spacer",RUBY_METHOD_FUNC(_prepend_spacer),1);

	rb_define_method(rb_cWXSizer,"add_stretch_spacer",RUBY_METHOD_FUNC(_add_stretch_spacer),-1);
	rb_define_method(rb_cWXSizer,"insert_stretch_spacer",RUBY_METHOD_FUNC(_insert_stretch_spacer),-1);
	rb_define_method(rb_cWXSizer,"prepend_stretch_spacer",RUBY_METHOD_FUNC(_prepend_stretch_spacer),-1);

	rb_define_method(rb_cWXSizer,"[]",RUBY_METHOD_FUNC(_getItem),1);

	rb_define_method(rb_cWXSizer,"each",RUBY_METHOD_FUNC(_each),0);
	rb_include_module(rb_cWXSizer,rb_mEnumerable);

	rb_define_method(rb_cWXSizer,"remove",RUBY_METHOD_FUNC(_remove),1);

	registerInfo<wxSizer>(rb_cWXSizer);
}

