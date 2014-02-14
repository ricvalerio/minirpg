load_map("assets/maps/City.tmx");

player:set_position( 1080.0, 1000.0);

dofile "assets/scripts/priest.lua"
dofile "assets/scripts/dragon.lua"

do
  local i = 0
  lua_update = function()
    increment(i);
    print(i);
  end
end