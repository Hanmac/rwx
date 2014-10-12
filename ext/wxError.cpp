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

class RubyExceptionLog : public wxLogInterposer
{
public:
	RubyExceptionLog() : wxLogInterposer() {}

protected:
	void DoLogRecord(wxLogLevel level,
	                             const wxString& msg,
	                             const wxLogRecordInfo& info)
	{
		wxLogInterposer::DoLogRecord(level, msg, info);

		const char * c = msg.GetData().AsChar();
		const char * f = info.func;
		switch(level)
		{
		case wxLOG_FatalError:
			rb_fatal("%s in %s", c, f);
			break;
		case wxLOG_Warning:
			rb_warn("%s in %s", c, f);
			break;
		case wxLOG_Error:
			rb_raise(rb_eWXError,"%s in %s", c, f);
			break;
		default:

			break;
		}


	}

};



DLL_LOCAL void Init_WXError(VALUE rb_mWX)
{
	rb_eWXError = rb_define_class_under(rb_mWX,"Error",rb_eException);
	new RubyExceptionLog();
	wxSetAssertHandler(wxrubyAssert);
}
