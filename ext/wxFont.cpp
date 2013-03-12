/*
 * wxFont.cpp
 *
 *  Created on: 28.02.2012
 *      Author: hanmac
 */

#include "wxFont.hpp"
#include "wxApp.hpp"
#include "wxSize.hpp"

#define _self unwrap<wxFont*>(self)

VALUE rb_cWXFont;

template <>
wxFont nullPtr<wxFont>(){ return wxNullFont;}


namespace RubyWX {
namespace Font {

APP_PROTECT(wxFont)

macro_attr(PointSize,int)
macro_attr(PixelSize,wxSize)
macro_attr_enum(Family,wxFontFamily)
macro_attr_enum(Style,wxFontStyle)
macro_attr_enum(Weight,wxFontWeight)

macro_attr(FaceName,wxString)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE size,family,style,weight;
	rb_scan_args(argc, argv, "13",&size,&family,&style,&weight);
	_self->Create(NUM2INT(size),
		unwrapenum<wxFontFamily>(family),
		unwrapenum<wxFontStyle>(style),
		unwrapenum<wxFontWeight>(weight));

//	rb_call_super(argc,argv);
	return self;
}

singlereturn(Smaller)
singlereturn(Larger)

singlefunc(MakeSmaller)
singlefunc(MakeLarger)

}
}

DLL_LOCAL void Init_WXFont(VALUE rb_mWX)
{
	using namespace RubyWX::Font;
	rb_cWXFont = rb_define_class_under(rb_mWX,"Font",rb_cObject);
	rb_define_alloc_func(rb_cWXFont,_alloc);

	rb_define_method(rb_cWXFont,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXFont,"smaller",RUBY_METHOD_FUNC(_Smaller),0);
	rb_define_method(rb_cWXFont,"larger",RUBY_METHOD_FUNC(_Larger),0);

	rb_define_method(rb_cWXFont,"smaller!",RUBY_METHOD_FUNC(_MakeSmaller),0);
	rb_define_method(rb_cWXFont,"larger!",RUBY_METHOD_FUNC(_MakeLarger),0);

	rb_define_attr_method(rb_cWXFont,"point_size",_getPointSize,_setPointSize);

	rb_define_attr_method(rb_cWXFont,"family",_getFamily,_setFamily);
	rb_define_attr_method(rb_cWXFont,"style",_getStyle,_setStyle);
	rb_define_attr_method(rb_cWXFont,"weight",_getWeight,_setWeight);
	rb_define_attr_method(rb_cWXFont,"face_name",_getFaceName,_setFaceName);

	registerInfo<wxFont>(rb_cWXFont);

	registerEnum<wxFontWeight>("WX::FontWeight",wxFONTWEIGHT_NORMAL)
		->add(wxFONTWEIGHT_NORMAL,"normal")
		->add(wxFONTWEIGHT_LIGHT,"light")
		->add(wxFONTWEIGHT_BOLD,"bold");
	registerEnum<wxFontStyle>("WX::FontStyle",wxFONTSTYLE_NORMAL)
		->add(wxFONTSTYLE_NORMAL,"normal")
		->add(wxFONTSTYLE_ITALIC,"italic")
		->add(wxFONTSTYLE_SLANT,"slant");
	registerEnum<wxFontFamily>("WX::FontStyle",wxFONTFAMILY_DEFAULT)
		->add(wxFONTFAMILY_DEFAULT,"default")
		->add(wxFONTFAMILY_DECORATIVE,"decorative")
		->add(wxFONTFAMILY_ROMAN,"roman")
		->add(wxFONTFAMILY_SCRIPT,"script")
		->add(wxFONTFAMILY_SWISS,"swiss")
		->add(wxFONTFAMILY_MODERN,"modern")
		->add(wxFONTFAMILY_TELETYPE,"teletype");

}
