/**
*
*   ██████╗   ██╗ ███████╗ ███╗   ███╗ ██╗   ██╗
*   ██╔══██╗ ███║ ██╔════╝ ████╗ ████║ ██║   ██║
*   ██████╔╝ ╚██║ █████╗   ██╔████╔██║ ██║   ██║
*   ██╔══██╗  ██║ ██╔══╝   ██║╚██╔╝██║ ██║   ██║
*   ██║  ██║  ██║ ███████╗ ██║ ╚═╝ ██║ ╚██████╔╝
*   ╚═╝  ╚═╝  ╚═╝ ╚══════╝ ╚═╝     ╚═╝  ╚═════╝
*
* @license GNU GENERAL PUBLIC LICENSE - Version 2, June 1991
*          See LICENSE file for further information
*/
#pragma once

// Camera
#define OFFSET_SetViewTM      (0x0AEE0A0 - 0x400000) // ICBT

// Items list
/* Search for string "ItemStar" */
#define OFFSET_convertIESToIR (0x090FF40 - 0x400000)
#define OFFSET_LuaGetObject   (0x103B6D0 - 0x400000)

// Packets
#define OFFSET_SendPacket     (0x0659510 - 0x400000) // ICBT
#define OFFSET_GetPacket      (0x0659910 - 0x400000) // ICBT
#define OFFSET_NetEncrypt     (0x0CFDC20 - 0x400000) // ICBT
#define OFFSET_CNetUsr__Recv  (0x0658AF0 - 0x400000) // ICBT

// Lua
#define OFFSET_lua_gettop     (0x0D187A0 - 0x400000) // Search "[no object]"

// String
#define OFFSET_StringId__c_str 0x0D75F10 // ICBT

// Lua
#define OFFSET_LuaExtern__DefineLuaGetter (0x00D16BB0 - 0x400000) // ICBT
#define OFFSET_LuaExtern__endDefinition (0x00D168D0 - 0x400000) // ICBT
#define OFFSET_LuaExtern__DefineVariable (0x00D16890 - 0x400000) // ICBT
#define OFFSET_LuaExtern__DefineVariableStructure (0x00D168F0 - 0x400000) // ICBT
#define OFFSET_LuaExtern__DefineEnum (0x00D16BE0 - 0x400000) // ICBT
#define OFFSET_LuaExtern__DefineLuaGetterAndSetter (0x00D16C10 - 0x400000) // ICBT
#define OFFSET_LuaExtern__DefineLuaType (0x00D169F0 - 0x400000) // ICBT
#define OFFSET_LuaExtern__declGlobalFunction (0x00D18570 - 0x400000) // ICBT // Search "GetEnv"
#define OFFSET_LuaExtern__registerGlobalFunction (0x00D13B60 - 0x400000) // ICBT
