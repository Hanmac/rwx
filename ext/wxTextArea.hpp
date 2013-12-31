/*
 * wxTextArea.hpp
 *
 *  Created on: 30.12.2013
 *      Author: hanmac
 */

#ifndef WXTEXTAREA_HPP_
#define WXTEXTAREA_HPP_

#include "wxControl.hpp"

extern VALUE rb_mWXTextArea;

void Init_WXTextArea(VALUE rb_mWX);

#if wxUSE_TEXTCTRL
#include <wx/textctrl.h>
#endif

#endif /* WXTEXTAREA_HPP_ */
