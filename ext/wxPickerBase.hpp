/*
 * wxPickerBase.hpp
 *
 *  Created on: 04.03.2012
 *      Author: hanmac
 */

#ifndef WXPICKERBASE_HPP_
#define WXPICKERBASE_HPP_

#include "wxControl.hpp"
#include <wx/pickerbase.h>

extern VALUE rb_cWXPickerBase;

void Init_WXPickerBase(VALUE rb_mWX);


namespace RubyWX {
namespace PickerBase {

void set_style_flags(VALUE hash,int& flags);

}
}


#endif /* WXPICKERBASE_HPP_ */
