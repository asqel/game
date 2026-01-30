local game = require("game")
interact = {}

function interact.tree(x, y)
	game.gui.open_dialogue("tree ?", nil, "tree.interact" .. math.random(1, 4))
end

return interact
