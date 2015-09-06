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

// Globals
char *loggerPath = NULL;
HANDLE mutex;
FILE *defaultOutput = NULL;
FILE *handlersOutput = NULL;
bool enablePacketEncryption = true;
char *sessionDate = NULL;

/** =================== Utils ================= */
void writePacketToFile (
    char *packetType,
    byte *packet,
    size_t packetSize
) {
    WaitForSingleObject (mutex, INFINITE);

    static char filename[10000];
    sprintf (filename, "%s/packets/%s/%s.txt", loggerPath, sessionDate, packetType);
	FILE *packetFile = fopen (filename, "a+");

    dbg_set_output (packetFile);
    buffer_print (packet, packetSize, NULL);
    fclose (packetFile);

    dbg_set_output (defaultOutput);
    ReleaseMutex (mutex);
}

/** =================== HOOKS ================= */
#define DECLARE_PACKET_HANDLER(x)                                                                            \
char __thiscall CNetUsr__PacketHandler_##x (void *this, PacketType packetType, BYTE *packet, int packetSize) \
{                                                                                                            \
    bool (__thiscall *hooked) (void *, PacketType, BYTE *, int) =                                            \
        (typeof(hooked)) HookEngine_get_original_function ((ULONG_PTR) CNetUsr__PacketHandler_##x);          \
                                                                                                             \
    char *packetTypeStr = PacketType_to_string (packetType);                                                 \
                                                                                                             \
    WaitForSingleObject (mutex, INFINITE);                                                                   \
    dbg ("[CLIENT RECEIVE] Packet type : <%s>", packetTypeStr);                                              \
    buffer_print (packet, packetSize, NULL);                                                                 \
    ReleaseMutex (mutex);                                                                                    \
                                                                                                             \
    writePacketToFile (packetTypeStr, packet, packetSize);                                                   \
                                                                                                             \
    return hooked (this, packetType, packet, packetSize);                                                    \
}
DECLARE_PACKET_HANDLER(0);
DECLARE_PACKET_HANDLER(1);
DECLARE_PACKET_HANDLER(2);
DECLARE_PACKET_HANDLER(3);
DECLARE_PACKET_HANDLER(4);
DECLARE_PACKET_HANDLER(5);
DECLARE_PACKET_HANDLER(6);

char __thiscall CNetUsr__Recv (void *this)
{
    /*
        CNetUsr contains a pointer to the current packet handler used in its vftable (PacketHandler).
        Hook this function whenever it is used.
    */
    #pragma pack(push, 1)
    struct CNetUsr {
        struct CNetUsr_vtable {
              int CNetUsr_virt00;
              int nullsub_327;
              unsigned __int8 (__thiscall *PacketHandler)(void *, int, BYTE *, size_t);
              unsigned __int8 (__thiscall *ServerMsg)(void *, DWORD, BYTE *, size_t);
        } *vtable;
    } *netUsr = (typeof (netUsr)) this;
    #pragma pack(pop)

    // Packet handlers already hooked
    static BbQueue packetHandlersHooked = bb_queue_local_decl ();
    static int handlersHooked = 0;

    if (!(bb_queue_exists (&packetHandlersHooked, netUsr->vtable->PacketHandler))) {
        // Not hooked yet : do it
        bb_queue_add (&packetHandlersHooked, netUsr->vtable->PacketHandler);
        dbg_set_output (handlersOutput);
        info ("New packet handler hooked at %p => CNetUsr__PacketHandler_%d", netUsr->vtable->PacketHandler, handlersHooked + 1);
        dbg_set_output (defaultOutput);
        ULONG_PTR packetHandler = NULL;
	    switch (handlersHooked++) {
	        // HookEngine_get_original_function requires a unique function per hook
            case 0: packetHandler = (ULONG_PTR) CNetUsr__PacketHandler_0; break;
            case 1: packetHandler = (ULONG_PTR) CNetUsr__PacketHandler_1; break;
            case 2: packetHandler = (ULONG_PTR) CNetUsr__PacketHandler_2; break;
            case 3: packetHandler = (ULONG_PTR) CNetUsr__PacketHandler_3; break;
            case 4: packetHandler = (ULONG_PTR) CNetUsr__PacketHandler_4; break;
            case 5: packetHandler = (ULONG_PTR) CNetUsr__PacketHandler_5; break;
            case 6: packetHandler = (ULONG_PTR) CNetUsr__PacketHandler_6; break;
            default : MessageBox (NULL, "Not enough packet handlers declared.", "! ERROR !", 0); break;
	    }
        HookEngine_hook ((ULONG_PTR) netUsr->vtable->PacketHandler, packetHandler);
    }

	int (__thiscall *hooked) (void *) =
		(typeof(hooked)) HookEngine_get_original_function ((ULONG_PTR) CNetUsr__Recv);

    return hooked (this);
}

int __cdecl imcCrypt__NetEncrypt (size_t plaintextSize, BYTE *plaintextIn, BYTE *ciphertextOut)
{
	int (__cdecl *hooked) (size_t plaintextSize, BYTE *plaintextIn, BYTE *ciphertextOut) =
		(typeof(hooked)) HookEngine_get_original_function ((ULONG_PTR) imcCrypt__NetEncrypt);

    WORD type = *(WORD *) (plaintextIn);
    char *packetType = PacketType_to_string (type);

    dbg ("[CLIENT SEND] Packet type : <%s>", packetType);
    buffer_print (plaintextIn, plaintextSize, NULL);

    writePacketToFile (packetType, plaintextIn, plaintextSize);

    if (enablePacketEncryption) {
        return hooked (plaintextSize, plaintextIn, ciphertextOut);
    } else {
        *(WORD *) ciphertextOut = plaintextSize;
        memcpy (&ciphertextOut[2], plaintextIn, plaintextSize);
        return plaintextSize+2;
    }
}

/** ============== MAIN ==================== */

void startInjection (void)
{
    /* Init globals */
    // Init mutex
    mutex = CreateMutex (NULL, FALSE, NULL);

    // Init path & dbg
	loggerPath = get_module_path ("PacketLogger.dll");

	// Init output path
	time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sessionDate = str_dup_printf ("%.02d_%.02d_%d - %.02dh%.02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
	CreateDirectory (str_dup_printf ("%s/packets/%s", loggerPath, sessionDate), NULL);

	defaultOutput = fopen (str_dup_printf("%s/packets/%s/capture.txt", loggerPath), "w+");
	handlersOutput = fopen (str_dup_printf("%s/packets/%s/handlers.txt", loggerPath), "w+");
	dbg_set_output (defaultOutput);

	if (!HookEngine_new (str_dup_printf ("%s/NtHookEngine.dll", loggerPath))) {
		MessageBox (NULL, "Error", "HookEngine loading error", 0);
		return;
	}

	// DWORD baseAddr = get_baseaddr ("Client_tos.exe");
	HookEngine_hook ((ULONG_PTR) 0x400000 + OFFSET_NetEncrypt,     (ULONG_PTR) imcCrypt__NetEncrypt);
	HookEngine_hook ((ULONG_PTR) 0x400000 + OFFSET_CNetUsr__Recv,  (ULONG_PTR) CNetUsr__Recv);
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
