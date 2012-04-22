/*
 * wxGauge.hpp
 *
 *  Created on: 27.02.2012
 *      Author: hanmac
 */

#ifndef WXGAUGE_HPP_
#define WXGAUGE_HPP_

#include "main.hpp"

extern VALUE rb_cWXGauge;

void Init_WXGauge(VALUE rb_mWX);

#if wxUSE_GAUGE
template <>
inline VALUE wrap< wxGauge >(wxGauge* window)
{
	return wrap(window,rb_cWXGauge);
}

template <>
inline wxGauge* wrap< wxGauge* >(const VALUE &vwindow)
{
	return unwrapPtr<wxGauge>(vwindow, rb_cWXGauge);
}
#endif

#endif /* WXGAUGE_HPP_ */
