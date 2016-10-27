/*
 * wxVariant.cpp
 *
 *  Created on: 08.05.2012
 *      Author: hanmac
 */
#include "wxVariant.hpp"
#include "wxDataView.hpp"
#include "wxFont.hpp"
#include "wxBitmap.hpp"
#include "wxColor.hpp"
template <>
VALUE wrap< wxVariant >(const wxVariant &var)
{
	wxString type = var.GetType();
	if(type == "null")
		return Qnil;
	if(type == "bool")
		return wrap(var.GetBool());
	else if(type == "string")
		return wrap(var.GetString());

	else if(type == "arrstring")
		return wrap(var.GetArrayString());
	else if(type == "long")
		return RB_LONG2NUM(var.GetLong());
	else if(type == "double")
		return DBL2NUM(var.GetDouble());
	else if(type == "wxFont")
		return wrap((wxFont)var);
#if wxUSE_ANY
	else if(type == "wxColour" || type == "wxColor")
		return wrap(wxANY_AS(var.GetAny(),wxColour));
#endif
	else if(type == "wxBitmap")
		return wrap((wxBitmap)var);
	else if(type == "wxIcon")
		return wrap((wxIcon)var);
	else
		std::cout << "unknown VariantType: " << type << std::endl;
	return Qnil;
}

wxVariant unwrapVariant(VALUE obj,const wxString &type)
{
	wxVariant result;
	if(type == "string")
		result = unwrap<wxString>(obj);
	else if(type == "arrstring")
		result = unwrap<wxArrayString>(obj);
	else if(type == "bool")
		result = RTEST(obj);
	else if(type == "long")
		result = RB_NUM2LONG(obj);
	else if(type == "double")
		result = NUM2DBL(obj);
	else if(type == "wxFont")
		result = wxVariant(unwrap<wxFont>(obj));
#if wxUSE_ANY
	else if(type == "wxColour" || type == "wxColor")
		result = wxVariant(wxAny(unwrap<wxColour>(obj)));
#endif
#if wxUSE_DATAVIEWCTRL
	else if(type == "wxDataViewIconText")
	{
		result = wxVariant(wxDataViewIconText(wxEmptyString,unwrap<wxIcon>(obj)));
	}
#endif
	else if(type == "wxBitmap")
		result = wxVariant(unwrap<wxBitmap>(obj));
	else if(type == "wxIcon")
		result = wxVariant(unwrap<wxIcon>(obj));
	else
		std::cout << "unknown VariantType: "  << type << std::endl;
	return result;
}



