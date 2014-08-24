Joystick = extends Sprite {
	__construct = function(){
		super()
		
		@resAnim = res.getResAnim("joystick")
		@alpha = 0.5
		
		@addEventListener(TouchEvent.TOUCH_DOWN, @onEvent.bind(this))
		@addEventListener(TouchEvent.TOUCH_UP, @onEvent.bind(this))
		@addEventListener(TouchEvent.MOVE, @onEvent.bind(this))
		
		@finger = Sprite().attrs {
			resAnim = res.getResAnim("finger"),
			parent = this,
			visible = false,
			anchor = vec2(0.5f, 0.5f),
			inputEnabled = false,
		}
		@pressed = false
		@dir = vec2(0, 0)
	},
		
	onEvent = function(ev){
		// print "Joystick.onEvent"
		var te = ev as TouchEvent || throw "TouchEvent required"

		//if player touched down
		if(te.type == TouchEvent.TOUCH_DOWN){
			@finger.visible = true
			@color = Color(0.7, 0, 0)
			@pressed = true
		}

		//if player touched up
		if(te.type == TouchEvent.TOUCH_UP){
			@finger.visible = false
			@color = Color(1, 1, 1)
			@pressed = false
		}

		if(te.type == TouchEvent.MOVE){
		
		}

		var center = @size / 2
		@dir = te.localPosition - center

		if(#@dir > 100)
			@dir.normalizeTo(100)

		@finger.pos = center + @dir

		if(!@pressed){
			@dir = vec2(0, 0)
		}
	}
}
