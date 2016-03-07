/*
 * wxCollapsibleHeaderCtrl.cpp
 *
 *  Created on: 07.03.2016
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"
#include "wxControl.hpp"
#include "wxCollapsibleHeaderCtrl.hpp"
#include "wxCommandEvent.hpp"

VALUE rb_cWXCollapsibleHeaderCtrl;

#if wxUSE_COLLPANE && HAVE_WXCOLLAPSIBLEHEADERCTRL
#define _self unwrap<wxCollapsibleHeaderCtrl*>(self)

namespace RubyWX {
namespace CollapsibleHeaderCtrl {

APP_PROTECT(wxCollapsibleHeaderCtrl)

macro_attr_bool(Collapsed)

/*
 * call-seq:
 *   CollapsibleHeaderCtrl.new(parent, name, [options])
 *   CollapsibleHeaderCtrl.new(parent, [options])
 *
 * creates a new Window widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)){
		wxWindowID id(wxID_ANY);
		int style(wxBORDER_NONE);
		wxString label;

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"label",label);
		}

		_self->Create(unwrap<wxWindow*>(parent), id, label,
			wxDefaultPosition, wxDefaultSize, style
		);
		
	}
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif


/* Document-attr: collapsed
 * says if the pane is collapsed or not. true/false
 */

DLL_LOCAL void Init_WXCollapsibleHeaderCtrl(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXEvent = rb_define_class_under(rb_mWX,"Event",rb_cObject);
#endif

#if wxUSE_COLLPANE && HAVE_WXCOLLAPSIBLEHEADERCTRL
	using namespace RubyWX::CollapsibleHeaderCtrl;
	rb_cWXCollapsibleHeaderCtrl = rb_define_class_under(rb_mWX,"CollapsibleHeaderCtrl",rb_cWXControl);
	rb_define_alloc_func(rb_cWXCollapsibleHeaderCtrl,_alloc);

#if 0
	rb_define_attr(rb_cWXCollapsibleHeaderCtrl,"collapsed",1,1);
	rb_define_attr(rb_cWXCollapsibleHeaderCtrl,"pane",1,0);
	rb_define_attr(rb_cWXCollapsibleHeaderCtrlEvent,"collapsed",1,1);
#endif

	rb_define_method(rb_cWXCollapsibleHeaderCtrl,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXCollapsibleHeaderCtrl,"collapsed",_getCollapsed,_setCollapsed);

	registerInfo<wxCollapsibleHeaderCtrl>(rb_cWXCollapsibleHeaderCtrl);

	registerEventType("collapsibleheader_changed",wxEVT_COLLAPSIBLEHEADER_CHANGED);
#endif
}





