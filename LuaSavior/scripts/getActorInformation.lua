function getActorInformation (actor)
	local name = actor:GetName();
	print ("actorName = " .. name);
	
	local brk = GetBarrackSystem(actor);
	local key = name;
	key = brk:GetCID();
	
	print ("CID = " .. key);
	
	local bpc = barrack.GetBarrackPCInfoByCID(key);
	if bpc == nil then
		print ("Error : the BPC is null...");
	end

	local apc = bpc:GetApc();
	if apc == nil then
		print ("Error : the APC is null...");
	end

	print ("Gender = " .. apc:GetGender());
	local jobid = apc:GetJob();
	print ("JobId = " .. jobid);
	
	local jobCls = GetClassByType("Job", jobid);
	print ("JobName = " .. jobCls.Name);
	
	for i = 0 , item.GetEquipSpotCount() - 1 do
		local name = item.GetEquipSpotName(i);
		print ("Equipement ID=" .. i);
		print ("Equipement Name= " .. name);
	end

	print ("End.");
end

function getBpcInformation (bpc)
	if bpc == nil then
		print ("Error : the BPC is null...");
	end
end