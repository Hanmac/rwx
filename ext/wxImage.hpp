/*
 * wxImage.hpp
 *
 *  Created on: 16.02.2012
 *      Author: hanmac
 */

#ifndef WXIMAGE_HPP_
#define WXIMAGE_HPP_

#include "wxSize.hpp"

extern VALUE rb_cWXImage;
void Init_WXImage(VALUE rb_mWX);

#if wxUSE_IMAGE
template <>
inline VALUE wrap< wxImage >(wxImage *image )
{
	return Data_Wrap_Struct(rb_cWXImage, NULL, NULL, image);
}

template <>
inline wxImage* wrap< wxImage* >(const VALUE &vimage)
{
	if(rb_obj_is_kind_of(vimage,rb_cWXImage))
		return unwrapPtr<wxImage>(vimage, rb_cWXImage);
	if(is_wrapable<wxSize>(vimage))
		return new wxImage(wrap<wxSize>(vimage));
	return unwrapPtr<wxImage>(rb_class_new_instance(1,const_cast<VALUE*>(&vimage),rb_cWXImage), rb_cWXImage);
}


template <>
inline wxImage wrap< wxImage >(const VALUE &vimage)
{
	return *wrap<wxImage*>(vimage);
}
#endif
#endif /* WXIMAGE_HPP_ */
