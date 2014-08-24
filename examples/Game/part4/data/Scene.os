Scene = extends EventDispatcher {
	__construct = function(){
		@view = Actor()
		@view.size = root.size
		print "${@classname}.view.size: ${@view.size}"
	},
	
	changeScene = function(next){
		@hide()
		next.show()
	},
	
	show = function(){
		root.addChild(@view)
		@view.alpha = 0
		@view.addTween("alpha", 255, 1000)
	},
	
	hide = function(){
		var self = this
		var tween = @view.addTween("alpha", 0, 1000)
		tween.detachActor = true
		tween.addDoneCallback {||
			self.dispatchEvent{"hidden"}
		}		
	},
}
