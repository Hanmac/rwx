/*
 * wxToplevel.hpp
 *
 *  Created on: 05.02.2012
 *      Author: hanmac
 */

#ifndef WXTOPLEVEL_HPP_
#define WXTOPLEVEL_HPP_

#include "wxWindow.hpp"

extern VALUE rb_cWXTopLevel;
void Init_WXTopLevel(VALUE rb_mWX);


#include "wxFrame.hpp"
#include "wxDialog.hpp"

template <>
inline VALUE wrap< wxTopLevelWindow >(wxTopLevelWindow* window)
{
	if(window==NULL)
		return Qnil;

	wxFrame *frame = dynamic_cast<wxFrame*>(window);
	if(frame)
		return wrap(frame);

	wxDialog *dialog = dynamic_cast<wxDialog*>(window);
		if(dialog)
			return wrap(dialog);

	return wrap(window,rb_cWXTopLevel);
}



template <>
inline wxTopLevelWindow* wrap< wxTopLevelWindow* >(const VALUE &vwindow)
{
	return unwrapPtr<wxTopLevelWindow>(vwindow, rb_cWXTopLevel);
}

#endif /* WXTOPLEVEL_HPP_ */
