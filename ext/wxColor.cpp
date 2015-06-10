/*
 * wxColor.cpp
 *
 *  Created on: 15.02.2012
 *      Author: hanmac
 */

#include "wxColor.hpp"
#include "wxApp.hpp"

#define _self unwrap<wxColor*>(self)

VALUE rb_cWXColor;
ID rwxID_red,rwxID_blue,rwxID_green,rwxID_alpha;

template <>
VALUE wrap< wxColor >(const wxColor& color )
{
	if(!color.IsOk())
		return Qnil;
	return wrapTypedPtr(new wxColor(color), rb_cWXColor);
}
template <>
VALUE wrap< wxColor >(wxColor *color )
{
	return wrapTypedPtr(color, rb_cWXColor);
}

template <>
bool is_wrapable< wxColor >(const VALUE &vcolor)
{
	if(rb_obj_is_kind_of(vcolor, rb_cWXColor) || FIXNUM_P(vcolor)){
		return true;
	}else if(rb_obj_is_kind_of(vcolor, rb_cString)){
		wxLogNull logNo;
		wxString name(unwrap<wxString>(vcolor));
		wxColor col(name);
		return col.IsOk();
	}
	else if(rb_respond_to(vcolor,rwxID_red) &&
		rb_respond_to(vcolor,rwxID_blue) &&
		rb_respond_to(vcolor,rwxID_green)){
		return true;
	}else
		return false;
}

template <>
wxColor* unwrap< wxColor* >(const VALUE &vcolor)
{
	return unwrapTypedPtr<wxColor>(vcolor, rb_cWXColor);
}

void set_color_part(char& cv,const VALUE &val, const ID &id)
{
	VALUE tmp = rb_funcall(val,id,0);
	if(FIXNUM_P(tmp))
		cv = NUM2CHR(tmp);
	else
		cv = NUM2DBL(tmp) * 256;
}

void set_color_part_array(char& cv,const VALUE &val, size_t idx)
{
	VALUE tmp = RARRAY_AREF(val, idx);
	if(FIXNUM_P(tmp))
		cv = NUM2CHR(tmp);
	else
		cv = NUM2DBL(tmp) * 256;
}



template <>
wxColor unwrap< wxColor >(const VALUE &vcolor)
{
	//NULLColor is not wanted
	//	if(NIL_P(vcolor))
	//		return wxNullColour;

	if(SYMBOL_P(vcolor)) {
		app_protected();
		wxSystemColour sys = unwrapenum<wxSystemColour>(vcolor);
		wxColor col = wxSystemSettings::GetColour(sys);
		if(!col.IsOk())
			not_valid(vcolor,rb_cWXColor);
		return col;
	}else if(rb_obj_is_kind_of(vcolor, rb_cString)){
		wxLogNull logNo;
		wxString name(unwrap<wxString>(vcolor));
		wxColor col(name);
		if(!col.IsOk())
			not_valid(vcolor,rb_cWXColor);
		return col;
	}else if(FIXNUM_P(vcolor))
		return wxColor(FIX2LONG(vcolor));
	else if(rb_obj_is_kind_of(vcolor, rb_cArray) && RARRAY_LEN(vcolor) >= 3 &&
		RARRAY_LEN(vcolor) <= 4){
		char red,green,blue,alpha(wxALPHA_OPAQUE);
		wxColor color;
		set_color_part_array(red, vcolor, 0);
		set_color_part_array(green, vcolor, 1);
		set_color_part_array(blue, vcolor, 2);

		if(RARRAY_LEN(vcolor) == 4) {
			set_color_part_array(blue, vcolor, 2);
			set_color_part(alpha, vcolor, 3);
		}

		color.Set(red, green, blue, alpha);
		return color;
	} if(!rb_obj_is_kind_of(vcolor, rb_cWXColor) &&
		rb_respond_to(vcolor,rwxID_red) &&
		rb_respond_to(vcolor,rwxID_green) &&
		rb_respond_to(vcolor,rwxID_blue)){
		char red,green,blue,alpha(wxALPHA_OPAQUE);
		wxColor color;

		set_color_part(red, vcolor, rwxID_red);
		set_color_part(green, vcolor, rwxID_green);
		set_color_part(blue, vcolor, rwxID_blue);

		if(rb_respond_to(vcolor,rwxID_alpha)) {
			set_color_part(alpha, vcolor, rwxID_alpha);
		}

		color.Set(red, green, blue, alpha);

		return color;
	}else{
		wxColor *col = unwrap<wxColor*>(vcolor);
		if(!col->IsOk())
			not_valid(vcolor,rb_cWXColor);
		return *col;
	}


}

