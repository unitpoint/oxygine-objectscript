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

print "root.size: ${root.size}"

MainMenuScene.instance.show()
// GameScene.instance.show()
