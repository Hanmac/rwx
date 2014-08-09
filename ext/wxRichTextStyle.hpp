/*
 * wxRichTextAttr.hpp
 *
 *  Created on: 03.08.2014
 *      Author: hanmac
 */

#ifndef WXRICHTEXTSTYLE_HPP_
#define WXRICHTEXTSTYLE_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXRichTextStyle;

void Init_WXRichTextStyle(VALUE rb_mWX);

#if wxUSE_RICHTEXT
#include <wx/richtext/richtextstyles.h>

template <>
wxRichTextStyleDefinition* unwrap< wxRichTextStyleDefinition* >(const VALUE &vcolor);

#endif

#endif /* WXRICHTEXTSTYLE_HPP_ */
