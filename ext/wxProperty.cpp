/*
 * wxProperty.cpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#include "wxProperty.hpp"
#include "wxPropertyGrid.hpp"
#include "wxVariant.hpp"
#include "wxBitmap.hpp"
#include "wxColor.hpp"
#include "wxFont.hpp"
#include "wxApp.hpp"

VALUE rb_cWXProperty;
#if wxUSE_PROPGRID

#include <wx/propgrid/props.h>
#include <wx/propgrid/advprops.h>

template<typename T>
DLL_LOCAL void registerProperty(VALUE mod,const char* name,const char* parent = NULL)
{
	VALUE pc = rb_cWXProperty;
	
	if(parent)
		pc = rb_const_get(mod,rb_intern(parent));
	
	VALUE klass = rb_define_class_under(mod,name,pc);
	registerInfo<T>(klass);
}


#define _self unwrap<wxPGProperty*>(self)

namespace RubyWX {
namespace Property {

//APP_PROTECT(wxPGProperty)

DLL_LOCAL VALUE _alloc(VALUE self)
{
	app_protected();

	for(VALUE klass = self; klass !=rb_cObject; klass = rb_class_get_superclass(klass))
	{
		for(infoholdertype::const_iterator it = infoklassholder.begin(); it != infoklassholder.end();++it)
			if(it->second == klass)
				return wrapPtr(it->first->CreateObject(),self);
	}

	return Qnil;
}

macro_attr(Name,wxString)
macro_attr(Label,wxString)
macro_attr(HelpString,wxString)

singlereturn(GetBaseName)
singlereturn(GetMainParent)
singlereturn(GetParent)

singlereturn(GetDepth)

singlereturn(GetGrid)

singlereturn(IsVisible)
singlereturn(IsRoot)
singlereturn(IsCategory)

singlereturn(HasVisibleChildren)

singlereturn(GetValue)
singlereturn(GetDefaultValue)
singlereturn(GetValueImage)

singlereturn(GetValueType)

singlefunc(DeleteChildren)

macro_attr(ValueImage,wxBitmap&)

macro_attr_selection(ChoiceSelection,GetChoices().GetCount)

macro_attr_bool(Expanded)
macro_attr_bool2(Enabled,Enable)

DLL_LOCAL VALUE _setValue(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	_self->SetValue(unwrapVariant(val,_self->GetValueType()));
	return val;
}

DLL_LOCAL VALUE _setDefaultValue(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	wxVariant var = unwrapVariant(val,_self->GetValueType());
	_self->SetDefaultValue(var);
	return val;
}

DLL_LOCAL VALUE _getAttribute(VALUE self,VALUE name)
{
	return wrap(_self->GetAttribute(unwrap<wxString>(name)));
}

DLL_LOCAL VALUE _setAttribute(VALUE self,VALUE name,VALUE val)
{
	rb_check_frozen(self);
	_self->SetAttribute(unwrap<wxString>(name),unwrapVariant(val,_self->GetAttribute(unwrap<wxString>(name)).GetType()));
	return val;
}

DLL_LOCAL VALUE _getClass(VALUE self)
{
	return wrap(wxString(_self->GetClassInfo()->GetClassName()));
}


DLL_LOCAL VALUE _setVisible(VALUE self,VALUE val)
{
	rb_check_frozen(self);
	_self->Hide(!RTEST(val));
	return val;
}

/*
 * call-seq:
 *   show([recurse]) -> self
 *
 * makes this property visible, children too if recurse is set.
 * ===Arguments
 * * recurse true/false
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _show(int argc,VALUE *argv,VALUE self)
{
	rb_check_frozen(self);

	VALUE recurse;

	rb_scan_args(argc, argv, "01",&recurse);

	bool rec = NIL_P(recurse) || RTEST(recurse);
	_self->Hide(false, rec ? wxPG_RECURSE : wxPG_DONT_RECURSE );
	return self;
}


/*
 * call-seq:
 *   hide([recurse]) -> self
 *
 * hides this property, children too if recurse is set.
 * ===Arguments
 * * recurse true/false
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _hide(int argc,VALUE *argv,VALUE self)
{
	rb_check_frozen(self);

	VALUE recurse;

	rb_scan_args(argc, argv, "01",&recurse);

	bool rec = NIL_P(recurse) || RTEST(recurse);
	_self->Hide(true, rec ? wxPG_RECURSE : wxPG_DONT_RECURSE );
	return self;
}



DLL_LOCAL VALUE _each_child_size(VALUE self)
{
	return UINT2NUM(_self->GetChildCount());
}



/*
 * call-seq:
 *   each_child -> Enumerator
 *   each_child { |child| } -> self
 *
 * iterates the children in this property.
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _each_child(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_child_size));
	std::size_t count = _self->GetChildCount();
	for(std::size_t i = 0; i < count; ++i)
		rb_yield(wrap(_self->Item(i)));
	return self;
}

DLL_LOCAL VALUE _each_choice_count(VALUE self)
{
	const wxPGChoices& choices = _self->GetChoices();
	if(!choices.IsOk())
		return INT2FIX(0);

	return wrap(choices.GetCount());
}



/*
 * call-seq:
 *   each_choice -> Enumerator
 *   each_choice { |child| } -> self
 *
 * iterates the choices in this property.
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _each_choice(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_choice_count));
	wxPGChoices& choices = const_cast<wxPGChoices&>(_self->GetChoices());

	if(!choices.IsOk())
		return self;

	std::size_t count = choices.GetCount();
	for(std::size_t i = 0; i < count; ++i)
	{
		rb_yield(wrap(&dynamic_cast<wxPGCell&>(choices[i])));
	}


	return self;
}


VALUE _set_choice(wxPGProperty* self,int idx,VALUE hash)
{
	wxPGChoices& choices = const_cast<wxPGChoices&>(self->GetChoices());

	if(!choices.IsOk())
		return Qnil;

	wxPGChoiceEntry& item = choices[idx];

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		wxBitmap bitmap;
		wxColour fg,bg;
		wxFont font(wxNullFont);

		if(set_hash_option(hash,"bitmap",bitmap))
			item.SetBitmap(bitmap);

		if(set_hash_option(hash,"bg_col",bg))
			item.SetBgCol(bg);
		if(set_hash_option(hash,"fg_col",fg))
			item.SetFgCol(fg);
		if(set_hash_option(hash,"font",font))
			item.SetFont(font);
	}
	return wrap(&dynamic_cast<wxPGCell&>(item));
}


/*
 * call-seq:
 *   add_choice(label,[value],**options) -> WX::PropertyCell
 *
 * adds a new choice into this property
 * ===Arguments
 * * label String
 * * value Integer/nil
 * * options:
 *   * bitmap WX::Bitmap
 *   * bg_col WX::Color
 *   * fg_col WX::Color
 *   * font WX::Font
 * ===Return value
 * WX::PropertyCell
 *
*/
DLL_LOCAL VALUE _add_choice(int argc,VALUE *argv,VALUE self)
{
	VALUE label,value,hash;

	rb_scan_args(argc, argv, "11:",&label,&value,&hash);

	int cvalue = wxPG_INVALID_VALUE;

	if(!NIL_P(value))
		cvalue = NUM2INT(value);

	return _set_choice(_self,_self->AddChoice(unwrap<wxString>(label),cvalue),hash);

}

