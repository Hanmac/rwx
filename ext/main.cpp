/*
 * main.cpp
 *
 *  Created on: 04.05.2012
 *      Author: hanmac
 */

#include "main.hpp"

#include <wx/arrstr.h>

#include <wx/filename.h>

infoholdertype infoklassholder;
typeholdertype typeklassholder;

enumregistertype enumregister;

VALUE global_holder;

typedef std::map<VALUE,std::size_t> ref_countertype;
ref_countertype ref_counter;


void rwx_refobject(VALUE object)
{
	if(ref_counter[object] == 0)
		rb_hash_aset(global_holder,INT2NUM(object),object);
	ref_counter[object]++;
}

void rwx_unrefobject(VALUE object)
{
	if(ref_counter[object] != 0)
		ref_counter[object]--;
	if(ref_counter[object] == 0)
		rb_hash_delete(global_holder,INT2NUM(object));

}

VALUE wrapClass(const wxClassInfo * info)
{
	infoholdertype::iterator it = infoklassholder.find(info);
	if(it != infoklassholder.end())
		return it->second;
	if(const wxClassInfo *base = info->GetBaseClass1())
		return wrapClass(base);
	return Qnil;
}

VALUE wrapPtr(void *arg,VALUE klass)
{
	if(arg)
		return Data_Wrap_Struct(klass, 0, 0, arg);
	return Qnil;
}

template <>
bool unwrap< bool >(const VALUE &val )
{
	return RTEST(val);
}

template <>
VALUE wrap< bool >(const bool &st )
{
	return st ? Qtrue : Qfalse;
}

template <>
int unwrap< int >(const VALUE &val )
{
	return NUM2INT(val);
}

template <>
VALUE wrap< int >(const int &st )
{
	return INT2NUM(st);
}

template <>
unsigned int unwrap< unsigned int >(const VALUE &val )
{
	return NUM2UINT(val);
}

template <>
VALUE wrap< unsigned int >(const unsigned int &st )
{
	return UINT2NUM(st);
}

template <>
long unwrap< long >(const VALUE &val )
{
	return NUM2LONG(val);
}

template <>
VALUE wrap< long >(const long &st )
{
	return LONG2NUM(st);
}

template <>
unsigned long unwrap< unsigned long >(const VALUE &val )
{
	return NUM2ULONG(val);
}

template <>
VALUE wrap< unsigned long >(const unsigned long &st )
{
	return ULONG2NUM(st);
}

template <>
VALUE wrap< wxString >(const wxString &st )
{
#ifdef HAVE_RUBY_ENCODING_H
	return rb_enc_str_new(st.c_str(),strlen(st.c_str()),rb_utf8_encoding());
#else
	return rb_str_new2(st.c_str());
#endif
}
template <>
VALUE wrap< wxChar >(const wxChar &c )
{
	return wrap(wxString(c));
}

template <>
char* unwrap< char* >(const VALUE &val )
{
	if(NIL_P(val))
		return NULL;
	if (rb_obj_is_kind_of(val, rb_cString)){
		return RSTRING_PTR(val);
	}
	else
		return unwrap< char* >(rb_funcall(val,rb_intern("to_s"),0));
}

template <>
wxString unwrap< wxString >(const VALUE &val )
{
	if(NIL_P(val))
		return wxEmptyString;
	else
		return wxString(unwrap< char* >(val));
}
template <>
VALUE wrap< wxArrayString >(const wxArrayString &st )
{
	VALUE ary = rb_ary_new();
	for(wxArrayString::const_iterator it = st.begin(); it != st.end(); ++it)
		rb_ary_push(ary,wrap(*it));
	return ary;
}

template <>
VALUE wrap< wxArrayInt >(const wxArrayInt &st )
{
	VALUE ary = rb_ary_new();
	for(wxArrayInt::const_iterator it = st.begin(); it != st.end(); ++it)
		rb_ary_push(ary,wrap(*it));
	return ary;
}

template <>
wxArrayString unwrap< wxArrayString >(const VALUE &val )
{
	wxArrayString arry;
	if(NIL_P(val))
		return arry;
	else if(rb_respond_to(val,rb_intern("to_a")))	{
		VALUE dp = rb_funcall(val,rb_intern("to_a"),0);
		std::size_t length = RARRAY_LEN(dp);
		for(std::size_t i = 0; i < length; ++i)
			arry.Add(unwrap<wxString>(RARRAY_PTR(dp)[i]));
	}else{
		arry.Add(unwrap<wxString>(val));
	}
	return arry;
}
template <>
wxArrayInt unwrap< wxArrayInt >(const VALUE &val )
{
	wxArrayInt arry;
	if(NIL_P(val))
		return arry;
	else if(rb_respond_to(val,rb_intern("to_a")))	{
		VALUE dp = rb_funcall(val,rb_intern("to_a"),0);
		std::size_t length = RARRAY_LEN(dp);
		for(std::size_t i = 0; i < length; ++i)
			arry.Add(NUM2INT(RARRAY_PTR(dp)[i]));
	}else{
		arry.Add(NUM2INT(val));
	}
	return arry;
}


