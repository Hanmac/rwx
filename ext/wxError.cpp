/*
 * wxError.cpp
 *
 *  Created on: 21.02.2012
 *      Author: hanmac
 */
#include "wxError.hpp"

VALUE rb_eWXError;

void wxrubyAssert(const wxString& file,
                                  int line,
                                  const wxString& func,
                                  const wxString& cond,
                                  const wxString& msg)
{
	rb_fatal("(%s) in %s \n %s",
		cond.GetData().AsChar(),
		func.GetData().AsChar(),
		msg.GetData().AsChar()
		);
}

class RubyExceptionLog : public wxLog
{
protected:
	void DoLogRecord(wxLogLevel level,
	                             const wxString& msg,
	                             const wxLogRecordInfo& info)
	{
		const char * c = msg.GetData().AsChar();
		switch(level)
		{
		case wxLOG_FatalError:
			rb_fatal("%s",c);
			break;
		case wxLOG_Warning:
			rb_warn("%s",c);
			break;
		default:
			rb_raise(rb_eWXError,"%s",c);
		}


	}

};



void Init_WXError(VALUE rb_mWX)
{
	rb_eWXError = rb_define_class_under(rb_mWX,"Error",rb_eException);
	wxLog::SetActiveTarget(new RubyExceptionLog);
	wxSetAssertHandler(wxrubyAssert);
}
