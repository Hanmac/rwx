/*
 * wxFrame.cpp
 *
 *  Created on: 06.02.2012
 *      Author: hanmac
 */

#include "wxEvtHandler.hpp"
VALUE rb_cWXFrame;

#define _self wrap<wxFrame*>(self)

namespace RubyWX {
namespace Frame {

VALUE _alloc(VALUE self)
{
	return wrap(new wxFrame(),self);
}


VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash,name;
	rb_scan_args(argc, argv, "12",&parent,&name,&hash);

	if(!_created) {
#if wxUSE_XRC
		if(rb_obj_is_kind_of(name,rb_cString)){
			if(!wxXmlResource::Get()->LoadFrame(_self,wrap<wxWindow*>(parent),wrap<wxString>(name)))
				rb_raise(rb_eNameError,"Named Frame '%s' is not found.",wrap<char*>(name));
		}
		else
#endif
		_self->Create(wrap<wxWindow*>(parent),wxID_ANY,wxEmptyString);
		_created = true;
	}
	if(rb_obj_is_kind_of(name,rb_cString)){
		VALUE args[] = {parent,hash};

		rb_call_super(2,args);
	}else {
		rb_call_super(argc,argv);
	}

	return self;
}


#if wxUSE_MENUS
//macro_attr(MenuBar,wxMenuBar*)
VALUE _getMenuBar(VALUE self)
{
	return wrap(_self->GetMenuBar());
}
VALUE _setMenuBar(VALUE self,VALUE obj)
{
	_self->SetMenuBar(wrap<wxMenuBar*>(obj));
	return obj;
}
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

void Init_WXFrame(VALUE rb_mWX)
{
	using namespace RubyWX::Frame;
	rb_cWXFrame = rb_define_class_under(rb_mWX,"Frame",rb_cWXTopLevel);
	rb_define_alloc_func(rb_cWXFrame,_alloc);

	rb_define_method(rb_cWXFrame,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

#if wxUSE_MENUS
	rb_define_attr_method(rb_cWXFrame,"menuBar",_getMenuBar,_setMenuBar);
#endif // wxUSE_MENUS
#if wxUSE_STATUSBAR
	rb_define_method(rb_cWXFrame,"createStatusBar",RUBY_METHOD_FUNC(_CreateStatusBar),0);
#endif // wxUSE_STATUSBAR
#if wxUSE_TOOLBAR
	rb_define_attr_method(rb_cWXFrame,"toolBar",_getToolBar,_setToolBar);
	rb_define_method(rb_cWXFrame,"createToolBar",RUBY_METHOD_FUNC(_CreateToolBar),0);
#endif // wxUSE_TOOLBAR

}
