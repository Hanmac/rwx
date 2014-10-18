/*
 * wxWindow.cpp
 *
 *  Created on: 04.02.2012
 *      Author: hanmac
 */
#include "wxWindow.hpp"
#include "wxEvtHandler.hpp"
#include "wxProgressDialog.hpp"
#include "wxFont.hpp"
#include "wxColor.hpp"
#include "wxSizer.hpp"
#include "wxPoint.hpp"
#include "wxRect.hpp"
#include "wxDC.hpp"

#include "wxCursor.hpp"

#include "wxApp.hpp"

#include "wxAui.hpp"

#include <sstream>

#ifdef __WXGTK__
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <GL/glx.h>
#endif

#define _self unwrap<wxWindow*>(self)

VALUE rb_cWXWindow;

static std::map<ID,wxWindowID> idholder;

VALUE wrapID(wxWindowID val)
{
	if(val == wxID_NONE)
		return Qnil;

	for(std::map<ID,wxWindowID>::const_iterator it = idholder.begin();
			it != idholder.end();
			it++)
	{
		if(it->second == val)
			return ID2SYM(it->first);
	}
#if wxUSE_XRC
	wxString str(wxXmlResource::FindXRCIDById(val));
	if(!str.IsEmpty()) {
		ID id = rb_intern(str.c_str());
		idholder.insert(std::make_pair(id,val));
		return ID2SYM(id);
	}
#endif
	return INT2NUM(val);
}

wxWindowID unwrapID(const VALUE &val)
{
	if(NIL_P(val))
		return wxID_ANY;
	if(SYMBOL_P(val))
	{
		ID id = SYM2ID(val);
		std::map<ID,wxWindowID>::iterator it = idholder.find(id);
		if(it != idholder.end())
			return it->second;
		else
		{
			wxWindowID newid;
#if wxUSE_XRC
			newid = XRCID(rb_id2name(id));
#else
			newid = wxNewId();
#endif
			idholder.insert(std::make_pair(id,newid));
			return newid;
		}
	}else
		return NUM2INT(val);
}

void registerID(const char *name,wxWindowID id)
{
	idholder.insert(std::make_pair(rb_intern(name),id));
}

#if wxUSE_TOOLTIPS

template <>
VALUE wrap< wxToolTip >(wxToolTip* window)
{
	if(window)
		return wrap(window->GetTip());
	return Qnil;
}

template <>
wxToolTip* unwrap< wxToolTip* >(const VALUE &vwindow)
{
	if(NIL_P(vwindow))
		return NULL;
	return new wxToolTip(unwrap<wxString>(vwindow));
}
#endif

