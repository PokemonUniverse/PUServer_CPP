function onAnswer(cid, answer)
	if getQuestProgress(1, cid) == 0 then
		if answer == 0 then
			sendDialogue("This is no place for a kid, go away before you get hurt.",
					"3-What's going on here?",
					"4-Gone",
					cid)
		elseif answer == 3 then
			sendDialogue("We are having a standoff against those Kurochi punks. Any minute this could turn into a dangerous battle.", cid)
			endDialogue(cid)
		elseif answer == 4 then
			endDialogue(cid)
		end

	else if getQuestProgress(1, cid) == 3 then

		if answer == 0 then
			sendDialogue("This is no place for a kid, go away before you get hurt.",
					"1-Did you take a little girl's potion?",
					"2-Sorry to bother you",
					cid)

		elseif answer == 1 then
			sendDialogue("Huh? I know nothing about a little girl, or a potion.", cid)
			endDialogue(cid)
		elseif answer == 2 then
			endDialogue(cid)
		end
	end
end