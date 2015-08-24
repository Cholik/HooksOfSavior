-- http://www.troubleshooters.com/codecorn/lua/lua_c_calls_lua.htm

account = session.barrack.GetMyAccount();

print ("BPC ID = 1"); -- Get the first slot in the commander list
bpc = account:GetBySlot(1);

actorPC = bpc:GetApc();
print ("actorPC.mapID = " .. actorPC.mapID);
print ("actorPC.channelID = " .. actorPC.channelID);

-- Get zoneInsts information
zoneInsts = session.serverState.GetMap(actorPC.mapID);
if zoneInsts == nil then
	print ("zoneInsts is NULL");
else
	local cnt = zoneInsts:GetZoneInstCount();
	print ("zoneInsts count = " .. cnt);
	for i = 0  , cnt - 1 do
		local zoneInst = zoneInsts:GetZoneInstByIndex(i);
		if zoneInst ~= nil then
			local str, gaugeString = GET_CHANNEL_STRING(zoneInst);
			print ("Channel String = " .. str .. " | gaugeString = " .. gaugeString);
		else
			print ("Error : zoneInst is NULL");
		end
	end
end