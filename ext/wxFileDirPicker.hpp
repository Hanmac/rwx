/*
 * wxFileDirPicker.hpp
 *
 *  Created on: 09.01.2013
 *      Author: hanmac
 */

#ifndef WXFILEDIRPICKER_HPP_
#define WXFILEDIRPICKER_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXFileDirPicker;
extern VALUE rb_cWXFileDirPickerEvent;

void Init_WXFileDirPicker(VALUE rb_mWX);
#if wxUSE_FILEPICKERCTRL || wxUSE_DIRPICKERCTRL
#include <wx/filepicker.h>

namespace RubyWX {
namespace FileDirPicker {

void check_style_flags(const int& style);

}
}
#endif

#endif /* WXDIRPICKER_HPP_ */
