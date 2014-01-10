/*
 * wxFontDialog.cpp
 *
 *  Created on: 05.03.2012
 *      Author: hanmac
 */

#include "wxFontDialog.hpp"
#include "wxFont.hpp"
#include "wxColor.hpp"

#include "wxApp.hpp"

VALUE rb_cWXFontDialog;
#if wxUSE_FONTDLG
#define _self unwrap<wxFontDialog*>(self)

namespace RubyWX {
namespace FontDialog {

APP_PROTECT(wxFontDialog)

/*
 * call-seq:
 *   FontDialog.new(parent, name, [options])
 *   FontDialog.new(parent, [options])
 *
 * creates a new FontDialog widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * *options: Hash with possible options to set:
 *   * font WX::Font
 *   * color WX::Color
 *   * symbols true/false Allows using SymbolFonts (default false)
 *   * show_help true/false (default false)
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxFontData data;

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			wxColour color(wxNullColour);
			wxFont font(wxNullFont);
			bool symbols(false);
			bool show_help(false);

			set_hash_option(hash,"color",color);
			set_hash_option(hash,"font",font);
			set_hash_option(hash,"symbols",symbols);
			set_hash_option(hash,"show_help",show_help);

			if(color.IsOk())
				data.SetColour(color);
			if(font.IsOk())
				data.SetInitialFont(font);

			data.SetAllowSymbols(symbols);
			data.SetShowHelp(show_help);

		}
		_self->Create(unwrap<wxWindow*>(parent),data);
		
	}

	rb_call_super(argc,argv);



	return self;
}

macro_attr_pre(Colour,wxColour,GetFontData)
macro_attr_pre(InitialFont,wxFont,GetFontData)
macro_attr_pre(ChosenFont,wxFont,GetFontData)

macro_attr_pre(AllowSymbols,bool,GetFontData)
macro_attr_pre(ShowHelp,bool,GetFontData)

//macro_attr_pre(EnableEffects,bool,GetFontData)

DLL_LOCAL VALUE _getUserFont(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,caption,font;
	rb_scan_args(argc, argv, "12",&parent,&caption,&font);

	app_protected();

	wxFont col = wxGetFontFromUser(unwrap<wxWindow*>(parent),unwrap<wxFont>(font),unwrap<wxString>(caption));
	return col.IsOk() ? wrap(col) : Qnil;
}

}
}

#endif

DLL_LOCAL void Init_WXFontDialog(VALUE rb_mWX)
{
#if 0
	rb_define_attr(rb_cWXFontDialog,"initial_font",1,1);
	rb_define_attr(rb_cWXFontDialog,"chosen_font",1,1);

	rb_define_attr(rb_cWXFontDialog,"color",1,1);

	rb_define_attr(rb_cWXFontDialog,"allow_symbols",1,1);
	rb_define_attr(rb_cWXFontDialog,"show_help",1,1);
#endif

#if wxUSE_FONTDLG
	using namespace RubyWX::FontDialog;
	rb_cWXFontDialog = rb_define_class_under(rb_mWX,"FontDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXFontDialog,_alloc);
	rb_define_method(rb_cWXFontDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXFontDialog,"initial_font",_getInitialFont,_setInitialFont);
	rb_define_attr_method(rb_cWXFontDialog,"chosen_font",_getChosenFont,_setChosenFont);

	rb_define_attr_method(rb_cWXFontDialog,"color",_getColour,_setColour);

	rb_define_attr_method(rb_cWXFontDialog,"allow_symbols",_getAllowSymbols,_setAllowSymbols);
	rb_define_attr_method(rb_cWXFontDialog,"show_help",_getShowHelp,_setShowHelp);


	rb_define_module_function(rb_mWX,"font_dialog",RUBY_METHOD_FUNC(_getUserFont),-1);

	registerInfo<wxFontDialog>(rb_cWXFontDialog);
#endif
}
