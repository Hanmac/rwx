/*
 * wxRect.hpp
 *
 *  Created on: 23.03.2012
 *      Author: hanmac
 */

#ifndef WXRECT_HPP_
#define WXRECT_HPP_

#include "main.hpp"

extern VALUE rb_cWXRect;
void Init_WXRect(VALUE rb_mWX);


template <>
VALUE wrap< wxRect >(wxRect *size );

template <>
bool is_wrapable< wxRect >(const VALUE &vsize);

template <>
wxRect* unwrap< wxRect* >(const VALUE &vsize);

template <>
wxRect unwrap< wxRect >(const VALUE &vsize);

#endif /* WXRECT_HPP_ */
