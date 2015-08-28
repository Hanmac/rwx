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


DLL_LOCAL wxAboutDialogInfo toInto(VALUE hash)
{
	wxAboutDialogInfo info;
	if(NIL_P(hash))
		return info;

	set_obj_option(hash, "name", &wxAboutDialogInfo::SetName, info);
	set_obj_option(hash, "version", &wxAboutDialogInfo::SetVersion, info);
	set_obj_option(hash, "description", &wxAboutDialogInfo::SetDescription, info);
	set_obj_option(hash, "copyright", &wxAboutDialogInfo::SetCopyright, info);
	set_obj_option(hash, "licence", &wxAboutDialogInfo::SetLicence, info);
	set_obj_option(hash, "web_site", &wxAboutDialogInfo::SetWebSite, info);

	set_obj_option(hash, "icon", &wxAboutDialogInfo::SetIcon, info);

	set_obj_option(hash, "developers", &wxAboutDialogInfo::SetDevelopers, info);
	set_obj_option(hash, "doc_writers", &wxAboutDialogInfo::SetDocWriters, info);
	set_obj_option(hash, "artists", &wxAboutDialogInfo::SetArtists, info);
	set_obj_option(hash, "translators", &wxAboutDialogInfo::SetTranslators, info);

	return info;
}

/*
 * call-seq:
 *   about_box(parent, [options])
 *
 * shows an about box.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *   * name String
 *   * version String
 *   * description String
 *   * copyright String
 *   * licence String
 *   * web_site [url, desc]
 *   * icon String, or WX::Bitmap
 *   * developers [String]
 *   * doc_writers [String]
 *   * artists [String]
 *   * translators [String]
*/
DLL_LOCAL VALUE _aboutBox(int argc,VALUE *argv,VALUE self)
{
	VALUE hash,parent;
	rb_scan_args(argc, argv, "10:",&parent,&hash);

	app_protected();

	if(NIL_P(hash))
		std::swap(hash,parent);

	wxAboutBox(toInto(hash),unwrap<wxWindow*>(parent));


	return self;
}


/*
 * call-seq:
 *   generic_about_box(parent, [options])
 *
 * shows a generic about box.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *   * name String
 *   * version String
 *   * description String
 *   * copyright String
 *   * licence String
 *   * web_site [url, desc]
 *   * icon String, or WX::Bitmap
 *   * developers [String]
 *   * doc_writers [String]
 *   * artists [String]
 *   * translators [String]
*/
DLL_LOCAL VALUE _genericaboutBox(int argc,VALUE *argv,VALUE self)
{
	VALUE hash,parent;
	rb_scan_args(argc, argv, "10:",&parent,&hash);

	app_protected();

	if(NIL_P(hash))
		std::swap(hash,parent);

	wxGenericAboutBox(toInto(hash),unwrap<wxWindow*>(parent));


	return self;
}

/*
 * call-seq:
 *   AboutDialog.new(parent, [options])
 *
 * creates a new AboutDialog widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set
 *   * name String
 *   * version String
 *   * description String
 *   * copyright String
 *   * licence String
 *   * web_site [url, desc]
 *   * icon String, or WX::Bitmap
 *   * developers [String]
 *   * doc_writers [String]
 *   * artists [String]
 *   * translators [String]
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent, name, hash;

	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		_self->Create(toInto(hash),unwrap<wxWindow*>(parent));
	}

	rb_call_super(argc,argv);
	return self;

}


/*
 * call-seq:
 *   add_control(control, [sizerflags]) -> self
 *   add_control(klass, [sizerflags], [options]) -> self
 *
 * add a control to the about dialog.
 * ===Arguments
 * * control. WX::Control
 * * klass Class
 * * options Hash to create the control
 * * sizerflags Hash
 * ===Return value
 * self
 *
*/
VALUE _addControl(int argc,VALUE *argv,VALUE self)
{
	VALUE control,sizer,arg;
	rb_scan_args(argc, argv, "11:",&control,&sizer,&arg);
	wxControl *c = NULL;
	if(rb_obj_is_kind_of(control,rb_cClass) && rb_class_inherited(control,rb_cWXControl)) {
		VALUE argv2[] = {self, arg };
		c = unwrap<wxControl*>(rb_class_new_instance(2,argv2,control));
	} else if(nil_check(control)) {
		window_parent_check(control,_self,c);
	}

	_self->AddControl(c,unwrap<wxSizerFlags>(sizer));
	return self;
}


/*
 * call-seq:
 *   add_text(text) -> self
 *
 * add a text to the about dialog.
 * ===Arguments
 * * text. String
 * ===Return value
 * self
 *
*/
VALUE _addText(VALUE self,VALUE text)
{
	_self->AddText(unwrap<wxString>(text));
	return self;
}

#if wxUSE_COLLPANE
/*
 * call-seq:
 *   add_collapsible_pane(label, text) -> self
 *
 * add a collapsible pane into the about dialog.
 * ===Arguments
 * * label. String
 * * text. String
 * ===Return value
 * self
 *
*/
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
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);

#endif
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