namespace RubyWX {
namespace Window {

macro_attr(Label,wxString)
macro_attr(Name,wxString)
macro_attr_enum(WindowVariant,wxWindowVariant)
macro_attr_enum(LayoutDirection,wxLayoutDirection)

macro_attr_with_func(Id,wrapID,unwrapID)

macro_attr(Position,wxPoint)

macro_attr(Size,wxSize)
macro_attr(VirtualSize,wxSize)

macro_attr(ClientSize,wxSize)

macro_attr(MinSize,wxSize)
macro_attr(MinClientSize,wxSize)

macro_attr(MaxSize,wxSize)
macro_attr(MaxClientSize,wxSize)

macro_attr_enum(BackgroundStyle,wxBackgroundStyle)

macro_attr(Font,wxFont)
macro_attr(Cursor,wxCursor)

macro_attr_with_func(WindowStyleFlag,LONG2NUM,NUM2LONG)
macro_attr_with_func(ExtraStyle,LONG2NUM,NUM2LONG)

#if wxUSE_VALIDATORS
//macro_attr(Validator,wxValidator)
#endif // wxUSE_VALIDATORS
#if wxUSE_CARET
//macro_attr(Caret,wxCaret*)
#endif // wxUSE_CARET
#if wxUSE_HELP
macro_attr(HelpText,wxString)
#endif // wxUSE_HELP
#if wxUSE_TOOLTIPS
macro_attr(ToolTip,wxToolTip*)
#endif // wxUSE_TOOLTIPS
#if wxUSE_DRAG_AND_DROP
//macro_attr(DropTarget,wxDropTarget*)
#endif // wxUSE_DRAG_AND_DROP
macro_attr(AutoLayout,bool)
macro_attr(ThemeEnabled,bool)

macro_attr(Sizer,wxSizer*)
macro_attr(ContainingSizer,wxSizer*)

singlereturn_frozen(Show)
singlereturn_frozen(Hide)

singlereturn(IsShown)

singlereturn_frozen(Enable)
singlereturn_frozen(Disable)

singlereturn(IsEnabled)
singlereturn(IsThisEnabled)

singlefunc(Raise)
singlefunc(Lower)

singlefunc(Freeze)
singlefunc(Thaw)

singlereturn(IsFrozen)

singlefunc(CaptureMouse)
singlefunc(ReleaseMouse)

singlereturn(HasCapture)

singlefunc(Update)
singlefunc(Refresh)

singlereturn(HasMultiplePages)

singlefunc(Fit)
singlefunc(FitInside)
singlereturn_frozen(Layout)

singlereturn(GetParent)
singlereturn(GetGrandParent)

macro_attr(BackgroundColour,wxColour)
macro_attr(ForegroundColour,wxColour)

singlereturn_frozen(LineUp)
singlereturn_frozen(LineDown)
singlereturn_frozen(PageUp)
singlereturn_frozen(PageDown)

singlereturn_frozen(Destroy)
singlereturn_frozen(DestroyChildren)

singlereturn(IsBeingDeleted)

singlereturn(GetRect)

DLL_LOCAL VALUE _SetParent(VALUE self,VALUE parent)
{
	rb_check_frozen(self);
	_self->Reparent(unwrap<wxWindow*>(parent));
	return parent;
}

DLL_LOCAL VALUE _SetRect(VALUE self,VALUE rect)
{
	rb_check_frozen(self);
	_self->SetSize(unwrap<wxRect>(rect));
	return rect;
}


APP_PROTECT(wxWindow)


/*
 * call-seq:
 *   Window.new(parent, name, [options])
 *   Window.new(parent, [options])
 *
 * creates a new Window widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * * options: Hash with possible options to set:
 *   * name String
 *   * label String depends on the control what is shown
 *   * help_text String
 *   * tool_tip String
 *   * id Symbol or Integer
 *   * size WX::Size
 *   * position WX::Point
 *   * font WX::Font
 *   * cursor WX::Cursor
 *   * background_color WX::Color
 *   * foreground_color WX::Color
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created) {
		wxWindowID id(wxID_ANY);
		if(rb_obj_is_kind_of(hash,rb_cHash))
			set_hash_option(hash,"id",id,unwrapID);
#if wxUSE_XRC
		if(!loadxrc(_self,name,unwrap<wxWindow*>(parent)))
#endif
		if(nil_check(parent)) {
			_self->Create(unwrap<wxWindow*>(parent),id);
		}
		
	}

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		VALUE temp;
		bool disabled = false;

		set_option(extra_style,ExtraStyle,int)
		set_option(name,Name,wxString)
		set_option(label,Label,wxString)

#if wxUSE_HELP
		set_option(help_text,HelpText,wxString)
#endif // wxUSE_HELP
#if wxUSE_TOOLTIPS
		set_option(tool_tip,ToolTip,wxToolTip*)
#endif // wxUSE_TOOLTIPS

		set_option(font,Font,wxFont)
		set_option(cursor,Cursor,wxCursor)

		set_option(background_color,BackgroundColour,wxColour)
		set_option(foreground_color,ForegroundColour,wxColour)

		set_option_func(id,Id,unwrapID)

		set_option(size,Size,wxSize)
		set_option(position,Position,wxPoint)

		set_hash_option(hash,"disabled",disabled);
		if(disabled)
			_self->Disable();

	}

	if(rb_block_given_p())
		rb_yield(self);
	return self;
}

DLL_LOCAL VALUE _getchild(VALUE self,VALUE id)
{
	return wrap(_self->FindWindow(unwrapID(id)));
}

DLL_LOCAL VALUE _GetHandle(VALUE self)
{
	std::stringstream handleStream;
#if defined(__WXMSW__)
	handleStream << (size_t)((HWND)_self->GetHandle());
#elif defined(__WXGTK__)
	GtkWidget* widget = _self->GetHandle();
    gtk_widget_realize( widget );
	GdkWindow *window = gtk_widget_get_window(widget);
	if(!window)
		return Qnil;
	Display* display = GDK_WINDOW_XDISPLAY( window );
	if(!display)
		return Qnil;
	::Window wid = GDK_WINDOW_XID(window);
    // Window is a typedef for XID, which is a typedef for unsigned int
	/* Get the right display (DisplayString() returns ":display.screen") */
	std::string displayStr(DisplayString( display ));
	displayStr = displayStr.substr( 1, ( displayStr.find( ".", 0 ) - 1 ) );
	/* Put all together */
	handleStream << displayStr << ':' << DefaultScreen( display ) << ':' << wid;

#endif
	return wrap(wxString(handleStream.str()));
}



DLL_LOCAL VALUE _draw(int argc,VALUE *argv,VALUE self)
{
	VALUE paint;
	rb_scan_args(argc, argv, "01",&paint);
	wxDC *dc;
if(NIL_P(paint) || RTEST(paint)) {
	wxPaintDC *mdc = new wxPaintDC(_self);
	_self->PrepareDC(*mdc);
#if wxUSE_GRAPHICS_CONTEXT
	 dc = new wxGCDC(*mdc);
	_self->PrepareDC(*dc);
#else
	dc = mdc;
#endif
} else {
	wxClientDC *cdc = new wxClientDC(_self);
	_self->PrepareDC(*cdc);
#if wxUSE_GRAPHICS_CONTEXT
	dc = new wxGCDC(*cdc);
	_self->PrepareDC(*dc);
#else
	dc = cdc;
#endif
}

	dc->Clear();
	rb_yield(wrap(dc));
	return self;
}

DLL_LOCAL VALUE _Close(int argc,VALUE *argv,VALUE self)
{
	VALUE force;
	rb_scan_args(argc, argv, "01",&force);
	return wrap(_self->Close(RTEST(force)));
}

DLL_LOCAL VALUE _wxClass(VALUE self)
{
	return wrap(wxString(_self->GetClassInfo()->GetClassName()));
}

DLL_LOCAL VALUE _IsDestroyed(VALUE self)
{
	return wrap(RTYPEDDATA_DATA(self) == NULL);
}


#if wxUSE_MENUS
DLL_LOCAL VALUE _popupmenu(int argc,VALUE *argv,VALUE self)
{
	VALUE menu,pos;
	wxPoint cpoint;
	if(rb_block_given_p())
	{
		rb_scan_args(argc, argv, "01",&pos);
		menu = wrap(new wxMenu);
		rb_yield(menu);
	}else{
		rb_scan_args(argc, argv, "11",&menu,&pos);
	}
	if(NIL_P(pos))
		cpoint = wxDefaultPosition;
	else
		cpoint = unwrap<wxPoint>(pos);


	return wrap(_self->PopupMenu(unwrap<wxMenu*>(menu),cpoint));
}
#endif

#if wxUSE_AUI
DLL_LOCAL VALUE _aui(VALUE self)
{
	wxWindow *wnd = _self;
	VALUE result = Qnil;
	wxAuiManager *mgr = wxAuiManager::GetManager(wnd);
	if(!mgr && rb_block_given_p())
	{
		result = wrap(new wxAuiManager(wnd));
	}
	if(rb_block_given_p())
		rb_yield(result);
	return result;
}
#endif

DLL_LOCAL VALUE _each_size(VALUE self)
{
	return UINT2NUM(_self->GetChildren().GetCount());
}


DLL_LOCAL VALUE _each(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_size));
	wxWindowList list = _self->GetChildren();
	for(wxWindowList::iterator it = list.begin();it != list.end();++it)
	{
		rb_yield(wrap(*it));
	}
	return self;
}

