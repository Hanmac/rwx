/*
 * wxProperty.cpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */

#include "wxProperty.hpp"
#include "wxVariant.hpp"
#include "wxBitmap.hpp"
#include "wxApp.hpp"

VALUE rb_cWXProperty;
#if wxUSE_PROPGRID

#include <wx/propgrid/props.h>
#include <wx/propgrid/advprops.h>

#define _self unwrap<wxPGProperty*>(self)

namespace RubyWX {
namespace Property {

//APP_PROTECT(wxPGProperty)

DLL_LOCAL VALUE _alloc(VALUE self)
{
	if(ruby_app_inited)
	{
		VALUE klass = self;
		while(true)
		{
			for(infoholdertype::const_iterator it = infoklassholder.begin(); it != infoklassholder.end();++it)
				if(it->second == klass)
					return wrapPtr(it->first->CreateObject(),self);
			klass = rb_class_get_superclass(klass);
		}
	
	}
	else
		rb_raise(rb_eArgError,"%s is not running.",rb_class2name(rb_cWXApp));
	return Qnil;
}

macro_attr(Name,wxString)
macro_attr(Label,wxString)
macro_attr(HelpString,wxString)

singlereturn(GetBaseName)
singlereturn(GetMainParent)
singlereturn(GetParent)

singlereturn(GetValue)
singlereturn(GetDefaultValue)
singlereturn(GetValueImage)

singlereturn(GetValueType)

macro_attr(ValueImage,wxBitmap&)

VALUE _setValue(VALUE self,VALUE val)
{
	_self->SetValue(unwrapVariant(val,_self->GetValueType()));
	return val;
}

VALUE _setDefaultValue(VALUE self,VALUE val)
{
	wxVariant var = unwrapVariant(val,_self->GetValueType());
	_self->SetDefaultValue(var);
	return val;
}

VALUE _getAttribute(VALUE self,VALUE name)
{
	return wrap(_self->GetAttribute(unwrap<wxString>(name)));
}

VALUE _setAttribute(VALUE self,VALUE name,VALUE val)
{
	_self->SetAttribute(unwrap<wxString>(name),unwrapVariant(val,_self->GetAttribute(unwrap<wxString>(name)).GetType()));
	return val;
}

VALUE _getClass(VALUE self)
{
	return wrap(wxString(_self->GetClassInfo()->GetClassName()));
}


VALUE _each_child(VALUE self)
{
	RETURN_ENUMERATOR(self,0,NULL);
	size_t count = _self->GetChildCount();
	for(size_t i = 0; i < count; ++i)
		rb_yield(wrap(_self->Item(i)));
	return self;
}


VALUE _each_choices(VALUE self)
{
	RETURN_ENUMERATOR(self,0,NULL);
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

VALUE _each_attributes(VALUE self)
{
	RETURN_ENUMERATOR(self,0,NULL);

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

DLL_LOCAL void Init_WXProperty(VALUE rb_mWX)
{
#if wxUSE_PROPGRID
	using namespace RubyWX::Property;
	rb_cWXProperty = rb_define_class_under(rb_mWX,"Property",rb_cObject);
	rb_define_alloc_func(rb_cWXProperty,_alloc);

	rb_define_attr_method(rb_cWXProperty,"name",_getName,_setName);
	rb_define_attr_method(rb_cWXProperty,"label",_getLabel,_setLabel);

	rb_define_attr_method(rb_cWXProperty,"value",_GetValue,_setValue);
	rb_define_attr_method(rb_cWXProperty,"default_value",_GetDefaultValue,_setDefaultValue);
	rb_define_attr_method(rb_cWXProperty,"value_image",_getValueImage,_setValueImage);

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
	registerProperty<wxStringProperty>(rb_mWX,"StringProperty");
	registerProperty<wxArrayStringProperty>(rb_mWX,"ArrayStringProperty");
	
	registerProperty<wxEnumProperty>(rb_mWX,"EnumProperty");
	
#endif
}



