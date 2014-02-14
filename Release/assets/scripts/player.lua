player = create_entity("Player");
camera = create_follow_camera(player);

player.on_collision = function( player, target )
  if target:is_container() then
    if quests["priest_potion"] > 2 and target:has_item("Potion of life") and player:has_item("Potion of life") == false then
      player:speak(60, "These must be the potions Father John", "was talking about.");
    end
    
    player:add_to_inventory(target);

    target:destroy();
  end
end


player.shoot_timer = 0;

player.on_update = function()
  if player:position().y < 100 then
      load_map("assets/maps/City.tmx");
      player:position().x = 1080.0;
      player:position().y = 900.0;
      dofile("assets/scripts/City.lua");
  end
  
  if player:position().x < 100 then
    load_map("assets/maps/CaveLevel.tmx");
    priest:destroy();
    dragon:destroy();
    
    player:position().x = 1000.0;
    player:position().y = 300.0;
  end
end


player.on_action = function()
  player:shoot():set_reload( 30 );
  
  fireball = create_entity("fireball");
  
  pos = player:position();
  fireball:set_position( pos.x, pos.y );
  fireball:position().z = -20.0;
  
  fireball:apply_force( player:facing(), 8.0 );

  subscribe("collision", fireball, spell_hit_callback);
end


subscribe("collision", player, player.on_collision);
subscribe("action", player, player.on_action);
subscribe("update", player, player.on_update);

