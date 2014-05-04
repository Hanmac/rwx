/*
 * wxChoice.hpp
 *
 *  Created on: 03.05.2012
 *      Author: hanmac
 */

#ifndef WXCHOICE_HPP_
#define WXCHOICE_HPP_

#include "main.hpp"

extern VALUE rb_cWXChoice;

void Init_WXChoice(VALUE rb_mWX);

#if wxUSE_CHOICE
#include <wx/choice.h>
#endif


namespace RubyWX {
namespace Choice {

void set_style_flags(VALUE hash,int& flags);

}
}

#endif /* WXCHOICE_HPP_ */
