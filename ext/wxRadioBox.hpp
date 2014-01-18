/*
 * wxRadioBox.hpp
 *
 *  Created on: 18.01.2014
 *      Author: hanmac
 */

#ifndef WXRADIOBOX_HPP_
#define WXRADIOBOX_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXRadioBox;

void Init_WXRadioBox(VALUE rb_mWX);
#if wxUSE_RADIOBOX
#include <wx/radiobox.h>
#endif

#endif /* WXRADIOBOX_HPP_ */
