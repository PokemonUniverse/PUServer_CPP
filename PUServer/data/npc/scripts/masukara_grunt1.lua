function onAnswer(cid, answer)
	if getQuestProgress(1, cid) == 0 then
		if answer == 0 then
			sendDialogue("Those Kurochi guys are going down.", cid)
			endDialogue(cid)
		end
	elseif getQuestProgress(1, cid) == 3 then

		if answer == 0 then
			sendDialogue("Those Kurochi guys are going down.",
					"1-Did you take a little girl's potion?",
					"2-Sorry to bother you",
					cid)

		elseif answer == 1 then
			sendDialogue("What are you talking about? Get out of here.", cid)
			endDialogue(cid)
		elseif answer == 2 then
			sendDialogue("Scram!", cid)
			endDialogue(cid)
		end
	end
end