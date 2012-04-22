/*
 * wxTimer.hpp
 *
 *  Created on: 11.03.2012
 *      Author: hanmac
 */

#ifndef WXTIMER_HPP_
#define WXTIMER_HPP_

#include "main.hpp"

extern VALUE rb_cWXTimer;
void Init_WXTimer(VALUE rb_mWX);

#if wxUSE_TIMER
template <>
inline VALUE wrap< wxTimer >(wxTimer *timer )
{
	return wrap(timer,rb_cWXTimer);
}

template <>
inline wxTimer* wrap< wxTimer* >(const VALUE &vtimer)
{
	return unwrapPtr<wxTimer>(vtimer, rb_cWXTimer);
}
#endif

#endif /* WXTIMER_HPP_ */
