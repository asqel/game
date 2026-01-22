local game = {}

game.sprite = {}
game.sprite.PAUSED = SPRITE_PAUSED
game.sprite.LOOP = LOOP
game.sprite.register = register_sprite
game.sprite.get_id = get_sprite_id

game.obj = {}
game.obj.register = register_obj
game.obj.register2 = register_obj2

game.gui = {}
game.gui.open = open_gui
game.gui.open_dialogue = open_dialogue
game.gui.close = close_gui

game.draw = {}
game.draw.text = draw_text

game.act = {}
game.act.NONE = GAME_ACT_NONE
game.act.UP = GAME_ACT_UP
game.act.RIGHT = GAME_ACT_RIGHT
game.act.DOWN = GAME_ACT_DOWN
game.act.LEFT = GAME_ACT_LEFT
game.act.INTERACT = GAME_ACT_INTERACT
game.act.CLOSE = GAME_ACT_CLOSE
game.act.EDIT_UP = GAME_ACT_EDIT_UP
game.act.EDIT_RIGHT = GAME_ACT_EDIT_RIGHT
game.act.EDIT_DOWN = GAME_ACT_EDIT_DOWN
game.act.EDIT_LEFT = GAME_ACT_EDIT_LEFT
game.act.EDIT_PLACE = GAME_ACT_EDIT_PLACE
game.act.EDIT_REMOVE = GAME_ACT_EDIT_REMOVE
game.act.EDIT_FILL = GAME_ACT_EDIT_FILL
game.act.EDIT_EMPTY = GAME_ACT_EDIT_EMPTY
game.act.EDIT_SAVE = GAME_ACT_EDIT_SAVE
game.act.set = set_action
game.act.get = get_action

game.time = time

return game
