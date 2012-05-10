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
VALUE wrap< wxColor >(wxColor *color );

template <>
bool is_wrapable< wxColor >(const VALUE &vcolor);

template <>
wxColor* wrap< wxColor* >(const VALUE &vcolor);

template <>
wxColor wrap< wxColor >(const VALUE &vcolor);

#endif /* WXCOLOR_HPP_ */
