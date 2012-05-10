/*
 * wxEvent.hpp
 *
 *  Created on: 18.02.2012
 *      Author: hanmac
 */

#ifndef WXEVENT_HPP_
#define WXEVENT_HPP_

#include <map>

#include "main.hpp"

extern VALUE rb_cWXEvent;
void Init_WXEvent(VALUE rb_mWX);

template <>
VALUE wrap< wxEvent >(wxEvent *event );

template <>
wxEvent* wrap< wxEvent* >(const VALUE &vcolor);


#endif /* WXEVENT_HPP_ */
