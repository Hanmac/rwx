/*
 * wxSize.cpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */

#include "wxSize.hpp"
VALUE rb_cWXSize;

#define _self unwrap<wxSize*>(self)

ID rwxID_width,rwxID_height;

template <>
VALUE wrap< wxSize >(wxSize *size )
{
	return Data_Wrap_Struct(rb_cWXSize, NULL, free, size);
}

template <>
bool is_wrapable< wxSize >(const VALUE &vsize)
{
	if (rb_obj_is_kind_of(vsize, rb_cWXSize)){
		return true;
	}else if(rb_respond_to(vsize,rwxID_width) &&
		rb_respond_to(vsize,rwxID_height)){
		return true;
	}else
		return false;
}

template <>
wxSize unwrap< wxSize >(const VALUE &vsize)
{
	if(rb_obj_is_kind_of(vsize, rb_cArray)){
			wxSize size;
			size.SetWidth(NUM2INT(rb_ary_entry(vsize,0)));
			size.SetHeight(NUM2INT(rb_ary_entry(vsize,1)));
			return size;
	}else if(!rb_obj_is_kind_of(vsize, rb_cWXSize) &&
		rb_respond_to(vsize,rwxID_width) &&
		rb_respond_to(vsize,rwxID_height)){
		wxSize size;
		size.SetWidth(NUM2INT(rb_funcall(vsize,rwxID_width,0)));
		size.SetHeight(NUM2INT(rb_funcall(vsize,rwxID_height,0)));
		return size;
	}else{
		return *unwrap<wxSize*>(vsize);
	}
}


namespace RubyWX {
namespace Size {

macro_attr(Width,int)
macro_attr(Height,int)

DLL_LOCAL VALUE _alloc(VALUE self)
{
	return wrap(new wxSize());
}

DLL_LOCAL VALUE _initialize(VALUE self,VALUE width,VALUE height)
{
	_setWidth(self,width);
	_setHeight(self,height);
	return self;
}

/*
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_setWidth(self,_getWidth(other));
	_setHeight(self,_getHeight(other));
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
	return rb_sprintf( "%s(%d, %d)",
		rb_obj_classname( self ),
		NUM2INT(_getWidth(self)),
		NUM2INT(_getHeight(self)));
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
    VALUE ptr[2];
    ptr[0] = _getWidth(self);
    ptr[1] = _getHeight(self);
    return rb_ary_new4(2, ptr);
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
    _setWidth(self, ptr[0]);
    _setHeight(self, ptr[1]);
    return Qnil;
}

}
}


/*
 * Document-class: WX::Size
 *
 * This class represents an Size.
*/

/* Document-attr: width
 * returns the width value of Size. */
/* Document-attr: height
 * returns the height value of Size. */


DLL_LOCAL void Init_WXSize(VALUE rb_mWX)
{

#if 0
	rb_define_attr(rb_cWXSize,"width",1,1);
	rb_define_attr(rb_cWXSize,"height",1,1);
#endif

	using namespace RubyWX::Size;
	rb_cWXSize = rb_define_class_under(rb_mWX,"Size",rb_cObject);

	rb_define_alloc_func(rb_cWXSize,_alloc);
	rb_define_method(rb_cWXSize,"initialize",RUBY_METHOD_FUNC(_initialize),2);
	rb_define_private_method(rb_cWXSize,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cWXSize,"width",_getWidth,_setWidth);
	rb_define_attr_method(rb_cWXSize,"height",_getHeight,_setHeight);

	rb_define_method(rb_cWXSize,"inspect",RUBY_METHOD_FUNC(_inspect),0);

	rb_define_method(rb_cWXSize,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXSize,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

	registerType<wxSize>(rb_cWXSize);

	rwxID_width = rb_intern("width");
	rwxID_height = rb_intern("height");
}