/*
 * call-seq:
 *   prepend_choice(pos,label,[value],**options) -> WX::PropertyCell
 *
 * prepends a new choice into this property
 * ===Arguments
 * * pos position of the new choice. Integer
 * * label String
 * * value Integer/nil
 * * options:
 *   * bitmap WX::Bitmap
 *   * bg_col WX::Color
 *   * fg_col WX::Color
 *   * font WX::Font
 * ===Return value
 * WX::PropertyCell
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of choices
 *
*/
DLL_LOCAL VALUE _insert_choice(int argc,VALUE *argv,VALUE self)
{
	VALUE label,idx,value,hash;

	rb_scan_args(argc, argv, "21:",&idx,&label,&value,&hash);

	int cvalue = wxPG_INVALID_VALUE;

	int cidx = NUM2INT(idx);

	std::size_t size = 0;

	const wxPGChoices& choices = _self->GetChoices();
	if(choices.IsOk())
		size = choices.GetCount();

	if(!NIL_P(value))
		cvalue = NUM2INT(value);

	if(check_index(cidx,size+1))
		return _set_choice(_self,_self->InsertChoice(unwrap<wxString>(label),cidx,cvalue),hash);
	return Qnil;
}


/*
 * call-seq:
 *   prepend_choice(label,[value],**options) -> WX::PropertyCell
 *
 * prepends a new choice into this property
 * ===Arguments
 * * label String
 * * value Integer/nil
 * * options:
 *   * bitmap WX::Bitmap
 *   * bg_col WX::Color
 *   * fg_col WX::Color
 *   * font WX::Font
 * ===Return value
 * WX::PropertyCell
 *
*/
DLL_LOCAL VALUE _prepend_choice(int argc,VALUE *argv,VALUE self)
{
	VALUE label,value,hash;

	rb_scan_args(argc, argv, "11:",&label,&value,&hash);

	int cvalue = wxPG_INVALID_VALUE;

	if(!NIL_P(value))
		cvalue = NUM2INT(value);

	return _set_choice(_self,_self->InsertChoice(unwrap<wxString>(label),0,cvalue),hash);

}


