/*
 * wxXRC.cpp
 *
 *  Created on: 04.11.2012
 *      Author: hanmac
 */


#include "wxWizard.hpp"

#if wxUSE_XRC

namespace RubyWX {
namespace XRC {

DLL_LOCAL VALUE _xrc_load_file(VALUE self,VALUE name)
{
	wxXmlResource *old = wxXmlResource::Get();
	if(rb_block_given_p())
		wxXmlResource::Set(new wxXmlResource);
	wxXmlResource::Get()->InitAllHandlers();
	wxXmlResource::Get()->LoadFile(unwrap<wxString>(name));
	if(rb_block_given_p())
	{
		rb_yield(Qnil);
		wxXmlResource::Get()->Unload(unwrap<wxString>(name));
		wxXmlResource::Get()->ClearHandlers();
		wxXmlResource::Set(old);
	}
	return self;
}

}
}

#endif

DLL_LOCAL void Init_WXXRC(VALUE rb_mWX)
{
#if wxUSE_XRC
	using namespace RubyWX::XRC;

	rb_define_module_function(rb_mWX,"xrc_load_file",RUBY_METHOD_FUNC(_xrc_load_file),1);
#endif
}



