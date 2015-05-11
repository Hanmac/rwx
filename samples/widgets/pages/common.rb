#Encoding: UTF-8


class CommonPage < WX::Panel
  def initialize(parent,*)
    super
    
    create_content
    layout
  end
  
  
  def add_checkbox(sizer,label,id = nil)
    cb = WX::CheckBox.new(self,:label => label, :id => id)
    sizer.add(cb)
    return cb
  end
  
  def add_button(sizer, label, id = nil, &block)
    cb = WX::Button.new(self,:label => label, :id => id)
    sizer.add(cb)
    cb.bind(:button, &block) unless block.nil?
    return cb    
  end
end