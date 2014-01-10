/*
 * wxStaticText.cpp
 *
 *  Created on: 21.03.2012
 *      Author: hanmac
 */


#include "wxStaticText.hpp"

VALUE rb_cWXStaticText;

#if wxUSE_STATLINE
#define _self unwrap<wxStaticText*>(self)

namespace RubyWX {
namespace StaticText {

APP_PROTECT(wxStaticText)

/*
 * call-seq:
 *   StaticText.new(parent, name, [options])
 *   StaticText.new(parent, [options])
 *
 * creates a new StaticText widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent, name, hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxWindowID id(wxID_ANY);
		wxString label(wxEmptyString);
		int style(0);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"label",label);
			set_hash_option(hash,"style",style);
		}

		_self->Create(unwrap<wxWindow*>(parent),id,label,wxDefaultPosition,wxDefaultSize,style);
	}
	
	rb_call_super(argc,argv);
	return self;
}

}
}
#endif
DLL_LOCAL void Init_WXStaticText(VALUE rb_mWX)
{
#if 0
	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);
#endif
#if wxUSE_STATLINE
	using namespace RubyWX::StaticText;
	rb_cWXStaticText = rb_define_class_under(rb_mWX,"StaticText",rb_cWXControl);
	rb_define_alloc_func(rb_cWXStaticText,_alloc);

	rb_define_method(rb_cWXStaticText,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxStaticText>(rb_cWXStaticText);
#endif

}