namespace RubyWX {
namespace Color {
DLL_LOCAL VALUE _alloc(VALUE self) {
	return wrapTypedPtr(new wxColor,self);
}

void define_const()
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXColor = rb_define_class_under(rb_mWX,"Color",rb_cObject);
#endif

	if(rb_const_defined(rb_cWXColor,rb_intern("BLACK")))
		return;

	rb_define_const(rb_cWXColor,"BLACK",wrap(wxBLACK));
	rb_define_const(rb_cWXColor,"BLUE",wrap(wxBLUE));
	rb_define_const(rb_cWXColor,"CYAN",wrap(wxCYAN));
	rb_define_const(rb_cWXColor,"GREEN",wrap(wxGREEN));
	rb_define_const(rb_cWXColor,"YELLOW",wrap(wxYELLOW));
	rb_define_const(rb_cWXColor,"LIGHT_GREY",wrap(wxLIGHT_GREY));
	rb_define_const(rb_cWXColor,"RED",wrap(wxRED));
	rb_define_const(rb_cWXColor,"WHITE",wrap(wxWHITE));
}

DLL_LOCAL void init_values(wxColor *self, char &red, char &green, char &blue, char &alpha)
{
	if(self->IsOk()) {
		red = self->Red();
		green = self->Green();
		blue = self->Blue();
		alpha = self->Alpha();
	}
}

DLL_LOCAL char val_to_char(VALUE val)
{
	char cval(0);
	if(FIXNUM_P(val))
		cval = NUM2CHR(val);
	else
		cval = NUM2DBL(val) * 256;
	return cval;
}

#define attr(name,val) DLL_LOCAL VALUE _get##name(VALUE self)\
{\
	if(_self->IsOk())\
		return CHR2FIX(_self->name());\
	else\
		return CHR2FIX(0);\
}\
DLL_LOCAL VALUE _set##name(VALUE self,VALUE cval)\
{\
	rb_check_frozen(self);\
\
	char red(0), green(0), blue(0), alpha(0);\
	init_values(_self, red, green, blue, alpha);\
	val = val_to_char(cval);\
	_self->Set(red, green, blue, alpha);\
\
	return val;\
}

attr(Red,red)
attr(Green,green)
attr(Blue,blue)

DLL_LOCAL VALUE _getAlpha(VALUE self)
{
	if(_self->IsOk())
		return CHR2FIX(_self->Alpha());
	else
		return CHR2FIX(0);
}

DLL_LOCAL VALUE _setAlpha(VALUE self,VALUE val)
{
	rb_check_frozen(self);

	char red(0), green(0), blue(0), alpha(0);

	init_values(_self, red, green, blue, alpha);

	if(!RTEST(val))
		alpha = wxALPHA_OPAQUE;
	else if(val == Qtrue)
		alpha = wxALPHA_TRANSPARENT;
	else
		alpha = val_to_char(val);

	_self->Set(red, green, blue, alpha);
	return val;
}

DLL_LOCAL VALUE _tos(VALUE self)
{
	return wrap(wxToString(*_self));
}

DLL_LOCAL VALUE _name(VALUE self)
{
	if(wxColourDatabase *database = wxTheColourDatabase) {
		wxString name(database->FindName(*_self).MakeLower());
		if(name.empty())
			return Qnil;
		return wrap(name);
	}
	return Qnil;
}


/*
 * call-seq:
 *   Color.new(red,green,blue[,alpha])
 *
 * creates a new Color Object.
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE red,green,blue,alpha;
	rb_scan_args(argc, argv, "31",&red,&green,&blue,&alpha);
	_setRed(self,red);
	_setGreen(self,green);
	_setBlue(self,blue);
	_setAlpha(self, alpha);
	return self;
}
/*
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_setRed(self,_getRed(other));
	_setGreen(self,_getGreen(other));
	_setBlue(self,_getBlue(other));
	_setAlpha(self,_getAlpha(other));
	return result;
}

/*
 * call-seq:
 *   inspect -> String
 *
 * Human-readable description.
 * ===Return value
 * String
*/
DLL_LOCAL VALUE _inspect(VALUE self)
{
	return rb_sprintf( "%s(%d, %d, %d, %d)",
		rb_obj_classname( self ),
		FIX2INT(_getRed(self)),
		FIX2INT(_getGreen(self)),
		FIX2INT(_getBlue(self)),
		FIX2INT(_getAlpha(self))
	);
}

/*
 * call-seq:
 *   marshal_dump -> Array
 *
 * Provides marshalling support for use by the Marshal library.
 * ===Return value
 * Array
 */
DLL_LOCAL VALUE _marshal_dump(VALUE self)
{
    VALUE ptr[4];
    ptr[0] = _getRed(self);
    ptr[1] = _getGreen(self);
    ptr[2] = _getBlue(self);
    ptr[3] = _getAlpha(self);
    return rb_ary_new4(4, ptr);
}

/*
 * call-seq:
 *   marshal_load(array) -> nil
 *
 * Provides marshalling support for use by the Marshal library.
 *
 *
 */
DLL_LOCAL VALUE _marshal_load(VALUE self, VALUE data)
{
	data = rb_Array(data);
    _setRed(self, RARRAY_AREF(data,0));
    _setGreen(self, RARRAY_AREF(data,1));
    _setBlue(self, RARRAY_AREF(data,2));
    _setAlpha(self, RARRAY_AREF(data,3));
    return Qnil;
}

struct equal_obj {
	wxColor* self;
	VALUE other;
};

VALUE _equal_block(equal_obj *obj)
{
	return wrap(*obj->self == unwrap<wxColor>(obj->other));
}

VALUE _equal_rescue(VALUE val)
{
	return Qfalse;
}

/*
 * call-seq:
 *   == color -> bool
 *
 * compares two colors.
 *
 *
 */
DLL_LOCAL VALUE _equal(VALUE self, VALUE other)
{
	equal_obj obj;
	obj.self = _self;
	obj.other = other;

	return rb_rescue(
		RUBY_METHOD_FUNC(_equal_block),(VALUE)&obj,
		RUBY_METHOD_FUNC(_equal_rescue),Qnil
	);
}


DLL_LOCAL VALUE _class_get(VALUE self, VALUE name)
{
	wxColor col;
	if(SYMBOL_P(name)) {
		if(ruby_app_inited)
			col = wxSystemSettings::GetColour(unwrapenum<wxSystemColour>(name));
	}else if(wxColourDatabase *database = wxTheColourDatabase) {
		col = database->Find(unwrap<wxString>(name));
	}
	if(col.IsOk())
		return wrap(col);
	return Qnil;
}

DLL_LOCAL VALUE _class_set(VALUE self, VALUE name, VALUE color)
{
	wxColor col;
	if(wxColourDatabase *database = wxTheColourDatabase) {
		database->AddColour(unwrap<wxString>(name), unwrap<wxColor>(color));
	}else
		return Qnil;
	return color;
}
}
}


