/*
 * wxDataView.hpp
 *
 *  Created on: 24.03.2012
 *      Author: hanmac
 */

#ifndef WXDATAVIEW_HPP_
#define WXDATAVIEW_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXDataView;

void Init_WXDataView(VALUE rb_mWX);
#if wxUSE_DATAVIEWCTRL
#include <wx/dataview.h>

class DataViewClientHolder
{
public:
	wxClientData *getClientValue( const wxDataViewItem& item) const;
	void setClientValue( const wxDataViewItem& item, wxClientData *data );
private:
	std::map<wxDataViewItem,wxClientData*> m_clientvalues;
};

class RubyDataViewNotifier : public wxDataViewModelNotifier
{
public:
    bool ItemAdded( const wxDataViewItem &parent, const wxDataViewItem &item );
    bool ItemDeleted( const wxDataViewItem &parent, const wxDataViewItem &item );
    bool ItemChanged( const wxDataViewItem &item );

    bool ValueChanged( const wxDataViewItem &item, unsigned int col );
    bool Cleared();

    void Resort();
};

VALUE wrap(wxDataViewModel *model, const wxDataViewItem& item);

#endif

#endif /* WXDATAVIEW_HPP_ */
