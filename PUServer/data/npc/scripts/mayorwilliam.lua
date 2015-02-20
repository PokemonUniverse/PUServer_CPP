function onAnswer(cid, answer)
	if answer == 0 then
			sendDialogue("Hello" .. getCreatureName(cid) .. ". What can I do for you?",
					"1-Nothing",
					"2-You're mayor?",
					"3-Tell me about Miryus",
					cid)
	elseif answer == 1 then
			sendDialogue("Come see me if you have any questions; I am mayor after all!", cid)
			endDialogue(cid)
	elseif answer == 2 then
			sendDialogue("Why yes I am, someone needs to rule this new land.", cid)
			endDialogue(cid)
	elseif answer == 3 then
			sendDialogue("Miryus is a new land that is still in development. You are here to test out this new region before we open it to settlement.",
					"11-Why me?",
					"12-That's nice",
					"13-What do I have to do?",
					cid)
	elseif answer == 11 then
			sendDialogue("You've been selected to receive this sneak peak about a phenomenon called Pokemon-Universe!",
					"21-K thanks, Bye!",
					"22-Where have I heard that before?",
					cid)
	elseif answer == 12 then
			endDialogue(cid)
	elseif answer == 13 then
			sendDialogue("Just enjoy yourself. If you have problems, please let one of our staff members know.", cid)
			endDialogue(cid)
	elseif answer == 21 then
			endDialogue(cid)
	elseif answer == 22 then
			sendDialogue("I got it from some twelve year old promo video.", cid)
			endDialogue(cid)
	end
end