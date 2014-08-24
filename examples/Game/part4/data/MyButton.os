MyButton = extends Sprite {
	__construct = function(){
		super()
		var self = this
		@addEventListener(TouchEvent.TOUCH_DOWN, {|ev| self.onTouchDown(ev) })
		@addEventListener(TouchEvent.TOUCH_UP, {|ev| self.onTouchUp(ev) })
		@addEventListener(TouchEvent.CLICK, {|ev| self.onClick(ev) })
	},
	
	onTouchDown = function(ev){
		print "onTouchDown"
		@color = Color(1, 0, 0)
	},
	
	onTouchUp = function(ev){
		print "onTouchUp"
		@color = Color(1, 1, 1)
	},
	
	onClick = function(ev){
		print "onClick"
		@scale = 1
		@addTween("scale", 0.9, 300, 1, true)
	},
}
