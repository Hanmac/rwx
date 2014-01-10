/*
 * wxInfobar.cpp
 *
 *  Created on: 09.03.2013
 *      Author: hanmac
 */

#include "wxInfoBar.hpp"

VALUE rb_cWXInfoBar;

#if wxUSE_INFOBAR
#define _self unwrap<wxInfoBar*>(self)

namespace RubyWX {
namespace InfoBar {

APP_PROTECT(wxInfoBar)



/*
 * call-seq:
 *   InfoBar.new(parent, name, [options])
 *   InfoBar.new(parent, [options])
 *
 * creates a new InfoBar widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * * options: Hash with possible options to set
 *
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

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
		}
		_self->Create(unwrap<wxWindow*>(parent),id);

	}
	
	rb_call_super(argc,argv);
	return self;
}


}
}
#endif
DLL_LOCAL void Init_WXInfoBar(VALUE rb_mWX)
{
#if 0
	rb_cWXInfoBarBase = rb_define_class_under(rb_mWX,"InfoBarBase",rb_cWXControl);
#endif

#if wxUSE_INFOBAR
	using namespace RubyWX::InfoBar;
	rb_cWXInfoBar = rb_define_class_under(rb_mWX,"InfoBar",rb_cWXInfoBarBase);
	rb_define_alloc_func(rb_cWXInfoBar,_alloc);

	rb_define_method(rb_cWXInfoBar,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxInfoBar>(rb_cWXInfoBar);
#endif

}

