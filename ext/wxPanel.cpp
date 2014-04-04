/*
 * wxPanel.cpp
 *
 *  Created on: 06.04.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"

VALUE rb_cWXPanel;

#define _self unwrap<wxPanel*>(self)

namespace RubyWX {
namespace Panel {

APP_PROTECT(wxPanel)

/*
 * call-seq:
 *   Panel.new(parent, name, [options])
 *   Panel.new(parent, [options])
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
		int style(wxTAB_TRAVERSAL | wxNO_BORDER);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"style",style);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,
			wxDefaultPosition,wxDefaultSize,style
		);
		
	}
	rb_call_super(argc,argv);
	return self;
}

}
}

DLL_LOCAL void Init_WXPanel(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
#endif

	using namespace RubyWX::Panel;
	rb_cWXPanel = rb_define_class_under(rb_mWX,"Panel",rb_cWXWindow);
	rb_define_alloc_func(rb_cWXPanel,_alloc);

	rb_define_method(rb_cWXPanel,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxPanel>(rb_cWXPanel);
}





