function onAnswer(cid, answer)
	if answer == 0 then
			sendDialogue("Those PU guys are great! They asked me to manage an open bar here. In return, I get to advertise for my business, I plan to start in the near future.",
					"1-You're going to open a bar here?",
					"2-Can I have a drink?",
					"3-Drinking kills your liver!",
					cid)
	elseif answer == 1 then
			sendDialogue("Yup; I plan to at least! But I still need a name for it, I want to name it after someone who can gain recognition for something great, he/she does.", cid)
			endDialogue(cid)
	elseif answer == 2 then
			sendDialogue("Sure! Oh, sorry, you seem to have forgotten your ID when you came to Miryus, I can't supply to minors.", cid)
			endDialogue(cid)
	elseif answer == 3 then
			sendDialogue("I'm thinking about getting into the funeral business too.", cid)
			endDialogue(cid)
	end
end