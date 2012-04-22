/*
 * wxWizardPage.hpp
 *
 *  Created on: 06.04.2012
 *      Author: hanmac
 */

#ifndef WXWIZARDPAGE_HPP_
#define WXWIZARDPAGE_HPP_


#include "main.hpp"

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


template <>
inline VALUE wrap< RubyWizardPage >(RubyWizardPage* window)
{
	return wrap(window,rb_cWXWizardPage);
}

template <>
inline VALUE wrap< wxWizardPage >(wxWizardPage* window)
{
	return wrap(window,rb_cWXWizardPage);
}


template <>
inline wxWizardPage* wrap< wxWizardPage* >(const VALUE &vwindow)
{
	return unwrapPtr<wxWizardPage>(vwindow, rb_cWXWizardPage);
}

template <>
inline RubyWizardPage* wrap< RubyWizardPage* >(const VALUE &vwindow)
{
	return unwrapPtr<RubyWizardPage>(vwindow, rb_cWXWizardPage);
}

#endif

#endif /* WXWIZARDPAGE_HPP_ */
