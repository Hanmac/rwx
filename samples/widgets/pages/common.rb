#Encoding: UTF-8


class CommonPage < WX::Panel
  def initialize(parent,*)
    super
    
    create_content
  end
  
  
  def add_checkbox(sizer,label,id = nil)
    cb = WX::CheckBox.new(self,:label => label, :id => id)
    sizer.add(cb)
    return cb
  end
end