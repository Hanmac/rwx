/*
 * wxHyperLinkBase.cpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */


#include "wxHyperLinkBase.hpp"
#include "wxColor.hpp"

VALUE rb_cWXHyperLinkBase;

#if wxUSE_HYPERLINKCTRL
#define _self unwrap<wxHyperlinkCtrlBase*>(self)

namespace RubyWX {
namespace HyperLinkBase {

macro_attr(HoverColour,wxColour)
macro_attr(NormalColour,wxColour)
macro_attr(VisitedColour,wxColour)

macro_attr(URL,wxString)

macro_attr(Visited,bool)

DLL_LOCAL void _set_options(VALUE hash, wxWindowID &id, wxString &label, wxString &url, int &style) {
	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"id",id,unwrapID);
		set_hash_option(hash,"label",label);
		set_hash_option(hash,"url",url);
		set_hash_option(hash,"style",style);
	}
}

DLL_LOCAL void _set_hash_options(VALUE hash, wxHyperlinkCtrlBase *self) {
	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_obj_option(hash, "hover_color", &wxHyperlinkCtrlBase::SetHoverColour, self);
		set_obj_option(hash, "normal_color", &wxHyperlinkCtrlBase::SetNormalColour, self);
		set_obj_option(hash, "visited_color", &wxHyperlinkCtrlBase::SetVisitedColour, self);

		set_obj_option(hash, "visited", &wxHyperlinkCtrlBase::SetVisited, self);
	}
}

}
}
#endif

/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */

/* Document-attr: hover_color
 * the hover color of the HyperLink. WX::Color
 */
/* Document-attr: normal_color
 * the normal color of the HyperLink. WX::Color
 */
/* Document-attr: visited_color
 * the visited color of the HyperLink. WX::Color
 */
/* Document-attr: url
 * the url of the HyperLink. String
 */
/* Document-attr: visited
 * if the link was visited. bool
 */

DLL_LOCAL void Init_WXHyperLinkBase(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif

#if wxUSE_HYPERLINKCTRL
	using namespace RubyWX::HyperLinkBase;
	rb_cWXHyperLinkBase = rb_define_class_under(rb_mWX,"HyperLinkBase",rb_cWXControl);
	rb_undef_alloc_func(rb_cWXHyperLinkBase);

#if 0
	rb_define_attr(rb_cWXHyperLinkBase,"hover_color",1,1);
	rb_define_attr(rb_cWXHyperLinkBase,"normal_color",1,1);
	rb_define_attr(rb_cWXHyperLinkBase,"visited_color",1,1);

	rb_define_attr(rb_cWXHyperLinkBase,"url",1,1);
	rb_define_attr(rb_cWXHyperLinkBase,"visited",1,1);
#endif


	rb_define_attr_method(rb_cWXHyperLinkBase,"hover_color",_getHoverColour,_setHoverColour);
	rb_define_attr_method(rb_cWXHyperLinkBase,"normal_color",_getNormalColour,_setNormalColour);
	rb_define_attr_method(rb_cWXHyperLinkBase,"visited_color",_getVisitedColour,_setVisitedColour);

	rb_define_attr_method(rb_cWXHyperLinkBase,"url",_getURL,_setURL);
	rb_define_attr_method(rb_cWXHyperLinkBase,"visited",_getVisited,_setVisited);

	rb_define_const(rb_cWXHyperLinkBase,"DEFAULT_STYLE",INT2NUM(wxHL_DEFAULT_STYLE));

	registerType<wxHyperlinkCtrlBase>(rb_cWXHyperLinkBase);
#endif

}


