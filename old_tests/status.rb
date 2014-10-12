#Encoding: UTF-8
require "../ext/rwx"

STDERR.reopen("file.log","w")

class MyStatusBar < WX::StatusBar
	def initialize(*)
		super
		self.fields_count = 4
		self.set_status_width(0,150)
		self.set_status_width(1,32)
		p self.get_status_width(0)
		p self.get_status_width(1)
		@check = WX::CheckBox.new(self,:label => "&Toggle clock" )
		@check.rect = self.get_field_rect(0)
		@check.bind(:checkbox_clicked) {|e|@bitmap.bitmap = e.checked? ? "green.xpm" : "red.xpm" } 
		@bitmap = WX::StaticBitmap.new(self,:bitmap => "red.xpm" )
		@bitmap.position = self.get_field_rect(1).position
		refresh
	end
end

class A < WX::App
	def on_init

		@frame = WX::Frame.new(nil)

		@frame.sizer = WX::BoxSizer.new {|siz|

			siz.add(WX::Button.new(@frame,:id => :ok) {|button|
				button.bind(:button_clicked) {|e|
					@frame.statusbar.fields_count = 2
					@frame.statusbar.each_statuspane.with_index {|pane,i| pane.text = "ABC#{i}" } 
				}
			})


		}
		@frame.statusbar = MyStatusBar.new(@frame) 
		@frame.show
		
	end
end
	
A.new.main_loop
