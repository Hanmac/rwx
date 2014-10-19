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
	if(_self->GetRGB(NUM2UINT(idx),&red,&green,&blue))
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
		rb_ary_store(ary,i*3,UINT2NUM(red));
		rb_ary_store(ary,i*3+1,UINT2NUM(green));
		rb_ary_store(ary,i*3+2,UINT2NUM(blue));
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
	std::size_t count = RARRAY_LEN(data) / 3;

	unsigned char red[count];
	unsigned char green[count];
	unsigned char blue[count];

	for(std::size_t i = 0; i < count; ++i)
	{
		red[i]   = NUM2UINT(RARRAY_AREF(data,i*3));
		green[i] = NUM2UINT(RARRAY_AREF(data,i*3+1));
		blue[i]  = NUM2UINT(RARRAY_AREF(data,i*3+2));
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

	rb_define_method(rb_cWXPalette,"size",RUBY_METHOD_FUNC(_GetColoursCount),0);
	rb_define_method(rb_cWXPalette,"each",RUBY_METHOD_FUNC(_each),0);

	rb_define_method(rb_cWXPalette,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXPalette,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),-2);

	registerInfo<wxPalette>(rb_cWXPalette);
#endif

}


