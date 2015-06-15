#Encoding: UTF-8


class FileCtrlPage < CommonPage
  def create_boxleft
    WX::BoxSizer.new(:vertical) {|boxleft|
      @mode = add_radiobox(boxleft, "wxFileCtrl mode", ["Open", "Save"]) {recreate_widget}

      boxleft.add(add_button_text("Set &directory") {|evt, txt| @ctrl.directory = txt.value } )
      boxleft.add(add_button_text("Set &path") {|evt, txt| @ctrl.path = txt.value } )
      boxleft.add(add_button_text("Set &filename") {|evt, txt| @ctrl.filename = txt.value } )
              
      boxleft.add(add_statbox("&Flags", :vertical) {|statbox|
        @flags = add_checklist(statbox, ["wxFC_MULTIPLE", "wxFC_NOSHOWHIDDEN"]) {recreate_widget}
      })
  
      boxleft.add(add_statbox("&Filter", :vertical) {|statbox|
        @filter = add_checklist(statbox, [
          "all files (%<v>s)|%<v>s" % {v: WX::FileCtrl::DEFAULT_WILDCARD},
          "C++ files (*.cpp; *.h)|*.cpp;*.h",
          "PNG images (*.png)|*.png"
        ]) {recreate_widget}
      })

      add_button(boxleft, "&Reset", :reset, :align => :center_horizontal) {
        reset_boxes
        recreate_widget
      }
      
      
      bind(:checkbox) {recreate_widget}
      
    }
  end
  
  def get_style
    style = [WX::FileCtrl::OPEN, WX::FileCtrl::SAVE][@mode.selection]
    data = [WX::FileCtrl::MULTIPLE, WX::FileCtrl::NOSHOWHIDDEN]
    @flags.each_checked {|f| style |= data[f] }
    return style
  end
  
  def reset_boxes
    @mode.selection = 0
    @flags.check_all(false)
    @filter.check_all(false)
  end
  
  def wildcard
    @filter.checked_items.map {|i| @filter.get_item_string(i) }
  end
  
  def create_widget
    ctrl = WX::FileCtrl.new(self,:style => get_style, :default_wildcard => wildcard)
    return ctrl
  end
  
  
end
