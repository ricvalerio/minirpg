dragon = create_entity("Dragon");
dragon:set_position( 1080.0, 1500.0);


dragon.on_damage = function( target )
  if dragon:health().current <= 0 then
    pos = dragon:position();

    bag = create_loot_bag( pos.x, pos.y );

    dragon:destroy();
  end
end

subscribe("damage", dragon, dragon.on_damage);