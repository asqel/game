local interact = require("/interact")
local obj = require("obj")
local sprite = require("sprite")



obj.register("air", 0, nil, 0)
obj.register("grass", sprite.get_id("grass"), nil, 0)
obj.register("tree", sprite.get_id("tree"), interact.fridge, 1)
obj.register2("player_house", sprite.get_id("player_house"), nil, -0.7, 0, 2.3, 1)
obj.register2("fridge", sprite.get_id("fridge"), nil, 0.3, 0.1, 0.4, 1)

return {}
