/*
 * wxColor.hpp
 *
 *  Created on: 15.02.2012
 *      Author: hanmac
 */

#ifndef WXCOLOR_HPP_
#define WXCOLOR_HPP_

#include "main.hpp"

extern VALUE rb_cWXColor;
void Init_WXColor(VALUE rb_mWX);


template <>
inline VALUE wrap< wxColor >(wxColor *color )
{
	return Data_Wrap_Struct(rb_cWXColor, NULL, free, color);
}

template <>
inline bool is_wrapable< wxColor >(const VALUE &vcolor)
{
	if (rb_obj_is_kind_of(vcolor, rb_cWXColor) ||
		rb_obj_is_kind_of(vcolor, rb_cString)){
		return true;
	}else if(rb_respond_to(vcolor,rb_intern("red")) &&
		rb_respond_to(vcolor,rb_intern("blue")) &&
		rb_respond_to(vcolor,rb_intern("green")) &&
		rb_respond_to(vcolor,rb_intern("alpha"))){
		return true;
	}else
		return false;
}

template <>
inline wxColor* wrap< wxColor* >(const VALUE &vcolor)
{
	if(rb_obj_is_kind_of(vcolor, rb_cString)){
		return new wxColour(wrap<wxString>(vcolor));
	}else if(!rb_obj_is_kind_of(vcolor, rb_cWXColor) &&
		rb_respond_to(vcolor,rb_intern("red")) &&
		rb_respond_to(vcolor,rb_intern("blue")) &&
		rb_respond_to(vcolor,rb_intern("green")) &&
		rb_respond_to(vcolor,rb_intern("alpha"))){
		double red,blue,green,alpha;
		wxColor *color = new wxColor;
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

		color->Set(red,blue,green,alpha);

		return color;
	}else{
		return unwrapPtr<wxColor>(vcolor, rb_cWXColor);
	}
}
template <>
inline wxColor wrap< wxColor >(const VALUE &vcolor)
{
	return *wrap<wxColor*>(vcolor);
}
#endif /* WXCOLOR_HPP_ */
