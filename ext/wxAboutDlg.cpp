/*
 * wxAboutDlg.cpp
 *
 *  Created on: 29.02.2012
 *      Author: hanmac
 */

#include "wxAboutDlg.hpp"
#include "wxApp.hpp"

namespace RubyWX {
namespace AboutDlg {

VALUE _aboutBox(int argc,VALUE *argv,VALUE self)
{
	VALUE hash,value,parent;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!ruby_app_inited)
		rb_raise(rb_eArgError,"%s is not running.",rb_class2name(rb_cWXApp));


	if(NIL_P(hash)){
		hash = parent;
		parent = Qnil;
	}
	wxAboutDialogInfo *info = new wxAboutDialogInfo;

	value = rb_hash_aref(hash,ID2SYM(rb_intern("name")));
	if(!NIL_P(value))
		info->SetName(wrap<wxString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("version")));
	if(!NIL_P(value))
		info->SetVersion(wrap<wxString>(value));



	value = rb_hash_aref(hash,ID2SYM(rb_intern("description")));
	if(!NIL_P(value))
		info->SetDescription(wrap<wxString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("copyright")));
	if(!NIL_P(value))
		info->SetCopyright(wrap<wxString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("licence")));
	if(!NIL_P(value))
		info->SetLicence(wrap<wxString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("web_site")));
	if(!NIL_P(value))
		info->SetWebSite(wrap<wxString>(value));



	value = rb_hash_aref(hash,ID2SYM(rb_intern("icon")));
	if(!NIL_P(value))
		info->SetIcon(wrap<wxIcon>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("developers")));
	if(!NIL_P(value))
		info->SetDevelopers(wrap<wxArrayString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("doc_writers")));
	if(!NIL_P(value))
		info->SetDocWriters(wrap<wxArrayString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("artists")));
	if(!NIL_P(value))
		info->SetArtists(wrap<wxArrayString>(value));

	value = rb_hash_aref(hash,ID2SYM(rb_intern("translators")));
	if(!NIL_P(value))
		info->SetTranslators(wrap<wxArrayString>(value));

	wxAboutBox(*info);


	return self;
}

}
}

void Init_WXAboutDlg(VALUE rb_mWX)
{
	using namespace RubyWX::AboutDlg;

	rb_define_module_function(rb_mWX,"about_box",RUBY_METHOD_FUNC(_aboutBox),-1);
}

