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

	VALUE klass = self;
	while(true)
	{
		for(infoholdertype::const_iterator it = infoklassholder.begin(); it != infoklassholder.end();++it)
			if(it->second == klass)
				return wrapPtr(it->first->CreateObject(),self);
		klass = rb_class_get_superclass(klass);
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

singlereturn(GetValue)
singlereturn(GetDefaultValue)
singlereturn(GetValueImage)

singlereturn(GetValueType)

macro_attr(ValueImage,wxBitmap&)

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

DLL_LOCAL VALUE _each_child_size(VALUE self)
{
	return UINT2NUM(_self->GetChildCount());
}



DLL_LOCAL VALUE _each_child(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,_each_child_size);
	std::size_t count = _self->GetChildCount();
	for(std::size_t i = 0; i < count; ++i)
		rb_yield(wrap(_self->Item(i)));
	return self;
}

DLL_LOCAL VALUE _each_choices_count(VALUE self)
{
	const wxPGChoices& choices = _self->GetChoices();
	if(!choices.IsOk())
		return INT2FIX(0);

	return wrap(choices.GetCount());
}



DLL_LOCAL VALUE _each_choices(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,_each_choices_count);
	wxPGChoices& choices = const_cast<wxPGChoices&>(_self->GetChoices());

	if(!choices.IsOk())
		return self;

	size_t count = choices.GetCount();
	//return INT2NUM(count);
	for(size_t i = 0; i < count; ++i)
	{
		rb_yield(wrap(&dynamic_cast<wxPGCell&>(choices[i])));
	}


	return self;
}

DLL_LOCAL VALUE _each_attributes_size(VALUE self)
{
	return wrap(_self->GetAttributes().GetCount());
}


DLL_LOCAL VALUE _each_attributes(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,_each_attributes_size);

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
/* Document-attr: value
 * the value of the Property.
 */
/* Document-attr: default_value
 * the default_value of the Property.
 */
/* Document-attr: value_image
 * the image of the Property. WX::Bitmap
 */


/* Document-attr: expanded
 * the expanded of the Property. bool
 */

/* Document-attr: enabled
 * the enabled of the Property. bool
 */

DLL_LOCAL void Init_WXProperty(VALUE rb_mWX)
{
#if 0

	rb_define_attr(rb_cWXProperty,"name",1,1);
	rb_define_attr(rb_cWXProperty,"label",1,1);

	rb_define_attr(rb_cWXProperty,"value",1,1);
	rb_define_attr(rb_cWXProperty,"default_value",1,1);
	rb_define_attr(rb_cWXProperty,"value_image",1,1);

	rb_define_attr(rb_cWXProperty,"expanded",1,1);
	rb_define_attr(rb_cWXProperty,"enabled",1,1);

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

	rb_define_attr_method(rb_cWXProperty,"name",_getName,_setName);
	rb_define_attr_method(rb_cWXProperty,"label",_getLabel,_setLabel);

	rb_define_attr_method(rb_cWXProperty,"value",_GetValue,_setValue);
	rb_define_attr_method(rb_cWXProperty,"default_value",_GetDefaultValue,_setDefaultValue);
	rb_define_attr_method(rb_cWXProperty,"value_image",_getValueImage,_setValueImage);

	rb_define_attr_method(rb_cWXProperty,"expanded",_getExpanded,_setExpanded);
	rb_define_attr_method(rb_cWXProperty,"enabled",_getEnabled,_setEnabled);

	rb_define_method(rb_cWXProperty,"each_child",RUBY_METHOD_FUNC(_each_child),0);
	rb_define_method(rb_cWXProperty,"each_choices",RUBY_METHOD_FUNC(_each_choices),0);
	rb_define_method(rb_cWXProperty,"each_attributes",RUBY_METHOD_FUNC(_each_attributes),0);

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



