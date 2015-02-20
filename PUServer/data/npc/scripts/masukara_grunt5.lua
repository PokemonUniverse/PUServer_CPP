function onAnswer(cid, answer)
	if getQuestProgress(1, cid) == 0 then
		
		if answer == 0 then
			sendDialogue("I don't even really want to be here, I'm only doing this because my brother is.", cid)
			endDialogue(cid)
		end

	elseif getQuestProgress(1, cid) == 3 then

		if answer == 0 then
			sendDialogue("I don't even really want to be here, I'm only doing this because my brother is.",
					"1-Did you take a little girl's potion?",
					"2-Sorry to bother you",
					cid)

		elseif answer == 1 then
			sendDialogue("That's not my style, I would never do such a thing to a little girl.", cid)
			endDialogue(cid)
		elseif answer == 2 then
			endDialogue(cid)
		end
	end
end