/*
 * wxFileCtrlGeneric.cpp
 *
 *  Created on: 07.05.2012
 *      Author: hanmac
 */

#include "wxFileCtrlGeneric.hpp"
#include "wxFileCtrl.hpp"

VALUE rb_cWXFileCtrlGeneric;

#if wxUSE_FILECTRL
#if defined(__WXGTK20__) && !defined(__WXUNIVERSAL__)
#define _self unwrap<wxGenericFileCtrl*>(self)

namespace RubyWX {
namespace FileCtrlGeneric {

APP_PROTECT(wxFileCtrl)


/*
 * call-seq:
 *   FileCtrlGeneric.new(parent, [options])
 *
 * creates a new FileCtrlGeneric widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!rb_obj_is_kind_of(hash,rb_cString))
	{
		_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;
}


}
}
#endif
#endif


DLL_LOCAL void Init_WXFileCtrlGeneric(VALUE rb_mWX)
{
#if wxUSE_FILECTRL
#if defined(__WXGTK20__) && !defined(__WXUNIVERSAL__)
	using namespace RubyWX::FileCtrlGeneric;
	rb_cWXFileCtrlGeneric = rb_define_class_under(rb_mWX,"FileCtrlGeneric",rb_cWXFileCtrlBase);
	rb_define_alloc_func(rb_cWXFileCtrlGeneric,_alloc);

	rb_define_method(rb_cWXFileCtrlGeneric,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	registerInfo<wxGenericFileCtrl>(rb_cWXFileCtrlGeneric);
#else
	rb_define_const(rb_mWX,"FileCtrlGeneric",rb_cWXFileCtrl);
#endif
#endif

}

