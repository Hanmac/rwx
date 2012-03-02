/*
 * wxFont.cpp
 *
 *  Created on: 28.02.2012
 *      Author: hanmac
 */

#include "wxFont.hpp"
#include "wxApp.hpp"

#define _self wrap<wxFont*>(self)

VALUE rb_cWXFont;


namespace RubyWX {
namespace Font {
VALUE _alloc(VALUE self) {
	if(ruby_app_inited)
		return wrap(new wxFont);
	else
		rb_raise(rb_eArgError,"%s is not running.",rb_class2name(rb_cWXApp));
	return Qnil;

}

macro_attr(PointSize,int)
macro_attr(PixelSize,wxSize)
macro_attr(Family,wxFontFamily)
macro_attr(Style,wxFontStyle)
macro_attr(Weight,wxFontWeight)

macro_attr(FaceName,wxString)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE size,family,style,weight;
	rb_scan_args(argc, argv, "13",&size,&family,&style,&weight);
	_self->Create(NUM2INT(size),
		wrap<wxFontFamily>(family),
		wrap<wxFontStyle>(style),
		wrap<wxFontWeight>(weight));

//	rb_call_super(argc,argv);
	return self;
}


}
}

void Init_WXFont(VALUE rb_mWX)
{
	using namespace RubyWX::Font;
	rb_cWXFont = rb_define_class_under(rb_mWX,"Font",rb_cObject);
	rb_define_alloc_func(rb_cWXFont,_alloc);

	rb_define_method(rb_cWXFont,"initialize",RUBY_METHOD_FUNC(_initialize),-1);


	rb_define_attr_method(rb_cWXFont,"pointSize",_getPointSize,_setPointSize);

	rb_define_attr_method(rb_cWXFont,"family",_getFamily,_setFamily);
	rb_define_attr_method(rb_cWXFont,"Style",_getStyle,_setStyle);
	rb_define_attr_method(rb_cWXFont,"weight",_getWeight,_setWeight);
	rb_define_attr_method(rb_cWXFont,"faceName",_getFaceName,_setFaceName);
}



