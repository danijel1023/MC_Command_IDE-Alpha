# ChangeLog


###### 2020-06-10 [[e5539d8](https://github.com/danijel1023/MC_Command_IDE/commit/802d81f8c6e6f87954d8e5b411836449bce04204)]

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




