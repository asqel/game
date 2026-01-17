
function tree_interact(x, y)
	open_dialogue("tree ?", nil, "tree.interact" .. math.random(1, 4))
end


register_obj("air", 0, nil, 0)
register_obj("grass", get_sprite_id("grass"), nil, 0)
register_obj("tree", get_sprite_id("tree"), tree_interact, 1)
register_obj2("player_house", get_sprite_id("player_house"), nil, -0.7, 0, 2.3, 1)
register_obj2("fridge", get_sprite_id("fridge"), nil, 0.3, 0.1, 0.4, 1)
