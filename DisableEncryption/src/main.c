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
#include "HookEngine/HookEngine.h"
#include "Utils/Utils.h"
#include "Win32Tools/Win32Tools.h"
#include "PacketType.h"
#include "dbg/dbg.h"

#include "FunctionOffset.h"

/** =================== HOOKS ================= */
int __cdecl imcCrypt__NetEncrypt (size_t plaintextSize, BYTE *plaintextIn, BYTE *ciphertextOut)
{
    *(WORD *) ciphertextOut = plaintextSize;
    memcpy (&ciphertextOut[2], plaintextIn, plaintextSize);
    return plaintextSize+2;
}

/** ============== MAIN ==================== */

void startInjection (void)
{
    char *loggerPath = NULL;
    FILE *defaultOutput = NULL;

    // Init path & dbg
	if (!(loggerPath = get_module_path ("DisableEncryption.dll"))) {
        MessageBox (NULL, "Cannot find the module.", "Error", 0);
        return;
	}
	defaultOutput = fopen (str_dup_printf("%s/log.txt", loggerPath), "w+");
	dbg_set_output (defaultOutput);

	if (!HookEngine_new (str_dup_printf ("%s/NtHookEngine.dll", loggerPath))) {
		MessageBox (NULL, "HookEngine loading error", "Error", 0);
		return;
	}

	DWORD baseAddr = get_baseaddr ("Client_tos.exe");
    HookEngine_hook ((ULONG_PTR) baseAddr + OFFSET_NetEncrypt,     (ULONG_PTR) imcCrypt__NetEncrypt);
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
