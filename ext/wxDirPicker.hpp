/*
 * wxDirPicker.hpp
 *
 *  Created on: 13.03.2012
 *      Author: hanmac
 */

#ifndef WXDIRPICKER_HPP_
#define WXDIRPICKER_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXDirPicker;

void Init_WXDirPicker(VALUE rb_mWX);
#if wxUSE_DIRPICKERCTRL
#include <wx/filepicker.h>
#endif

#endif /* WXDIRPICKER_HPP_ */
