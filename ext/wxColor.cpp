/*
 * wxColor.cpp
 *
 *  Created on: 15.02.2012
 *      Author: hanmac
 */

#include "wxColor.hpp"

#define _self unwrap<wxColor*>(self)

VALUE rb_cWXColor;

template <>
VALUE wrap< wxColor >(wxColor *color )
{
	return Data_Wrap_Struct(rb_cWXColor, NULL, free, color);
}

template <>
bool is_wrapable< wxColor >(const VALUE &vcolor)
{
	if (rb_obj_is_kind_of(vcolor, rb_cWXColor) ||
		rb_obj_is_kind_of(vcolor, rb_cString) || FIXNUM_P(vcolor)){
		return true;
	} else if(rb_respond_to(vcolor,rb_intern("red")) &&
		rb_respond_to(vcolor,rb_intern("blue")) &&
		rb_respond_to(vcolor,rb_intern("green")) &&
		rb_respond_to(vcolor,rb_intern("alpha"))){
		return true;
	}else
		return false;
}

template <>
wxColor* unwrap< wxColor* >(const VALUE &vcolor)
{
	return unwrapPtr<wxColor>(vcolor, rb_cWXColor);
}

template <>
wxColor unwrap< wxColor >(const VALUE &vcolor)
{
	if(rb_obj_is_kind_of(vcolor, rb_cString)){
		return wxColour(unwrap<wxString>(vcolor));
	}else if(FIXNUM_P(vcolor))
		return wxColor(NUM2INT(vcolor));
	else if(!rb_obj_is_kind_of(vcolor, rb_cWXColor) &&
		rb_respond_to(vcolor,rb_intern("red")) &&
		rb_respond_to(vcolor,rb_intern("blue")) &&
		rb_respond_to(vcolor,rb_intern("green")) &&
		rb_respond_to(vcolor,rb_intern("alpha"))){
		double red,blue,green,alpha;
		wxColor color;
		red = NUM2DBL(rb_funcall(vcolor,rb_intern("red"),0));
		if(red < 1.0)
			red *=256;

		blue = NUM2DBL(rb_funcall(vcolor,rb_intern("blue"),0));
		if(blue < 1.0)
			blue *=256;

		green = NUM2DBL(rb_funcall(vcolor,rb_intern("green"),0));
		if(green < 1.0)
			green *=256;

		alpha = NUM2DBL(rb_funcall(vcolor,rb_intern("alpha"),0));
		if(alpha < 1.0)
			alpha *=256;

		color.Set(red,blue,green,alpha);

		return color;
	}else{
		return *unwrap<wxColor*>(vcolor);
	}


}

