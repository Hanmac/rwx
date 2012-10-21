/*
 * wxTreeCtrl.hpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */

#ifndef WXTREECTRL_HPP_
#define WXTREECTRL_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXTreeCtrl;

void Init_WXTreeCtrl(VALUE rb_mWX);
#if wxUSE_TREECTRL
#include <wx/treectrl.h>

class RubyTreeCtrlItem : public wxTreeItemData
{
public:
	RubyTreeCtrlItem(wxTreeCtrl* tree,wxTreeItemId id);

	~RubyTreeCtrlItem();
	VALUE getRuby(){ return mRuby; }

	bool GetBold();
	wxString GetText();
	wxColour GetTextColour();
	wxColour GetBackgroundColour();
	wxFont GetFont();
	int GetState();

	void SetBold(const bool bold);
	void SetText(const wxString& text);
	void SetTextColour(const wxColour& col);
	void SetBackgroundColour(const wxColour& col);
	void SetFont(const wxFont& font);
	void SetState(int state);

	VALUE AppendItem(const wxString& text, int image = -1, int selImage = -1);
	VALUE PrependItem(const wxString& text, int image = -1, int selImage = -1);

	VALUE GetNextSibling();
	VALUE GetPrevSibling();

	void Delete();

	void each();
	VALUE compare(RubyTreeCtrlItem *other);

//	wxTreeCtrl* getTree();
private:
	void checkDestroyed();
	VALUE mRuby;
	wxTreeCtrl* mTree;

};

template <>
inline wxTreeItemId unwrap< wxTreeItemId >(const VALUE &vwindow)
{
	return unwrapPtr<RubyTreeCtrlItem>(vwindow, rb_cWXTreeCtrl)->GetId();
}



VALUE wrap(wxTreeCtrl* tree,wxTreeItemId id);

#endif
#endif /* WXTREECTRL_HPP_ */
