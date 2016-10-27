/*
 * wxPalette.cpp
 *
 *  Created on: 20.11.2013
 *      Author: hanmac
 */

#include "wxPalette.hpp"
#include "wxColor.hpp"

#if wxUSE_PALETTE

#define _self unwrap<wxPalette*>(self)

VALUE rb_cWXPalette;

template <>
wxPalette* unwrap< wxPalette* >(const VALUE &vimage)
{
	return unwrapTypedPtr<wxPalette>(vimage, rb_cWXPalette);
}


template <>
wxPalette unwrap< wxPalette >(const VALUE &vimage)
{
	return *unwrap<wxPalette*>(vimage);
}


namespace RubyWX {
namespace Palette {

singlereturn(GetColoursCount)

DLL_LOCAL VALUE _alloc(VALUE self) {
	return wrap(new wxPalette);
}

DLL_LOCAL VALUE _initialize(int argc, VALUE *argv, VALUE self) {
	if(argc > 0)
	{
		unsigned char red[argc];
		unsigned char green[argc];
		unsigned char blue[argc];
		for(std::size_t i = 0; i < (std::size_t)argc; ++i)
		{
			wxColor temp = unwrap<wxColor>(argv[i]);
			red[i]   = temp.Red();
			green[i] = temp.Green();
			blue[i]  = temp.Blue();
		}

		_self->Create(argc,red,green,blue);
	}
	return self;
}

DLL_LOCAL VALUE _initialize_copy(VALUE self,VALUE other) {
	wxPalette *cother = unwrap<wxPalette*>(other);
	std::size_t count(cother->GetColoursCount());

	unsigned char red[count];
	unsigned char green[count];
	unsigned char blue[count];

	for(std::size_t i = 0; i < count; ++i)
	{
		cother->GetRGB(i,&red[i],&green[i],&blue[i]);
	}

	_self->Create(count,red,green,blue);

	return self;
}

/*
 * call-seq:
 *   image[idx] -> WX::Color or nil
 *
 * Returns the color at the given position or nil if out of range
 * ===Arguments
 * * idx is Integer
 * ===Return value
 * WX::Color or nil
*/
DLL_LOCAL VALUE _get(VALUE self,VALUE idx)
{
	unsigned char red,green,blue;
	if(_self->GetRGB(RB_NUM2UINT(idx),&red,&green,&blue))
		return wrap(new wxColor(red,green,blue));
	return Qnil;
}


/*
 * call-seq:
 *   each -> Enumerator
 *   each { |color| } -> self
 *
 * iterates the colors in this Palette.
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_GetColoursCount));

	unsigned char red,green,blue;

	std::size_t count = _self->GetColoursCount();
	for(std::size_t i = 0; i < count; ++i)
	{
		_self->GetRGB(i,&red,&green,&blue);
		rb_yield(wrap(new wxColor(red,green,blue)));
	}
	return self;
}


/*
 * call-seq:
 *   hash -> Fixnum
 *
 * Generates a Fixnum hash value for this object.
 *
 *
 */
DLL_LOCAL VALUE _getHash(VALUE self)
{
	std::size_t count = _self->GetColoursCount();
	st_index_t h = rb_hash_start(count);

	for(std::size_t i = 0; i < count; ++i)
	{
		unsigned char red,green,blue;
		_self->GetRGB(i,&red,&green,&blue);
		h = rb_hash_uint32(h, red | (green << 8) | (blue << 16));
	}

	h = rb_hash_end(h);
	return RB_LONG2FIX(h);
}

bool check_equal(const wxPalette &self, const wxPalette &cother)
{
	if(self.GetColoursCount() != cother.GetColoursCount()){
		return false;
	}

	std::size_t count = self.GetColoursCount();
	for(std::size_t i = 0; i < count; ++i)
	{

		unsigned char red, green, blue;
		unsigned char cred, cgreen, cblue;
		self.GetRGB(i,&red,&green,&blue);

		cother.GetRGB(i,&cred,&cgreen,&cblue);
		
		if(red != cred || green != cgreen || blue != cblue)
			return false;
	}
	return true;
}

struct equal_obj {
	wxPalette* self;
	VALUE other;
};

VALUE _equal_block(equal_obj *obj)
{
	return wrap(check_equal(*obj->self, unwrap<wxPalette>(obj->other)));
}

/*
 * call-seq:
 *   == palette -> bool
 *
 * compares two palettes.
 *
 *
 */
DLL_LOCAL VALUE _equal(VALUE self, VALUE other)
{
	equal_obj obj;
	obj.self = _self;
	obj.other = other;

	return rb_rescue(
		RUBY_METHOD_FUNC(_equal_block),(VALUE)&obj,
		RUBY_METHOD_FUNC(_equal_rescue),Qnil
	);
}

/*
 * call-seq:
 *   marshal_dump -> Array
 *
 * Provides marshalling support for use by the Marshal library.
 * ===Return value
 * Array
 */
DLL_LOCAL VALUE _marshal_dump(VALUE self)
{
	VALUE ary = rb_ary_new(); //2(_self->GetColoursCount()*3);
	std::size_t count = _self->GetColoursCount();
	for(std::size_t i = 0; i < count; ++i)
	{
		unsigned char red,green,blue;
		_self->GetRGB(i,&red,&green,&blue);
		rb_ary_push(ary, RB_UINT2NUM(red | (green << 8) | (blue << 16)));
	}
	return ary;
}

/*
 * call-seq:
 *   marshal_load(array) -> nil
 *
 * Provides marshalling support for use by the Marshal library.
 *
 *
 */
DLL_LOCAL VALUE _marshal_load(VALUE self,VALUE data)
{
	data = rb_Array(data);
	std::size_t count = RARRAY_LEN(data);

	unsigned char red[count];
	unsigned char green[count];
	unsigned char blue[count];

	for(std::size_t i = 0; i < count; ++i)
	{
		unsigned int colRGB = RB_NUM2UINT(RARRAY_AREF(data,i));
		red[i]   = 0xFF & colRGB;
		green[i] = 0xFF & (colRGB >> 8);
		blue[i]  = 0xFF & (colRGB >> 16);
	}

	_self->Create(count,red,green,blue);

	return self;
}



}
}

#endif

/* Document-method: size
 * call-seq:
 *   size -> Integer
 *
 * returns the count of colors in this Palette list.
 * ===Return value
 * Integer
 */

DLL_LOCAL void Init_WXPalette(VALUE rb_mWX)
{
#if wxUSE_PALETTE

	using namespace RubyWX::Palette;
	rb_cWXPalette = rb_define_class_under(rb_mWX,"Palette",rb_cObject);
	rb_define_alloc_func(rb_cWXPalette,_alloc);

	rb_include_module(rb_cWXPalette,rb_mEnumerable);

	rb_define_method(rb_cWXPalette,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
	rb_define_private_method(rb_cWXPalette,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_method(rb_cWXPalette,"[]",RUBY_METHOD_FUNC(_get),1);

	rb_define_method(rb_cWXPalette,"hash",RUBY_METHOD_FUNC(_getHash),0);

	rb_define_method(rb_cWXPalette,"==",RUBY_METHOD_FUNC(_equal),1);
	rb_define_alias(rb_cWXPalette,"eql?","==");

	rb_define_method(rb_cWXPalette,"size",RUBY_METHOD_FUNC(_GetColoursCount),0);
	rb_define_method(rb_cWXPalette,"each",RUBY_METHOD_FUNC(_each),0);

	rb_define_method(rb_cWXPalette,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXPalette,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

	registerInfo<wxPalette>(rb_cWXPalette);
#endif

}


