/*
 * wxTaskBar.hpp
 *
 *  Created on: 14.04.2012
 *      Author: hanmac
 */

#ifndef WXTASKBAR_HPP_
#define WXTASKBAR_HPP_

#include "main.hpp"
extern VALUE rb_cWXTaskBar;
DLL_LOCAL void Init_WXTaskBar(VALUE rb_mWX);

#if wxUSE_TASKBARICON

#include <wx/taskbar.h>

class RubyTaskBarIcon : public wxTaskBarIcon {

public:
	wxMenu* GetMenu() const {return mMenu;};
	void SetMenu(wxMenu* menu) {mMenu = menu;};

	const wxIcon& GetIcon() const;
	void SetIcon(const wxIcon& icon);

	wxString GetToolTip() const;
	void SetToolTip(const wxString& tooltip);
private:
	wxMenu* mMenu;
	const wxIcon* mIcon;
	wxString mTooltip;

	wxMenu* CreatePopupMenu() { return mMenu;}
};

#endif


#endif /* WXTASKBAR_HPP_ */
