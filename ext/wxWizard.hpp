/*
 * wxWizard.hpp
 *
 *  Created on: 06.04.2012
 *      Author: hanmac
 */

#ifndef WXWIZARD_HPP_
#define WXWIZARD_HPP_


#include "wxDialog.hpp"

extern VALUE rb_cWXWizard;
DLL_LOCAL void Init_WXWizard(VALUE rb_mWX);

#if wxUSE_WIZARDDLG
#include <wx/wizard.h>
#endif
#endif /* WXWIZARD_HPP_ */
