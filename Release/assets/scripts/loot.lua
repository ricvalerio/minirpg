
function create_loot_bag( x, y )
  bag = create_entity("bag");
      
  bag:set_position( x, y);
  bag:apply_force( 0, 0, -10.0 );

  v = math.random(3, 5);
  while v > 0 do
    item = create_random_potion();
    bag:add_item(item);
    v = v-1;      
  end      
        
  return bag;
end


function create_random_potion()
  p = math.random(0, 100);
  if p < 33 then
    item = create_entity("healing_potion");
  elseif p < 66 then
    item = create_entity("teleport_potion");
  else
    item = create_entity("phoenix_potion");
  end      

  return item;
end

