/*
 * wxStream.hpp
 *
 *  Created on: 22.02.2012
 *      Author: hanmac
 */

#ifndef WXSTREAM_HPP_
#define WXSTREAM_HPP_

#include "main.hpp"
#include <fstream>

class RubyInputStream : public wxInputStream
{
public:
	RubyInputStream(VALUE obj) : mRuby(obj) {}

	bool Eof() const;
	bool CanRead() const;
	bool IsSeekable() const;

protected:
	size_t OnSysRead(void *buffer, size_t size);
	wxFileOffset OnSysSeek(wxFileOffset seek, wxSeekMode mode);
	wxFileOffset OnSysTell() const;
private:
	VALUE mRuby;
};
//
//class RubyOutputStream :virtual public wxOutputStream
//{
//public:
//	RubyOutputStream(VALUE obj);
//
//	bool IsSeekable() const {
//		return true;
//	}
//protected:
//	size_t OnSysWrite(const void *buffer, size_t bufsize);
//	wxFileOffset OnSysSeek(wxFileOffset seek, wxSeekMode mode);
//	wxFileOffset OnSysTell() const;
//private:
//	VALUE mRuby;
//};

#endif /* WXSTREAM_HPP_ */
