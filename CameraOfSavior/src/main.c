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

typedef struct D3DXMATRIX {
    FLOAT _11; FLOAT _12; FLOAT _13; FLOAT _14;
    FLOAT _21; FLOAT _22; FLOAT _23; FLOAT _24;
    FLOAT _31; FLOAT _32; FLOAT _33; FLOAT _34;
    FLOAT _41; FLOAT _42; FLOAT _43; FLOAT _44;
} D3DXMATRIX;
/** =================== Utils ================= */

/** =================== HOOKS ================= */
D3DXMATRIX original = {
    0.7071066, -0.4454092, -0.5491909, 0.0,
    0.0,       0.7766731,  -0.6299037, 0.0,
    0.7071069, 0.4454091,  0.5491907,  0.0,
    707.6651,  -271.2636,  325.5788,   1.000000
};
int __thiscall imcd3d9_CDevice__SetViewTM (int this, D3DXMATRIX *view)
{
    // 55 8B EC 53 8B D9 83 7B 08 00 56 57
    static bool init = false;

    if (!init) {
        memcpy (&original, view, sizeof (original));
        init = true;
    }

	int (__thiscall *hooked) (int this, D3DXMATRIX *view) =
		(typeof(hooked)) HookEngine_get_original_function ((ULONG_PTR) imcd3d9_CDevice__SetViewTM);

    memcpy (view, &original, sizeof (D3DXMATRIX));

    return hooked (this, view);
}

/** ============== MAIN ==================== */

void startInjection (void)
{
    // Init path & dbg
	char *loggerPath = get_module_path ("CameraOfSavior.dll");
	dbg_set_output (fopen (str_dup_printf("%s/log.txt", loggerPath), "w+"));

	if (!HookEngine_new (str_dup_printf ("%s/NtHookEngine.dll", loggerPath))) {
		MessageBox (NULL, "Error", "HookEngine loading error", 0);
		return;
	}

	DWORD baseAddr = get_baseaddr ("Client_tos.exe");
	HookEngine_hook ((ULONG_PTR) baseAddr + OFFSET_SetViewTM, (ULONG_PTR) imcd3d9_CDevice__SetViewTM);
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
