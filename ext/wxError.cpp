/*
 * wxError.cpp
 *
 *  Created on: 21.02.2012
 *      Author: hanmac
 */
#include "wxError.hpp"

VALUE rb_eWXError;

class RubyExceptionLog : public wxLog
{
protected:
	void DoLogRecord(wxLogLevel level,
	                             const wxString& msg,
	                             const wxLogRecordInfo& info)
	{
		rb_raise(rb_eWXError,"%s",msg.GetData().AsChar());
	}

};

void Init_WXError(VALUE rb_mWX)
{
	rb_eWXError = rb_define_class_under(rb_mWX,"Error",rb_eException);
	wxLog::SetActiveTarget(new RubyExceptionLog);
}
