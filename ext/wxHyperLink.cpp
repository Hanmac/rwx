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

/*
 * call-seq:
 *   HyperLink.new(parent, [options])
 *
 * creates a new HyperLink widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 * * *url String
 * * *hover_color wxColor
 * * *normal_color wxColor
 * * *visited_color wxColor
 * * *visited true/false
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	wxString label("label");
	wxString url(wxEmptyString);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"label",label);
		set_hash_option(hash,"url",url);
	}

	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY,
			label,url);
	_created = true;
	rb_call_super(argc,argv);
	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		set_option(hover_color,HoverColour,wxColour)
		set_option(normal_color,NormalColour,wxColour)
		set_option(visited_color,VisitedColour,wxColour)

		set_option(visited,Visited,int)
	}

	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXHyperLink(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXHyperLink,"hover_color",1,1);
	rb_define_attr(rb_cWXHyperLink,"normal_color",1,1);
	rb_define_attr(rb_cWXHyperLink,"visited_color",1,1);

	rb_define_attr(rb_cWXHyperLink,"url",1,1);
	rb_define_attr(rb_cWXHyperLink,"visited",1,1);

#endif

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


