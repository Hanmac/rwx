/*
 * wxCollapsiblePane.cpp
 *
 *  Created on: 06.04.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"
#include "wxControl.hpp"
#include "wxCollapsiblePane.hpp"
#include "wxCommandEvent.hpp"

VALUE rb_cWXCollapsiblePane, rb_cWXCollapsiblePaneEvent;

#if wxUSE_COLLPANE
#define _self unwrap<wxCollapsiblePane*>(self)

namespace RubyWX {
namespace CollapsiblePane {

APP_PROTECT(wxCollapsiblePane)

macro_attr_bool2(Collapsed,Collapse)
singlereturn(GetPane)

/*
 * call-seq:
 *   CollapsiblePane.new(parent, name, [options])
 *   CollapsiblePane.new(parent, [options])
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
		int style(wxCP_DEFAULT_STYLE);
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

namespace Event
{
#undef _self
#define _self unwrapTypedPtr<wxCollapsiblePaneEvent>(self,rb_cWXCollapsiblePaneEvent)
macro_attr(Collapsed,bool)
}

}
}
#endif

DLL_LOCAL void Init_WXCollapsiblePane(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
	rb_cWXEvent = rb_define_class_under(rb_mWX,"Event",rb_cObject);
#endif

#if wxUSE_COLLPANE
	using namespace RubyWX::CollapsiblePane;
	rb_cWXCollapsiblePane = rb_define_class_under(rb_mWX,"CollapsiblePane",rb_cWXControl);
	rb_define_alloc_func(rb_cWXCollapsiblePane,_alloc);
	rb_cWXCollapsiblePaneEvent = rb_define_class_under(rb_cWXEvent,"CollapsiblePane",rb_cWXCommandEvent);

#if 0
	rb_define_attr(rb_cWXCollapsiblePane,"collapsed",1,1);
	rb_define_attr(rb_cWXCollapsiblePaneEvent,"collapsed",1,1);
#endif

	rb_define_method(rb_cWXCollapsiblePane,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXCollapsiblePane,"collapsed",_getCollapsed,_setCollapsed);

	rb_define_method(rb_cWXCollapsiblePane,"pane",RUBY_METHOD_FUNC(_GetPane),0);

	rb_define_attr_method(rb_cWXCollapsiblePaneEvent,"collapsed",Event::_getCollapsed,Event::_setCollapsed);


	registerInfo<wxCollapsiblePane>(rb_cWXCollapsiblePane);

	registerEventType("collapsiblepane_changed",wxEVT_COLLAPSIBLEPANE_CHANGED,rb_cWXCollapsiblePaneEvent);
#endif
}





