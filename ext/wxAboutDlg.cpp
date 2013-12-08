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

	if(!ruby_app_inited)
		rb_raise(rb_eArgError,"%s is not running.",rb_class2name(rb_cWXApp));

	if(NIL_P(hash))
		std::swap(hash,parent);

	wxAboutBox(toInto(hash),unwrap<wxWindow*>(parent));


	return self;
}

DLL_LOCAL VALUE _genericaboutBox(int argc,VALUE *argv,VALUE self)
{
	VALUE hash,parent;
	rb_scan_args(argc, argv, "11",&parent,&hash);

	if(!ruby_app_inited)
		rb_raise(rb_eArgError,"%s is not running.",rb_class2name(rb_cWXApp));

	if(NIL_P(hash))
		std::swap(hash,parent);

	wxGenericAboutBox(toInto(hash),unwrap<wxWindow*>(parent));


	return self;
}

#endif
}
}

DLL_LOCAL void Init_WXAboutDlg(VALUE rb_mWX)
{
#if wxUSE_ABOUTDLG
	using namespace RubyWX::AboutDlg;
	rb_define_module_function(rb_mWX,"about_box",RUBY_METHOD_FUNC(_aboutBox),-1);
	rb_define_module_function(rb_mWX,"generic_about_box",RUBY_METHOD_FUNC(_genericaboutBox),-1);
#endif

}

