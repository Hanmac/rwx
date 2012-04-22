/*
 * wxPoint.cpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#include "wxPoint.hpp"
VALUE rb_cWXPoint;

#define _self wrap<wxPoint*>(self)


namespace RubyWX {
namespace Point {

VALUE _getX(VALUE self)
{
	return INT2NUM(_self->x);
}

VALUE _getY(VALUE self)
{
	return INT2NUM(_self->y);
}

VALUE _setX(VALUE self,VALUE val)
{
	_self->x = NUM2INT(val);
	return val;
}

VALUE _setY(VALUE self,VALUE val)
{
	_self->y = NUM2INT(val);
	return val;
}

VALUE _alloc(VALUE self)
{
	return wrap(new wxPoint());
}

VALUE _initialize(VALUE self,VALUE x,VALUE y)
{
	_setX(self,x);
	_setY(self,y);
	return self;
}

/*
*/
VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_setX(self,_getX(other));
	_setY(self,_getY(other));
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
	VALUE array[4];
	array[0]=rb_str_new2("#<%s:(%d, %d)>");
	array[1]=rb_class_of(self);
	array[2]=_getX(self);
	array[3]=_getY(self);
	return rb_f_sprintf(4,array);
}

}
}


void Init_WXPoint(VALUE rb_mWX)
{
	using namespace RubyWX::Point;
	rb_cWXPoint = rb_define_class_under(rb_mWX,"Point",rb_cObject);

	rb_define_alloc_func(rb_cWXPoint,_alloc);

	rb_define_method(rb_cWXPoint,"initialize",RUBY_METHOD_FUNC(_initialize),4);
	rb_define_private_method(rb_cWXPoint,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXPoint,"x",_getX,_setX);
	rb_define_attr_method(rb_cWXPoint,"y",_getY,_setY);

	rb_define_method(rb_cWXPoint,"inspect",RUBY_METHOD_FUNC(_inspect),0);
}



