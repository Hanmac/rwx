#Encoding: UTF-8
require "../../ext/rwx"

require_relative "pages/common"

require_relative "pages/colorpicker"
require_relative "pages/activityindicator"

class Widgets < WX::App
  
  def on_init
  
    @frame = WX::Frame.new(nil,
      :icon => "../sample.xpm",:title => "widgets demo"
    )
    
    image_list = []
    
    image_list << WX::Image.new("../sample.xpm").scale([16,16]) 
    image_list << "icons/clrpicker.xpm"
    image_list << "icons/activityindicator.xpm"
    
    
    book = WX::TreeBookCtrl.new(@frame,:image_list => image_list)
    book.add_page(nil,"Picker",false,0)
    
    book.add_sub_page(ColorPickerPage,"ColorPicker",false,1)
    
    book.add_page(nil,"Native",false,0)
    
    book.add_sub_page(ActivityIndicatorPage,"ActivityIndicator",false,2)
    
    @frame.layout
    
    @frame.show
  end
end

Widgets.new.main_loop
