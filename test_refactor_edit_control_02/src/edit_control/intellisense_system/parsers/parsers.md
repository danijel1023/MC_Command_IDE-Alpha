##### Brigadier_Bool
-  Values: `"true"` or `"false"`

###### In use: `gamerule announceAdvancements <Brigadier_Bool>`


---
##### Brigadier_Double
-  Double

###### In use: `data get block ~ ~ ~ Path <Brigadier_Double>`


---
##### Brigadier_Float
-  Float (/double)

###### In use: `particle minecraft:ambient_entity_effect ~ ~ ~ 0 0 0 <Brigadier_Float>`


---
##### Brigadier_Integer
-  Integer

###### In use: `give @s minecraft:acacia_boat <Brigadier_Integer>`


---
##### Brigadier_String
In "properties" object, under "type":

- `"word"`: only one word that contains lowercase and uppercase ASCII letters, numbers, `_`, `.`, `-` and `+`
- `"greedy"`: any string **(with spaces)** and any combination of caracters till the end of the line
- `"phrase"`: [TODO: add content]

###### In use: `me <Brigadier_String>`


---
##### Minecraft_Block_Pos
- [TODO: format this sentence] 3 double values with maybe `~` or `^` in front of the number
- Example: `0 0 0`, `~ ~ ~`, `^ ^ ^6`, `5 0 ~`

###### NOTE
- Cannot mix `^` with anything else (~~`^ ^ 0`~~, ~~`^ ~ ~`~~, ~~`~ ^ 0`~~)


---
##### Minecraft_Block_Predicate
-  All blocks in game
[found here](https://github.com/Arcensoth/mcdata/tree/master/processed/reports)
**with** blocks in `data/minecraft/tags/blocks` blocks
[found here](https://github.com/Arcensoth/mcdata/tree/master/processed/data/minecraft/tags/blocks)


---
##### Minecraft_Color
- Values: `"aqua"`,
`"black"`,
`"blue"`,
`"dark_aqua"`,
`"dark_blue"`,
`"dark_gray"`,
`"dark_green"`,
`"dark_purple"`,
`"dark_red"`,
`"gold"`,
`"gray"`,
`"green"`,
`"light_purple"`,
`"red"`,
`"reset"`,
`"white"`,
`"yellow"`


---
##### Minecraft_Column_Pos
-  Two double values


---
##### Minecraft_Component
-  [TODO: add content]


---
##### Minecraft_Dimension
-  Values: `"minecraft:overworld"`,
`"minecraft:the_end"`,
`"minecraft:the_nether"`


---
##### Minecraft_Entity
-  [TODO: add content]


---
##### Minecraft_Entity_Anchor
-  Values: `"eyes"` or `"feet"`


---
##### Minecraft_Entity_Summon
- List of all entityes


---
##### Minecraft_Function
-  Syntax: `<namespace>:<path>`
- `<namespace>`: one word that contains lowercase ASCII letters, numbers, `_`, `-` and `.`
- `<path>`: like `<namespaces>` with `/` seperating the directories

- Example: `Test_Namespace:dir0/dir1/dir2/My_function`


---
##### Minecraft_Game_Profile
-  [TODO: add content]


---
##### Minecraft_Int_Range
-  One integer or two integers seperated by two `.` (first int must be less or equal than second int)

-  Example: `8`, `3..9`


---
##### Minecraft_Item_Enchantment
-  List of all enchantmens


---
##### Minecraft_Item_Predicate
-  All items **with** items in `data/minecraft/tags/items` items
[found here](https://github.com/Arcensoth/mcdata/tree/master/processed/data/minecraft/tags/items)


---
##### Minecraft_Item_Stack
-  All items **without** items in `data/minecraft/tags/items` items
[found here](https://github.com/Arcensoth/mcdata/tree/master/processed/data/minecraft/tags/items)



---
##### Minecraft_Message
-  Any string **(with spaces)** and any combination of caracters till the end of the line


---
##### Minecraft_Mob_Effect
-  List of all effects


---
##### Minecraft_Nbt_Compound_Tag
-  [TODO: add content]


---
##### Minecraft_Nbt_Path
-  [TODO: add content]


---
##### Minecraft_Nbt_Tag
-  [TODO: add content]


---
##### Minecraft_Objective
-  one word that contains lowercase and uppercase ASCII letters, numbers, `_`, `.`, `-` and `+`


---
##### Minecraft_Objective_Criteria
-  List of all objective criteria


---
##### Minecraft_Operation
-  List of all operations


---
##### Minecraft_Particle
-  List of all particles

###### Note
-  Special case on `minecraft:block` and `minecraft:item`,
you need to get second paramater before moving to next (???) syntax something...

---
##### Minecraft_Resource_Location
-  [TODO: add content]


---
##### Minecraft_Rotation
-  Two doubles with possible relative (`~`) signs


---
##### Minecraft_Score_Holder
-  [TODO: add content]


---
##### Minecraft_Scoreboard_Slot
-  List of scoreboard slots


---
##### Minecraft_Swizzle
-  Combination of values: `x`, `y` or `z`


---
##### Minecraft_Team
-  One word that contains lowercase and uppercase ASCII letters, numbers, `_`, `.`, `-` and `+`


---
##### Minecraft_Time
-  integer with optional `d`/`s`/`t`


---
##### Minecraft_Vec2
-  Two doubles


---
##### Minecraft_Vec3
-  Three doubles




---
