
TODO // Raycaster Engine Paul Passeron 2022


	-Implementing some kind of pixel "smudging" s that the sampling doesn't look so bad standing far away
from a wall.
	-Have a way of creating maps // Done 
	-Have a way of loading maps // Not really done but getting there
	-Have multiple types of walls in map
	-Maybe some doors, etc
	-Billboard perspective for mobs, items and other objects as plants, pillars...
	-Have a hand with a gun or other fire arms, maybe sword...
	-Animations (create animation handler)
	-Animation handler:
		# Use the computer clock for timing (in order not to stop everything else while animation is playing)
		# Maybe interpolation ?
	-Add floor and ceiling
	-Maybe some music/sfx ?
	-Make some test maps
	-Jump ??? (maybe not)
	

For multiple wall sizes, we should have the wall start at an height independent of the wall height
(not centered on the screen) because if we don't, bigger walls' starts would be lower than smaller walls' starts. 