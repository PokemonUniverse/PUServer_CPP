function onAnswer(cid, answer)
	if answer == 0 then
		sendDialogue("Hey " .. getCreatureName(cid) .. "! Sup?",
					 "1-Heal me plx!", 
					 "2-Die.", 
					 cid)
	elseif answer == 1 then
		healParty(cid)
		sendDialogue("Done. Bye!", cid)	
		endDialogue(cid)
	elseif answer == 2 then
		sendDialogue("NO U!", cid)	
		endDialogue(cid)
	end	
end
