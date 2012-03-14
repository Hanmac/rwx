/*
 * wxTreeCtrl.hpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#ifndef WXTREECTRL_HPP_
#define WXTREECTRL_HPP_

#include "main.hpp"

extern VALUE rb_cWXTreeCtrl;

void Init_WXTreeCtrl(VALUE rb_mWX);
#if wxUSE_TREECTRL
#include <wx/treectrl.h>
template <>
inline VALUE wrap< wxTreeCtrl >(wxTreeCtrl* window)
{
	return getEvtObj(window,rb_cWXTreeCtrl);
}

template <>
inline wxTreeCtrl* wrap< wxTreeCtrl* >(const VALUE &vwindow)
{
	return unwrapPtr<wxTreeCtrl>(vwindow, rb_cWXTreeCtrl);
}

class RubyTreeCtrlItem : public wxTreeItemData
{
public:
	RubyTreeCtrlItem(wxTreeCtrl* tree,wxTreeItemId id);

	~RubyTreeCtrlItem();
	VALUE getRuby(){ return mRuby; }

	wxString GetText();
	wxColour GetTextColour();
	wxColour GetBackgroundColour();
	wxFont GetFont();

	void SetText(const wxString& text);
	void SetTextColour(const wxColour& col);
	void SetBackgroundColour(const wxColour& col);
	void SetFont(const wxFont& font);

	VALUE AppendItem(const wxString& text, int image = -1, int selImage = -1);

	void Delete();
private:
	void checkDestroyed();
	VALUE mRuby;
	wxTreeCtrl* mTree;

};

VALUE wrap(wxTreeCtrl* tree,wxTreeItemId id);

#endif
#endif /* WXTREECTRL_HPP_ */
