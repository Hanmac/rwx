#Encoding: UTF-8

class ActivityIndicatorPage < CommonPage
  def create_boxleft
    WX::StaticBox.new(self, :label => "&Operations").containing_sizer.tap {|box|
        box.orientation = :vertical

        @startbutton = add_button(box,"&Start") { @widget.start }
        @stopbutton = add_button(box,"&Stop") { @widget.stop }

      }
  end

  def create_boxright
    WX::BoxSizer.new(:vertical) {|boxright|
      boxright.add_stretch_spacer(5)
      boxright.add(create_widget,:align => :center)
      boxright.add_stretch_spacer(5)
    }
  end

  def create_content
    self.sizer = WX::BoxSizer.new {|siz|

      siz.add(create_boxleft, :expand => true)

      siz.add(create_boxright, :expand => true)

    }

  end

  def create_widget
    @widget = WX::ActivityIndicator.new(self)
  end
end