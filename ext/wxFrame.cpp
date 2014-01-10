/*
 * wxFrame.cpp
 *
 *  Created on: 06.02.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"
VALUE rb_cWXFrame;

#define _self unwrap<wxFrame*>(self)

namespace RubyWX {
namespace Frame {

APP_PROTECT(wxFrame)


/*
 * call-seq:
 *   Frame.new(parent, name, [options])
 *   Frame.new(parent, [options])
 *
 * creates a new Frame widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash,name;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	if(!_created) {
#if wxUSE_XRC
		if(!loadxrc(_self,name,unwrap<wxWindow*>(parent)))
#endif
		{
			wxWindowID id(wxID_ANY);
			int style(wxDEFAULT_FRAME_STYLE);

			wxString title(wxEmptyString);

			if(rb_obj_is_kind_of(hash,rb_cHash))
			{
				set_hash_option(hash,"id",id,unwrapID);
				set_hash_option(hash,"style",style);

				set_hash_option(hash,"title",title);
			}

			if(title.empty() && !wxTheApp->GetTopWindow())
				title = wxTheApp->GetAppName();

			_self->Create(
				unwrap<wxWindow*>(parent),id,title,
				wxDefaultPosition,wxDefaultSize,style
			);

		}
	}

	rb_call_super(argc,argv);

	return self;
}


#if wxUSE_MENUS
macro_attr(MenuBar,wxMenuBar*)
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
macro_attr(StatusBar,wxStatusBar*)
macro_attr(StatusBarPane,int)
singlereturn(CreateStatusBar)
#endif // wxUSE_STATUSBAR

#if wxUSE_TOOLBAR
macro_attr(ToolBar,wxToolBar*)
singlereturn(CreateToolBar)
#endif // wxUSE_TOOLBAR
}
}


/* Document-attr: menubar
 * the menu bar of the Frame. WX::MenuBar
 */
/* Document-attr: statusbar
 * the status bar of the Frame. WX::StatusBar
 */
/* Document-attr: toolbar
 * the tool bar of the Frame. WX::ToolBar
 */


DLL_LOCAL void Init_WXFrame(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXFrame,"menubar",1,1);
	rb_define_attr(rb_cWXFrame,"statusbar",1,1);
	rb_define_attr(rb_cWXFrame,"statusbar_pane",1,1);
	rb_define_attr(rb_cWXFrame,"toolbar",1,1);
#endif

	using namespace RubyWX::Frame;
	rb_cWXFrame = rb_define_class_under(rb_mWX,"Frame",rb_cWXTopLevel);
	rb_define_alloc_func(rb_cWXFrame,_alloc);

	rb_define_method(rb_cWXFrame,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

#if wxUSE_MENUS
	rb_define_attr_method(rb_cWXFrame,"menubar",_getMenuBar,_setMenuBar);
#endif // wxUSE_MENUS
#if wxUSE_STATUSBAR
	rb_define_attr_method(rb_cWXFrame,"statusbar",_getStatusBar,_setStatusBar);
	rb_define_attr_method(rb_cWXFrame,"statusbar_pane",_getStatusBarPane,_setStatusBarPane);
	rb_define_method(rb_cWXFrame,"create_statusbar",RUBY_METHOD_FUNC(_CreateStatusBar),0);
#endif // wxUSE_STATUSBAR
#if wxUSE_TOOLBAR
	rb_define_attr_method(rb_cWXFrame,"toolbar",_getToolBar,_setToolBar);
	rb_define_method(rb_cWXFrame,"create_toolbar",RUBY_METHOD_FUNC(_CreateToolBar),0);
#endif // wxUSE_TOOLBAR

	registerInfo<wxFrame>(rb_cWXFrame);
}
