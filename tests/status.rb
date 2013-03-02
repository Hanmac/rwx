#Encoding: UTF-8
require "../ext/rwx"

STDERR.reopen("file.log","w")

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
		@frame.create_statusbar
		@frame.show
		
	end
end
	
A.new.main_loop
