local ent = require("entity")
local sprite = require("sprite")

function func(x)
end

ent.register("test", sprite.get_id("red"), func, 1, 1, 1, 1, 1)

return {}
