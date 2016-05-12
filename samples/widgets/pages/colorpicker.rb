#Encoding: UTF-8


class ColorPickerPage < CommonPage
  def create_boxleft
    WX::BoxSizer.new(:vertical) {|boxleft|
      boxleft.add(
        add_statbox("&ColourPicker style", :vertical) {|box|
          @textctrl = add_checkbox(box,"With textctrl")
          @withLabel = add_checkbox(box,"With label")
          if WX::ColorPicker::const_defined?(:SHOW_ALPHA)
            @withAlpha = add_checkbox(box,"With alpha")
          end

          reset_boxes
        },
        :expand => true
      )
      p boxleft.orientation
      p boxleft.vertical?
      add_button(boxleft, "&Reset", :reset, :align => :center_horizontal) {
        reset_boxes
        recreate_widget
      }

      bind(:checkbox) {recreate_widget}

    }
  end
  
  def get_style
    style = 0
    style |= WX::ColorPicker::USE_TEXTCTRL if @textctrl.value
    style |= WX::ColorPicker::SHOW_LABEL if @withLabel.value
    if WX::ColorPicker::const_defined?(:SHOW_ALPHA)
      style |= WX::ColorPicker::SHOW_ALPHA if @withAlpha.value
    end

    return style
  end
  
  def reset_boxes
    default = WX::ColorPicker::DEFAULT_STYLE
    @textctrl.value = (default & WX::ColorPicker::USE_TEXTCTRL) != 0
    @withLabel.value = (default & WX::ColorPicker::SHOW_LABEL) != 0
    if WX::ColorPicker::const_defined?(:SHOW_ALPHA)
      @withAlpha.value = (default & WX::ColorPicker::SHOW_ALPHA) != 0
    end
  end
  
  def create_widget
    @picker = WX::ColorPicker.new(self,:style => get_style)
  end
  
  def recreate_widget

    @ctrl_sizer.remove(1)

    @picker.destroy

    @ctrl_sizer.insert(1,create_widget,:align => :center)

    @ctrl_sizer.layout

  end
end
