/*
 * wxHyperLink.cpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */


#include "wxHyperLink.hpp"
#include "wxColor.hpp"

VALUE rb_cWXHyperLink;

#if wxUSE_HYPERLINKCTRL
#define _self unwrap<wxHyperlinkCtrl*>(self)

namespace RubyWX {
namespace HyperLink {

macro_attr(HoverColour,wxColour)
macro_attr(NormalColour,wxColour)
macro_attr(VisitedColour,wxColour)

macro_attr(URL,wxString)

macro_attr(Visited,bool)

APP_PROTECT(wxHyperlinkCtrl)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	wxString label("label");

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"label",label);
	}

	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,
			label,wxEmptyString);
	_created = true;
	rb_call_super(argc,argv);
	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		set_option(url,URL,wxString)
	}

	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXHyperLink(VALUE rb_mWX)
{
#if wxUSE_HYPERLINKCTRL
	using namespace RubyWX::HyperLink;
	rb_cWXHyperLink = rb_define_class_under(rb_mWX,"HyperLink",rb_cWXControl);
	rb_define_alloc_func(rb_cWXHyperLink,_alloc);

	rb_define_method(rb_cWXHyperLink,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXHyperLink,"hover_color",_getHoverColour,_setHoverColour);
	rb_define_attr_method(rb_cWXHyperLink,"normal_color",_getNormalColour,_setNormalColour);
	rb_define_attr_method(rb_cWXHyperLink,"visited_color",_getVisitedColour,_setVisitedColour);


	rb_define_attr_method(rb_cWXHyperLink,"url",_getURL,_setURL);
	rb_define_attr_method(rb_cWXHyperLink,"visited",_getVisited,_setVisited);

	registerInfo<wxHyperlinkCtrl>(rb_cWXHyperLink);
#endif

}


