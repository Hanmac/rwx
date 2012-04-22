/*
 * wxWizard.hpp
 *
 *  Created on: 06.04.2012
 *      Author: hanmac
 */

#ifndef WXWIZARD_HPP_
#define WXWIZARD_HPP_


#include "main.hpp"

extern VALUE rb_cWXWizard;
void Init_WXWizard(VALUE rb_mWX);

#if wxUSE_WIZARDDLG
#include <wx/wizard.h>

template <>
inline VALUE wrap< wxWizard >(wxWizard* window)
{
	return wrap(window,rb_cWXWizard);
}



template <>
inline wxWizard* wrap< wxWizard* >(const VALUE &vwindow)
{
	return unwrapPtr<wxWizard>(vwindow, rb_cWXWizard);
}
#endif
#endif /* WXWIZARD_HPP_ */
