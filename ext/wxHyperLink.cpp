/*
 * wxHyperLink.cpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */


#include "wxEvtHandler.hpp"

VALUE rb_cWXHyperLink;

#if wxUSE_GAUGE
#define _self wrap<wxHyperlinkCtrl*>(self)

namespace RubyWX {
namespace HyperLink {

macro_attr(HoverColour,wxColour)
macro_attr(NormalColour,wxColour)
macro_attr(VisitedColour,wxColour)

macro_attr(URL,wxString)

macro_attr(Visited,bool)

VALUE _alloc(VALUE self)
{
	return wrap(new wxHyperlinkCtrl(),self);
}

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	wxString label("label");

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		label = wrap<wxString>(rb_hash_aref(hash,ID2SYM(rb_intern("label"))));
	}
	_self->Create(wrap<wxWindow*>(parent),wxID_ANY,
			label,wxEmptyString);
	_created = true;
	rb_call_super(argc,argv);
	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("url")))))
			_self->SetURL(wrap<wxString>(temp));
	}

	return self;
}

}
}
#endif
void Init_WXHyperLink(VALUE rb_mWX)
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
#endif

}


