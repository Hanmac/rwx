/*
 * wxTextAttr.hpp
 *
 *  Created on: 28.04.2012
 *      Author: hanmac
 */

#ifndef WXTEXTATTR_HPP_
#define WXTEXTATTR_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXTextAttr;

void Init_WXTextAttr(VALUE rb_mWX);

#if wxUSE_TEXTCTRL
#include <wx/textctrl.h>


template <>
VALUE wrap< wxTextAttr >(wxTextAttr *color );

template <>
wxTextAttr unwrap< wxTextAttr >(const VALUE &vcolor);

#endif

#endif /* WXTEXTATTR_HPP_ */
