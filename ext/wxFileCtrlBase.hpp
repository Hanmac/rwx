/*
 * wxFileCtrlBase.hpp
 *
 *  Created on: 07.05.2012
 *      Author: hanmac
 */

#ifndef WXFILECTRLBASE_HPP_
#define WXFILECTRLBASE_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXFileCtrlBase;

void Init_WXFileCtrlBase(VALUE rb_mWX);

wxString unwrapWildCard(const VALUE& val);
bool check_filter_index(int& filter,const wxString& wildcard);

#if wxUSE_FILECTRL
#include <wx/filectrl.h>
#endif


namespace RubyWX {
namespace FileCtrlBase {

void _set_options(VALUE hash,
		wxWindowID &id,
		int &style,
		wxString &defaultDirectory,
		wxString &defaultFilename,
		wxString &defaultWildCard
);

void set_style_flags(VALUE hash,int& flags);

}
}

#endif /* WXFILECTRLBASE_HPP_ */
