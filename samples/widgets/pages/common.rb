#Encoding: UTF-8


class CommonPage < WX::Panel
  def initialize(parent,*)
    super

    create_content
  end

  def with_text(ctrl, id = nil)
    WX::BoxSizer.new {|siz|
      siz.add(ctrl)
      text = WX::TextCtrl.new(self, :id => id, :process_enter => true)
      yield text if block_given?
      siz.add(text)
    }
  end

  def add_label_text(label, textid = nil)
    btn = WX::StaticText.new(self, :label => label)
    return with_text(btn, textid)
  end

  def add_button_text(label, buttonid = nil, textid = nil)
    btn = WX::Button.new(self, :label => label, :id => buttonid)
    return with_text(btn, textid) {|text|
      btn.bind(:button) {|evt| yield evt, text } if block_given?
    }
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
