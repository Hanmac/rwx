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

namespace RubyWX {
namespace DirPicker {

void set_style_flags(VALUE hash,int& style);

}
}
#endif

#endif /* WXDIRPICKER_HPP_ */
