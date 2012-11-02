#Encoding: UTF-8
require "../ext/rwx"

STDERR.reopen("file.log","w")

class A < WX::App
	def on_init
		@frame = WX::Frame.new(nil)
		@frame.title = "hallo"

		@proc = proc {|e| p e,e.id, e.event_type,e.event_object}
		@frame.bind(:button_clicked,&@proc)
		@frame.bind(:checkbox_clicked,&@proc)

		@frame.bind(:filepicker_changed){|e| p e.path }
		#@frame.bind(:paint,&@proc)

		@frame.sizer = WX::BoxSizer.new {|siz|


			siz.add(WX::Button.new(@frame,id: :ok) {|button|
				button.bind(:button_clicked) {p "button clicked"}
			})
			siz.add(WX::CheckBox.new(@frame,label: "checkbox" ) {|checkbox|
				checkbox.bind(:checkbox_clicked) {|e| p "checkbox #{e.checked? ? '' : 'un'}checked" }
			})

			siz.add(WX::Button.new(@frame,id: :cancel) {|button|
				button.bind(:button_clicked) {button.call(:filepicker_changed) {|e| e.path="abc"}}
			})
			siz.add(WX::CheckBox.new(@frame,id: :open,label: "checkbox" ))


		}
		@frame.show
		
	end
end
	
A.new.main_loop
