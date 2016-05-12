#Encoding: UTF-8


class FileCtrlPage < CommonPage

  def create_left_radioboxes(boxleft)
    @klass = add_radiobox(boxleft, "wxFileCtrl implementation", ["Native", "Generic"]) {recreate_widget}
    @klass.set_item_enabled(0, false) if WX::FileCtrl == WX::FileCtrlGeneric
    @mode = add_radiobox(boxleft, "wxFileCtrl mode", ["Open", "Save"]) {recreate_widget}
  end

  def create_left_buttons(boxleft)
    boxleft.add(add_button_text("Set &directory") {|evt, txt| @ctrl.directory = txt.value } )
    boxleft.add(add_button_text("Set &path") {|evt, txt| @ctrl.path = txt.value } )
    boxleft.add(add_button_text("Set &filename") {|evt, txt| @ctrl.filename = txt.value } )
  end

  def create_left_statboxes(boxleft)
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
  end

  def create_boxleft
    WX::BoxSizer.new(:vertical) {|boxleft|
      create_left_radioboxes(boxleft)
      create_left_buttons(boxleft)
      create_left_statboxes(boxleft)

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
    return data.values_at(*@flags.checked_items).inject(style, :|)
  end

  def reset_boxes
    @mode.selection = 0
    @flags.check_all(false)
    @filter.check_all(false)
  end

  def wildcard
    @filter.checked_strings
  end

  def create_widget
    klass = [WX::FileCtrl,WX::FileCtrlGeneric][@klass.selection]
    ctrl = klass.new(self, :style => get_style, :default_wildcard => wildcard, :default_directory => ".")
    return ctrl
  end
end
