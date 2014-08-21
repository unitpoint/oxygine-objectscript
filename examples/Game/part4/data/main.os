print "--"
print "[start] ${DateTime.now()}"

require "std.os"
require "mathlib.os"

print "root.x: ${root.x}"

var actor = Sprite()
actor.setResAnimTest("sky")
actor.anchor = vec2(0.5, 0.1)
actor.x = -150
actor.y = 100
actor.width = 250
actor.height = 100
// actor.priority = -1
print "actor.x: ${actor.x}, actor.anchor: ${actor.anchor}"

root.addChild(actor)
