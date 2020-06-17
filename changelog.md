# ChangeLog


###### 2020-06-17 [[7f8fe45](https://github.com/danijel1023/MC_Command_IDE/commit/7f8fe45fcd512a5069cf6419c479f2d59e4f35cd)]

 - Transfer `intellisence_structures` folder for visual studios copy command
 - Fix:
   - `Minecraft_Entity` (@s set for all entityes - true but mc allows to be set for players)
   - `Minecraft_Item_Predicate` (reading erased string)
   - `Minecraft_Item_Stack` (reading erased string)
   - In `void Analise_Line(std::wstring& Line, std::vector<COLORREF>& Colors)` single & double quote
 - Add:
   - `Analise_Line` for "outside class" access
   - `m_Color_Obj` for custom syntax highlighting
   - IntelliSence trigger on `void Insert(const std::vector<std::wstring>& Text, Caret_Struct Caret);`
   - Save & open file action
   - `Set_Text` to erase all the text in editor and input new one

---


###### 2020-06-16 [[5a079a8](https://github.com/danijel1023/MC_Command_IDE/commit/5a079a83994eda87f2834fe3c813e7c57bdb6b04)]

 - Fix:
   - `Brigadier_Bool` (always returns false)
   - `Brigadier_Double` (didn't return value)
   - `Brigadier_Integer` (didn't return value)
   - `Minecraft_Block_Predicate` (missing default namespace, edited the origianl word - pass by reference)
   - `Minecraft_Block_State` (missing default namespace, edited the origianl word - pass by reference)
   - `Minecraft_Double_Range` (wrongly compared ranged values)
   - `Minecraft_Entity` (Json tree reading problem - didn't return to root)
   - `Minecraft_Entity_Summon` (missing default namespace)
   - `Minecraft_Entity_Type` (missing default namespace)
   - `Minecraft_Function` (missing default namespace, wrong formating)
   - `Minecraft_Integer_Range` (wrongly compared ranged values)
   - `Minecraft_Item_Predicate` (missing default namespace, missing SNBT parser)
   - `Minecraft_Item_Stack` (missing default namespace, missing SNBT parser)
   - `Minecraft_Mob_Effect` (missing default namespace)
   - `Minecraft_Nbt_Path` (wrong formating)
   - `Minecraft_Resource` location (not done)
   - `Minecraft_Rotation` (not done)
   - `Minecraft_Score_Holder` (not done)

 - `parser.cpp` - copying bug
 - Add SNBT and state formaters / parsers
 - Add `Std::` custom standard functions

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




