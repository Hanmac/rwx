/*
 * rwx.cpp
 *
 *  Created on: 07.02.2012
 *      Author: hanmac
 */

#include "wxError.hpp"

#include "wxSize.hpp"
#include "wxRect.hpp"
#include "wxPoint.hpp"

#include "wxColor.hpp"
#include "wxFont.hpp"
#include "wxPen.hpp"
#include "wxBrush.hpp"

#include "wxCursor.hpp"

#include "wxImage.hpp"
#include "wxBitmap.hpp"
#include "wxImageList.hpp"

#include "wxEvent.hpp"
#include "wxCommandEvent.hpp"
#include "wxNotifyEvent.hpp"
#include "wxScrollEvent.hpp"
#include "wxKeyEvent.hpp"
#include "wxMouseEvent.hpp"
#include "wxPaintEvent.hpp"

#include "wxEvtHandler.hpp"

#include "wxMenu.hpp"
#include "wxMenuItem.hpp"

#include "wxToolBarTool.hpp"

#include "wxApp.hpp"
#include "wxWindow.hpp"

#include "wxMenuBar.hpp"
#include "wxPanel.hpp"
#include "wxControl.hpp"
#include "wxToplevel.hpp"


#include "wxFrame.hpp"
#include "wxDialog.hpp"

#include "wxColorDialog.hpp"
#include "wxFontDialog.hpp"

#include "wxFileDialog.hpp"
#include "wxDirDialog.hpp"
#include "wxNumberEntryDialog.hpp"
#include "wxTextEntryDialog.hpp"
#include "wxPasswordEntryDialog.hpp"

#include "wxProgressDialog.hpp"

#include "wxMessageDialogBase.hpp"
#include "wxMessageDialog.hpp"
#include "wxMessageDialogGeneric.hpp"

#include "wxRichMessageDialog.hpp"

#include "wxWizard.hpp"

#include "wxAnyChoiceDialog.hpp"
#include "wxSingleChoiceDialog.hpp"
#include "wxMultiChoiceDialog.hpp"

#include "wxAui.hpp"

#include "wxAnyButton.hpp"
#include "wxButton.hpp"
#include "wxBitmapButton.hpp"
#include "wxContextHelpButton.hpp"
#include "wxToggleButton.hpp"
#include "wxBitmapToggleButton.hpp"


#include "wxToolBarBase.hpp"
#include "wxToolBar.hpp"
#include "wxButtonToolBar.hpp"

#include "wxStatusBar.hpp"

#include "wxGauge.hpp"

#include "wxListBox.hpp"
#include "wxCheckListBox.hpp"

#include "wxRearrangeList.hpp"
#include "wxRearrangeCtrl.hpp"
#include "wxRearrangeDialog.hpp"

#include "wxCheckBox.hpp"
#include "wxRadioButton.hpp"
#include "wxRadioBox.hpp"

#include "wxStaticBitmap.hpp"
#include "wxStaticBitmapGeneric.hpp"

#include "wxStaticLine.hpp"

#include "wxStaticText.hpp"
#include "wxStaticBox.hpp"

#include "wxItemContainer.hpp"

#include "wxChoice.hpp"
#include "wxComboBox.hpp"
#include "wxComboCtrl.hpp"
#include "wxOwnerDrawnComboBox.hpp"
#include "wxBitmapComboBox.hpp"

#include "wxScrollBar.hpp"
#include "wxSlider.hpp"

#include "wxSpinButton.hpp"
#include "wxSpinCtrl.hpp"
#include "wxSpinCtrlDouble.hpp"

#include "wxTextArea.hpp"
#include "wxTextEntry.hpp"

#include "wxTextCtrl.hpp"
#include "wxTextAttr.hpp"
#include "wxSearchCtrl.hpp"


#include "wxPickerBase.hpp"
#include "wxTimePicker.hpp"
#include "wxFontPicker.hpp"
#include "wxColorPicker.hpp"
#include "wxFileDirPicker.hpp"
#include "wxFilePicker.hpp"
#include "wxDirPicker.hpp"
#include "wxDatePicker.hpp"

#include "wxFilePickerWidget.hpp"
#include "wxDirPickerWidget.hpp"
#include "wxFontPickerWidget.hpp"
#include "wxColorPickerWidget.hpp"

#include "wxTreeCtrl.hpp"

#include "wxFileCtrlBase.hpp"
#include "wxFileCtrl.hpp"
#include "wxFileCtrlGeneric.hpp"
#include "wxDirCtrl.hpp"

#include "wxCalendarCtrlBase.hpp"
#include "wxCalendarCtrl.hpp"
#include "wxCalendarCtrlGeneric.hpp"