/*
 * call-seq:
 *   delete_choice(pos) -> self
 *
 * delete the choice at the given position.
 * ===Arguments
 * * pos position of the deleting choice. Integer
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of choices
 *
*/
DLL_LOCAL VALUE _delete_choice(VALUE self,VALUE idx)
{

	int cidx = NUM2INT(idx);

	std::size_t size = 0;

	const wxPGChoices& choices = _self->GetChoices();
	if(choices.IsOk())
		size = choices.GetCount();

	if(check_index(cidx,size))
		_self->DeleteChoice(cidx);

	return self;
}


/*
 * call-seq:
 *   add_child(property) -> WX::Property
 *   add_child(property) { |property| } -> WX::Property
 *
 * adds a new Property as child into this one
 * ===Arguments
 * * property WX::Property or class that inherits from WX::Property
 * ===Return value
 * WX::Property
 *
*/
DLL_LOCAL VALUE _add_child(VALUE self,VALUE prop)
{
	return wrap(_self->AppendChild(unwrap<wxPGProperty*>(prop)));
}


/*
 * call-seq:
 *   add_private_child(property) -> self
 *
 * adds a new Property as private child into this one
 * ===Arguments
 * * property WX::Property or class that inherits from WX::Property
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _add_private_child(VALUE self,VALUE prop)
{
	_self->AddPrivateChild(unwrap<wxPGProperty*>(prop));
	return self;
}


/*
 * call-seq:
 *   insert_child(pos,property) -> WX::Property
 *   insert_child(pos,property) { |property| } -> WX::Property
 *
 * adds a new Property as child into this one
 * ===Arguments
 * * pos position of the new child property. Integer
 * * property WX::Property or class that inherits from WX::Property
 * ===Return value
 * WX::Property
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of children
 *
*/
DLL_LOCAL VALUE _insert_child(VALUE self,VALUE idx,VALUE prop)
{
	int cidx = NUM2INT(idx);

	if(check_index(cidx,_self->GetChildCount()+1))
		return wrap(_self->InsertChild(cidx,unwrap<wxPGProperty*>(prop)));

	return Qnil;
}


/*
 * call-seq:
 *   prepend_child(property) -> WX::Property
 *   prepend_child(property) { |property| } -> WX::Property
 *
 * prepends a new Property as child into this one
 * ===Arguments
 * * property WX::Property or class that inherits from WX::Property
 * ===Return value
 * WX::Property
 *
*/
DLL_LOCAL VALUE _prepend_child(VALUE self,VALUE prop)
{
	return wrap(_self->InsertChild(0,unwrap<wxPGProperty*>(prop)));
}




DLL_LOCAL VALUE _each_attribute_size(VALUE self)
{
	return wrap(_self->GetAttributes().GetCount());
}


/*
 * call-seq:
 *   each_attribute -> Enumerator
 *   each_attribute { |name, value| } -> self
 *
 * iterates the attributes in this property.
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _each_attribute(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_attribute_size));

	const wxPGAttributeStorage& attrs = _self->GetAttributes();

	wxPGAttributeStorage::const_iterator it = attrs.StartIteration();
	wxVariant var;
	while(attrs.GetNext(it,var))
		rb_yield_values(2,wrap(var.GetName()),wrap(var));
	return self;
}


VALUE find_prop_class(VALUE self,VALUE name)
{
	return wxClassInfo::FindClass(wxString("wx") + unwrap<wxString>(name)) ? Qtrue : Qfalse;
}

}
}

#endif


/* Document-method: parent
 * call-seq:
 *   parent -> WX::Property or nil
 *
 * returns the parent property of this one or nil of this is already the parent.
 * ===Return value
 * WX::Property or nil
 */

