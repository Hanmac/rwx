#Encoding: UTF-8
require "../../ext/rwx"

require_relative "pages/common"

require_relative "pages/colorpicker"

class A < WX::App
  
  def on_init
  
    @frame = WX::Frame.new(nil,
      :icon => "../sample.xpm",:title => "widgets demo"
    )
    
    image_list = ["../sample.xpm"]
    image_list << "icons/clrpicker.xpm"
    
    
    book = WX::TreeBookCtrl.new(@frame,:image_list => image_list)
    book.add_page(nil,"Picker",false,0)
    
    book.add_sub_page(ColorPickerPage,"ColorPicker",false,1)
    
    @frame.show
  end
end

A.new.main_loop
