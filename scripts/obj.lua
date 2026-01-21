interact = require("/interact")


register_obj("air", 0, nil, 0)
register_obj("grass", get_sprite_id("grass"), nil, 0)
register_obj("tree", get_sprite_id("tree"), interact.tree, 1)
register_obj2("player_house", get_sprite_id("player_house"), nil, -0.7, 0, 2.3, 1)
register_obj2("fridge", get_sprite_id("fridge"), nil, 0.3, 0.1, 0.4, 1)

return {}
