local interact = require("/interact")
local obj = require("obj")
local sprite = require("sprite")



obj.register("air", 0, 0)
obj.register("grass", sprite.get_id("grass"), 0)
obj.register("tree", sprite.get_id("tree"), 1)
obj.register("player_house", sprite.get_id("player_house"), 1)
--obj.register2("fridge", sprite.get_id("fridge"), 0.3, 0.1, 0.4, 0.9)
obj.register2("fridge", sprite.get_id("fridge"), 0, 0, 1, 1)

return {}
