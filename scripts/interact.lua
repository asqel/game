interact = {}

function interact.tree(x, y)
	open_dialogue("tree ?", nil, "tree.interact" .. math.random(1, 4))
end

return interact
