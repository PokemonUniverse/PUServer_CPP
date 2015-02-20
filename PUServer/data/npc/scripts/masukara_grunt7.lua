function onAnswer(cid, answer)
	if getQuestProgress(1, cid) == 0 then
		
		if answer == 0 then
			sendDialogue("Those Kurochi are going to look so stupid when they see what Lugia can do.", cid)
			endDialogue(cid)
		end

	if getQuestProgress(1, cid) == 3 then

		if answer == 0 then
			sendDialogue("Those Kurochi are going to look so stupid when they see what Lugia can do.",
					"1-Did you take a little girl's potion?",
					"2-Sorry to bother you",
					cid)

		elseif answer == 1 then
			sendDialogue("Now is not the time, I have to focus. Go away.", cid)
			endDialogue(cid)
		elseif answer == 2 then
			endDialogue(cid)
	end
end