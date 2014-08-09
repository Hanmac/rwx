/*
 * wxRichTextStyleSheet.hpp
 *
 *  Created on: 03.08.2014
 *      Author: hanmac
 */

#ifndef WXRICHTEXTSTYLESHEET_HPP_
#define WXRICHTEXTSTYLESHEET_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXRichTextStyleSheet;

void Init_WXRichTextStyleSheet(VALUE rb_mWX);

#if wxUSE_RICHTEXT
#include <wx/richtext/richtextstyles.h>
#endif

#endif /* WXRICHTEXTSTYLESHEET_HPP_ */
