/*
 * wxPen.hpp
 *
 *  Created on: 16.03.2012
 *      Author: hanmac
 */

#include "wxPen.hpp"
#include "wxColor.hpp"
#include "wxBitmap.hpp"

#define _self unwrap<wxPen*>(self)

VALUE rb_cWXPen;

template <>
VALUE wrap< wxPen >(wxPen *bitmap )
{
	if(!bitmap)
		return Qnil;
	if(!bitmap->IsOk())
		return Qnil;
	return wrapPtr(bitmap,rb_cWXPen);
}

template <>
VALUE wrap< wxPen >(const wxPen &bitmap )
{
	if(bitmap.IsOk())
		return wrapPtr(const_cast<wxPen*>(&bitmap),rb_cWXPen);
	return Qnil;
}

template <>
wxPen* unwrap< wxPen* >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return &wxNullPen;
	return unwrapPtr<wxPen>(vbitmap, rb_cWXPen);
}

template <>
wxPen unwrap< wxPen >(const VALUE &vbitmap)
{
	if(NIL_P(vbitmap))
		return wxNullPen;
	return *unwrap<wxPen*>(vbitmap);
}


namespace RubyWX {
namespace Pen {
DLL_LOCAL VALUE _alloc(VALUE self) {
	return wrap(new wxPen(*wxBLACK));
}

macro_attr(Width,int)
macro_attr(Colour,wxColour)
macro_attr_enum(Style,wxPenStyle)
macro_attr(Stipple,wxBitmap)


/*
 * call-seq:
 *   Brush.new(color[,style])
 *
 * creates a new Brush Object.
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE color, width, style;
	rb_scan_args(argc, argv, "12",&color,&width,&style);
	_setColour(self,color);
	if(!NIL_P(width))
		_setWidth(self,width);
	if(!NIL_P(style))
		_setStyle(self,style);
	return self;
}

/*
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_setColour(self,_getColour(other));
	_setWidth(self,_getWidth(other));
	_setStyle(self,_getStyle(other));
	_setStipple(self,_getStipple(other));
	return result;
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
    VALUE ptr[4];
    ptr[0] = _getColour(self);
    ptr[1] = _getWidth(self);
    ptr[2] = _getStyle(self);
    ptr[3] = _getStipple(self);
    return rb_ary_new4(4, ptr);
}

/*
 * call-seq:
 *   marshal_load(array) -> nil
 *
 * Provides marshalling support for use by the Marshal library.
 *
 *
 */
DLL_LOCAL VALUE _marshal_load(VALUE self, VALUE data)
{
    VALUE* ptr = RARRAY_PTR(data);
    _setColour(self, ptr[0]);
    _setWidth(self, ptr[1]);
    _setStyle(self, ptr[2]);
    _setStipple(self, ptr[3]);
    return Qnil;
}

/*
 * call-seq:
 *   == pen -> bool
 *
 * compares two pens.
 *
 *
 */
DLL_LOCAL VALUE _equal(VALUE self, VALUE other)
{
	return wrap((*_self) == unwrap<wxPen>(other));
}


}
}

DLL_LOCAL void Init_WXPen(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXPen,"width",1,1);
	rb_define_attr(rb_cWXPen,"color",1,1);

	rb_define_attr(rb_cWXPen,"style",1,1);
	rb_define_attr(rb_cWXPen,"stipple",1,1);
#endif

	using namespace RubyWX::Pen;
	rb_cWXPen = rb_define_class_under(rb_mWX,"Pen",rb_cObject);
	rb_define_alloc_func(rb_cWXPen,_alloc);

	rb_define_method(rb_cWXPen,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
	rb_define_private_method(rb_cWXPen,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXPen,"width",_getWidth,_setWidth);
	rb_define_attr_method(rb_cWXPen,"color",_getColour,_setColour);

	rb_define_attr_method(rb_cWXPen,"style",_getStyle,_setStyle);
	rb_define_attr_method(rb_cWXPen,"stipple",_getStipple,_setStipple);

	rb_define_method(rb_cWXPen,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXPen,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

	rb_define_method(rb_cWXPen,"==",RUBY_METHOD_FUNC(_equal),1);

//
//	rb_define_method(rb_cWXPen,"to_s",RUBY_METHOD_FUNC(_tos),0);
//	rb_define_method(rb_cWXPen,"inspect",RUBY_METHOD_FUNC(_inspect),0);

	registerEnum<wxPenStyle>("WX::PenStyle",wxPENSTYLE_TRANSPARENT)
		->add(wxPENSTYLE_SOLID,"solid")
		->add(wxPENSTYLE_DOT,"dot")
		->add(wxPENSTYLE_LONG_DASH,"long_dash")
		->add(wxPENSTYLE_SHORT_DASH,"short_dash")
		->add(wxPENSTYLE_DOT_DASH,"dot_dash")
		->add(wxPENSTYLE_USER_DASH,"user_dash")
		->add(wxPENSTYLE_TRANSPARENT,"transparent")
		->add(wxPENSTYLE_STIPPLE_MASK_OPAQUE,"stipple_mask_opaque")
		->add(wxPENSTYLE_STIPPLE_MASK,"stipple_mask")
		->add(wxPENSTYLE_STIPPLE,"stipple")
		->add(wxPENSTYLE_BDIAGONAL_HATCH,"bdiagonal_hatch")
		->add(wxPENSTYLE_CROSSDIAG_HATCH,"crossdiag_hatch")
		->add(wxPENSTYLE_FDIAGONAL_HATCH,"fdiagonal_hatch")
		->add(wxPENSTYLE_CROSS_HATCH,"cross_hatch")
		->add(wxPENSTYLE_HORIZONTAL_HATCH,"horizontal_hatch")
		->add(wxPENSTYLE_VERTICAL_HATCH,"vertical_hatch");

}


