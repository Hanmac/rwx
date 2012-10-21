/*
 * wxPropertyGridInterface.cpp
 *
 *  Created on: 09.05.2012
 *      Author: hanmac
 */

#include "wxPropertyGridInterface.hpp"
#include "wxPropertyGrid.hpp"
#include "wxPropertyGridManager.hpp"

VALUE rb_mWXPropertyGridInterface;

#if wxUSE_PROPGRID
// i need this because if i try to access the interface directly it fails
template <>
wxPropertyGridInterface* unwrap< wxPropertyGridInterface* >(const VALUE &obj)
{
 if(rb_obj_is_kind_of(obj,rb_cWXPropertyGrid))
	 return unwrap<wxPropertyGrid*>(obj);
 if(rb_obj_is_kind_of(obj,rb_cWXPropertyGridManager))
	 return unwrap<wxPropertyGridManager*>(obj);

 return NULL;
}

#define _self unwrap<wxPropertyGridInterface*>(self)

namespace RubyWX {
namespace PropertyGridInterface {

VALUE _append(VALUE self,VALUE val)
{
	return wrap(_self->Append(unwrap<wxPGProperty*>(val)));
}

}
}
#endif

DLL_LOCAL void Init_WXPropertyGridInterface(VALUE rb_mWX)
{
#if wxUSE_PROPGRID
	using namespace RubyWX::PropertyGridInterface;
	rb_mWXPropertyGridInterface = rb_define_module_under(rb_mWX,"PropertyGridInterface");

	rb_define_method(rb_mWXPropertyGridInterface,"append",RUBY_METHOD_FUNC(_append),1);

	wxPropertyGrid::InitAllTypeHandlers();
#endif

}



