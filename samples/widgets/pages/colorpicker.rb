#Encoding: UTF-8


class ColorPickerPage < CommonPage
  def create_boxleft
    WX::BoxSizer.new(:vertical) {|boxleft|
      boxleft.add(WX::StaticBox.new(self, :label => "&ColourPicker style").containing_sizer.tap {|box|
        box.orientation = :vertical
          
        @textctrl = add_checkbox(box,"With textctrl")
        @withLabel = add_checkbox(box,"With label")
        
        reset_boxes
      }, :expand => true)
      boxleft.add(WX::Button.new(self,:id => :reset,:label => "&Reset") {|but|
        but.bind(:button) {
          reset_boxes
          recreate_picker
        }
      },:align => :center)
      
    }
  end
  
  def create_boxright
    WX::BoxSizer.new(:vertical) {|boxright|
      boxright.add_stretch_spacer(5)
      boxright.add(create_picker,:align => :center)  
      boxright.add_stretch_spacer(5)
    }
  end
  
  def create_content
    self.sizer = WX::BoxSizer.new {|siz|
    
      siz.add(create_boxleft, :expand => true)
      
      siz.add(@ctrl_sizer = create_boxright, :expand => true)
      
    }
    
    bind(:checkbox) {recreate_picker}
  end
  
  def get_style
    style = 0
    style |= WX::ColorPicker::USE_TEXTCTRL if @textctrl.value
    style |= WX::ColorPicker::SHOW_LABEL if @withLabel.value
    return style
  end
  
  def reset_boxes
    default = WX::ColorPicker::DEFAULT_STYLE
    @textctrl.value = (default & WX::ColorPicker::USE_TEXTCTRL) != 0
    @withLabel.value = (default & WX::ColorPicker::SHOW_LABEL) != 0
  end
  
  def create_picker
    @picker = WX::ColorPicker.new(self,:style => get_style)
  end
  
  def recreate_picker
    
    @ctrl_sizer.remove(1)
    
    @picker.destroy
    
    @ctrl_sizer.insert(1,create_picker,:align => :center)
    
    @ctrl_sizer.layout
  end
end
