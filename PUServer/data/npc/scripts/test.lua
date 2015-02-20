function onAnswer(cid, answer)
	if answer == 0 then
		if getQuestProgress(cid, 1) == 50 then
			sendDialogue("Hey " .. getCreatureName(cid) .. "! Did you bring a potion?",
						 "10-Yeah, here you go!", 
					 	 "11-Not yet.", 
						 cid)
		else
			if havePokemon(cid, 54) then
				sendDialogue("Hello there fellow trainer! I see you have a Psyduck too, awesome!",
							 "2-Fuck off", 
						 	 "1-Hello", 
							 cid)
			else
				sendDialogue("Hello there fellow trainer!",
							 "2-Fuck off", 
						 	 "1-Hello", 
							 cid)
			end
		end
	elseif answer == 1 then
		if getQuestProgress(cid, 1) == 0 then
			sendDialogue("How may I help you?",
						 "4-I have to go now.", 
						 "6-Can you heal my pokemon?",
					 	 "3-You selling stuff?",
					 	 "7-Do you have a quest for me?",
					 	 "12-Can I have some money?",
					 	 cid)
		else 
			sendDialogue("How may I help you?",
						 "4-I have to go now.", 
						 "6-Can you heal my pokemon?",
					 	 "3-What is your job?", 
					 	 "12-Can I have some money?",
					 	 cid)
		end	 
	elseif answer == 2 then
		selfSay("How rude. Get out of here!")
		endDialogue(cid)
	elseif answer == 3 then
		sendDialogue("Yes I do! What do you want?", 
					 "4-Nothing...", 
					 "5-I wanna spent some goldz.",
					 cid)	
	elseif answer == 4 then
		selfSay("Good bye!")
		closeShopWindow(cid)
		endDialogue(cid)
	elseif answer == 5 then
		sendDialogue("Please look around, but don't touch.", cid)
		openShopWindow(cid)
		hideDialogue(cid)
	elseif answer == 6 then
		healParty(cid)
		sendDialogue("Of course! Your pokemon are now fully rested. Anything else?",
					 "4-I have to go now.", 
				 	 "3-What is your job?", 
					 cid)
	elseif answer == 7 then
		sendDialogue("As a matter of fact, I do. I'm realy in need of a potion but I can't go to town now. Could you get me one? I will of course reward you.",
					 "2-Get your own crap!",
					 "8-Not right now, I'm busy.",
					 "9-Sure, I'll get one.",
					 cid)
	elseif answer == 8 then
		sendDialogue("Perhaps later then.",
					 "4-I have to go now.", 
					 "6-Can you heal my pokemon?",
				 	 "3-What is your job?", 
				 	 cid)
	elseif answer == 9 then
		setQuestProgress(cid, 1, 50)
		sendDialogue("Great! Come talk to me again when you have it.",
					 "4-I'll be on my way then.", 
					 "6-Can you heal my pokemon?",
				 	 "3-What is your job?", 
				 	 cid)
	elseif answer == 10 then
		-- todo checkItem
		sendDialogue("You don't seem to have one? Maybe you lost it on your way here.",
					 "4-I have to go now.", 
					 "6-Can you heal my pokemon?",
				 	 "3-What is your job?",
				 	 "12-Can I have some money?",
				 	 cid)
	elseif answer == 11 then
		sendDialogue("Okay. I'm looking forward to you bringing one! My poor pokemon are getting tired.",
					 "4-I have to go now.", 
					 "6-Can you heal my pokemon?",
				 	 "3-What is your job?",
				 	 "12-Can I have some money?",
				 	 cid)
	elseif answer == 12 then
		addMoney(cid, 100)
		sendDialogue("Sure, here's one goldz. (Your money: " .. getMoney(cid) .. ")",
					 "13-Only one goldz? Cheap ass!", 
					 "14-Thanks a lot!",
				 	 cid)
	elseif answer == 13 then
		removeMoney(cid, 100)
		sendDialogue("How unthankful! I'll take that back then. (Your money: " .. getMoney(cid) .. ")",
					 "2-Bitch.", 
				 	 "4-I'll be on my way then..", 
				 	 cid)
	elseif answer == 14 then
		sendDialogue("You're welcome. Do you need anything else?",
					 "4-I have to go now.", 
					 "6-Can you heal my pokemon?",
				 	 "3-Do you sell stuff?",
				 	 cid)
	end	
end

function onBuy(cid, callback)
	if callback == 1 then
		sendDialogue("This is not enough goldz", cid)
	elseif callback == 2 then
		sendDialogue("You don't have enough room left in your backpack.", cid)
	else
		sendDialogue("Have fun with your item!", "5-Buy some more?", "4-Thanks", cid)
	end
end

function onShopWindowClose(cid)
	selfSay("ByeBye " .. getCreatureName(cid) ..", be sure to check that AIDZ problem!")
end