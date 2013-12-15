/*
 * wxWizardPage.cpp
 *
 *  Created on: 06.04.2012
 *      Author: hanmac
 */


#include "wxWizardPage.hpp"

VALUE rb_cWXWizardPage;

#if wxUSE_WIZARDDLG

#define _self unwrap<RubyWizardPage*>(self)


RubyWizardPage::RubyWizardPage() : wxWizardPageSimple(),rubycall(false)
{
}

wxWizardPage* RubyWizardPage::GetPrev() const
{
	VALUE self = static_cast<RubyClientData*>(GetClientObject())->mRuby;
	if(!rubycall)
	{
		rubycall = true;
		wxWizardPage* result = unwrap<wxWizardPage*>(rb_funcall(self,rb_intern("prev"),0));
		rubycall = false;
		return result;
	}
	return wxWizardPageSimple::GetPrev();
}

wxWizardPage* RubyWizardPage::GetNext() const
{
	VALUE self = static_cast<RubyClientData*>(GetClientObject())->mRuby;
	if(!rubycall)
	{
		rubycall = true;
		wxWizardPage* result = unwrap<wxWizardPage*>(rb_funcall(self,rb_intern("next"),0));
		rubycall = false;
		return result;
	}
	return wxWizardPageSimple::GetNext();
}

template <>
wxWizardPage* unwrap<wxWizardPage*>(const VALUE &arg)
{
	return unwrapPtr<wxWizardPage>(arg,rb_cWXWizardPage);
}


namespace RubyWX {
namespace WizardPage {

macro_attr(Prev,wxWizardPage*)
macro_attr(Next,wxWizardPage*)
macro_attr(Bitmap,wxBitmap)

APP_PROTECT(RubyWizardPage)

/*
 * call-seq:
 *   WizardPage.new(parent, [options])
 *
 * creates a new WizardPage widget.
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
	if(!_created)
		_self->Create(unwrap<wxWizard*>(parent));
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

}
}


#endif

DLL_LOCAL void Init_WXWizardPage(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXWizardPage,"prev",1,1);
	rb_define_attr(rb_cWXWizardPage,"next",1,1);

	rb_define_attr(rb_cWXWizardPage,"bitmap",1,1);

#endif

#if wxUSE_WIZARDDLG
	using namespace RubyWX::WizardPage;
	rb_cWXWizardPage = rb_define_class_under(rb_mWX,"WizardPage",rb_cWXPanel);
	rb_define_alloc_func(rb_cWXWizardPage,_alloc);

	rb_define_method(rb_cWXWizardPage,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXWizardPage,"prev",_getPrev,_setPrev);
	rb_define_attr_method(rb_cWXWizardPage,"next",_getNext,_setNext);

	rb_define_attr_method(rb_cWXWizardPage,"bitmap",_getBitmap,_setBitmap);

	registerInfo<wxWizardPage>(rb_cWXWizardPage);
	registerInfo<wxWizardPageSimple>(rb_cWXWizardPage);
	registerInfo<RubyWizardPage>(rb_cWXWizardPage);
#endif
}