DLL_LOCAL VALUE _GetMousePosition(VALUE self)
{
	app_protected();

	return wrap(wxGetMousePosition());
}

DLL_LOCAL VALUE _GetCapture(VALUE self)
{
	app_protected();

	return wrap(wxWindow::GetCapture());
}


DLL_LOCAL VALUE _ClientToScreen(VALUE self,VALUE point)
{
	return wrap(_self->ClientToScreen(unwrap<wxPoint>(point)));
}

DLL_LOCAL VALUE _ScreenToClient(VALUE self,VALUE point)
{
	return wrap(_self->ScreenToClient(unwrap<wxPoint>(point)));
}

DLL_LOCAL VALUE _ClientToWindowSize(VALUE self,VALUE point)
{
	return wrap(_self->ClientToWindowSize(unwrap<wxSize>(point)));
}

DLL_LOCAL VALUE _WindowToClientSize(VALUE self,VALUE point)
{
	return wrap(_self->WindowToClientSize(unwrap<wxSize>(point)));
}

}
}

/* Document-method: window_freeze
 * call-seq:
 *   window_freeze -> self
 *
 * freeze the Window, frozen windows are not redrawn until they are thawed.
 * ===Return value
 * self
 */

/* Document-method: window_thaw
 * call-seq:
 *   window_thaw -> self
 *
 * thaw the Window, frozen windows are not redrawn until they are thawed.
 * ===Return value
 * self
 */

