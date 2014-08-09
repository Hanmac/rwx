/*
 * wxRichTextCtrl.hpp
 *
 *  Created on: 22.06.2014
 *      Author: hanmac
 */

#ifndef WXRICHTEXTCTRL_HPP_
#define WXRICHTEXTCTRL_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXRichTextCtrl;

void Init_WXRichTextCtrl(VALUE rb_mWX);

#if wxUSE_RICHTEXT
#include <wx/richtext/richtextctrl.h>
#endif

#endif /* WXRICHTEXTCTRL_HPP_ */