#include "wxInfoBarBase.hpp"
#include "wxInfoBar.hpp"
#include "wxInfoBarGeneric.hpp"


#include "wxDataView.hpp"
#include "wxDataViewList.hpp"

#include "wxHyperLinkBase.hpp"
#include "wxHyperLink.hpp"
#include "wxHyperLinkGeneric.hpp"
#include "wxCollapsiblePane.hpp"
#include "wxCollapsibleHeaderCtrl.hpp"

#include "wxBookCtrl.hpp"
#include "wxChoiceBook.hpp"
#include "wxNoteBook.hpp"
#include "wxSimpleBook.hpp"
#include "wxListBook.hpp"
#include "wxToolBook.hpp"
#include "wxAuiBook.hpp"
#include "wxTreeBook.hpp"

#include "wxRichTextCtrl.hpp"
#include "wxRichTextAttr.hpp"
#include "wxRichTextStyle.hpp"
#include "wxRichTextStyleSheet.hpp"

#include "wxActivityIndicator.hpp"

#include "wxSTC.hpp"
#include "wxXRC.hpp"

#include "wxAboutDlg.hpp"
#include "wxStartUpTip.hpp"

#include "wxFileHistory.hpp"
#include "wxConfig.hpp"

#include "wxDC.hpp"
#include "wxSizer.hpp"
#include "wxSizerItem.hpp"

#include "wxPalette.hpp"

#include "wxGridTable.hpp"
#include "wxGridCellRenderer.hpp"
#include "wxGridCellAttr.hpp"

#include "wxPropertyGrid.hpp"
#include "wxPropertyGridPage.hpp"
#include "wxPropertyGridManager.hpp"
#include "wxPropertyGridInterface.hpp"

#include "wxProperty.hpp"

#include "wxPropertyCell.hpp"

#include "wxPreferences.hpp"

#include "wxAddRemoveCtrl.hpp"

#include <wx/apptrait.h>

VALUE rb_mWX;

void rb_define_attr_method_base(VALUE klass,const std::string& name,VALUE(get)(ANYARGS),VALUE(set)(ANYARGS), bool missing = false)
{
    /* let's get this to compile: macro expects a constant */
	if(get && missing)
		rb_define_method(klass,name.c_str(),RUBY_METHOD_FUNC(get), -1);
    else if (get && ! missing)
		rb_define_method(klass,name.c_str(),RUBY_METHOD_FUNC(get), 0);    
	if(set && missing)
		rb_define_method(klass,(name + "=").c_str(),RUBY_METHOD_FUNC(set), -1);
    else if (set && ! missing)
		rb_define_method(klass,(name + "=").c_str(),RUBY_METHOD_FUNC(set), 1);

}

void rb_define_attr_method(VALUE klass,const std::string& name,VALUE(get)(VALUE),VALUE(set)(VALUE,VALUE))
{
	rb_define_attr_method_base(
		klass, name,
		RUBY_METHOD_FUNC(get), RUBY_METHOD_FUNC(set)
	);
}

void rb_define_attr_method_missing(VALUE klass,const std::string& name, bool get, bool set)
{
	rb_define_attr_method_base(
		klass, name,
		get ? RUBY_METHOD_FUNC(rb_f_notimplement) : NULL,
		set ? RUBY_METHOD_FUNC(rb_f_notimplement) : NULL,
		true
	);
}



/* Document-const: WXVERSION
 *   String version of wxwidgets
 */

/* Document-const: WXVERSION_MAYOR
 *   Integer of mayor of version of wxwidgets
 */

/* Document-const: WXVERSION_MINOR
 *   Integer of minor of version of wxwidgets
 */

