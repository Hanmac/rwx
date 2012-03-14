/*
 * wxConfig.hpp
 *
 *  Created on: 06.03.2012
 *      Author: hanmac
 */

#ifndef WXCONFIG_HPP_
#define WXCONFIG_HPP_

#include "main.hpp"
extern VALUE rb_cWXConfig;
void Init_WXConfig(VALUE rb_mWX);
#if wxUSE_CONFIG
#include <wx/config.h>

template <>
inline VALUE wrap< wxConfig >(wxConfig* window)
{
	if(window==NULL)
		return Qnil;

	return wrap(window,rb_cWXConfig);
}


template <>
inline wxConfig* wrap< wxConfig* >(const VALUE &vwindow)
{
	return unwrapPtr<wxConfig>(vwindow, rb_cWXConfig);
}
#endif
#endif /* WXCONFIG_HPP_ */
