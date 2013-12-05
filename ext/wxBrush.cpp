/*
 * wxBrush.cpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#include "wxBrush.hpp"
#include "wxColor.hpp"
#include "wxBitmap.hpp"

#define _self unwrap<wxBrush*>(self)

VALUE rb_cWXBrush;

template <>
wxBrush* nullPtr<wxBrush*>(){ return &wxNullBrush;}
template <>
wxBrush nullPtr<wxBrush>(){ return wxNullBrush;}

namespace RubyWX {
namespace Brush {
DLL_LOCAL VALUE _alloc(VALUE self) {
	return wrap(new wxBrush(*wxBLACK));
}

macro_attr(Colour,wxColour)
macro_attr_enum(Style,wxBrushStyle)
macro_attr(Stipple,wxBitmap)

singlereturn(IsHatch)


/*
 * call-seq:
 *   Brush.new(color[,style])
 *
 * creates a new Brush Object.
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE color,style;
	rb_scan_args(argc, argv, "11",&color,&style);
	_setColour(self,color);
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
    VALUE ptr[3];
    ptr[0] = _getColour(self);
    ptr[1] = _getStyle(self);
    ptr[2] = _getStipple(self);
    return rb_ary_new4(3, ptr);
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
    _setStyle(self, ptr[1]);
    _setStipple(self, ptr[2]);
    return Qnil;
}


/*
 * call-seq:
 *   == brush -> bool
 *
 * compares two brush.
 *
 *
 */
DLL_LOCAL VALUE _equal(VALUE self, VALUE other)
{
	return wrap((*_self) == unwrap<wxBrush>(other));
}


}
}

DLL_LOCAL void Init_WXBrush(VALUE rb_mWX)
{
#if 0

	rb_define_attr(rb_cWXBrush,"color",1,1);
	rb_define_attr(rb_cWXBrush,"style",1,1);
	rb_define_attr(rb_cWXBrush,"stipple",1,1);

#endif

	using namespace RubyWX::Brush;
	rb_cWXBrush = rb_define_class_under(rb_mWX,"Brush",rb_cObject);
	rb_define_alloc_func(rb_cWXBrush,_alloc);

	rb_define_method(rb_cWXBrush,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
	rb_define_private_method(rb_cWXBrush,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXBrush,"color",_getColour,_setColour);
	rb_define_attr_method(rb_cWXBrush,"style",_getStyle,_setStyle);
	rb_define_attr_method(rb_cWXBrush,"stipple",_getStipple,_setStipple);

	rb_define_method(rb_cWXBrush,"hatch?",RUBY_METHOD_FUNC(_IsHatch),0);

	rb_define_method(rb_cWXBrush,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXBrush,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

	rb_define_method(rb_cWXBrush,"==",RUBY_METHOD_FUNC(_equal),1);

	//
//	rb_define_method(rb_cWXBrush,"to_s",RUBY_METHOD_FUNC(_tos),0);
//	rb_define_method(rb_cWXBrush,"inspect",RUBY_METHOD_FUNC(_inspect),0);

	registerInfo<wxBrush>(rb_cWXBrush);
	registerEnum<wxBrushStyle>("WX::BrushStyle",wxBRUSHSTYLE_TRANSPARENT)
		->add(wxBRUSHSTYLE_SOLID,"solid")
		->add(wxBRUSHSTYLE_TRANSPARENT,"transparent")
		->add(wxBRUSHSTYLE_STIPPLE_MASK_OPAQUE,"stipple_mask_opaque")
		->add(wxBRUSHSTYLE_STIPPLE_MASK,"stipple_mask")
		->add(wxBRUSHSTYLE_STIPPLE,"stipple")
		->add(wxBRUSHSTYLE_BDIAGONAL_HATCH,"bdiagonal_hatch")
		->add(wxBRUSHSTYLE_CROSSDIAG_HATCH,"crossdiag_hatch")
		->add(wxBRUSHSTYLE_FDIAGONAL_HATCH,"fdiagonal_hatch")
		->add(wxBRUSHSTYLE_CROSS_HATCH,"cross_hatch")
		->add(wxBRUSHSTYLE_HORIZONTAL_HATCH,"horizontal_hatch")
		->add(wxBRUSHSTYLE_VERTICAL_HATCH,"vertical_hatch");

}




