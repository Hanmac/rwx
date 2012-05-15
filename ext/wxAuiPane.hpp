/*
 * wxAuiPane.hpp
 *
 *  Created on: 15.04.2012
 *      Author: hanmac
 */

#ifndef WXAUIPANE_HPP_
#define WXAUIPANE_HPP_

#include "wxBitmap.hpp"
#include "wxAui.hpp"

extern VALUE rb_cWXAuiPane;

void Init_WXAuiPane(VALUE rb_mWX);


#if wxUSE_AUI

template <>
inline wxAuiManagerDock wrap< wxAuiManagerDock >(const VALUE &vwindow)
{
	if(SYMBOL_P(vwindow))
	{
		ID id = SYM2ID(vwindow);
		if(id == rb_intern("left"))
			return wxAUI_DOCK_LEFT;
		if(id == rb_intern("right"))
			return wxAUI_DOCK_RIGHT;
		if(id == rb_intern("bottom"))
			return wxAUI_DOCK_BOTTOM;
		if(id == rb_intern("top"))
			return wxAUI_DOCK_TOP;
		if(id == rb_intern("center"))
			return wxAUI_DOCK_CENTER;
		if(id == rb_intern("centre"))
			return wxAUI_DOCK_CENTRE;
	}
	return wxAUI_DOCK_NONE;
}

template <>
inline VALUE wrap< wxAuiPaneInfo >(wxAuiPaneInfo* window)
{
	return wrap(window,rb_cWXAuiPane);
}



template <>
inline wxAuiPaneInfo wrap< wxAuiPaneInfo >(const VALUE &vinfo)
{
	if(NIL_P(vinfo))
		return wxAuiPaneInfo();
	else if(rb_obj_is_kind_of(vinfo,rb_cHash))
	{
		wxAuiPaneInfo info;
		VALUE temp;
		if(!NIL_P(temp=rb_hash_aref(vinfo,ID2SYM(rb_intern("caption")))))
			info.caption = wrap<wxString>(temp);
		if(!NIL_P(temp=rb_hash_aref(vinfo,ID2SYM(rb_intern("name")))))
			info.name = wrap<wxString>(temp);
		if(!NIL_P(temp=rb_hash_aref(vinfo,ID2SYM(rb_intern("icon")))))
			info.icon = wrap<wxBitmap>(temp);
		if(!NIL_P(temp=rb_hash_aref(vinfo,ID2SYM(rb_intern("direction")))))
			info.Direction(wrap< wxAuiManagerDock >(temp));
		return info;
	}else
		return *unwrapPtr<wxAuiPaneInfo>(vinfo, rb_cWXAuiPane);
}

template <>
inline wxAuiPaneInfo* wrap< wxAuiPaneInfo* >(const VALUE &vinfo)
{
	return unwrapPtr<wxAuiPaneInfo>(vinfo, rb_cWXAuiPane);
}

#endif

#endif /* WXAUIPANE_HPP_ */
