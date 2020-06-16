# ChangeLog


###### 2020-06-16 [[5a079a8](https://github.com/danijel1023/MC_Command_IDE/commit/5a079a83994eda87f2834fe3c813e7c57bdb6b04)]

 - Fixed:
   - brigadier bool (always returns false)
   - brigadier double (didn't return value)
   - brigadier integer (didn't return value)
   - minecraft block predicate (missing default namespace, edited the origianl word - pass by reference)
   - minecraft block state (missing default namespace, edited the origianl word - pass by reference)
   - minecraft double range (wrongly compared ranged values)
   - minecraft entity (Json tree reading problem - didn't return to root)
   - minecraft entity summon (missing default namespace)
   - minecraft entity type (missing default namespace)
   - minecraft function (missing default namespace, wrong formating)
   - minecraft integer range (wrongly compared ranged values)
   - minecraft item predicate (missing default namespace, missing SNBT parser)
   - minecraft item stack (missing default namespace, missing SNBT parser)
   - minecraft mob effect (missing default namespace)
   - minecraft nbt path (wrong formating)
   - minecraft resource location (not done)
   - minecraft rotation (not done)
   - minecraft score holder (not done)

 - `parser.cpp` - copying bug
 - add SNBT and state formaters / parsers
 - add `Std::` custom standard functions

---


###### 2020-06-10 [[802d81f](https://github.com/danijel1023/MC_Command_IDE/commit/802d81f8c6e6f87954d8e5b411836449bce04204)]

 - Redo a lot of the parsers to match official minecraft specification
 - Add Minecraft_Entity_Type that contains all entityes for entity selector
 - Add custom parsers for entity selector

---


###### 2020-05-24 [[29b519b](https://github.com/danijel1023/MC_Command_IDE/commit/29b519b58cbdf406d97e1cf12ad75b7653f9d5fa)]

- Add `Minecraft_Block_State` parser (.json, folder, .cpp & linked it in parser.cpp)
- Complete Parsers:
`Minecraft_Block_Pos`,
`Minecraft_Column_Pos`,
`Minecraft_Message`
- Move `changelog.md` to solution folder from test_refactor_edit_control_02 folder
- Fix `Brigadier_Double` & `Brigadier_Integer` not failing on wrongly formated number
- Remove double "pch.h" include
---


###### 2020-05-24 [[f1ad2a6](https://github.com/danijel1023/MC_Command_IDE/commit/f1ad2a6314dbb9636bb5440dc8b9c514c67689e4)]

- `changelog.md`
- Most of the parsers
- `Generic_Split` (splits string by space while keeping spaces inside of brackets and strings ('\\"' / '\\''))
- Correct typo (paraser -> parser)
- Add `parsers.md` & `done_parsers.md`
- Update `README.md`
- Complete Parsers:
`Brigadier_Bool`,
`Brigadier_Double`,
`Brigadier_Float`,
`Brigadier_Integer`,
`Minecraft_Block_Predicate`,
`Minecraft_Color`,
`Minecraft_Dimension`,
`Minecraft_Entity_Anchor`,
`Minecraft_Entity_Summon`,
`Minecraft_Function`,
`Minecraft_Item_Enchantment`,
`Minecraft_Item_Predicate`,
`Minecraft_Item_Stack`,
`Minecraft_Mob_Effect`,
`Minecraft_Objective_Criteria`,
`Minecraft_Operation`,
`Minecraft_Particle`,
`Minecraft_Scoreboard_Slot`
---




