--[[

register_obj(name, sprite_id, interact | nil, has_hitbox(0 || 1))
register_obj2(name, sprite_id, interact | nil, hitx, hity, hitw, hith)

]]
register_obj("air", 0, nil, 0)
register_obj("grass", 0, nil, 0)
register_obj("tree", 1, nil, 1)
register_obj2("player_house", 3, nil, -0.7, 0, 2.3, 1)
register_obj2("fridge", 4, nil, 0, 0, 0.4, 0.4)