template <>
VALUE wrap< wxDateTime >(const wxDateTime &st )
{
	return rb_time_new(st.GetTicks(),st.GetMillisecond() * 10);
}

template <>
wxDateTime unwrap< wxDateTime >(const VALUE &val )
{
	wxDateTime result;
	result.SetToCurrent();
	result.MakeTimezone(
		NUM2UINT(rb_funcall(val,rb_intern("gmt_offset"),0)) - 3600,
		RTEST(rb_funcall(val,rb_intern("dst?"),0))
	);

	result.Set(
		NUM2UINT(rb_funcall(val,rb_intern("day"),0)),
		(wxDateTime::Month)(NUM2UINT(rb_funcall(val,rb_intern("month"),0))-1),
		NUM2UINT(rb_funcall(val,rb_intern("year"),0)),
		NUM2UINT(rb_funcall(val,rb_intern("hour"),0)),
		NUM2UINT(rb_funcall(val,rb_intern("min"),0)),
		NUM2UINT(rb_funcall(val,rb_intern("sec"),0)),
		NUM2UINT(rb_funcall(val,rb_intern("usec"),0)) / 1000
	);
	return result;
}


int unwrap_iconflag(const VALUE &val,int mask)
{
	if(NIL_P(val))
		return wxICON_NONE;
	
	int result = 0;
	if(SYMBOL_P(val))
	{
		ID id(SYM2ID(val));
		if(id == rb_intern("exclamation"))
			result = wxICON_EXCLAMATION;
		else if(id == rb_intern("hand"))
			result = wxICON_HAND;
		else if(id == rb_intern("warning"))
			result = wxICON_WARNING;
		else if(id == rb_intern("error"))
			result = wxICON_ERROR;
		else if(id == rb_intern("question"))
			result = wxICON_QUESTION;
		else if(id == rb_intern("information"))
			result = wxICON_INFORMATION;
		else if(id == rb_intern("stop"))
			result = wxICON_STOP;
		else if(id == rb_intern("asterisk"))
			result = wxICON_ASTERISK;
	}else
		result = NUM2INT(val);
		
	if((result & mask) != result)
		rb_raise(rb_eTypeError,"%s is not a in %d mask",unwrap<char*>(val),mask);
	
	return result;

}

int unwrap_buttonflag(const VALUE& val)
{
	if(rb_obj_is_kind_of(val,rb_cArray))
	{

		int result(0);
		std::size_t count = RARRAY_LEN(val);
		for(std::size_t i = 0;i < count; ++i)
		{
			result |= unwrap_buttonflag(RARRAY_PTR(val)[i]);
		}
		return result;
	}else if(SYMBOL_P(val))
	{
		ID id(SYM2ID(val));
		if(id == rb_intern("yes"))
			return wxYES;
		if(id == rb_intern("ok"))
			return wxOK;
		if(id == rb_intern("no"))
			return wxNO;
		if(id == rb_intern("yes_no"))
			return wxYES_NO;
		if(id == rb_intern("cancel"))
			return wxCANCEL;
		if(id == rb_intern("apply"))
			return wxAPPLY;
		if(id == rb_intern("close"))
			return wxCLOSE;
		if(id == rb_intern("help"))
			return wxHELP;
	}else
			return NUM2INT(val);
	return wxOK;
}

bool check_file_loadable(const wxString& path)
{
	errno = 0;
	wxFileName dir(wxPathOnly(path));
	dir.MakeAbsolute(wxGetCwd());

	wxFileName file(path);
	file.MakeAbsolute(wxGetCwd());

	if(dir.DirExists())
	{
		if(file.FileExists()){
			if(!file.IsFileReadable())
				errno = EACCES;
		}else
			errno = ENOENT;
	}else
		errno = ENOENT;

	if(errno)
	{
		rb_sys_fail(path.c_str());
		return false;
	}
	return true;

}
bool check_file_saveable(const wxString& path)
{
	errno = 0;

	wxFileName dir(wxPathOnly(path));
	dir.MakeAbsolute(wxGetCwd());

	wxFileName file(path);
	file.MakeAbsolute(wxGetCwd());
	if(dir.DirExists())
	{
		if(file.FileExists() && !file.IsFileWritable())
			errno = EACCES;
		else if(!dir.IsDirWritable())
			errno = EACCES;
	}else
		errno = ENOENT;

	if(errno)
	{
		rb_sys_fail(path.c_str());
		return false;
	}
	return true;
}