namespace RubyWX {
namespace Color {
DLL_LOCAL VALUE _alloc(VALUE self) {
	return wrap(new wxColor);
}

DLL_LOCAL VALUE _getRed(VALUE self)
{
	if(_self->IsOk())
		return INT2NUM(_self->Red());
	else
		return UINT2NUM(0);
}

DLL_LOCAL VALUE _getGreen(VALUE self)
{
	if(_self->IsOk())
		return INT2NUM(_self->Green());
	else
		return UINT2NUM(0);
}

DLL_LOCAL VALUE _getBlue(VALUE self)
{
	if(_self->IsOk())
		return INT2NUM(_self->Blue());
	else
		return UINT2NUM(0);
}

DLL_LOCAL VALUE _getAlpha(VALUE self)
{
	if(_self->IsOk())
		return INT2NUM(_self->Alpha());
	else
		return UINT2NUM(0);
}


DLL_LOCAL VALUE _setRed(VALUE self,VALUE val)
{
	if(_self->IsOk())
		_self->Set(NUM2CHR(val),_self->Green(),_self->Blue(),_self->Alpha());
	else
		_self->Set(NUM2CHR(val),0,0,0);
	return val;
}

DLL_LOCAL VALUE _setGreen(VALUE self,VALUE val)
{
	if(_self->IsOk())
		_self->Set(_self->Red(),NUM2CHR(val),_self->Blue(),_self->Alpha());
	else
		_self->Set(0,NUM2CHR(val),0,0);
	return val;
}
DLL_LOCAL VALUE _setBlue(VALUE self,VALUE val)
{
	if(_self->IsOk())
		_self->Set(_self->Red(),_self->Green(),NUM2CHR(val),_self->Alpha());
	else
		_self->Set(0,0,NUM2CHR(val),0);
	return val;
}
DLL_LOCAL VALUE _setAlpha(VALUE self,VALUE val)
{
	if(_self->IsOk())
		_self->Set(_self->Red(),_self->Green(),_self->Blue(),NUM2CHR(val));
	else
		_self->Set(0,0,0,NUM2CHR(val));
	return val;
}

DLL_LOCAL VALUE _tos(VALUE self)
{
	return wrap(wxToString(*_self));
}
/*
 * call-seq:
 *   Color.new(red,green,blue[,alpha])
 *
 * creates a new Color Object.
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE red,green,blue,alpha;
	rb_scan_args(argc, argv, "31",&red,&green,&blue,&alpha);
	_setRed(self,red);
	_setGreen(self,green);
	_setBlue(self,blue);
	_setAlpha(self,NIL_P(alpha) ? INT2NUM(wxALPHA_OPAQUE) : alpha);
	return self;
}
/*
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_setRed(self,_getRed(other));
	_setGreen(self,_getGreen(other));
	_setBlue(self,_getBlue(other));
	_setAlpha(self,_getAlpha(other));
	return result;
}

/*
 * call-seq:
 *   inspect -> String
 *
 * Human-readable description.
 * ===Return value
 * String
*/
DLL_LOCAL VALUE _inspect(VALUE self)
{
	return rb_sprintf( "%s(%d, %d, %d, %d)",
		rb_obj_classname( self ),
		FIX2INT(_getRed(self)),
		FIX2INT(_getGreen(self)),
		FIX2INT(_getBlue(self)),
		FIX2INT(_getAlpha(self)));
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
    ptr[0] = _getRed(self);
    ptr[1] = _getGreen(self);
    ptr[2] = _getBlue(self);
    ptr[3] = _getAlpha(self);
    return rb_ary_new4( 4, ptr );
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
    VALUE* ptr = RARRAY_PTR( data );
    _setRed(self, ptr[0]);
    _setGreen(self, ptr[1]);
    _setBlue(self, ptr[2]);
    _setAlpha(self, ptr[3]);
    return Qnil;
}



}
}


/*
 * Document-class: WX::Color
 *
 * This class represents an Color.
*/

/* Document-attr: red
 * returns the red value of Color. */
/* Document-attr: blue
 * returns the blue value of Color. */
/* Document-attr: green
 * returns the green value of Color. */
/* Document-attr: alpha
 * returns the alpha value of Color. */


DLL_LOCAL void Init_WXColor(VALUE rb_mWX)
{
	using namespace RubyWX::Color;

#if 0
	rb_define_attr(rb_cWXColor,"red",1,1);
	rb_define_attr(rb_cWXColor,"green",1,1);
	rb_define_attr(rb_cWXColor,"blue",1,1);
	rb_define_attr(rb_cWXColor,"alpha",1,1);
#endif


	rb_cWXColor = rb_define_class_under(rb_mWX,"Color",rb_cObject);
	rb_define_alloc_func(rb_cWXColor,_alloc);

	rb_define_method(rb_cWXColor,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
	rb_define_private_method(rb_cWXColor,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);


	rb_define_attr_method(rb_cWXColor,"red",_getRed,_setRed);
	rb_define_attr_method(rb_cWXColor,"green",_getGreen,_setGreen);
	rb_define_attr_method(rb_cWXColor,"blue",_getBlue,_setBlue);
	rb_define_attr_method(rb_cWXColor,"alpha",_getAlpha,_setAlpha);

	rb_define_method(rb_cWXColor,"to_s",RUBY_METHOD_FUNC(_tos),0);
	rb_define_method(rb_cWXColor,"inspect",RUBY_METHOD_FUNC(_inspect),0);

	rb_define_method(rb_cWXColor,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXColor,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);
}
