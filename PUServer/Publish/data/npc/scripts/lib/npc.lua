-- do one step to reach position
function moveToPosition(x,y,z)
	selfMoveTo(x, y, z)
end

function selfGotoIdle()
		following = false
		attacking = false
		selfAttackCreature(0)
		target = 0
end
