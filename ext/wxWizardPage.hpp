/*
 * wxWizardPage.hpp
 *
 *  Created on: 06.04.2012
 *      Author: hanmac
 */

#ifndef WXWIZARDPAGE_HPP_
#define WXWIZARDPAGE_HPP_


#include "wxPanel.hpp"

extern VALUE rb_cWXWizardPage;
void Init_WXWizardPage(VALUE rb_mWX);

#if wxUSE_WIZARDDLG
#include <wx/wizard.h>

class RubyWizardPage : public wxWizardPageSimple {
public:
	RubyWizardPage();

	wxWizardPage* GetPrev() const;
	wxWizardPage* GetNext() const;

	void SetBitmap(const wxBitmap& bitmap ) { m_bitmap = bitmap; }
private:
	bool mutable rubycall;
};

#endif

#endif /* WXWIZARDPAGE_HPP_ */
