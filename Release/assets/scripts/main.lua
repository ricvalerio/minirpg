print("loading main.lua");

load_animations("assets/definitions/metadata.pgf");
load_metadata("assets/definitions/metadata.pgf");
print("meta data loaded");



dofile "assets/scripts/callbacks.lua"
dofile "assets/scripts/loot.lua"
dofile "assets/scripts/quests.lua"
dofile "assets/scripts/player.lua";


spell_hit_callback = function( spell, target )
  if target:is_player() then
    return;
  end

  if target:is_attackable() then
    target:take_damage( spell:damage() );
    damage_callback(spell, target);
  end
    
  spell:destroy();
end


function generic_collision_callback( a, b )
  if a:is_spell() then
    spell_collision_callback(a, b);
    return
  end
end



print("loading map file...");
dofile "assets/scripts/City.lua"; 