extern "C" void Init_rwx()
{
	//wxInitialize();

	global_holder = rb_hash_new();
	rb_global_variable(&global_holder);

	rb_mWX = rb_define_module("WX");

	Init_WXEvtHandler(rb_mWX);

	Init_WXError(rb_mWX);

	Init_WXColor(rb_mWX);
	Init_WXPalette(rb_mWX);
	Init_WXFont(rb_mWX);
	Init_WXPen(rb_mWX);
	Init_WXBrush(rb_mWX);

	Init_WXImage(rb_mWX);
	Init_WXBitmap(rb_mWX);
	Init_WXImageList(rb_mWX);

	Init_WXCursor(rb_mWX);

	Init_WXSize(rb_mWX);
	Init_WXRect(rb_mWX);
	Init_WXPoint(rb_mWX);

	Init_WXEvent(rb_mWX);
	Init_WXCommandEvent(rb_mWX);
	Init_WXScrollEvent(rb_mWX);

	Init_WXNotifyEvent(rb_mWX);

	Init_WXKeyEvent(rb_mWX);
	Init_WXMouseEvent(rb_mWX);

	Init_WXPaintEvent(rb_mWX);

	Init_WXTimer(rb_mWX);
	Init_WXTaskBar(rb_mWX);

	Init_WXMenu(rb_mWX);
		Init_WXMenuItem(rb_mWX);

	Init_WXApp(rb_mWX);

	Init_WXWindow(rb_mWX);
	Init_WXPanel(rb_mWX);

	Init_WXTopLevel(rb_mWX);
	Init_WXFrame(rb_mWX);

	Init_WXDialog(rb_mWX);
	Init_WXColorDialog(rb_mWX);
	Init_WXFontDialog(rb_mWX);
	Init_WXFileDialog(rb_mWX);
	Init_WXDirDialog(rb_mWX);
	Init_WXNumberEntryDialog(rb_mWX);
	Init_WXTextEntryDialog(rb_mWX);
	Init_WXPasswordEntryDialog(rb_mWX);
	Init_WXProgressDialog(rb_mWX);
	Init_WXWizard(rb_mWX);
	Init_WXWizardPage(rb_mWX);

	Init_WXAnyChoiceDialog(rb_mWX);
	Init_WXSingleChoiceDialog(rb_mWX);
	Init_WXMultiChoiceDialog(rb_mWX);

	Init_WXMessageDialogBase(rb_mWX);
	Init_WXMessageDialog(rb_mWX);
	Init_WXMessageDialogGeneric(rb_mWX);

	Init_WXRichMessageDialog(rb_mWX);

	Init_WXMenuBar(rb_mWX);

	Init_WXControl(rb_mWX);
	Init_WXToolBarBase(rb_mWX);
	Init_WXToolBar(rb_mWX);
	Init_WXToolBarTool(rb_mWX);

	Init_WXButtonToolBar(rb_mWX);

	Init_WXPickerBase(rb_mWX);
	Init_WXTimePicker(rb_mWX);
	Init_WXColorPicker(rb_mWX);
	Init_WXFontPicker(rb_mWX);
	Init_WXFileDirPicker(rb_mWX);
	Init_WXFilePicker(rb_mWX);
	Init_WXDirPicker(rb_mWX);
	Init_WXDatePicker(rb_mWX);

	Init_WXFileCtrlBase(rb_mWX);
	Init_WXFileCtrl(rb_mWX);
	Init_WXFileCtrlGeneric(rb_mWX);
	Init_WXDirCtrl(rb_mWX);

	Init_WXCalendarCtrlBase(rb_mWX);
	Init_WXCalendarCtrl(rb_mWX);
	Init_WXCalendarCtrlGeneric(rb_mWX);

	Init_WXInfoBarBase(rb_mWX);
	Init_WXInfoBar(rb_mWX);
	Init_WXInfoBarGeneric(rb_mWX);

	Init_WXItemContainer(rb_mWX);

	Init_WXListBox(rb_mWX);
	Init_WXCheckListBox(rb_mWX);

	Init_WXRearrangeList(rb_mWX);
	Init_WXRearrangeCtrl(rb_mWX);
	Init_WXRearrangeDialog(rb_mWX);

	Init_WXDataView(rb_mWX);
	Init_WXDataViewList(rb_mWX);

	Init_WXScrollBar(rb_mWX);
	Init_WXSlider(rb_mWX);
	Init_WXSpinButton(rb_mWX);
	Init_WXSpinCtrl(rb_mWX);
	Init_WXSpinCtrlDouble(rb_mWX);

	Init_WXHyperLinkBase(rb_mWX);
	Init_WXHyperLink(rb_mWX);
	Init_WXHyperLinkGeneric(rb_mWX);

	Init_WXCollapsiblePane(rb_mWX);
	Init_WXCollapsibleHeaderCtrl(rb_mWX);

	Init_WXStaticBitmapBase(rb_mWX);
	Init_WXStaticBitmap(rb_mWX);
	Init_WXStaticBitmapGeneric(rb_mWX);

	Init_WXStaticLine(rb_mWX);
	Init_WXStaticText(rb_mWX);
	Init_WXStaticBox(rb_mWX);

	Init_WXAnyButton(rb_mWX);
	Init_WXButton(rb_mWX);
	Init_WXBitmapButton(rb_mWX);
	Init_WXContextHelpButton(rb_mWX);
	Init_WXToggleButton(rb_mWX);
	Init_WXBitmapToggleButton(rb_mWX);

	Init_WXCheckBox(rb_mWX);
	Init_WXRadioButton(rb_mWX);
	Init_WXRadioBox(rb_mWX);

	Init_WXFilePickerWidget(rb_mWX);
	Init_WXDirPickerWidget(rb_mWX);

	Init_WXFontPickerWidget(rb_mWX);
	Init_WXColorPickerWidget(rb_mWX);

	Init_WXTextArea(rb_mWX);
	Init_WXTextEntry(rb_mWX);

	Init_WXTextCtrl(rb_mWX);

	Init_WXTextAttr(rb_mWX);

	Init_WXSearchCtrl(rb_mWX);

	Init_WXChoice(rb_mWX);
	Init_WXComboBox(rb_mWX);
	Init_WXComboCtrl(rb_mWX);
	Init_WXOwnerDrawnComboBox(rb_mWX);
	Init_WXBitmapComboBox(rb_mWX);

	Init_WXTreeCtrl(rb_mWX);
	Init_WXStatusBar(rb_mWX);

	Init_WXGauge(rb_mWX);

	Init_WXAboutDlg(rb_mWX);
	Init_ShowTip(rb_mWX);

	Init_WXFileHistory(rb_mWX);
	Init_WXConfig(rb_mWX);

	Init_WXDC(rb_mWX);

	Init_WXSizer(rb_mWX);
	Init_WXBoxSizer(rb_mWX);

	Init_WXSizerItem(rb_mWX);

	Init_WXAuiManager(rb_mWX);
	Init_WXAuiPane(rb_mWX);

	Init_WXActivityIndicator(rb_mWX);

	Init_WXBookCtrl(rb_mWX);
	Init_WXChoiceBookCtrl(rb_mWX);
	Init_WXToolBookCtrl(rb_mWX);
	Init_WXNoteBookCtrl(rb_mWX);
	Init_WXSimpleBookCtrl(rb_mWX);
	Init_WXListBookCtrl(rb_mWX);
	Init_WXAuiNoteBookCtrl(rb_mWX);
	Init_WXTreeBookCtrl(rb_mWX);

	Init_WXAuiToolBar(rb_mWX);

	Init_WXSTC(rb_mWX);

	Init_WXGrid(rb_mWX);
	Init_WXGridTable(rb_mWX);
	Init_WXGridCellRenderer(rb_mWX);
	Init_WXGridCellAttr(rb_mWX);

	Init_WXProperty(rb_mWX);

	Init_WXPropertyGridInterface(rb_mWX);
	Init_WXPropertyGrid(rb_mWX);
	Init_WXPropertyGridManager(rb_mWX);
	Init_WXPropertyGridPage(rb_mWX);

	Init_WXPropertyCell(rb_mWX);

	Init_WXPreferences(rb_mWX);

	Init_WXRichTextCtrl(rb_mWX);
	Init_WXRichTextAttr(rb_mWX);
	Init_WXRichTextStyle(rb_mWX);
	Init_WXRichTextStyleSheet(rb_mWX);

	Init_WXXRC(rb_mWX);

	Init_WXAddRemoveCtrl(rb_mWX);

	rb_define_const(rb_mWX,"WXVERSION",wrap(wxString(wxVERSION_NUM_DOT_STRING)));

	rb_define_const(rb_mWX,"WXVERSION_MAYOR",wrap(wxMAJOR_VERSION));
	rb_define_const(rb_mWX,"WXVERSION_MINOR",wrap(wxMINOR_VERSION));
//	for(klassholdertype::iterator it = klassholder.begin();
//			it != klassholder.end();++it)
//		std::cout << it->first << std::endl;


	registerEnum("wxButtonFlag","button_flag",wxOK)
		->add(wxYES,"yes")
		->add(wxOK,"ok")
		->add(wxNO,"no")
		->add(wxYES_NO,"yes_no")
		->add(wxCANCEL,"cancel")
		->add(wxAPPLY,"apply")
		->add(wxCLOSE,"close")
		->add(wxHELP,"help");

	registerEnum("wxIconFlag","icon_flag",wxICON_NONE)
		->add(wxICON_EXCLAMATION,"exclamation")
		->add(wxICON_HAND,"hand")
		->add(wxICON_WARNING,"warning")
		->add(wxICON_ERROR,"error")
		->add(wxICON_QUESTION,"question")
		->add(wxICON_INFORMATION,"information")
		->add(wxICON_STOP,"stop")
		->add(wxICON_ASTERISK,"asterisk")
		->allow_array = false;

/*
	int min,max;
	wxString port(wxPlatformInfo::GetPortIdName(wxGUIAppTraits().GetToolkitVersion(&min,&max), false));

	rb_warn("%s %d.%d", port.c_str().AsChar(), min, max);
//*/

}
