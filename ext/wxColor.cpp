/*
 * wxColor.cpp
 *
 *  Created on: 15.02.2012
 *      Author: hanmac
 */

#include "wxColor.hpp"

#define _self wrap<wxColor*>(self)

VALUE rb_cWXColor;


namespace RubyWX {
namespace Color {
VALUE _alloc(VALUE self) {
	return wrap(new wxColor);
}

VALUE _getRed(VALUE self)
{
	if(_self->IsOk())
		return INT2NUM(_self->Red());
	else
		return UINT2NUM(0);
}

VALUE _getGreen(VALUE self)
{
	if(_self->IsOk())
		return INT2NUM(_self->Green());
	else
		return UINT2NUM(0);
}

VALUE _getBlue(VALUE self)
{
	if(_self->IsOk())
		return INT2NUM(_self->Blue());
	else
		return UINT2NUM(0);
}

VALUE _getAlpha(VALUE self)
{
	if(_self->IsOk())
		return INT2NUM(_self->Alpha());
	else
		return UINT2NUM(0);
}


VALUE _setRed(VALUE self,VALUE val)
{
	if(_self->IsOk())
		_self->Set(NUM2CHR(val),_self->Green(),_self->Blue(),_self->Alpha());
	else
		_self->Set(NUM2CHR(val),0,0,0);
	return val;
}

VALUE _setGreen(VALUE self,VALUE val)
{
	if(_self->IsOk())
		_self->Set(_self->Red(),NUM2CHR(val),_self->Blue(),_self->Alpha());
	else
		_self->Set(0,NUM2CHR(val),0,0);
	return val;
}
VALUE _setBlue(VALUE self,VALUE val)
{
	if(_self->IsOk())
		_self->Set(_self->Red(),_self->Green(),NUM2CHR(val),_self->Alpha());
	else
		_self->Set(0,0,NUM2CHR(val),0);
	return val;
}
VALUE _setAlpha(VALUE self,VALUE val)
{
	if(_self->IsOk())
		_self->Set(_self->Red(),_self->Green(),_self->Blue(),NUM2CHR(val));
	else
		_self->Set(0,0,0,NUM2CHR(val));
	return val;
}

VALUE _tos(VALUE self)
{
	return wrap(wxToString(*_self));
}
/*
 * call-seq:
 *   Color.new(red,green,blue[,alpha])
 *
 * creates a new Color Object.
*/
VALUE _initialize(int argc,VALUE *argv,VALUE self)
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
VALUE _initialize_copy(VALUE self, VALUE other)
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
VALUE _inspect(VALUE self)
{
	VALUE array[6];
	array[0]=rb_str_new2("#<%s:(%d, %d, %d, %d)>");
	array[1]=rb_class_of(self);
	array[2]=_getRed(self);
	array[3]=_getGreen(self);
	array[4]=_getBlue(self);
	array[5]=_getAlpha(self);
	return rb_f_sprintf(6,array);
}


}
}

void Init_WXColor(VALUE rb_mWX)
{
	using namespace RubyWX::Color;
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
}
