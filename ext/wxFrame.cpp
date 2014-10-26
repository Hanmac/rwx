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

				TopLevel::set_style_flags(hash,style);
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


/*
 * call-seq:
 *   create_statusbar(**options) -> WX::StatusBar
 *
 * creates a new status bar and add it to this frame.
 * ===Arguments
 * * options
 *   * number Integer
 *   * style  Integer
 *   * id     Symbol/Integer
 *
 * ===Return value
 * WX::StatusBar
*/
DLL_LOCAL VALUE _CreateStatusBar(int argc,VALUE *argv,VALUE self)
{
	VALUE hash;
	rb_scan_args(argc, argv, ":",&hash);

	int number = 1;
	long style = wxSTB_DEFAULT_STYLE;
	wxWindowID wid = wxID_ANY;
	wxString name(wxStatusLineNameStr);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"number",number);
		set_hash_option(hash,"style",style);
		set_hash_option(hash,"id",wid,unwrapID);
		set_hash_option(hash,"name",name);
	}

	return wrap(_self->CreateStatusBar(number, style, wid, name));

}

/*
 * call-seq:
 *   push_status_text(text,[pos]) -> self
 *
 * pushes status text to the given pane.
 * ===Arguments
 * * text String
 * * pos is a Integer
 *
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of panes
*/
DLL_LOCAL VALUE _pushStatusText(int argc,VALUE *argv,VALUE self)
{
	VALUE str,num;
	rb_scan_args(argc, argv, "11",&str,&num);
	rb_check_frozen(self);

	int cidx(0);

	if(!NIL_P(num))
		cidx = NUM2INT(num);

	if(check_index(cidx,_self->GetStatusBar()->GetFieldsCount()))
		_self->PushStatusText(unwrap<wxString>(str),cidx);

	return self;
}


/*
 * call-seq:
 *   pop_status_text([pos]) -> self
 *
 * pops status text from the given pane.
 * ===Arguments
 * * pos is a Integer
 *
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of panes
*/
DLL_LOCAL VALUE _popStatusText(int argc,VALUE *argv,VALUE self)
{
	VALUE num;
	rb_scan_args(argc, argv, "01",&num);
	rb_check_frozen(self);

	int cidx(0);

	if(!NIL_P(num))
		cidx = NUM2INT(num);

	if(check_index(cidx,_self->GetStatusBar()->GetFieldsCount()))
		_self->PopStatusText(cidx);

	return self;
}
#endif // wxUSE_STATUSBAR

#if wxUSE_TOOLBAR
macro_attr(ToolBar,wxToolBar*)

/*
 * call-seq:
 *   create_toolbar(**options) -> WX::StatusBar
 *
 * creates a new tool bar and add it to this frame.
 * ===Arguments
 * * options
 *   * style  Integer
 *   * id     Symbol/Integer
 *   * name   name of the tool bar
 *
 * ===Return value
 * WX::ToolBar
*/
DLL_LOCAL VALUE _CreateToolBar(int argc,VALUE *argv,VALUE self)
{
	VALUE hash;
	rb_scan_args(argc, argv, ":",&hash);

	long style = -1;
	wxWindowID wid = wxID_ANY;
	wxString name(wxToolBarNameStr);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"style",style);
		set_hash_option(hash,"id",wid,unwrapID);
		set_hash_option(hash,"name",name);
	}

	return wrap(_self->CreateToolBar(style, wid, name));

}

#endif // wxUSE_TOOLBAR
}
}

/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */

/* Document-attr: menubar
 * the menu bar of the Frame. WX::MenuBar
 */
/* Document-attr: statusbar
 * the status bar of the Frame. WX::StatusBar
 */
/* Document-attr: toolbar
 * the tool bar of the Frame. WX::ToolBar
 */


/* Document-attr: statusbar_pane
 * the status bar pane where the help text are showing. Integer
 */
DLL_LOCAL void Init_WXFrame(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
#endif

	using namespace RubyWX::Frame;
	rb_cWXFrame = rb_define_class_under(rb_mWX,"Frame",rb_cWXTopLevel);
	rb_define_alloc_func(rb_cWXFrame,_alloc);

#if 0
	rb_define_attr(rb_cWXFrame,"menubar",1,1);
	rb_define_attr(rb_cWXFrame,"statusbar",1,1);
	rb_define_attr(rb_cWXFrame,"statusbar_pane",1,1);
	rb_define_attr(rb_cWXFrame,"toolbar",1,1);
#endif

	rb_define_method(rb_cWXFrame,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

#if wxUSE_MENUS
	rb_define_attr_method(rb_cWXFrame,"menubar",_getMenuBar,_setMenuBar);
#endif // wxUSE_MENUS
#if wxUSE_STATUSBAR
	rb_define_attr_method(rb_cWXFrame,"statusbar",_getStatusBar,_setStatusBar);
	rb_define_attr_method(rb_cWXFrame,"statusbar_pane",_getStatusBarPane,_setStatusBarPane);
	rb_define_method(rb_cWXFrame,"create_statusbar",RUBY_METHOD_FUNC(_CreateStatusBar),-1);


	rb_define_method(rb_cWXFrame,"push_status_text",RUBY_METHOD_FUNC(_pushStatusText),-1);
	rb_define_method(rb_cWXFrame,"pop_status_text",RUBY_METHOD_FUNC(_popStatusText),-1);

#endif // wxUSE_STATUSBAR
#if wxUSE_TOOLBAR
	rb_define_attr_method(rb_cWXFrame,"toolbar",_getToolBar,_setToolBar);
	rb_define_method(rb_cWXFrame,"create_toolbar",RUBY_METHOD_FUNC(_CreateToolBar),-1);
#endif // wxUSE_TOOLBAR

	rb_define_const(rb_cWXFrame,"DEFAULT_STYLE",INT2NUM(wxDEFAULT_FRAME_STYLE));


	registerInfo<wxFrame>(rb_cWXFrame);
}