/*
 * Document-class: WX::Color
 *
 * This class represents an Color.
*/

/* Document-attr: red
 * returns the red value of Color.
 */
/* Document-attr: blue
 * returns the blue value of Color.
 */
/* Document-attr: green
 * returns the green value of Color.
 */
/* Document-attr: alpha
 * returns the alpha value of Color.
 */
/* Document-attr: name
 * returns the name of the Color or nil if no name found
 */

/* Document-const: BLACK
 * predefined Color constant.
 */
/* Document-const: BLUE
 * predefined Color constant.
 */
/* Document-const: CYAN
 * predefined Color constant.
 */
/* Document-const: GREEN
 * predefined Color constant.
 */
/* Document-const: YELLOW
 * predefined Color constant.
 */
/* Document-const: LIGHT_GREY
 * predefined Color constant.
 */
/* Document-const: RED
 * predefined Color constant.
 */
/* Document-const: WHITE
 * predefined Color constant.
 */
DLL_LOCAL void Init_WXColor(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
#endif
	using namespace RubyWX::Color;
	rb_cWXColor = rb_define_class_under(rb_mWX,"Color",rb_cObject);

#if 0
	rb_define_attr(rb_cWXColor,"red",1,1);
	rb_define_attr(rb_cWXColor,"green",1,1);
	rb_define_attr(rb_cWXColor,"blue",1,1);
	rb_define_attr(rb_cWXColor,"alpha",1,1);
	rb_define_attr(rb_cWXColor,"name",1,0);

	rb_define_const(rb_cWXColor,"BLACK",wrap(wxBLACK));
	rb_define_const(rb_cWXColor,"BLUE",wrap(wxBLUE));
	rb_define_const(rb_cWXColor,"CYAN",wrap(wxCYAN));
	rb_define_const(rb_cWXColor,"GREEN",wrap(wxGREEN));
	rb_define_const(rb_cWXColor,"YELLOW",wrap(wxYELLOW));
	rb_define_const(rb_cWXColor,"LIGHT_GREY",wrap(wxLIGHT_GREY));
	rb_define_const(rb_cWXColor,"RED",wrap(wxRED));
	rb_define_const(rb_cWXColor,"WHITE",wrap(wxWHITE));
#endif

	rb_define_alloc_func(rb_cWXColor,_alloc);

	rb_define_method(rb_cWXColor,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
	rb_define_private_method(rb_cWXColor,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);


	rb_define_attr_method(rb_cWXColor,"red",_getRed,_setRed);
	rb_define_attr_method(rb_cWXColor,"green",_getGreen,_setGreen);
	rb_define_attr_method(rb_cWXColor,"blue",_getBlue,_setBlue);
	rb_define_attr_method(rb_cWXColor,"alpha",_getAlpha,_setAlpha);

	rb_define_attr_method(rb_cWXColor,"name",_name,NULL);

	rb_define_method(rb_cWXColor,"to_s",RUBY_METHOD_FUNC(_tos),0);
	rb_define_method(rb_cWXColor,"inspect",RUBY_METHOD_FUNC(_inspect),0);

	rb_define_method(rb_cWXColor,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cWXColor,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

	rb_define_method(rb_cWXColor,"==",RUBY_METHOD_FUNC(_equal),1);

	rb_define_singleton_method(rb_cWXColor,"[]",RUBY_METHOD_FUNC(_class_get),1);
	rb_define_singleton_method(rb_cWXColor,"[]=",RUBY_METHOD_FUNC(_class_set),2);

	rwxID_red = rb_intern("red");
	rwxID_blue = rb_intern("blue");
	rwxID_green = rb_intern("green");
	rwxID_alpha = rb_intern("alpha");

	registerType<wxColor>(rb_cWXColor, true);
	
	//*
	registerEnum<wxSystemColour>("wxSystemColour")
		->add(wxSYS_COLOUR_SCROLLBAR,"scrollbar")
		->add(wxSYS_COLOUR_DESKTOP,"desktop")
		->add(wxSYS_COLOUR_ACTIVECAPTION,"active_caption")
		->add(wxSYS_COLOUR_INACTIVECAPTION,"inactive_caption")
		->add(wxSYS_COLOUR_MENU,"menu")
		->add(wxSYS_COLOUR_WINDOW,"window")
		->add(wxSYS_COLOUR_WINDOWFRAME,"window_frame")
		->add(wxSYS_COLOUR_MENUTEXT,"menu_text")
		->add(wxSYS_COLOUR_WINDOWTEXT,"window_text")
		->add(wxSYS_COLOUR_CAPTIONTEXT,"caption_text")
		->add(wxSYS_COLOUR_ACTIVEBORDER,"active_border")
		->add(wxSYS_COLOUR_INACTIVEBORDER,"inactive_border")
		->add(wxSYS_COLOUR_APPWORKSPACE,"app_workspace")
		->add(wxSYS_COLOUR_HIGHLIGHT,"highlight")
		->add(wxSYS_COLOUR_HIGHLIGHTTEXT,"highlight_text")
		->add(wxSYS_COLOUR_BTNFACE,"btn_face")
		->add(wxSYS_COLOUR_BTNSHADOW,"btn_shadow")
		->add(wxSYS_COLOUR_GRAYTEXT,"gray_text")
		->add(wxSYS_COLOUR_BTNTEXT,"btn_text")
		->add(wxSYS_COLOUR_INACTIVECAPTIONTEXT,"inactive_captiontext")
		->add(wxSYS_COLOUR_BTNHIGHLIGHT,"btn_highlight")
		->add(wxSYS_COLOUR_3DDKSHADOW,"threeD_shadow")
		->add(wxSYS_COLOUR_3DLIGHT,"threeD_light")
		->add(wxSYS_COLOUR_INFOTEXT,"info_text")
		->add(wxSYS_COLOUR_INFOBK,"info_background")
		->add(wxSYS_COLOUR_LISTBOX,"listbox")
		->add(wxSYS_COLOUR_HOTLIGHT,"hotlight")
		->add(wxSYS_COLOUR_GRADIENTACTIVECAPTION,"gradient_active_caption")
		->add(wxSYS_COLOUR_GRADIENTINACTIVECAPTION,"gradient_inactive_caption")
		->add(wxSYS_COLOUR_MENUHILIGHT,"menu_highlight")
		->add(wxSYS_COLOUR_MENUBAR,"menubar")
		->add(wxSYS_COLOUR_LISTBOXTEXT,"listbox_text")
		->add(wxSYS_COLOUR_LISTBOXHIGHLIGHTTEXT,"listbox_highlight_text");
	//*/
}
