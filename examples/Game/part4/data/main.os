print "--"
print "[start] ${DateTime.now()}"

require "std.os"
require "mathlib.os"

// Sprite()
// return;

root.addEventListener(RootActor.ACTIVATE, function(){
	print "RootActor.ACTIVATE"
})

root.addEventListener(RootActor.DEACTIVATE, function(){
	print "RootActor.DEACTIVATE"
})

print "root.x: ${root.x}"

MainMenuScene.instance.show()
// GameScene.instance.show()
return;

var actor = Sprite()
// actor.setResAnimTest("sky")
// actor.setResAnim(res.getResAnim("sky"))
actor.anchor = vec2(0.5, 0.1)
actor.x = -150
actor.y = 100
actor.width = 250
actor.height = 100
// actor.priority = -1
print "actor.x: ${actor.x}, actor.anchor: ${actor.anchor}"
print "actor == root: ${actor == root}"

root.addChild(actor)

actor.addEventListener(TouchEvent.CLICK, function(ev){
	print "CLICK: ${ev}"
	ev.stopImmediatePropagation()
})

GameScene = extends Scene {
	__construct = function(){
		@view = Sprite().attrs {
			resAnim = res.getResAnim("sky"),
			anchor = vec2(0.5, 0.1),
			x = 250,
			y = 200,
			width = 250,
			height = 100,
		}
	},
}

var scene = Scene()
print "scene: ${scene}"

var gameScene = GameScene()
print "gameScene: ${gameScene}"
gameScene.show()
