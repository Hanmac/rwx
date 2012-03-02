##Encoding: UTF-8
require "../ext/rwx"

class A < WX::App
	def on_init
	
		#f = WX::Font.new(10,:script)
		#p f.faceName
		@frame = WX::Frame.new
		@frame.title = "hallo"

		#f= @frame.font
		#p f.weight

		#c = WX::Control.new
		#g = WX::Gauge.new(@frame)
		#g = WX::Gauge.allocate
		#g.range = 10
		#g.value = 5
		
		#g.foregroundColour = WX::Color.new(255,0,0)
		# g.foregroundColour
		#g.pulse
		#g.pulse
		m = WX::Menu::Bar.new
		m.append("Datei") {|menu|
			menu << 5002 << 5000
			menu.appendNormal("last files") {|o|
				o.appendNormal(5051,"a")
				o.appendNormal(5052,"b")
			}
			menu << 5003
			menu.appendNormal(5014) {|e|
				WX.about_box(
					:icon => "ruby32x32.png",
					:developers => "Hanmac",
					:translators => "Quintus <sutniuq@gmx.net>",
					:artists => ["Tango project ( http://tango.freedesktop.org )",
					"Yukihiro Matsumoto ( http://www.rubyidentity.org )"],
					:copyright => "Copyright © 2010 OpenRubyRMK Team",
					:web_site => "http://devel.pegasus-alpha.eu/projects/openrubyrmk"
				)
			}
		}
		@frame.menuBar = m
		
		#i=WX::Image.new("new16x16.png")
		t = @frame.createToolBar
		t.addNormal(WX::Gauge) {|g|
			g.range = 10
			g.value = 5
		}
		t.addNormal(5002,nil,"new16x16.png")
		t.addNormal(-1,nil,"new16x16.png"){ p "yea"}
		t.addRadio(5003,nil,"new16x16.png")
		t.addRadio(5003,nil,"new16x16.png")
		
		
		#WX.about_box(:icon => "new16x16.png",:developers => "Hanmac")
		
		#p t.each_tool.to_a
		t.realize
		s = @frame.createStatusBar
		@frame.show
		
		#p @frame.each.to_a
		
	end
end

A.new.main_loop
