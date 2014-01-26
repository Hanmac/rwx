/*
 * wxAboutDlg.cpp
 *
 *  Created on: 29.02.2012
 *      Author: hanmac
 */

#include "wxAboutDlg.hpp"
#include "wxDialog.hpp"
#include "wxSizer.hpp"
#include "wxApp.hpp"

VALUE rb_cWXAboutDialog;

#if wxUSE_ABOUTDLG

#define mRuby static_cast<RubyClientData*>(GetClientObject())->mRuby

class RubyAboutDlg : public wxGenericAboutDialog {

public:
	using wxGenericAboutDialog::AddControl;
	using wxGenericAboutDialog::AddText;
#if wxUSE_COLLPANE
	using wxGenericAboutDialog::AddCollapsiblePane;
#endif

	void DoAddCustomControls() {
		if(rb_respond_to(mRuby,rb_intern("custom_controls")))
			rb_funcall(mRuby,rb_intern("custom_controls"),0);
	}
};

#define _self unwrap<RubyAboutDlg*>(self)


namespace RubyWX {
namespace AboutDlg {

APP_PROTECT(RubyAboutDlg)

#undef set_option
#define set_option(name,cname,type) \
	if(RTEST(value = rb_hash_aref(hash,ID2SYM(rb_intern(#name))))) \
		info.Set##cname(unwrap<type>(value));

#define set_option2(name,cname) \
	if(RTEST(value = rb_hash_aref(hash,ID2SYM(rb_intern(#name))))) \
	{\
		value = rb_ary_to_ary(value);\
		if(RARRAY_LEN(value) > 1) {\
			info.Set##cname(unwrap<wxString>(RARRAY_AREF(value,0)),unwrap<wxString>(RARRAY_AREF(value,1)));\
		}else \
		{\
			info.Set##cname(unwrap<wxString>(RARRAY_AREF(value,0)));\
		}\
	}

wxAboutDialogInfo toInto(VALUE hash)
{
	wxAboutDialogInfo info;
	if(NIL_P(hash))
		return info;

	VALUE value;

	set_option(name,Name,wxString)
	set_option2(version,Version)
	set_option(description,Description,wxString)
	set_option(copyright,Copyright,wxString)
	set_option(licence,Licence,wxString)
	set_option2(web_site,WebSite)
	set_option(icon,Icon,wxIcon)

	set_option(developers,Developers,wxArrayString)
	set_option(doc_writers,DocWriters,wxArrayString)
	set_option(artists,Artists,wxArrayString)
	set_option(translators,Translators,wxArrayString)

	return info;
}

DLL_LOCAL VALUE _aboutBox(int argc,VALUE *argv,VALUE self)
{
	VALUE hash,parent;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	app_protected();

	if(NIL_P(hash))
		std::swap(hash,parent);

	wxAboutBox(toInto(hash),unwrap<wxWindow*>(parent));


	return self;
}

DLL_LOCAL VALUE _genericaboutBox(int argc,VALUE *argv,VALUE self)
{
	VALUE hash,parent;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	app_protected();

	if(NIL_P(hash))
		std::swap(hash,parent);

	wxGenericAboutBox(toInto(hash),unwrap<wxWindow*>(parent));


	return self;
}

/*
 * call-seq:
 *   SingleChoiceDialog.new(parent, [options])
 *
 * creates a new SingleChoiceDialog widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,info;
	rb_scan_args(argc, argv, "11",&parent,&info);


	if(!_created){

		_self->Create(toInto(info),unwrap<wxWindow*>(parent));
		
	}
	rb_call_super(argc,argv);
	return self;

}

VALUE _addControl(int argc,VALUE *argv,VALUE self)
{
	VALUE control,sizer,arg;
	rb_scan_args(argc, argv, "11*",&control,&sizer,&arg);
	wxControl *c = NULL;
	if(rb_obj_is_kind_of(control,rb_cClass) && rb_class_inherited(control,rb_cWXControl)) {
		rb_scan_args(argc, argv, "11",&control,&arg);
		VALUE argv2[] = {self, arg };
		c = unwrap<wxControl*>(rb_class_new_instance(2,argv2,control));
	}else
	{
		window_parent_check(control,_self,c);
	}

	_self->AddControl(c,unwrap<wxSizerFlags>(sizer));
	return self;
}


VALUE _addText(VALUE self,VALUE text)
{
	_self->AddText(unwrap<wxString>(text));
	return self;
}

#if wxUSE_COLLPANE
VALUE _addCollapsiblePane(VALUE self,VALUE title,VALUE text)
{
	_self->AddCollapsiblePane(unwrap<wxString>(title),unwrap<wxString>(text));
	return self;
}
#endif


}
}

#endif

DLL_LOCAL void Init_WXAboutDlg(VALUE rb_mWX)
{
#if wxUSE_ABOUTDLG
	using namespace RubyWX::AboutDlg;

	rb_cWXAboutDialog = rb_define_class_under(rb_mWX,"AboutDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXAboutDialog,_alloc);

	rb_define_method(rb_cWXAboutDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXAboutDialog,"add_control",RUBY_METHOD_FUNC(_addControl),-1);
	rb_define_method(rb_cWXAboutDialog,"add_text",RUBY_METHOD_FUNC(_addText),1);

#if wxUSE_COLLPANE
	rb_define_method(rb_cWXAboutDialog,"add_collapsible_pane",RUBY_METHOD_FUNC(_addCollapsiblePane),2);
#endif

	registerInfo<RubyAboutDlg>(rb_cWXAboutDialog);

	rb_define_module_function(rb_mWX,"about_box",RUBY_METHOD_FUNC(_aboutBox),-1);
	rb_define_module_function(rb_mWX,"generic_about_box",RUBY_METHOD_FUNC(_genericaboutBox),-1);


#endif

}

