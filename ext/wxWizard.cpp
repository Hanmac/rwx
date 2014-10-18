/*
 * wxWizard.cpp
 *
 *  Created on: 06.04.2012
 *      Author: hanmac
 */

#include "wxWizard.hpp"
#include "wxToplevel.hpp"
#include "wxNotifyEvent.hpp"

VALUE rb_cWXWizard;
VALUE rb_cWXWizardEvent;


#if wxUSE_WIZARDDLG

#define _self unwrap<wxWizard*>(self)

namespace RubyWX {
namespace Wizard {

macro_attr_con(PageSize,wxSize,!_self->IsRunning())

macro_attr(Bitmap,wxBitmap)

singlereturn(IsRunning)
singlereturn(GetCurrentPage)

APP_PROTECT(wxWizard)

/*
 * call-seq:
 *   Wizard.new(parent, name, [options])
 *   Wizard.new(parent, [options])
 *
 * creates a new Wizard widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *   * bitmap WX::Bitmap
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(hash,rb_cString)){

		wxWindowID id(wxID_ANY);
		wxString title(wxEmptyString);
		int style(wxDEFAULT_DIALOG_STYLE);
		wxBitmap bitmap(wxNullBitmap);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"title",title);
			set_hash_option(hash,"style",style);
			set_hash_option(hash,"bitmap",bitmap);

			TopLevel::set_style_flags(hash,style);
		}

		_self->Create(unwrap<wxWindow*>(parent),
			id,title,bitmap,wxDefaultPosition,style
		);
		
	}
	rb_call_super(argc,argv);

	return self;
}



/*
 * call-seq:
 *   show_page(page, forward = true) -> true/false
 *
 * shows the given page
 * return false if the page change was canceled
 * ===Arguments
 * * page is WX::WizardPage
 *
 * ===Return value
 * true/false
 */
DLL_LOCAL VALUE _showPage(int argc,VALUE *argv,VALUE self)
{
	VALUE page,goingForward;
	rb_scan_args(argc, argv, "11",&page,&goingForward);
	rb_check_frozen(self);

	if(!nil_check(page,rb_cWXWizardPage,true))
		return Qnil;

	wxWizardPage* cpage = unwrap<wxWizardPage*>(page);

	//prevent WX from raising stupid assert
	if(cpage == _self->GetCurrentPage())
		return Qnil;

	bool forward(NIL_P(goingForward) ? true : RTEST(goingForward));
	return wrap(_self->ShowPage(cpage,forward));
}

/*
 * call-seq:
 *   run_wizard([page]) -> true/false
 *
 * starts the wizard with the given page or first page if nil
 * return true if the wizard is finished, and false if the wizard is canceled
 * ===Arguments
 * * page is WX::WizardPage or nil
 *
 * ===Return value
 * true/false
 */
DLL_LOCAL VALUE _runWizard(int argc,VALUE *argv,VALUE self)
{
	VALUE page;
	wxWizardPage *wpage = NULL;

	rb_scan_args(argc, argv, "01",&page);
	rb_check_frozen(self);
	if(NIL_P(page))
	{
		wxWindowList list = _self->GetChildren();
		for(wxWindowList::const_iterator it = list.begin();it != list.end();++it)
			if((wpage = wxDynamicCast(*it,wxWizardPage)) != NULL)
				break;
		if(!wpage)
		{
			rb_raise(rb_eIndexError,"can't find WizardPages inside Wizard.");
			return Qnil;
		}
	}else
		wpage = unwrap<wxWizardPage*>(page);

	return wrap(_self->RunWizard(wpage));
}


/*
 * call-seq:
 *   add_page([klass=WX::WizardPage],[**options]) [ {|page|} ] -> WX::WizardPage
 *
 * adds a new page into the wizard. this is a helper for klass.new(self,**options)
 * ===Arguments
 * * klass is a class inherited from WX::WizardPage or WX::WizardPage as default
 * * page is WX::WizardPage or nil
 *
 * ===Return value
 * WX::WizardPage
 */
DLL_LOCAL VALUE _addPage(int argc,VALUE *argv,VALUE self)
{
	VALUE obj,hash;
	rb_scan_args(argc, argv, "01:",&obj,&hash);
	rb_check_frozen(self);
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


/*
 * call-seq:
 *   wizard.chain_pages([pages*]) -> self
 *   Wizard.chain_pages([pages*]) -> self
 *
 * does chain multiple wizard pages together with prev/next
 * ===Arguments
 * * pages  [WX::WizardPage]
 *
 * ===Return value
 * self
 */
DLL_LOCAL VALUE _chainPages(int argc,VALUE *argv,VALUE self)
{
	VALUE page1,page2,args;
	rb_scan_args(argc, argv, "2*",&page1,&page2,&args);

	if(!nil_check(page1,rb_cWXWizardPage,true))
		return self;

	for(int i = 0; i < argc - 1; ++i)
	{
		if(!nil_check(argv[i+1],rb_cWXWizardPage,true))
			return self;

		wxWizardPageSimple::Chain(
			unwrap<RubyWizardPage*>(argv[i]),
			unwrap<RubyWizardPage*>(argv[i+1])
		);
	}

	return self;
}

namespace Event
{
#undef _self
#define _self unwrapTypedPtr<wxWizardEvent>(self,rb_cWXWizardEvent)

singlereturn(GetDirection)
singlereturn(GetPage)

}

}
}

#endif


/* Document-attr: bitmap
 * the current shown bitmap. can be changed by pages. WX;;Bitmap
 */

/* Document-attr: page_size
 * the size of the current page, nil if wizard is not running. WX::Size
 */


/* Document-method: current_page
 * call-seq:
 *   current_page -> WX::WizardPage
 *
 * returns the current wizard page or nil.
 * ===Return value
 * WX::WizardPage
 */

/* Document-method: running?
 * call-seq:
 *   running? -> true/false
 *
 * returns true if the wizard is currently running.
 * ===Return value
 * true/false
 */


/* Document-attr: direction
 * returns the direction of the wizard event. true means forward
 */
/* Document-attr: page
 * returns the wizard page of the wizard event. WX::WizardPage
 */

DLL_LOCAL void Init_WXWizard(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);

