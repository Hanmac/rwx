/*
 * wxWizard.cpp
 *
 *  Created on: 06.04.2012
 *      Author: hanmac
 */

#include "wxWizard.hpp"

VALUE rb_cWXWizard;
#if wxUSE_WIZARDDLG
#define _self unwrap<wxWizard*>(self)

namespace RubyWX {
namespace Wizard {

macro_attr(PageSize,wxSize)
macro_attr(Bitmap,wxBitmap)

singlereturn(IsRunning)
singlereturn(GetCurrentPage)

APP_PROTECT(wxWizard)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	if(!rb_obj_is_kind_of(hash,rb_cString)){

		wxBitmap bitmap = wxNullBitmap;

		if(rb_obj_is_kind_of(hash,rb_cHash)){
			VALUE temp;
			if(!NIL_P(temp=rb_hash_aref(hash,ID2SYM(rb_intern("bitmap")))))
				bitmap = unwrap<wxBitmap>(temp);
		}

		_self->Create(unwrap<wxWindow*>(parent),
			wxID_ANY,wxEmptyString,bitmap
		);
		_created = true;
	}
	rb_call_super(argc,argv);

	return self;
}

DLL_LOCAL VALUE _showPage(int argc,VALUE *argv,VALUE self)
{
	VALUE page,goingForward;
	rb_scan_args(argc, argv, "11",&page,&goingForward);
	return wrap(_self->ShowPage(unwrap<wxWizardPage*>(page),RTEST(goingForward)));
}

DLL_LOCAL VALUE _runWizard(int argc,VALUE *argv,VALUE self)
{
	VALUE page;
	wxWizardPage *wpage = NULL;

	rb_scan_args(argc, argv, "01",&page);
	if(NIL_P(page))
	{
		wxWindowList list = _self->GetChildren();
		for(wxWindowList::iterator it = list.begin();it != list.end();++it)
			if((wpage = wxDynamicCast(*it,wxWizardPage)) != NULL)
				break;
		if(!wpage)
			rb_raise(rb_eIndexError,"can't find WizardPages inside Wizard.");
	}else
		wpage = unwrap<wxWizardPage*>(page);

	return wrap(_self->RunWizard(wpage));
}

DLL_LOCAL VALUE _addPage(int argc,VALUE *argv,VALUE self)
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

DLL_LOCAL VALUE _chainPages(int argc,VALUE *argv,VALUE self)
{
	VALUE page1,page2,args;
	rb_scan_args(argc, argv, "2*",&page1,&page2,&args);
	for(int i = 0; i < argc - 1; ++i)
	{
		wxWizardPageSimple::Chain(
			unwrap<RubyWizardPage*>(argv[i]),
			unwrap<RubyWizardPage*>(argv[i+1])
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

	rb_define_attr_method(rb_cWXWizard, "bitmap",_getBitmap,_setBitmap);

	rb_define_method(rb_cWXWizard,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXWizard,"show_page",RUBY_METHOD_FUNC(_showPage),-1);

	rb_define_method(rb_cWXWizard,"run_wizard",RUBY_METHOD_FUNC(_runWizard),-1);


	rb_define_method(rb_cWXWizard,"add_page",RUBY_METHOD_FUNC(_addPage),-1);

	rb_define_module_function(rb_cWXWizard,"chain_pages",RUBY_METHOD_FUNC(_chainPages),-1);

	rb_define_method(rb_cWXWizard,"chain_pages",RUBY_METHOD_FUNC(_chainPages),-1);

	registerInfo<wxWizard>(rb_cWXWizard);
#endif
}
