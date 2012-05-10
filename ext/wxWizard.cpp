/*
 * wxWizard.cpp
 *
 *  Created on: 06.04.2012
 *      Author: hanmac
 */

#include "wxWizard.hpp"

VALUE rb_cWXWizard;
#if wxUSE_WIZARDDLG
#define _self wrap<wxWizard*>(self)

namespace RubyWX {
namespace Wizard {

macro_attr(PageSize,wxSize)
macro_attr(Bitmap,wxBitmap)

singlereturn(IsRunning)
singlereturn(GetCurrentPage)

APP_PROTECT(wxWizard)

VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!_created)
		_self->Create(wrap<wxWindow*>(parent),wxID_ANY,"");
	_created = true;
	rb_call_super(argc,argv);
	return self;
}

VALUE _showPage(int argc,VALUE *argv,VALUE self)
{
	VALUE page,goingForward;
	rb_scan_args(argc, argv, "11",&page,&goingForward);
	return wrap(_self->ShowPage(wrap<wxWizardPage*>(page),RTEST(goingForward)));
}

VALUE _runWizard(VALUE self,VALUE page)
{
	return wrap(_self->RunWizard(wrap<wxWizardPage*>(page)));
}

VALUE _addPage(int argc,VALUE *argv,VALUE self)
{
	VALUE obj,hash;
	rb_scan_args(argc, argv, "02",&obj,&hash);
	if(NIL_P(obj))
		obj = rb_cWXWizardPage;
	if(rb_obj_is_kind_of(obj,rb_cHash))
	{
		hash = obj;
		obj = rb_cWXWizardPage;
	}
	VALUE argv2[] = {self, hash };
	VALUE result = 	rb_class_new_instance(2,argv2,obj);
	return result;
}

VALUE _chainPages(int argc,VALUE *argv,VALUE self)
{
	VALUE page1,page2,args;
	rb_scan_args(argc, argv, "2*",&page1,&page2,&args);
	for(int i = 0; i < argc - 1; ++i)
	{
		wxWizardPageSimple::Chain(
			wrap<RubyWizardPage*>(argv[i]),
			wrap<RubyWizardPage*>(argv[i+1])
		);
	}

	return self;
}

}
}

#endif

DLL_LOCAL void Init_WXWizard(VALUE rb_mWX)
{
#if wxUSE_WIZARDDLG
	using namespace RubyWX::Wizard;
	rb_cWXWizard = rb_define_class_under(rb_mWX,"Wizard",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXWizard,_alloc);

	rb_define_method(rb_cWXWizard,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXWizard,"showPage",RUBY_METHOD_FUNC(_showPage),-1);

	rb_define_method(rb_cWXWizard,"runWizard",RUBY_METHOD_FUNC(_runWizard),1);


	rb_define_method(rb_cWXWizard,"addPage",RUBY_METHOD_FUNC(_addPage),-1);

	rb_define_module_function(rb_cWXWizard,"chainPages",RUBY_METHOD_FUNC(_chainPages),-1);

	rb_define_method(rb_cWXWizard,"chainPages",RUBY_METHOD_FUNC(_chainPages),-1);

	registerType<wxWizard>(rb_cWXWizard);
#endif
}
