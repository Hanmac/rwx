/*
 * wxEvent.hpp
 *
 *  Created on: 18.02.2012
 *      Author: hanmac
 */

#ifndef WXEVENT_HPP_
#define WXEVENT_HPP_


#include "main.hpp"

extern VALUE rb_cWXEvent;
void Init_WXEvent(VALUE rb_mWX);


template <>
inline VALUE wrap< wxEvent >(wxEvent *color )
{
	return Data_Wrap_Struct(rb_cWXEvent, NULL, free, color);
}

template <>
inline wxEvent* wrap< wxEvent* >(const VALUE &vcolor)
{
	return unwrapPtr<wxEvent>(vcolor, rb_cWXEvent);
}


#endif /* WXEVENT_HPP_ */
