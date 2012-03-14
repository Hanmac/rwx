/*
 * wxDC.hpp
 *
 *  Created on: 07.03.2012
 *      Author: hanmac
 */

#ifndef WXDC_HPP_
#define WXDC_HPP_

#include "main.hpp"
#if wxUSE_GRAPHICS_CONTEXT
#include <wx/dcgraph.h>
#endif
extern VALUE rb_cWXDC;
void Init_WXDC(VALUE rb_mWX);
#if wxUSE_CONFIG
#include <wx/config.h>

template <>
inline VALUE wrap< wxDC >(wxDC* window)
{
	if(window==NULL)
		return Qnil;

	return wrap(window,rb_cWXDC);
}


template <>
inline wxDC* wrap< wxDC* >(const VALUE &vwindow)
{
	return unwrapPtr<wxDC>(vwindow, rb_cWXDC);
}
#endif

#endif /* WXDC_HPP_ */
