priest = create_entity("Priest");
priest:set_position( 1080.0, 350.0);


priest.on_range = function(target)
  if target:is_player() == false then
    return;
  end
  
  if quests["priest_potion"] == 2 then
    priest:speak(90, "Do you have the potion?");
  elseif quests["priest_potion"] == 2 and player:has_item("Potion of life") then
    priest:speak(90, "Brilliant! You have the potions!", "Thank you very much!");
    priest:dialogue_get_potion();
  elseif quests["priest_potion"] == 1 then
    priest:dialogue_church();
  end
end

priest.dialogue_get_potion = function()
  player:remove_item("Potion of life");
  wait(60);
  move_to(0, priest.id, 1080, 1000);
  quests["priest_potion"] = 4;
end

priest.dialogue_church = function()
  quests["priest_potion"] = 2;

  stop_music(3000);
  freeze_player();
  wait(0);
print(priest:get_name());  
  priest:speak(0, "Hello there! You arrived just in time!", "I am " .. priest:get_name() );
  wait(15);
  
  priest:speak(0, "I wonder if you can help me." );
  wait(15);

  priest:speak(0, "You see, outside the church", "there is an evil dragon...");
  wait(15);
  
  priest:speak(0, "And the dragon stole my potion of life!");
  wait(15);

  if player:has_item("Potion of life") then
    player:speak(0, "This potion I have?");
    wait(15);
    priest:speak(0, "Wonderful! Thank you very much!");
    wait(15);
    unfreeze_player();
    priest:dialogue_get_potion();
    wait(15);
  else    
    priest:speak(0, "I am too weak too fight him.", "Can you help me?");
    wait(15);
    unfreeze_player();
    wait(15);

  end
  --play_music(3000, "TownTheme.mp3");

end

subscribe("range", priest, priest.on_range);
