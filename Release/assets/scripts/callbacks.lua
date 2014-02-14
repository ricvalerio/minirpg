

callbacks = {};
callbacks["range"] = {};
callbacks["collision"] = {};
callbacks["update"] = {};
callbacks["damage"] = {};
callbacks["action"] = {};
callbacks["update"] = {};


generic_collision_callback = function()
end


function add_callback(type, entity_id, callback)
  callbacks[type][entity_id] = callback;
end


function collision_callback(entity, target)
  if callbacks["collision"][entity.id] then
    callbacks["collision"][entity.id]( entity, target );
  end
end


function range_callback(entity, target)
  if callbacks["range"][entity.id] then
    callbacks["range"][entity.id]( target );
  end
end


function damage_callback(entity, target)
  if callbacks["damage"][target.id] then
    callbacks["damage"][target.id]( entity );
  end
end


function action_callback(entity)
  if callbacks["action"][entity.id] then
    callbacks["action"][entity.id]();
  end
end


function update_callback(entity)
  if callbacks["update"][entity.id] then
    callbacks["update"][entity.id]();
  end
end
