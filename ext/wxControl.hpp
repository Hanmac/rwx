/*
 * wxControl.hpp
 *
 *  Created on: 08.02.2012
 *      Author: hanmac
 */

#ifndef WXCONTROL_HPP_
#define WXCONTROL_HPP_

#include "wxWindow.hpp"

class RubyControl : public wxControl, RubyWindow {
public:
	RubyControl(VALUE klass);
};

#include "wxAnyButton.hpp"

extern VALUE rb_cWXControl;
void Init_WXControl(VALUE rb_mWX);

template <>
inline VALUE wrap< wxControl >(wxControl* window)
{
	if(window==NULL)
		return Qnil;

	wxAnyButton *anybutton = reinterpret_cast<wxAnyButton*>(window);
		if(anybutton)
			return wrap(anybutton);

	return Qnil;
}


template <>
inline wxControl* wrap< wxControl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxControl>(vwindow, rb_cWXControl);
}

#endif /* WXCONTROL_HPP_ */
