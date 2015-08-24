#include "HookEngine/HookEngine.h"
#include "Utils/Utils.h"
#include "Win32Tools/Win32Tools.h"
#include "PacketType.h"
#ifdef __DBG_ACTIVATED__
#undef __DBG_ACTIVATED__
#endif
#define __DBG_ACTIVATED__ TRUE
#include "dbg/dbg.h"
#include "Scanner/Scanner.h"

#include "FunctionOffset.h"

/** =================== HOOKS ================= */
char __thiscall CClientNet__SendPacket (int this, int a2, size_t a3)
{
    bool (__thiscall *hooked) (int, int, size_t) =
		(typeof(hooked)) HookEngine_get_original_function ((ULONG_PTR) CClientNet__SendPacket);

    DWORD caller = (DWORD) __builtin_return_address (0);

    dbg ("caller = %x", caller);

    static unsigned char pattern[] = {
        /*
            8945 F6         mov [dword ss:ebp-0A], eax
            8945 FA         mov [dword ss:ebp-6], eax
            B8 0F0C0000     mov eax, 0C0F
        */
        0x89, 0x45, '?',
        0x89, 0x45, '?',
        0xB8, '?', '?', '?', '?'
    };

    /*
    DWORD start = caller - 80;
    DWORD size  = caller - start;
    DWORD address = -1;
	DWORD sendPacketBuilder = mem_scanner ("sendPacketBuilder",
		start, size,
		pattern,
        "x??"
        "x??"
        "x????",

        "xxx"
        "xxx"
        "x????",
		&address
	);

    dbg ("sendPacketBuilder = %d (%s) is at address %#x", sendPacketBuilder, PacketType_to_string (sendPacketBuilder), address);
    */

    return hooked (this, a2, a3);
}

/** ============== MAIN ==================== */
void startInjection (void)
{
	char *tosDllPath = get_module_path ("getClientPacketBuilders.dll");
	dbg_set_output (fopen (str_dup_printf("%s/getClientPacketBuilders_log.txt", tosDllPath), "w+"));

	if (!HookEngine_new (str_dup_printf ("%s/NtHookEngine.dll", tosDllPath))) {
		MessageBox (NULL, "Error", "HookEngine loading error", 0);
		return;
	}

	DWORD baseAddr = get_baseaddr ("Client_tos.exe");
	HookEngine_hook ((ULONG_PTR) baseAddr + OFFSET_SendPacket, (ULONG_PTR) CClientNet__SendPacket);
}

void endInjection (void)
{
	HookEngine_unhook_all ();
}


bool WINAPI DllMain (HMODULE dll, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
			CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) startInjection, NULL, 0, NULL);
		break;

		case DLL_PROCESS_DETACH:
			endInjection ();
		break;
	}

	return true;
}
