/*
 * wxAboutDlg.cpp
 *
 *  Created on: 29.02.2012
 *      Author: hanmac
 */

#include "wxAboutDlg.hpp"
#include "wxWindow.hpp"
#include "wxApp.hpp"

namespace RubyWX {
namespace AboutDlg {

#if wxUSE_ABOUTDLG

wxAboutDialogInfo toInto(VALUE hash)
{
	wxAboutDialogInfo info;
	if(NIL_P(hash))
		return info;

	VALUE value = rb_hash_aref(hash,ID2SYM(rb_intern("name")));
	if(!NIL_P(value))
		info.SetName(unwrap<wxString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("version")));
	if(!NIL_P(value))
		info.SetVersion(unwrap<wxString>(value));



	value = rb_hash_aref(hash,ID2SYM(rb_intern("description")));
	if(!NIL_P(value))
		info.SetDescription(unwrap<wxString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("copyright")));
	if(!NIL_P(value))
		info.SetCopyright(unwrap<wxString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("licence")));
	if(!NIL_P(value))
		info.SetLicence(unwrap<wxString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("web_site")));
	if(!NIL_P(value))
		info.SetWebSite(unwrap<wxString>(value));



	value = rb_hash_aref(hash,ID2SYM(rb_intern("icon")));
	if(!NIL_P(value))
		info.SetIcon(unwrap<wxIcon>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("developers")));
	if(!NIL_P(value))
		info.SetDevelopers(unwrap<wxArrayString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("doc_writers")));
	if(!NIL_P(value))
		info.SetDocWriters(unwrap<wxArrayString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("artists")));
	if(!NIL_P(value))
		info.SetArtists(unwrap<wxArrayString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("translators")));
	if(!NIL_P(value))
		info.SetTranslators(unwrap<wxArrayString>(value));
	return info;
}

VALUE _aboutBox(int argc,VALUE *argv,VALUE self)
{
	VALUE hash,parent;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!ruby_app_inited)
		rb_raise(rb_eArgError,"%s is not running.",rb_class2name(rb_cWXApp));

	if(NIL_P(hash))
	{
		hash = parent;
		parent = Qnil;
	}

	wxAboutBox(toInto(hash),unwrap<wxWindow*>(parent));


	return self;
}

VALUE _genericaboutBox(int argc,VALUE *argv,VALUE self)
{
	VALUE hash,parent;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!ruby_app_inited)
		rb_raise(rb_eArgError,"%s is not running.",rb_class2name(rb_cWXApp));

	if(NIL_P(hash))
	{
		hash = parent;
		parent = Qnil;
	}

	wxGenericAboutBox(toInto(hash),unwrap<wxWindow*>(parent));


	return self;
}

#endif
}
}

void Init_WXAboutDlg(VALUE rb_mWX)
{
#if wxUSE_ABOUTDLG
	using namespace RubyWX::AboutDlg;
	rb_define_module_function(rb_mWX,"about_box",RUBY_METHOD_FUNC(_aboutBox),-1);
	rb_define_module_function(rb_mWX,"generic_about_box",RUBY_METHOD_FUNC(_genericaboutBox),-1);
#endif

}

