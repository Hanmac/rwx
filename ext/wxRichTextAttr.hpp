/*
 * wxRichTextAttr.hpp
 *
 *  Created on: 03.08.2014
 *      Author: hanmac
 */

#ifndef WXRICHTEXTATTR_HPP_
#define WXRICHTEXTATTR_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXRichTextAttr;

void Init_WXRichTextAttr(VALUE rb_mWX);

#if wxUSE_RICHTEXT
#include <wx/richtext/richtextctrl.h>

template <>
VALUE wrap< wxRichTextAttr >(wxRichTextAttr *color );

template <>
wxRichTextAttr unwrap< wxRichTextAttr >(const VALUE &vcolor);

#endif

#endif /* WXRICHTEXTATTR_HPP_ */