/* Document-method: window_frozen?
 * call-seq:
 *   window_frozen? -> true/false
 *
 * returns true if the Window is still frozen.
 * ===Return value
 * true/false
 */


/* Document-method: show
 * call-seq:
 *   show -> true/false
 *
 * shows the Window, returns true if the window was hidden before.
 * ===Return value
 * true/false
 */

/* Document-method: hide
 * call-seq:
 *   hide -> true/false
 *
 * hides the Window, returns true if the window was shown before.
 * ===Return value
 * true/false
 */

/* Document-method: shown?
 * call-seq:
 *   shown? -> true/false
 *
 * returns true if the window is shown.
 * ===Return value
 * true/false
 */

/* Document-method: enable
 * call-seq:
 *   enable -> true/false
 *
 * enable the Window, returns true if the window was disabled before.
 * ===Return value
 * true/false
 */

/* Document-method: disable
 * call-seq:
 *   disable -> true/false
 *
 * disable the Window, returns true if the window was enabled before.
 * ===Return value
 * true/false
 */

/* Document-method: enabled?
 * call-seq:
 *   enabled? -> true/false
 *
 * returns true if the window and parents are enabled.
 * ===Return value
 * true/false
 */

/* Document-method: this_enabled?
 * call-seq:
 *   this_enabled? -> true/false
 *
 * returns true if the window is enabled.
 * ===Return value
 * true/false
 */


/* Document-method: line_up
 * call-seq:
 *   line_up -> true/false
 *
 * scrolls this window one line up, returns true if window has changed.
 * ===Return value
 * true/false
 */

/* Document-method: line_down
 * call-seq:
 *   line_down -> true/false
 *
 * scrolls this window one line down, returns true if window has changed.
 * ===Return value
 * true/false
 */

/* Document-method: page_up
 * call-seq:
 *   page_up -> true/false
 *
 * scrolls this window one page up, returns true if window has changed.
 * ===Return value
 * true/false
 */

/* Document-method: page_down
 * call-seq:
 *   page_down -> true/false
 *
 * scrolls this window one page down, returns true if window has changed.
 * ===Return value
 * true/false
 */

/* Document-attr: label
 * the label of the Window, is mostly used for WX::Control windows. String
 */
/* Document-attr: name
 * the name of the Window, is not shown directly. String
 */
/* Document-attr: parent
 * the parent of the window, type: WX::Window or nil for no parent
 */
/* Document-attr: id
 * the WindowId of the Window, is Symbol for system or self defined Ids, and Integer for generated
 */

/* Document-attr: background_color
 * the WX::Color of the background, not all windows use them.
 */
/* Document-attr: foreground_color
 * the WX::Color of the foreground, not all windows use them.
 */
/* Document-attr: font
 * the WX::Font of the window, not all windows use them.
 */
/* Document-attr: cursor
 * the WX::Cursor of the window. Shown when the mouse is over this window.
 */


/* Document-attr: size
 * the size of this window. WX::Size
 */
/* Document-attr: min_size
 * the minimum size of this window. WX::Size
 */
/* Document-attr: max_size
 * the maximum size of this window. WX::Size
 */

/* Document-attr: client_size
 * the size of this window. WX::Size
 */
/* Document-attr: min_client_size
 * the minimum client size of this window. WX::Size
 */
/* Document-attr: max_client_size
 * the maximum client size of this window. WX::Size
 */


/* Document-attr: help_text
 * the help text of this window, not always available. String
 */
/* Document-attr: tool_tip
 * the tool tip of this window, not always available. String
 */