/* Document-method: main_parent
 * call-seq:
 *   parent -> WX::Property or nil
 *
 * returns highest level parent property of this one or nil of this is already the highest.
 * ===Return value
 * WX::Property or nil
 */

/* Document-method: depth
 * call-seq:
 *   depth -> Integer
 *
 * returns the depth of this property inside a grid.
 * ===Return value
 * Integer
 */

/* Document-method: grid
 * call-seq:
 *   grid -> WX::PropertyGrid or nil
 *
 * returns the property grid that has this property of nil.
 * ===Return value
 * WX::PropertyGrid or nil
 */


/* Document-attr: label
 * the label of the Property. String
 */
/* Document-attr: name
 * the name of the Property. String
 */
/* Document-attr: help_string
 * the help string of the Property. String
 */
/* Document-attr: value
 * the value of the Property.
 */
/* Document-attr: default_value
 * the default_value of the Property.
 */
/* Document-attr: value_image
 * the image of the Property. WX::Bitmap
 */

/* Document-attr: visible
 * the visible of the Property. bool
 */

/* Document-attr: expanded
 * the expanded of the Property. bool
 */

/* Document-attr: enabled
 * the enabled of the Property. bool
 */

/* Document-attr: choice_selection
 * the index with choice of the Property is selected. Integer/nil
 */