	rb_cWXEvent = rb_define_class_under(rb_mWX,"Event",rb_cObject);
	rb_cWXCommandEvent = rb_define_class_under(rb_cWXEvent,"Command",rb_cWXEvent);
	rb_cWXNotifyEvent = rb_define_class_under(rb_cWXEvent,"Notify",rb_cWXCommandEvent);
#endif

#if wxUSE_WIZARDDLG
	using namespace RubyWX::Wizard;
	rb_cWXWizard = rb_define_class_under(rb_mWX,"Wizard",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXWizard,_alloc);
	rb_cWXWizardEvent = rb_define_class_under(rb_cWXEvent,"Wizard",rb_cWXNotifyEvent);

#if 0
	rb_define_attr(rb_cWXWizard, "bitmap",1,1);
	rb_define_attr(rb_cWXWizard, "page_size",1,1);

	rb_define_attr(rb_cWXWizardEvent, "direction",1,0);
	rb_define_attr(rb_cWXWizardEvent, "page",1,0);
#endif

	rb_define_attr_method(rb_cWXWizard, "bitmap",_getBitmap,_setBitmap);
	rb_define_attr_method(rb_cWXWizard, "page_size",_getPageSize,_setPageSize);

	rb_define_method(rb_cWXWizard,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXWizard,"show_page",RUBY_METHOD_FUNC(_showPage),-1);

	rb_define_method(rb_cWXWizard,"run_wizard",RUBY_METHOD_FUNC(_runWizard),-1);

	rb_define_method(rb_cWXWizard,"running?",RUBY_METHOD_FUNC(_IsRunning),0);
	rb_define_method(rb_cWXWizard,"current_page",RUBY_METHOD_FUNC(_GetCurrentPage),0);

	rb_define_method(rb_cWXWizard,"add_page",RUBY_METHOD_FUNC(_addPage),-1);

	rb_define_singleton_method(rb_cWXWizard,"chain_pages",RUBY_METHOD_FUNC(_chainPages),-1);

	rb_define_method(rb_cWXWizard,"chain_pages",RUBY_METHOD_FUNC(_chainPages),-1);

	registerInfo<wxWizard>(rb_cWXWizard);

	rb_define_attr_method(rb_cWXWizardEvent,"direction",Event::_GetDirection,NULL);
	rb_define_attr_method(rb_cWXWizardEvent,"page",Event::_GetPage,NULL);

	registerEventType("wizard_page_changed",wxEVT_WIZARD_PAGE_CHANGED,rb_cWXWizardEvent);
	registerEventType("wizard_page_changing",wxEVT_WIZARD_PAGE_CHANGING,rb_cWXWizardEvent);
	registerEventType("wizard_cancel",wxEVT_WIZARD_CANCEL,rb_cWXWizardEvent);
	registerEventType("wizard_help",wxEVT_WIZARD_HELP,rb_cWXWizardEvent);
	registerEventType("wizard_finished",wxEVT_WIZARD_FINISHED,rb_cWXWizardEvent);
	registerEventType("wizard_page_shown",wxEVT_WIZARD_PAGE_SHOWN,rb_cWXWizardEvent);
	registerEventType("wizard_before_page_changed",wxEVT_WIZARD_BEFORE_PAGE_CHANGED,rb_cWXWizardEvent);

#endif
}