DLL_LOCAL void Init_WXWindow(VALUE rb_mWX)
{
	using namespace RubyWX::Window;
#if 0
	rb_mWX = rb_define_module("WX");
	rb_mWXEvtHandler = rb_define_module_under(rb_mWX,"EvtHandler");

	rb_define_attr(rb_cWXWindow, "label",1,1);
	rb_define_attr(rb_cWXWindow, "name",1,1);
	rb_define_attr(rb_cWXWindow, "parent",1,1);

	rb_define_attr(rb_cWXWindow, "id",1,1);

	rb_define_attr(rb_cWXWindow, "window_variant",1,1);
	rb_define_attr(rb_cWXWindow, "layout_direction",1,1);
	rb_define_attr(rb_cWXWindow, "background_style",1,1);

	rb_define_attr(rb_cWXWindow, "size",1,1);
	rb_define_attr(rb_cWXWindow, "min_size",1,1);
	rb_define_attr(rb_cWXWindow, "max_size",1,1);

	rb_define_attr(rb_cWXWindow, "client_size",1,1);
	rb_define_attr(rb_cWXWindow, "min_client_size",1,1);
	rb_define_attr(rb_cWXWindow, "max_client_size",1,1);

	rb_define_attr(rb_cWXWindow, "sizer",1,1);
	rb_define_attr(rb_cWXWindow, "containing_sizer",1,1);

	rb_define_attr(rb_cWXWindow, "background_color",1,1);
	rb_define_attr(rb_cWXWindow, "foreground_color",1,1);

	rb_define_attr(rb_cWXWindow, "font",1,1);
	rb_define_attr(rb_cWXWindow, "cursor",1,1);

	rb_define_attr(rb_cWXWindow, "help_text",1,1);
	rb_define_attr(rb_cWXWindow, "tool_tip",1,1);

#endif

	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);
	rb_define_alloc_func(rb_cWXWindow,_alloc);

	rb_undef_method(rb_cWXWindow,"initialize_copy");
	rb_undef_method(rb_cWXWindow,"_load");
	rb_undef_method(rb_cWXWindow,"_dump");

	rb_define_method(rb_cWXWindow,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_include_module(rb_cWXWindow,rb_mWXEvtHandler);

	rb_define_attr_method(rb_cWXWindow, "label",_getLabel,_setLabel);
	rb_define_attr_method(rb_cWXWindow, "name",_getName,_setName);
	rb_define_attr_method(rb_cWXWindow, "parent",_GetParent,_SetParent);

	rb_define_attr_method(rb_cWXWindow, "id",_getId,_setId);

	rb_define_attr_method(rb_cWXWindow, "window_variant",_getWindowVariant,_setWindowVariant);
	rb_define_attr_method(rb_cWXWindow, "layout_direction",_getLayoutDirection,_setLayoutDirection);
	rb_define_attr_method(rb_cWXWindow, "background_style",_getBackgroundStyle,_setBackgroundStyle);

	rb_define_attr_method(rb_cWXWindow, "size",_getSize,_setSize);
	rb_define_attr_method(rb_cWXWindow, "min_size",_getMinSize,_setMinSize);
	rb_define_attr_method(rb_cWXWindow, "max_size",_getMaxSize,_setMaxSize);

	rb_define_attr_method(rb_cWXWindow, "client_size",_getClientSize,_setClientSize);
	rb_define_attr_method(rb_cWXWindow, "min_client_size",_getMinClientSize,_setMinClientSize);
	rb_define_attr_method(rb_cWXWindow, "max_client_size",_getMaxClientSize,_setMaxClientSize);

	rb_define_attr_method(rb_cWXWindow, "virtual_size",_getVirtualSize,_setVirtualSize);

	rb_define_attr_method(rb_cWXWindow, "position",_getPosition,_setPosition);
	rb_define_attr_method(rb_cWXWindow, "rect",_GetRect,_SetRect);

	rb_define_attr_method(rb_cWXWindow, "sizer",_getSizer,_setSizer);
	rb_define_attr_method(rb_cWXWindow, "containing_sizer",_getContainingSizer,_setContainingSizer);

	rb_define_attr_method(rb_cWXWindow, "background_color",_getBackgroundColour,_setBackgroundColour);
	rb_define_attr_method(rb_cWXWindow, "foreground_color",_getForegroundColour,_setForegroundColour);

	rb_define_attr_method(rb_cWXWindow, "font",_getFont,_setFont);
	rb_define_attr_method(rb_cWXWindow, "cursor",_getCursor,_setCursor);
#if wxUSE_HELP
	rb_define_attr_method(rb_cWXWindow, "help_text",_getHelpText,_setHelpText);
#endif // wxUSE_HELP
#if wxUSE_TOOLTIPS
	rb_define_attr_method(rb_cWXWindow, "tool_tip",_getToolTip,_setToolTip);
#endif // wxUSE_TOOLTIPS

	rb_define_method(rb_cWXWindow,"show",RUBY_METHOD_FUNC(_Show),0);
	rb_define_method(rb_cWXWindow,"hide",RUBY_METHOD_FUNC(_Hide),0);

	rb_define_method(rb_cWXWindow,"shown?",RUBY_METHOD_FUNC(_IsShown),0);

	rb_define_method(rb_cWXWindow,"enable",RUBY_METHOD_FUNC(_Enable),0);
	rb_define_method(rb_cWXWindow,"disable",RUBY_METHOD_FUNC(_Disable),0);

	rb_define_method(rb_cWXWindow,"enabled?",RUBY_METHOD_FUNC(_IsEnabled),0);
	rb_define_method(rb_cWXWindow,"this_enabled?",RUBY_METHOD_FUNC(_IsThisEnabled),0);

	rb_define_method(rb_cWXWindow,"window_raise",RUBY_METHOD_FUNC(_Raise),0);
	rb_define_method(rb_cWXWindow,"window_lower",RUBY_METHOD_FUNC(_Lower),0);

	rb_define_method(rb_cWXWindow,"window_freeze",RUBY_METHOD_FUNC(_Freeze),0);
	rb_define_method(rb_cWXWindow,"window_frozen?",RUBY_METHOD_FUNC(_IsFrozen),0);
	rb_define_method(rb_cWXWindow,"window_thaw",RUBY_METHOD_FUNC(_Thaw),0);


	rb_define_method(rb_cWXWindow,"[]",RUBY_METHOD_FUNC(_getchild),1);

	rb_define_method(rb_cWXWindow,"wx_class",RUBY_METHOD_FUNC(_wxClass),0);

	rb_define_method(rb_cWXWindow,"each_child",RUBY_METHOD_FUNC(_each),0);

	rb_define_method(rb_cWXWindow,"fit",RUBY_METHOD_FUNC(_Fit),0);
	rb_define_method(rb_cWXWindow,"fit_inside",RUBY_METHOD_FUNC(_FitInside),0);
	rb_define_method(rb_cWXWindow,"layout",RUBY_METHOD_FUNC(_Layout),0);

	rb_define_method(rb_cWXWindow,"update",RUBY_METHOD_FUNC(_Update),0);
	rb_define_method(rb_cWXWindow,"refresh",RUBY_METHOD_FUNC(_Refresh),0);

	rb_define_method(rb_cWXWindow,"line_up",RUBY_METHOD_FUNC(_LineUp),0);
	rb_define_method(rb_cWXWindow,"line_down",RUBY_METHOD_FUNC(_LineDown),0);

	rb_define_method(rb_cWXWindow,"page_up",RUBY_METHOD_FUNC(_PageUp),0);
	rb_define_method(rb_cWXWindow,"page_down",RUBY_METHOD_FUNC(_PageDown),0);


	rb_define_method(rb_cWXWindow,"capture_mouse",RUBY_METHOD_FUNC(_CaptureMouse),0);
	rb_define_method(rb_cWXWindow,"release_mouse",RUBY_METHOD_FUNC(_ReleaseMouse),0);
	rb_define_method(rb_cWXWindow,"capture?",RUBY_METHOD_FUNC(_HasCapture),0);

	rb_define_method(rb_cWXWindow,"handle",RUBY_METHOD_FUNC(_GetHandle),0);

	rb_define_method(rb_cWXWindow,"draw",RUBY_METHOD_FUNC(_draw),-1);
	rb_define_method(rb_cWXWindow,"close",RUBY_METHOD_FUNC(_Close),-1);
	rb_define_method(rb_cWXWindow,"destroy",RUBY_METHOD_FUNC(_Destroy),0);
	rb_define_method(rb_cWXWindow,"destroy_children",RUBY_METHOD_FUNC(_DestroyChildren),0);

	rb_define_method(rb_cWXWindow,"destroyed?",RUBY_METHOD_FUNC(_IsDestroyed),0);
	rb_define_method(rb_cWXWindow,"deleted?",RUBY_METHOD_FUNC(_IsBeingDeleted),0);

#if wxUSE_MENUS
	rb_define_method(rb_cWXWindow,"popup_menu",RUBY_METHOD_FUNC(_popupmenu),-1);
#endif
#if wxUSE_AUI
	rb_define_method(rb_cWXWindow,"aui",RUBY_METHOD_FUNC(_aui),0);
#endif

	rb_define_module_function(rb_mWX,"mouse_position",RUBY_METHOD_FUNC(_GetMousePosition),0);
	rb_define_module_function(rb_mWX,"capture",RUBY_METHOD_FUNC(_GetCapture),0);

	rb_define_method(rb_cWXWindow,"client_to_screen",RUBY_METHOD_FUNC(_ClientToScreen),1);
	rb_define_method(rb_cWXWindow,"screen_to_client",RUBY_METHOD_FUNC(_ScreenToClient),1);

	rb_define_method(rb_cWXWindow,"client_to_window_size",RUBY_METHOD_FUNC(_ClientToWindowSize),1);
	rb_define_method(rb_cWXWindow,"window_to_client_size",RUBY_METHOD_FUNC(_WindowToClientSize),1);


	registerInfo<wxWindow>(rb_cWXWindow);

	registerID("open",wxID_OPEN);
	registerID("close",wxID_CLOSE);
	registerID("new",wxID_NEW);
	registerID("save",wxID_SAVE);
	registerID("save_as",wxID_SAVEAS);
	registerID("revert",wxID_REVERT);
	registerID("exit",wxID_EXIT);
	registerID("undo",wxID_UNDO);
	registerID("redo",wxID_REDO);
	registerID("help",wxID_HELP);
	registerID("print",wxID_PRINT);
	registerID("print_setup",wxID_PRINT_SETUP);
	registerID("page_setup",wxID_PAGE_SETUP);
	registerID("preview",wxID_PREVIEW);
	registerID("about",wxID_ABOUT);
	registerID("help_contents",wxID_HELP_CONTENTS);
	registerID("help_index",wxID_HELP_INDEX);
	registerID("help_search",wxID_HELP_SEARCH);
	registerID("help_commands",wxID_HELP_COMMANDS);
	registerID("help_procedurs",wxID_HELP_PROCEDURES);
	registerID("help_context",wxID_HELP_CONTEXT);
	registerID("close_all",wxID_CLOSE_ALL);
	registerID("preferences",wxID_PREFERENCES);

	registerID("edit",wxID_EDIT);
	registerID("cut",wxID_CUT);
	registerID("copy",wxID_COPY);
	registerID("paste",wxID_PASTE);
	registerID("clear",wxID_CLEAR);
	registerID("find",wxID_FIND);
	registerID("duplicate",wxID_DUPLICATE);
	registerID("select_all",wxID_SELECTALL);
	registerID("delete",wxID_DELETE);
	registerID("replace",wxID_REPLACE);
	registerID("replace_all",wxID_REPLACE_ALL);
	registerID("properties",wxID_PROPERTIES);

	registerID("view_details",wxID_VIEW_DETAILS);
	registerID("view_largeicons",wxID_VIEW_LARGEICONS);
	registerID("view_smallicons",wxID_VIEW_SMALLICONS);
	registerID("view_list",wxID_VIEW_LIST);
	registerID("view_sortdate",wxID_VIEW_SORTDATE);
	registerID("view_sortname",wxID_VIEW_SORTNAME);
	registerID("view_sortsize",wxID_VIEW_SORTSIZE);
	registerID("view_sorttype",wxID_VIEW_SORTTYPE);

	registerID("file",wxID_FILE);
	registerID("file1",wxID_FILE1);
	registerID("file2",wxID_FILE2);
	registerID("file3",wxID_FILE3);
	registerID("file4",wxID_FILE4);
	registerID("file5",wxID_FILE5);
	registerID("file6",wxID_FILE6);
	registerID("file7",wxID_FILE7);
	registerID("file8",wxID_FILE8);
	registerID("file9",wxID_FILE9);

    /*  Standard button and menu IDs */
	registerID("ok",wxID_OK);
	registerID("cancel",wxID_CANCEL);
	registerID("apply",wxID_APPLY);
	registerID("yes",wxID_YES);
	registerID("no",wxID_NO);
	registerID("static",wxID_STATIC);
	registerID("forward",wxID_FORWARD);
	registerID("backward",wxID_BACKWARD);
	registerID("default",wxID_DEFAULT);
	registerID("more",wxID_MORE);
	registerID("setup",wxID_SETUP);
	registerID("reset",wxID_RESET);
	registerID("context_help",wxID_CONTEXT_HELP);
	registerID("yes_to_all",wxID_YESTOALL);
	registerID("no_to_all",wxID_NOTOALL);
	registerID("abort",wxID_ABORT);
	registerID("retry",wxID_RETRY);
	registerID("ignore",wxID_IGNORE);
	registerID("add",wxID_ADD);
	registerID("remove",wxID_REMOVE);

	registerID("up",wxID_UP);
	registerID("down",wxID_DOWN);
	registerID("home",wxID_HOME);
	registerID("refresh",wxID_REFRESH);
	registerID("stop",wxID_STOP);
	registerID("index",wxID_INDEX);

	registerID("bold",wxID_BOLD);
	registerID("italic",wxID_ITALIC);
	registerID("justify_center",wxID_JUSTIFY_CENTER);
	registerID("justify_fill",wxID_JUSTIFY_FILL);
	registerID("justify_right",wxID_JUSTIFY_RIGHT);
	registerID("justify_left",wxID_JUSTIFY_LEFT);
	registerID("underline",wxID_UNDERLINE);
	registerID("indent",wxID_INDENT);
	registerID("unindent",wxID_UNINDENT);
	registerID("zoom_100",wxID_ZOOM_100);
	registerID("zoom_fit",wxID_ZOOM_FIT);
	registerID("zoom_in",wxID_ZOOM_IN);
	registerID("zoom_out",wxID_ZOOM_OUT);
	registerID("undelete",wxID_UNDELETE);
	registerID("revert_to_saved",wxID_REVERT_TO_SAVED);
	registerID("cdrom",wxID_CDROM);
	registerID("convert",wxID_CONVERT);
	registerID("execute",wxID_EXECUTE);
	registerID("floppy",wxID_FLOPPY);
	registerID("harddisk",wxID_HARDDISK);
	registerID("bottom",wxID_BOTTOM);
	registerID("first",wxID_FIRST);
	registerID("last",wxID_LAST);
	registerID("top",wxID_TOP);
	registerID("info",wxID_INFO);
	registerID("jump_to",wxID_JUMP_TO);
	registerID("network",wxID_NETWORK);
	registerID("select_color",wxID_SELECT_COLOR);
	registerID("select_font",wxID_SELECT_FONT);
	registerID("sort_asc",wxID_SORT_ASCENDING);
	registerID("sort_desc",wxID_SORT_DESCENDING);
	registerID("spell_check",wxID_SPELL_CHECK);
	registerID("strikethrough",wxID_STRIKETHROUGH);

    /*  System menu IDs (used by wxUniv): */
	registerID("system_menu",wxID_SYSTEM_MENU);
	registerID("close_frame",wxID_CLOSE_FRAME);
	registerID("move_frame",wxID_MOVE_FRAME);
	registerID("resize_frame",wxID_RESIZE_FRAME);
	registerID("maximize_frame",wxID_MAXIMIZE_FRAME);
	registerID("iconize_frame",wxID_ICONIZE_FRAME);
	registerID("restore_frame",wxID_RESTORE_FRAME);

	registerEventType("paint",wxEVT_PAINT);
	registerEventType("erase_background",wxEVT_ERASE_BACKGROUND);


	registerEventType("move",wxEVT_MOVE);
	registerEventType("size",wxEVT_SIZE);
	registerEventType("sizing",wxEVT_SIZING);


	registerEnum<wxWindowVariant>("wxWindowVariant")
		->add(wxWINDOW_VARIANT_NORMAL, "normal")
		->add(wxWINDOW_VARIANT_SMALL, "small")
		->add(wxWINDOW_VARIANT_MINI, "mini")
		->add(wxWINDOW_VARIANT_LARGE, "large");

	registerEnum<wxLayoutDirection>("wxLayoutDirection")
		->add(wxLayout_Default, "default")
		->add(wxLayout_LeftToRight, "left_to_right")
		->add(wxLayout_RightToLeft, "right_to_left");

	registerEnum<wxBackgroundStyle>("wxBackgroundStyle")
		->add(wxBG_STYLE_ERASE, "erase")
		->add(wxBG_STYLE_SYSTEM, "system")
		->add(wxBG_STYLE_PAINT, "paint");

}