DLL_LOCAL void Init_WXProperty(VALUE rb_mWX)
{
#if 0

	rb_define_attr(rb_cWXProperty,"name",1,1);
	rb_define_attr(rb_cWXProperty,"label",1,1);
	rb_define_attr(rb_cWXProperty,"help_string",1,1);

	rb_define_attr(rb_cWXProperty,"value",1,1);
	rb_define_attr(rb_cWXProperty,"default_value",1,1);
	rb_define_attr(rb_cWXProperty,"value_image",1,1);

	rb_define_attr(rb_cWXProperty,"visible",1,1);
	rb_define_attr(rb_cWXProperty,"expanded",1,1);
	rb_define_attr(rb_cWXProperty,"enabled",1,1);

	rb_define_attr(rb_cWXProperty,"choice_selection",1,1);

#endif

#if wxUSE_PROPGRID
	using namespace RubyWX::Property;
	rb_cWXProperty = rb_define_class_under(rb_mWX,"Property",rb_cObject);
	rb_define_alloc_func(rb_cWXProperty,_alloc);

	//TODO check if its possible to add this methods
	rb_undef_method(rb_cWXProperty,"initialize_copy");
	rb_undef_method(rb_cWXProperty,"_load");
	rb_undef_method(rb_cWXProperty,"_dump");

	rb_define_method(rb_cWXProperty,"base_name",RUBY_METHOD_FUNC(_GetBaseName),0);
	rb_define_method(rb_cWXProperty,"main_parent",RUBY_METHOD_FUNC(_GetMainParent),0);
	rb_define_method(rb_cWXProperty,"parent",RUBY_METHOD_FUNC(_GetParent),0);

	rb_define_method(rb_cWXProperty,"depth",RUBY_METHOD_FUNC(_GetDepth),0);

	rb_define_method(rb_cWXProperty,"grid",RUBY_METHOD_FUNC(_GetGrid),0);

	rb_define_method(rb_cWXProperty,"visible_children?",RUBY_METHOD_FUNC(_HasVisibleChildren),0);
	rb_define_method(rb_cWXProperty,"root?",RUBY_METHOD_FUNC(_IsRoot),0);
	rb_define_method(rb_cWXProperty,"category?",RUBY_METHOD_FUNC(_IsCategory),0);

	rb_define_attr_method(rb_cWXProperty,"name",_getName,_setName);
	rb_define_attr_method(rb_cWXProperty,"label",_getLabel,_setLabel);
	rb_define_attr_method(rb_cWXProperty,"help_string",_getHelpString,_setHelpString);

	rb_define_attr_method(rb_cWXProperty,"value",_GetValue,_setValue);
	rb_define_attr_method(rb_cWXProperty,"default_value",_GetDefaultValue,_setDefaultValue);
	rb_define_attr_method(rb_cWXProperty,"value_image",_getValueImage,_setValueImage);

	rb_define_attr_method(rb_cWXProperty,"visible",_IsVisible,_setVisible);
	rb_define_attr_method(rb_cWXProperty,"expanded",_getExpanded,_setExpanded);
	rb_define_attr_method(rb_cWXProperty,"enabled",_getEnabled,_setEnabled);

	rb_define_attr_method(rb_cWXProperty,"choice_selection",_getChoiceSelection,_setChoiceSelection);

	rb_define_method(rb_cWXProperty,"show",RUBY_METHOD_FUNC(_show),-1);
	rb_define_method(rb_cWXProperty,"hide",RUBY_METHOD_FUNC(_hide),-1);

	rb_define_method(rb_cWXProperty,"add_child",RUBY_METHOD_FUNC(_add_child),1);
	rb_define_method(rb_cWXProperty,"add_private_child",RUBY_METHOD_FUNC(_add_private_child),1);
	rb_define_method(rb_cWXProperty,"insert_child",RUBY_METHOD_FUNC(_insert_child),2);
	rb_define_method(rb_cWXProperty,"prepend_child",RUBY_METHOD_FUNC(_prepend_child),1);

	rb_define_method(rb_cWXProperty,"add_choice",RUBY_METHOD_FUNC(_add_choice),-1);
	rb_define_method(rb_cWXProperty,"insert_choice",RUBY_METHOD_FUNC(_insert_choice),-1);
	rb_define_method(rb_cWXProperty,"prepend_choice",RUBY_METHOD_FUNC(_prepend_choice),-1);

	rb_define_method(rb_cWXProperty,"delete_children",RUBY_METHOD_FUNC(_DeleteChildren),0);
	rb_define_method(rb_cWXProperty,"delete_choice",RUBY_METHOD_FUNC(_delete_choice),1);

	rb_define_method(rb_cWXProperty,"each_child",RUBY_METHOD_FUNC(_each_child),0);
	rb_define_method(rb_cWXProperty,"each_choice",RUBY_METHOD_FUNC(_each_choice),0);
	rb_define_method(rb_cWXProperty,"each_attribute",RUBY_METHOD_FUNC(_each_attribute),0);

	rb_define_method(rb_cWXProperty,"wxclass",RUBY_METHOD_FUNC(_getClass),0);
	rb_define_method(rb_cWXProperty,"type",RUBY_METHOD_FUNC(_GetValueType),0);

	rb_define_method(rb_cWXProperty,"[]",RUBY_METHOD_FUNC(_getAttribute),1);
	rb_define_method(rb_cWXProperty,"[]=",RUBY_METHOD_FUNC(_setAttribute),2);

	registerInfo<wxPGProperty>(rb_cWXProperty);
	
	rb_define_singleton_method(rb_cWXProperty,"find",RUBY_METHOD_FUNC(find_prop_class),1);
	
	registerProperty<wxIntProperty>(rb_mWX,"IntProperty");
	registerProperty<wxPropertyCategory>(rb_mWX,"CategoryProperty");
	registerProperty<wxBoolProperty>(rb_mWX,"BoolProperty");
	registerProperty<wxFontProperty>(rb_mWX,"FontProperty");
	registerProperty<wxFileProperty>(rb_mWX,"FileProperty");

#if wxUSE_IMAGE
	registerProperty<wxImageFileProperty>(rb_mWX,"ImageFileProperty","FileProperty");
#endif

#if wxUSE_DATETIME
	registerProperty<wxDateProperty>(rb_mWX,"DateProperty");
#endif

	registerProperty<wxStringProperty>(rb_mWX,"StringProperty");
	registerProperty<wxArrayStringProperty>(rb_mWX,"ArrayStringProperty");
	
	registerProperty<wxEnumProperty>(rb_mWX,"EnumProperty");
	registerProperty<wxFlagsProperty>(rb_mWX,"FlagsProperty");
	
	registerProperty<wxSystemColourProperty>(rb_mWX,"SystemColorProperty","EnumProperty");
	registerProperty<wxColourProperty>(rb_mWX,"ColorProperty","SystemColorProperty");
	registerProperty<wxCursorProperty>(rb_mWX,"CursorProperty","EnumProperty");
	
#endif
}



