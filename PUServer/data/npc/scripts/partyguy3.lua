function onAnswer(cid, answer)
	if answer == 0 then
			sendDialogue("Who are you? *hic* I, uh, I, I need more beer! Can you get me some more?",
					"1-Yeah, ok, be right back.",
					"2-You're too drunk already!",
					"3-Sorry, I'm too young.",
					cid)
	elseif answer == 1 then
			sendDialogue("Why? You going somewhere?", cid)
			endDialogue(cid)
	elseif answer == 2 then
			selfSay("No, you're a...*hic* Zzzz...*hic*", cid)
			endDialogue(cid)
	elseif answer == 3 then
			sendDialogue("You're both too young? That's too bad!", cid)
			endDialogue(cid)
	end
end