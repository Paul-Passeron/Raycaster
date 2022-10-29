# Raycaster Engine

This is a small Raycaster engine project, feel free to take/use/improve the code. It is written in C++ with SFML2.

## Syntax of .rcmap files

```
#HEADBEGIN
This is the head of the document, it acts like a multiline
comment, it is used to display information on the map.
(Won't be shown in-game in any way).
#HEADEND

#: This is a single line comment

#COMMENTBEGIN
This is a
multiline
comment.
#COMMENTEND

#WBEGIN
#: Here you write the informations about the walls, one line per wall:
<float p1x> <float p1y> <float p2x> <float p2y> <int textureIdentifier>
#ENDBEGIN

#IBEGIN
#: Same thing but for items
<flaot px> <float py> <float size> <int textureIdentifire>
#IEND

#PBEGIN
#: Write the player spawn point here (please not inside a wall or an item)
<float px> <float py>
#PEND
```
There must be no indentation and values are only separated by spaces and lines.
