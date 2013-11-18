/*
 * wxCommandEvent.cpp
 *
 *  Created on: 12.03.2012
 *      Author: hanmac
 */



#include "wxEvent.hpp"
#include "wxWindow.hpp"

#define _self unwrap<wxCommandEvent*>(self)

VALUE rb_cWXCommandEvent;

namespace RubyWX {
namespace CommandEvent {

singlereturn(IsChecked)
singlereturn(IsSelection)
singlereturn(GetString)
singlereturn(GetSelection)

}
}


DLL_LOCAL void Init_WXCommandEvent(VALUE rb_mWX)
{
	using namespace RubyWX::CommandEvent;

	rb_cWXCommandEvent = rb_define_class_under(rb_cWXEvent,"Command",rb_cWXEvent);

	rb_define_method(rb_cWXCommandEvent,"checked?",RUBY_METHOD_FUNC(_IsChecked),0);
	rb_define_method(rb_cWXCommandEvent,"selection?",RUBY_METHOD_FUNC(_IsSelection),0);
	rb_define_method(rb_cWXCommandEvent,"string",RUBY_METHOD_FUNC(_GetString),0);
	rb_define_method(rb_cWXCommandEvent,"selection",RUBY_METHOD_FUNC(_GetSelection),0);

	registerInfo<wxCommandEvent>(rb_cWXCommandEvent);
}
