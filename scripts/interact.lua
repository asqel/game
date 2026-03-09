local game = require("game")
local ent = require("entity")
interact = {}

function interact.tree(x, y)
	game.gui.open_dialogue("tree ?", nil, "tree.interact" .. math.random(1, 4))
	ent.add(0, 10, 10)
end

return interact
