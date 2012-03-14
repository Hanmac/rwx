##Encoding: UTF-8
require "../ext/rwx"

#STDERR.reopen("file","w")

class A < WX::App
	def on_init
	
		#f = WX::Font.new(10,:script)
		#p f.faceName
		@frame = WX::Frame.new(nil)
		@frame.title = "hallo"

		#p @frame.icon
		@history = WX::FileHistory.new
		@history << "b" << "a"
		#f= @frame.font
		#p f.weight

		#c = WX::Control.new
		#g = WX::Gauge.new(@frame)
		#g = WX::Gauge.allocate
		#g.range = 10
		#g.value = 5
		#@history.save
		#g.foregroundColour = WX::Color.new(255,0,0)
		# g.foregroundColour
		#g.pulse
		#g.pulse
		@frame.menuBar = WX::Menu::Bar.new(nil) {|m|
			m.append("Datei") {|menu|
				menu << :new
				menu.appendNormal(:open) {
					p WX.load_dialog("file there","txt")
				}
				menu.appendNormal("last files") {|o| @history >> o }
				menu << 5003
				menu.appendNormal(:about) {|e|
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
		}
		#@frame.menuBar = m
		
		#i=WX::Image.new("new16x16.png")
		t = @frame.createToolBar

		#t.addNormal(WX::TimePickerCtrl)
		#t.addNormal(WX::RadioButton) {|g| g.label_text = "abc" }
		t.addNormal(WX::ColorPicker){|g|
			g.bind(:colorpicker_changed) {|e| p e.color }
		}
		t.addNormal(WX::FontPicker) {|g|
		}
		t.addNormal(WX::StaticBitmap) {|g|
			g.bitmap = "new16x16.png"
		}

		t.addNormal(WX::HyperLink) {|g|
			g.label =  "abc"
			g.url = "http://google.de"
		}
		#time	= Time.now
		#c=WX::TimePickerCtrl.new(@frame)
		#c.value = time
		#p time, c.value
		
		#c=WX::RadioButton.new(@frame)
		#c.label_text = "abc"
#		t.addNormal(WX::TimePickerCtrl.new(t))
		
		t.addNormal(:new,nil) {
			#@t = WX::Timer.new(nil) { p "a" }
			#@t.start(1000)
			WX::ProgressDialog.new(nil,:title=>"title") { |pd|p 1; pd.update(50,"message") }
			#@p.update(50,"message")
			#@p.pulse("pulseing")
			
			#WX.busy {
			#	gets
			#}
		}
		t.addNormal(-1,nil,"new16x16.png"){
			#p WX.dir_dialog
			d=WX::DirDialog.new(@frame)
			#d.path = ".."
			d.show_modal
			#p d.path
			
			#cdlg=WX::ColorDialog.new(@frame)
			#cdlg.custom_colors = [WX::Color.new(255,0,0)]
			#cdlg.show_modal
		}
		t.addRadio(5003,nil,"new16x16.png")
		t.addRadio(5003,nil,"new16x16.png")
		
		
		#p t.each_tool.to_a
		t.realize
		s = @frame.createStatusBar
		
		#WX.show_tip(nil,["abc","def"],0)
		
		tc = WX::TreeCtrl.new(@frame)
		tc.root("abc",-1,-1) {|item1|
			item1.append("def") {|item2|
				item2.append("ghi")
			}
		}
		@frame.show
		
		#p @frame.each.to_
	end
end

A.new.main_loop
