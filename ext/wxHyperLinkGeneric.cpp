/*
 * wxHyperLinkGeneric.cpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */


#include "wxHyperLinkGeneric.hpp"
#include "wxHyperLinkBase.hpp"
#include "wxColor.hpp"

VALUE rb_cWXHyperLinkGeneric;

#if wxUSE_HYPERLINKCTRL

#ifndef wxHAS_NATIVE_HYPERLINKCTRL
#if defined(__WXGTK210__) && !defined(__WXUNIVERSAL__)
    #define wxHAS_NATIVE_HYPERLINKCTRL
// Note that the native control is only available in Unicode version under MSW.
#elif defined(__WXMSW__) && wxUSE_UNICODE && !defined(__WXUNIVERSAL__)
    #define wxHAS_NATIVE_HYPERLINKCTRL
#endif
#endif

#define _self unwrap<wxGenericHyperlinkCtrl*>(self)

namespace RubyWX {
namespace HyperLinkGeneric {

APP_PROTECT(wxGenericHyperlinkCtrl)

/*
 * call-seq:
 *   HyperLink.new(parent, name, [options])
 *   HyperLink.new(parent, [options])
 *
 * creates a new HyperLink widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * url String
 *   * hover_color wxColor
 *   * normal_color wxColor
 *   * visited_color wxColor
 *   * visited true/false
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(NIL_P(hash))
		name = hash;

	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		wxString label("label");
		wxString url(wxEmptyString);
		int style(wxHL_DEFAULT_STYLE);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"label",label);
			set_hash_option(hash,"url",url);
			set_hash_option(hash,"style",style);
		}
		if(nil_check(parent))
			_self->Create(
				unwrap<wxWindow*>(parent),id,label,url,
				wxDefaultPosition,wxDefaultSize,style
			);

	}
	
	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_obj_option(hash, "hover_color", &wxGenericHyperlinkCtrl::SetHoverColour, _self);
		set_obj_option(hash, "normal_color", &wxGenericHyperlinkCtrl::SetNormalColour, _self);
		set_obj_option(hash, "visited_color", &wxGenericHyperlinkCtrl::SetVisitedColour, _self);

		set_obj_option(hash, "visited", &wxGenericHyperlinkCtrl::SetVisited, _self);
	}

	return self;
}

}
}
#endif

DLL_LOCAL void Init_WXHyperLinkGeneric(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXHyperLinkBase = rb_define_class_under(rb_mWX,"HyperLink",rb_cWXControl);
#endif

#if wxUSE_HYPERLINKCTRL
#ifndef wxHAS_NATIVE_HYPERLINKCTRL
	rb_define_const(rb_mWX,"HyperLinkGeneric",rb_cWXHyperLink);
#else
	using namespace RubyWX::HyperLinkGeneric;
	rb_cWXHyperLinkGeneric = rb_define_class_under(rb_mWX,"HyperLinkGeneric",rb_cWXHyperLinkBase);
	rb_define_alloc_func(rb_cWXHyperLinkGeneric,_alloc);

	rb_define_method(rb_cWXHyperLinkGeneric,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxGenericHyperlinkCtrl>(rb_cWXHyperLinkGeneric);
#endif
#endif
}

