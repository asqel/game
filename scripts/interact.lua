local game = require("game")
interact = {}

function interact.tree(x, y)
	game.gui.open_dialogue("tree ?", nil, "tree.interact" .. math.random(1, 4))
end

function interact.fridge(x, y)
	function on_tick()
		if game.act.get(game.act.INTERACT) ~= 0 then
			game.gui.close()
		end
	end
	function on_render()
		game.draw.text("")
	end
	function on_end()
	end
	game.gui.open(nil, on_tick, on_render, on_end)
end

return interact
