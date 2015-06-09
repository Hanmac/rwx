#Encoding: UTF-8


class CommonPage < WX::Panel
  def initialize(parent,*)
    super

    create_content
  end

  def add_checkbox(sizer,label,id = nil, value: false, &block)
    cb = WX::CheckBox.new(self,:label => label, :id => id, :value => value)
    sizer.add(cb)
    cb.bind(:checkbox, &block) unless block.nil?
    return cb
  end

  def add_button(sizer, label, id = nil, **opt, &block)
    cb = WX::Button.new(self,:label => label, :id => id)
    sizer.add(cb, **opt)
    cb.bind(:button, &block) unless block.nil?
    return cb
  end

  def add_statbox(label, orientation, &block)
    WX::StaticBox.new(
      self, :label => label, :orientation => orientation
    ).containing_sizer.tap(&block)
  end

  def create_content
    self.sizer = WX::BoxSizer.new {|siz|

      siz.add(create_boxleft, :expand => true)

      if respond_to?(:create_boxmiddle)
        siz.add(create_boxmiddle, :expand => true)
      end

      siz.add(@ctrl_sizer = create_boxright, :expand => true)

      siz.layout
    }

  end

  def create_boxright
    WX::BoxSizer.new(:vertical) {|boxright|
      boxright.add_stretch_spacer(5)
      boxright.add(create_widget,:align => :center)
      boxright.add_stretch_spacer(5)
    }
  end
end
