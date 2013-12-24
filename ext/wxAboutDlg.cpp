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

wxAboutDialogInfo toInto(VALUE hash)
{
	wxAboutDialogInfo info;
	if(NIL_P(hash))
		return info;

	VALUE value;

	set_option(name,Name,wxString)
	set_option(version,Version,wxString)
	set_option(description,Description,wxString)
	set_option(copyright,Copyright,wxString)
	set_option(licence,Licence,wxString)
	set_option(web_site,WebSite,wxString)
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
		_created = true;
	}
	rb_call_super(argc,argv);
	return self;

}

VALUE _addControl(int argc,VALUE *argv,VALUE self)
{
	VALUE control,sizer;
	rb_scan_args(argc, argv, "11",&control,&sizer);

	_self->AddControl(unwrap<wxControl*>(control),unwrap<wxSizerFlags>(sizer));
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

	rb_define_method(rb_cWXAboutDialog,"add_control",RUBY_METHOD_FUNC(_addControl),1);
	rb_define_method(rb_cWXAboutDialog,"add_text",RUBY_METHOD_FUNC(_addText),1);

#if wxUSE_COLLPANE
	rb_define_method(rb_cWXAboutDialog,"add_collapsible_pane",RUBY_METHOD_FUNC(_addCollapsiblePane),2);
#endif

	registerInfo<RubyAboutDlg>(rb_cWXAboutDialog);

	rb_define_module_function(rb_mWX,"about_box",RUBY_METHOD_FUNC(_aboutBox),-1);
	rb_define_module_function(rb_mWX,"generic_about_box",RUBY_METHOD_FUNC(_genericaboutBox),-1);


#endif

}

