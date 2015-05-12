#Encoding: UTF-8

class ActivityIndicatorPage < CommonPage
  def create_boxleft
    add_statbox("&Operations", :vertical) {|box|
      @startbutton = add_button(box,"&Start") { @widget.start }
      @stopbutton = add_button(box,"&Stop") { @widget.stop }
    }
  end

  def create_widget
    @widget = WX::ActivityIndicator.new(self)
  end
end
