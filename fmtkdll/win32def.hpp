#ifndef WIN32DEF_HPP
#define WIN32DEF_HPP

int (__stdcall * Real_AbortDoc)(HDC a0)
    = AbortDoc;

BOOL (__stdcall * Real_AbortPath)(HDC a0)
    = AbortPath;

HKL (__stdcall * Real_ActivateKeyboardLayout)(HKL a0,
                                              UINT a1)
    = ActivateKeyboardLayout;

ATOM (__stdcall * Real_AddAtomA)(LPCSTR a0)
    = AddAtomA;

ATOM (__stdcall * Real_AddAtomW)(LPCWSTR a0)
    = AddAtomW;

int (__stdcall * Real_AddFontResourceA)(LPCSTR a0)
    = AddFontResourceA;

int (__stdcall * Real_AddFontResourceW)(LPCWSTR a0)
    = AddFontResourceW;

BOOL (__stdcall * Real_AdjustWindowRect)(LPRECT a0,
                                         DWORD a1,
                                         BOOL a2)
    = AdjustWindowRect;

BOOL (__stdcall * Real_AdjustWindowRectEx)(LPRECT a0,
                                           DWORD a1,
                                           BOOL a2,
                                           DWORD a3)
    = AdjustWindowRectEx;

BOOL (__stdcall * Real_AllocConsole)(void)
    = AllocConsole;

BOOL (__stdcall * Real_AngleArc)(HDC a0,
                                 int a1,
                                 int a2,
                                 DWORD a3,
                                 FLOAT a4,
                                 FLOAT a5)
    = AngleArc;

BOOL (__stdcall * Real_AnimatePalette)(HPALETTE a0,
                                       UINT a1,
                                       UINT a2,
                                       CONST PALETTEENTRY* a3)
    = AnimatePalette;

BOOL (__stdcall * Real_AnyPopup)(void)
    = AnyPopup;

BOOL (__stdcall * Real_AppendMenuA)(HMENU a0,
                                    UINT a1,
                                    UINT_PTR a2,
                                    LPCSTR a3)
    = AppendMenuA;

BOOL (__stdcall * Real_AppendMenuW)(HMENU a0,
                                    UINT a1,
                                    UINT_PTR a2,
                                    LPCWSTR a3)
    = AppendMenuW;

BOOL (__stdcall * Real_Arc)(HDC a0,
                            int a1,
                            int a2,
                            int a3,
                            int a4,
                            int a5,
                            int a6,
                            int a7,
                            int a8)
    = Arc;

BOOL (__stdcall * Real_ArcTo)(HDC a0,
                              int a1,
                              int a2,
                              int a3,
                              int a4,
                              int a5,
                              int a6,
                              int a7,
                              int a8)
    = ArcTo;

BOOL (__stdcall * Real_AreFileApisANSI)(void)
    = AreFileApisANSI;

UINT (__stdcall * Real_ArrangeIconicWindows)(HWND a0)
    = ArrangeIconicWindows;

BOOL (__stdcall * Real_AttachThreadInput)(DWORD a0,
                                          DWORD a1,
                                          BOOL a2)
    = AttachThreadInput;

BOOL (__stdcall * Real_BackupRead)(HANDLE a0,
                                   LPBYTE a1,
                                   DWORD a2,
                                   LPDWORD a3,
                                   BOOL a4,
                                   BOOL a5,
                                   LPVOID* a6)
    = BackupRead;

BOOL (__stdcall * Real_BackupSeek)(HANDLE a0,
                                   DWORD a1,
                                   DWORD a2,
                                   LPDWORD a3,
                                   LPDWORD a4,
                                   LPVOID* a5)
    = BackupSeek;

BOOL (__stdcall * Real_BackupWrite)(HANDLE a0,
                                    LPBYTE a1,
                                    DWORD a2,
                                    LPDWORD a3,
                                    BOOL a4,
                                    BOOL a5,
                                    LPVOID* a6)
    = BackupWrite;

BOOL (__stdcall * Real_Beep)(DWORD a0,
                             DWORD a1)
    = Beep;

HDWP (__stdcall * Real_BeginDeferWindowPos)(int a0)
    = BeginDeferWindowPos;

HDC (__stdcall * Real_BeginPaint)(HWND a0,
                                  LPPAINTSTRUCT a1)
    = BeginPaint;

BOOL (__stdcall * Real_BeginPath)(HDC a0)
    = BeginPath;

HANDLE (__stdcall * Real_BeginUpdateResourceA)(LPCSTR a0,
                                               BOOL a1)
    = BeginUpdateResourceA;

HANDLE (__stdcall * Real_BeginUpdateResourceW)(LPCWSTR a0,
                                               BOOL a1)
    = BeginUpdateResourceW;

HRESULT (__stdcall * Real_BindMoniker)(IMoniker* a0,
                                       DWORD a1,
                                       CONST IID& a2,
                                       LPVOID* a3)
    = BindMoniker;

BOOL (__stdcall * Real_BitBlt)(HDC a0,
                               int a1,
                               int a2,
                               int a3,
                               int a4,
                               HDC a5,
                               int a6,
                               int a7,
                               DWORD a8)
    = BitBlt;

BOOL (__stdcall * Real_BringWindowToTop)(HWND a0)
    = BringWindowToTop;

long (__stdcall * Real_BroadcastSystemMessageA)(DWORD a0,
                                                LPDWORD a1,
                                                UINT a2,
                                                WPARAM a3,
                                                LPARAM a4)
    = BroadcastSystemMessageA;

long (__stdcall * Real_BroadcastSystemMessageW)(DWORD a0,
                                                LPDWORD a1,
                                                UINT a2,
                                                WPARAM a3,
                                                LPARAM a4)
    = BroadcastSystemMessageW;

BOOL (__stdcall * Real_BuildCommDCBA)(LPCSTR a0,
                                      LPDCB a1)
    = BuildCommDCBA;

BOOL (__stdcall * Real_BuildCommDCBAndTimeoutsA)(LPCSTR a0,
                                                 LPDCB a1,
                                                 LPCOMMTIMEOUTS a2)
    = BuildCommDCBAndTimeoutsA;

BOOL (__stdcall * Real_BuildCommDCBAndTimeoutsW)(LPCWSTR a0,
                                                 LPDCB a1,
                                                 LPCOMMTIMEOUTS a2)
    = BuildCommDCBAndTimeoutsW;

BOOL (__stdcall * Real_BuildCommDCBW)(LPCWSTR a0,
                                      LPDCB a1)
    = BuildCommDCBW;

HRESULT (__stdcall * Real_CLSIDFromProgID)(LPCOLESTR a0,
                                           LPGUID a1)
    = CLSIDFromProgID;

#ifdef _Real_WINNT_WIN7
HRESULT (__stdcall * Real_CLSIDFromString)(LPCOLESTR a0,
                                           LPGUID a1)
    = CLSIDFromString;
#else
HRESULT (__stdcall * Real_CLSIDFromString)(LPOLESTR a0,
                                           LPGUID a1)
    = CLSIDFromString;
#endif

BOOL (__stdcall * Real_CallMsgFilterA)(LPMSG a0,
                                       int a1)
    = CallMsgFilterA;

BOOL (__stdcall * Real_CallMsgFilterW)(LPMSG a0,
                                       int a1)
    = CallMsgFilterW;

BOOL (__stdcall * Real_CallNamedPipeA)(LPCSTR a0,
                                       LPVOID a1,
                                       DWORD a2,
                                       LPVOID a3,
                                       DWORD a4,
                                       LPDWORD a5,
                                       DWORD a6)
    = CallNamedPipeA;

BOOL (__stdcall * Real_CallNamedPipeW)(LPCWSTR a0,
                                       LPVOID a1,
                                       DWORD a2,
                                       LPVOID a3,
                                       DWORD a4,
                                       LPDWORD a5,
                                       DWORD a6)
    = CallNamedPipeW;

LRESULT (__stdcall * Real_CallNextHookEx)(HHOOK a0,
                                          int a1,
                                          WPARAM a2,
                                          LPARAM a3)
    = CallNextHookEx;

LRESULT (__stdcall * Real_CallWindowProcA)(WNDPROC a0,
                                           HWND a1,
                                           UINT a2,
                                           WPARAM a3,
                                           LPARAM a4)
    = CallWindowProcA;

LRESULT (__stdcall * Real_CallWindowProcW)(WNDPROC a0,
                                           HWND a1,
                                           UINT a2,
                                           WPARAM a3,
                                           LPARAM a4)
    = CallWindowProcW;

BOOL (__stdcall * Real_CancelDC)(HDC a0)
    = CancelDC;

BOOL (__stdcall * Real_CancelIo)(HANDLE a0)
    = CancelIo;

BOOL (__stdcall * Real_CancelWaitableTimer)(HANDLE a0)
    = CancelWaitableTimer;

WORD (__stdcall * Real_CascadeWindows)(HWND a0,
                                       UINT a1,
                                       CONST RECT* a2,
                                       UINT a3,
                                       CONST HWND * a4)
    = CascadeWindows;

BOOL (__stdcall * Real_ChangeClipboardChain)(HWND a0,
                                             HWND a1)
    = ChangeClipboardChain;

LONG (__stdcall * Real_ChangeDisplaySettingsA)(LPDEVMODEA a0,
                                               DWORD a1)
    = ChangeDisplaySettingsA;

LONG (__stdcall * Real_ChangeDisplaySettingsExA)(LPCSTR a0,
                                                 LPDEVMODEA a1,
                                                 HWND a2,
                                                 DWORD a3,
                                                 LPVOID a4)
    = ChangeDisplaySettingsExA;

LONG (__stdcall * Real_ChangeDisplaySettingsExW)(LPCWSTR a0,
                                                 LPDEVMODEW a1,
                                                 HWND a2,
                                                 DWORD a3,
                                                 LPVOID a4)
    = ChangeDisplaySettingsExW;

LONG (__stdcall * Real_ChangeDisplaySettingsW)(LPDEVMODEW a0,
                                               DWORD a1)
    = ChangeDisplaySettingsW;

BOOL (__stdcall * Real_ChangeMenuA)(HMENU a0,
                                    UINT a1,
                                    LPCSTR a2,
                                    UINT a3,
                                    UINT a4)
    = ChangeMenuA;

BOOL (__stdcall * Real_ChangeMenuW)(HMENU a0,
                                    UINT a1,
                                    LPCWSTR a2,
                                    UINT a3,
                                    UINT a4)
    = ChangeMenuW;

LPSTR (__stdcall * Real_CharLowerA)(LPSTR a0)
    = CharLowerA;

DWORD (__stdcall * Real_CharLowerBuffA)(LPSTR a0,
                                        DWORD a1)
    = CharLowerBuffA;

DWORD (__stdcall * Real_CharLowerBuffW)(LPWSTR a0,
                                        DWORD a1)
    = CharLowerBuffW;

LPWSTR (__stdcall * Real_CharLowerW)(LPWSTR a0)
    = CharLowerW;

LPSTR (__stdcall * Real_CharNextA)(LPCSTR a0)
    = CharNextA;

LPSTR (__stdcall * Real_CharNextExA)(WORD a0,
                                     LPCSTR a1,
                                     DWORD a2)
    = CharNextExA;

LPWSTR (__stdcall * Real_CharNextW)(LPCWSTR a0)
    = CharNextW;

LPSTR (__stdcall * Real_CharPrevA)(LPCSTR a0,
                                   LPCSTR a1)
    = CharPrevA;

LPSTR (__stdcall * Real_CharPrevExA)(WORD a0,
                                     LPCSTR a1,
                                     LPCSTR a2,
                                     DWORD a3)
    = CharPrevExA;

LPWSTR (__stdcall * Real_CharPrevW)(LPCWSTR a0,
                                    LPCWSTR a1)
    = CharPrevW;

BOOL (__stdcall * Real_CharToOemA)(LPCSTR a0,
                                   LPSTR a1)
    = CharToOemA;

BOOL (__stdcall * Real_CharToOemBuffA)(LPCSTR a0,
                                       LPSTR a1,
                                       DWORD a2)
    = CharToOemBuffA;

BOOL (__stdcall * Real_CharToOemBuffW)(LPCWSTR a0,
                                       LPSTR a1,
                                       DWORD a2)
    = CharToOemBuffW;

BOOL (__stdcall * Real_CharToOemW)(LPCWSTR a0,
                                   LPSTR a1)
    = CharToOemW;

LPSTR (__stdcall * Real_CharUpperA)(LPSTR a0)
    = CharUpperA;

DWORD (__stdcall * Real_CharUpperBuffA)(LPSTR a0,
                                        DWORD a1)
    = CharUpperBuffA;

DWORD (__stdcall * Real_CharUpperBuffW)(LPWSTR a0,
                                        DWORD a1)
    = CharUpperBuffW;

LPWSTR (__stdcall * Real_CharUpperW)(LPWSTR a0)
    = CharUpperW;

BOOL (__stdcall * Real_CheckColorsInGamut)(
                                           HDC a0,
#ifdef GDIPLUS_TS_QUERYVER
                                           LPRGBTRIPLE a1,
#else
                                           LPVOID a1,
#endif
                                           LPVOID a2,
                                           DWORD a3
                                          )
    = CheckColorsInGamut;

BOOL (__stdcall * Real_CheckDlgButton)(HWND a0,
                                       int a1,
                                       UINT a2)
    = CheckDlgButton;

DWORD (__stdcall * Real_CheckMenuItem)(HMENU a0,
                                       UINT a1,
                                       UINT a2)
    = CheckMenuItem;

BOOL (__stdcall * Real_CheckMenuRadioItem)(HMENU a0,
                                           UINT a1,
                                           UINT a2,
                                           UINT a3,
                                           UINT a4)
    = CheckMenuRadioItem;

BOOL (__stdcall * Real_CheckRadioButton)(HWND a0,
                                         int a1,
                                         int a2,
                                         int a3)
    = CheckRadioButton;

HWND (__stdcall * Real_ChildWindowFromPoint)(HWND a0,
                                             POINT a1)
    = ChildWindowFromPoint;

HWND (__stdcall * Real_ChildWindowFromPointEx)(HWND a0,
                                               POINT a1,
                                               UINT a2)
    = ChildWindowFromPointEx;

int (__stdcall * Real_ChoosePixelFormat)(HDC a0,
                                         CONST PIXELFORMATDESCRIPTOR* a1)
    = ChoosePixelFormat;

BOOL (__stdcall * Real_Chord)(HDC a0,
                              int a1,
                              int a2,
                              int a3,
                              int a4,
                              int a5,
                              int a6,
                              int a7,
                              int a8)
    = Chord;

BOOL (__stdcall * Real_ClearCommBreak)(HANDLE a0)
    = ClearCommBreak;

BOOL (__stdcall * Real_ClearCommError)(HANDLE a0,
                                       LPDWORD a1,
                                       LPCOMSTAT a2)
    = ClearCommError;

BOOL (__stdcall * Real_ClientToScreen)(HWND a0,
                                       LPPOINT a1)
    = ClientToScreen;

BOOL (__stdcall * Real_ClipCursor)(CONST RECT* a0)
    = ClipCursor;

BOOL (__stdcall * Real_CloseClipboard)(void)
    = CloseClipboard;

BOOL (__stdcall * Real_CloseDesktop)(HDESK a0)
    = CloseDesktop;

HENHMETAFILE (__stdcall * Real_CloseEnhMetaFile)(HDC a0)
    = CloseEnhMetaFile;

BOOL (__stdcall * Real_CloseFigure)(HDC a0)
    = CloseFigure;

BOOL (__stdcall * Real_CloseHandle)(HANDLE a0)
    = CloseHandle;

HMETAFILE (__stdcall * Real_CloseMetaFile)(HDC a0)
    = CloseMetaFile;

BOOL (__stdcall * Real_CloseWindow)(HWND a0)
    = CloseWindow;

BOOL (__stdcall * Real_CloseWindowStation)(HWINSTA a0)
    = CloseWindowStation;

ULONG (__stdcall * Real_CoAddRefServerProcess)(void)
    = CoAddRefServerProcess;

DWORD (__stdcall * Real_CoBuildVersion)(void)
    = CoBuildVersion;

HRESULT (__stdcall * Real_CoCopyProxy)(IUnknown* a0,
                                       IUnknown** a1)
    = CoCopyProxy;

HRESULT (__stdcall * Real_CoCreateFreeThreadedMarshaler)(LPUNKNOWN a0,
                                                         LPUNKNOWN* a1)
    = CoCreateFreeThreadedMarshaler;

HRESULT (__stdcall * Real_CoCreateGuid)(GUID* a0)
    = CoCreateGuid;

HRESULT (__stdcall * Real_CoCreateInstance)(CONST IID& a0,
                                            LPUNKNOWN a1,
                                            DWORD a2,
                                            CONST IID& a3,
                                            LPVOID* a4)
    = CoCreateInstance;

HRESULT (__stdcall * Real_CoCreateInstanceEx)(CONST IID& a0,
                                              IUnknown* a1,
                                              DWORD a2,
                                              COSERVERINFO* a3,
                                              DWORD a4,
                                              MULTI_QI* a5)
    = CoCreateInstanceEx;

HRESULT (__stdcall * Real_CoDisconnectObject)(LPUNKNOWN a0,
                                              DWORD a1)
    = CoDisconnectObject;

BOOL (__stdcall * Real_CoDosDateTimeToFileTime)(WORD a0,
                                                WORD a1,
                                                FILETIME* a2)
    = CoDosDateTimeToFileTime;

HRESULT (__stdcall * Real_CoFileTimeNow)(FILETIME* a0)
    = CoFileTimeNow;

BOOL (__stdcall * Real_CoFileTimeToDosDateTime)(FILETIME* a0,
                                                LPWORD a1,
                                                LPWORD a2)
    = CoFileTimeToDosDateTime;

void (__stdcall * Real_CoFreeAllLibraries)(void)
    = CoFreeAllLibraries;

void (__stdcall * Real_CoFreeLibrary)(HINSTANCE a0)
    = CoFreeLibrary;

void (__stdcall * Real_CoFreeUnusedLibraries)(void)
    = CoFreeUnusedLibraries;

HRESULT (__stdcall * Real_CoGetCallContext)(CONST IID& a0,
                                            void** a1)
    = CoGetCallContext;

HRESULT (__stdcall * Real_CoGetClassObject)(CONST IID& a0,
                                            DWORD a1,
                                            LPVOID a2,
                                            CONST IID& a3,
                                            LPVOID* a4)
    = CoGetClassObject;

DWORD (__stdcall * Real_CoGetCurrentProcess)(void)
    = CoGetCurrentProcess;

HRESULT (__stdcall * Real_CoGetInstanceFromFile)(COSERVERINFO* a0,
                                                 CLSID* a1,
                                                 IUnknown* a2,
                                                 DWORD a3,
                                                 DWORD a4,
                                                 OLECHAR* a5,
                                                 DWORD a6,
                                                 MULTI_QI* a7)
    = CoGetInstanceFromFile;

HRESULT (__stdcall * Real_CoGetInstanceFromIStorage)(COSERVERINFO* a0,
                                                     CLSID* a1,
                                                     IUnknown* a2,
                                                     DWORD a3,
                                                     IStorage* a4,
                                                     DWORD a5,
                                                     MULTI_QI* a6)
    = CoGetInstanceFromIStorage;

HRESULT (__stdcall * Real_CoGetInterfaceAndReleaseStream)(LPSTREAM a0,
                                                          CONST IID& a1,
                                                          LPVOID* a2)
    = CoGetInterfaceAndReleaseStream;

HRESULT (__stdcall * Real_CoGetMalloc)(DWORD a0,
                                       IMalloc** a1)
    = CoGetMalloc;

HRESULT (__stdcall * Real_CoGetMarshalSizeMax)(ULONG* a0,
                                               CONST IID& a1,
                                               LPUNKNOWN a2,
                                               DWORD a3,
                                               LPVOID a4,
                                               DWORD a5)
    = CoGetMarshalSizeMax;

HRESULT (__stdcall * Real_CoGetObject)(LPCWSTR a0,
                                       BIND_OPTS* a1,
                                       CONST IID& a2,
                                       void** a3)
    = CoGetObject;

HRESULT (__stdcall * Real_CoGetPSClsid)(CONST IID& a0,
                                        CLSID* a1)
    = CoGetPSClsid;

HRESULT (__stdcall * Real_CoGetStandardMarshal)(CONST IID& a0,
                                                LPUNKNOWN a1,
                                                DWORD a2,
                                                LPVOID a3,
                                                DWORD a4,
                                                IMarshal** a5)
    = CoGetStandardMarshal;

HRESULT (__stdcall * Real_CoGetStdMarshalEx)(LPUNKNOWN a0,
                                             DWORD a1,
                                             LPUNKNOWN* a2)
    = CoGetStdMarshalEx;

HRESULT (__stdcall * Real_CoGetTreatAsClass)(CONST IID& a0,
                                             LPGUID a1)
    = CoGetTreatAsClass;

HRESULT (__stdcall * Real_CoImpersonateClient)(void)
    = CoImpersonateClient;

HRESULT (__stdcall * Real_CoInitialize)(LPVOID a0)
    = CoInitialize;

HRESULT (__stdcall * Real_CoInitializeEx)(LPVOID a0,
                                          DWORD a1)
    = CoInitializeEx;

HRESULT (__stdcall * Real_CoInitializeSecurity)(PSECURITY_DESCRIPTOR a0,
                                                LONG a1,
                                                SOLE_AUTHENTICATION_SERVICE* a2,
                                                void* a3,
                                                DWORD a4,
                                                DWORD a5,
                                                void* a6,
                                                DWORD a7,
                                                void* a8)
    = CoInitializeSecurity;

BOOL (__stdcall * Real_CoIsHandlerConnected)(LPUNKNOWN a0)
    = CoIsHandlerConnected;

BOOL (__stdcall * Real_CoIsOle1Class)(CONST IID& a0)
    = CoIsOle1Class;

HINSTANCE (__stdcall * Real_CoLoadLibrary)(LPOLESTR a0,
                                           BOOL a1)
    = CoLoadLibrary;

HRESULT (__stdcall * Real_CoLockObjectExternal)(LPUNKNOWN a0,
                                                BOOL a1,
                                                BOOL a2)
    = CoLockObjectExternal;

HRESULT (__stdcall * Real_CoMarshalHresult)(LPSTREAM a0,
                                            HRESULT a1)
    = CoMarshalHresult;

HRESULT (__stdcall * Real_CoMarshalInterThreadInterfaceInStream)(CONST IID& a0,
                                                                 LPUNKNOWN a1,
                                                                 LPSTREAM* a2)
    = CoMarshalInterThreadInterfaceInStream;

HRESULT (__stdcall * Real_CoMarshalInterface)(LPSTREAM a0,
                                              CONST IID& a1,
                                              LPUNKNOWN a2,
                                              DWORD a3,
                                              LPVOID a4,
                                              DWORD a5)
    = CoMarshalInterface;

HRESULT (__stdcall * Real_CoQueryAuthenticationServices)(DWORD* a0,
                                                         SOLE_AUTHENTICATION_SERVICE** a1)
    = CoQueryAuthenticationServices;

HRESULT (__stdcall * Real_CoQueryClientBlanket)(DWORD* a0,
                                                DWORD* a1,
                                                OLECHAR** a2,
                                                DWORD* a3,
                                                DWORD* a4,
                                                RPC_AUTHZ_HANDLE* a5,
                                                DWORD* a6)
    = CoQueryClientBlanket;

HRESULT (__stdcall * Real_CoQueryProxyBlanket)(IUnknown* a0,
                                               DWORD* a1,
                                               DWORD* a2,
                                               OLECHAR** a3,
                                               DWORD* a4,
                                               DWORD* a5,
                                               RPC_AUTH_IDENTITY_HANDLE* a6,
                                               DWORD* a7)
    = CoQueryProxyBlanket;

HRESULT (__stdcall * Real_CoRegisterChannelHook)(CONST GUID& a0,
                                                 IChannelHook* a1)
    = CoRegisterChannelHook;

HRESULT (__stdcall * Real_CoRegisterClassObject)(CONST IID& a0,
                                                 LPUNKNOWN a1,
                                                 DWORD a2,
                                                 DWORD a3,
                                                 LPDWORD a4)
    = CoRegisterClassObject;

HRESULT (__stdcall * Real_CoRegisterMallocSpy)(IMallocSpy* a0)
    = CoRegisterMallocSpy;

HRESULT (__stdcall * Real_CoRegisterMessageFilter)(LPMESSAGEFILTER a0,
                                                   LPMESSAGEFILTER* a1)
    = CoRegisterMessageFilter;

HRESULT (__stdcall * Real_CoRegisterPSClsid)(CONST IID& a0,
                                             CONST IID& a1)
    = CoRegisterPSClsid;

HRESULT (__stdcall * Real_CoRegisterSurrogate)(LPSURROGATE a0)
    = CoRegisterSurrogate;

HRESULT (__stdcall * Real_CoReleaseMarshalData)(LPSTREAM a0)
    = CoReleaseMarshalData;

ULONG (__stdcall * Real_CoReleaseServerProcess)(void)
    = CoReleaseServerProcess;

HRESULT (__stdcall * Real_CoResumeClassObjects)(void)
    = CoResumeClassObjects;

HRESULT (__stdcall * Real_CoRevertToSelf)(void)
    = CoRevertToSelf;

HRESULT (__stdcall * Real_CoRevokeClassObject)(DWORD a0)
    = CoRevokeClassObject;

HRESULT (__stdcall * Real_CoRevokeMallocSpy)(void)
    = CoRevokeMallocSpy;

HRESULT (__stdcall * Real_CoSetProxyBlanket)(IUnknown* a0,
                                             DWORD a1,
                                             DWORD a2,
                                             OLECHAR* a3,
                                             DWORD a4,
                                             DWORD a5,
                                             RPC_AUTH_IDENTITY_HANDLE a6,
                                             DWORD a7)
    = CoSetProxyBlanket;

HRESULT (__stdcall * Real_CoSuspendClassObjects)(void)
    = CoSuspendClassObjects;

HRESULT (__stdcall * Real_CoSwitchCallContext)(IUnknown* a0,
                                               IUnknown** a1)
    = CoSwitchCallContext;

LPVOID (__stdcall * Real_CoTaskMemAlloc)(SIZE_T a0)
    = CoTaskMemAlloc;

void (__stdcall * Real_CoTaskMemFree)(LPVOID a0)
    = CoTaskMemFree;

LPVOID (__stdcall * Real_CoTaskMemRealloc)(LPVOID a0,
                                           SIZE_T a1)
    = CoTaskMemRealloc;

HRESULT (__stdcall * Real_CoTreatAsClass)(CONST IID& a0,
                                          CONST IID& a1)
    = CoTreatAsClass;

void (__stdcall * Real_CoUninitialize)(void)
    = CoUninitialize;

HRESULT (__stdcall * Real_CoUnmarshalHresult)(LPSTREAM a0,
                                              HRESULT* a1)
    = CoUnmarshalHresult;

HRESULT (__stdcall * Real_CoUnmarshalInterface)(LPSTREAM a0,
                                                CONST IID& a1,
                                                LPVOID* a2)
    = CoUnmarshalInterface;

BOOL (__stdcall * Real_ColorMatchToTarget)(HDC a0,
                                           HDC a1,
                                           DWORD a2)
    = ColorMatchToTarget;

int (__stdcall * Real_CombineRgn)(HRGN a0,
                                  HRGN a1,
                                  HRGN a2,
                                  int a3)
    = CombineRgn;

BOOL (__stdcall * Real_CombineTransform)(LPXFORM a0,
                                         CONST XFORM* a1,
                                         CONST XFORM* a2)
    = CombineTransform;

BOOL (__stdcall * Real_CommConfigDialogA)(LPCSTR a0,
                                          HWND a1,
                                          LPCOMMCONFIG a2)
    = CommConfigDialogA;

BOOL (__stdcall * Real_CommConfigDialogW)(LPCWSTR a0,
                                          HWND a1,
                                          LPCOMMCONFIG a2)
    = CommConfigDialogW;

LONG (__stdcall * Real_CompareFileTime)(CONST FILETIME* a0,
                                        CONST FILETIME* a1)
    = CompareFileTime;

int (__stdcall * Real_CompareStringA)(LCID a0,
                                      DWORD a1,
                                      LPCSTR a2,
                                      int a3,
                                      LPCSTR a4,
                                      int a5)
    = CompareStringA;

int (__stdcall * Real_CompareStringW)(LCID a0,
                                      DWORD a1,
                                      LPCWSTR a2,
                                      int a3,
                                      LPCWSTR a4,
                                      int a5)
    = CompareStringW;

BOOL (__stdcall * Real_ConnectNamedPipe)(HANDLE a0,
                                         LPOVERLAPPED a1)
    = ConnectNamedPipe;

BOOL (__stdcall * Real_ContinueDebugEvent)(DWORD a0,
                                           DWORD a1,
                                           DWORD a2)
    = ContinueDebugEvent;

LCID (__stdcall * Real_ConvertDefaultLocale)(LCID a0)
    = ConvertDefaultLocale;

LPVOID (__stdcall * Real_ConvertThreadToFiber)(LPVOID a0)
    = ConvertThreadToFiber;

int (__stdcall * Real_CopyAcceleratorTableA)(HACCEL a0,
                                             LPACCEL a1,
                                             int a2)
    = CopyAcceleratorTableA;

int (__stdcall * Real_CopyAcceleratorTableW)(HACCEL a0,
                                             LPACCEL a1,
                                             int a2)
    = CopyAcceleratorTableW;

HENHMETAFILE (__stdcall * Real_CopyEnhMetaFileA)(HENHMETAFILE a0,
                                                 LPCSTR a1)
    = CopyEnhMetaFileA;

HENHMETAFILE (__stdcall * Real_CopyEnhMetaFileW)(HENHMETAFILE a0,
                                                 LPCWSTR a1)
    = CopyEnhMetaFileW;

BOOL (__stdcall * Real_CopyFileA)(LPCSTR a0,
                                  LPCSTR a1,
                                  BOOL a2)
    = CopyFileA;

BOOL (__stdcall * Real_CopyFileExA)(LPCSTR a0,
                                    LPCSTR a1,
                                    LPPROGRESS_ROUTINE a2,
                                    LPVOID a3,
                                    LPBOOL a4,
                                    DWORD a5)
    = CopyFileExA;

BOOL (__stdcall * Real_CopyFileExW)(LPCWSTR a0,
                                    LPCWSTR a1,
                                    LPPROGRESS_ROUTINE a2,
                                    LPVOID a3,
                                    LPBOOL a4,
                                    DWORD a5)
    = CopyFileExW;

BOOL (__stdcall * Real_CopyFileW)(LPCWSTR a0,
                                  LPCWSTR a1,
                                  BOOL a2)
    = CopyFileW;

HICON (__stdcall * Real_CopyIcon)(HICON a0)
    = CopyIcon;

HANDLE (__stdcall * Real_CopyImage)(HANDLE a0,
                                    UINT a1,
                                    int a2,
                                    int a3,
                                    UINT a4)
    = CopyImage;

HMETAFILE (__stdcall * Real_CopyMetaFileA)(HMETAFILE a0,
                                           LPCSTR a1)
    = CopyMetaFileA;

HMETAFILE (__stdcall * Real_CopyMetaFileW)(HMETAFILE a0,
                                           LPCWSTR a1)
    = CopyMetaFileW;

BOOL (__stdcall * Real_CopyRect)(LPRECT a0,
                                 CONST RECT* a1)
    = CopyRect;

int (__stdcall * Real_CountClipboardFormats)(void)
    = CountClipboardFormats;

HACCEL (__stdcall * Real_CreateAcceleratorTableA)(LPACCEL a0,
                                                  int a1)
    = CreateAcceleratorTableA;

HACCEL (__stdcall * Real_CreateAcceleratorTableW)(LPACCEL a0,
                                                  int a1)
    = CreateAcceleratorTableW;

HRESULT (__stdcall * Real_CreateAntiMoniker)(IMoniker** a0)
    = CreateAntiMoniker;

HRESULT (__stdcall * Real_CreateBindCtx)(DWORD a0,
                                         IBindCtx** a1)
    = CreateBindCtx;

HBITMAP (__stdcall * Real_CreateBitmap)(int a0,
                                        int a1,
                                        UINT a2,
                                        UINT a3,
                                        CONST void* a4)
    = CreateBitmap;

HBITMAP (__stdcall * Real_CreateBitmapIndirect)(CONST BITMAP* a0)
    = CreateBitmapIndirect;

HBRUSH (__stdcall * Real_CreateBrushIndirect)(CONST LOGBRUSH* a0)
    = CreateBrushIndirect;

BOOL (__stdcall * Real_CreateCaret)(HWND a0,
                                    HBITMAP a1,
                                    int a2,
                                    int a3)
    = CreateCaret;

HRESULT (__stdcall * Real_CreateClassMoniker)(CONST IID& a0,
                                              IMoniker** a1)
    = CreateClassMoniker;

HCOLORSPACE (__stdcall * Real_CreateColorSpaceA)(LPLOGCOLORSPACEA a0)
    = CreateColorSpaceA;

HCOLORSPACE (__stdcall * Real_CreateColorSpaceW)(LPLOGCOLORSPACEW a0)
    = CreateColorSpaceW;

HBITMAP (__stdcall * Real_CreateCompatibleBitmap)(HDC a0,
                                                  int a1,
                                                  int a2)
    = CreateCompatibleBitmap;

HDC (__stdcall * Real_CreateCompatibleDC)(HDC a0)
    = CreateCompatibleDC;

HANDLE (__stdcall * Real_CreateConsoleScreenBuffer)(DWORD a0,
                                                    DWORD a1,
                                                    CONST SECURITY_ATTRIBUTES* a2,
                                                    DWORD a3,
                                                    LPVOID a4)
    = CreateConsoleScreenBuffer;

HCURSOR (__stdcall * Real_CreateCursor)(HINSTANCE a0,
                                        int a1,
                                        int a2,
                                        int a3,
                                        int a4,
                                        CONST void* a5,
                                        CONST void* a6)
    = CreateCursor;

HDC (__stdcall * Real_CreateDCA)(LPCSTR a0,
                                 LPCSTR a1,
                                 LPCSTR a2,
                                 CONST DEVMODEA * a3)
    = CreateDCA;

HDC (__stdcall * Real_CreateDCW)(LPCWSTR a0,
                                 LPCWSTR a1,
                                 LPCWSTR a2,
                                 CONST DEVMODEW * a3)
    = CreateDCW;

HBRUSH (__stdcall * Real_CreateDIBPatternBrush)(HGLOBAL a0,
                                                UINT a1)
    = CreateDIBPatternBrush;

HBRUSH (__stdcall * Real_CreateDIBPatternBrushPt)(CONST void* a0,
                                                  UINT a1)
    = CreateDIBPatternBrushPt;

HBITMAP (__stdcall * Real_CreateDIBSection)(HDC a0,
                                            CONST BITMAPINFO* a1,
                                            UINT a2,
                                            void** a3,
                                            HANDLE a4,
                                            DWORD a5)
    = CreateDIBSection;

HBITMAP (__stdcall * Real_CreateDIBitmap)(HDC a0,
                                          CONST BITMAPINFOHEADER* a1,
                                          DWORD a2,
                                          CONST void* a3,
                                          CONST BITMAPINFO* a4,
                                          UINT a5)
    = CreateDIBitmap;

HRESULT (__stdcall * Real_CreateDataAdviseHolder)(LPDATAADVISEHOLDER* a0)
    = CreateDataAdviseHolder;

HRESULT (__stdcall * Real_CreateDataCache)(LPUNKNOWN a0,
                                           CONST IID& a1,
                                           CONST IID& a2,
                                           LPVOID* a3)
    = CreateDataCache;

#if _MSC_VER < 1300
HDESK (__stdcall * Real_CreateDesktopA)(LPSTR a0,
                                        LPSTR a1,
                                        LPDEVMODEA a2,
                                        DWORD a3,
                                        ACCESS_MASK a4,
                                        LPSECURITY_ATTRIBUTES a5)
    = CreateDesktopA;
#else
HDESK (__stdcall * Real_CreateDesktopA)(LPCSTR a0,
                                        LPCSTR a1,
                                        LPDEVMODEA a2,
                                        DWORD a3,
                                        ACCESS_MASK a4,
                                        LPSECURITY_ATTRIBUTES a5)
    = CreateDesktopA;

#endif

#if _MSC_VER < 1300
HDESK (__stdcall * Real_CreateDesktopW)(LPWSTR a0,
                                        LPWSTR a1,
                                        LPDEVMODEW a2,
                                        DWORD a3,
                                        ACCESS_MASK a4,
                                        LPSECURITY_ATTRIBUTES a5)
    = CreateDesktopW;
#else
HDESK (__stdcall * Real_CreateDesktopW)(LPCWSTR a0,
                                        LPCWSTR a1,
                                        LPDEVMODEW a2,
                                        DWORD a3,
                                        ACCESS_MASK a4,
                                        LPSECURITY_ATTRIBUTES a5)
    = CreateDesktopW;
#endif

HWND (__stdcall * Real_CreateDialogIndirectParamA)(HINSTANCE a0,
                                                   LPCDLGTEMPLATEA a1,
                                                   HWND a2,
                                                   DLGPROC a3,
                                                   LPARAM a4)
    = CreateDialogIndirectParamA;

HWND (__stdcall * Real_CreateDialogIndirectParamW)(HINSTANCE a0,
                                                   LPCDLGTEMPLATEW a1,
                                                   HWND a2,
                                                   DLGPROC a3,
                                                   LPARAM a4)
    = CreateDialogIndirectParamW;

HWND (__stdcall * Real_CreateDialogParamA)(HINSTANCE a0,
                                           LPCSTR a1,
                                           HWND a2,
                                           DLGPROC a3,
                                           LPARAM a4)
    = CreateDialogParamA;

HWND (__stdcall * Real_CreateDialogParamW)(HINSTANCE a0,
                                           LPCWSTR a1,
                                           HWND a2,
                                           DLGPROC a3,
                                           LPARAM a4)
    = CreateDialogParamW;

BOOL (__stdcall * Real_CreateDirectoryA)(LPCSTR a0,
                                         LPSECURITY_ATTRIBUTES a1)
    = CreateDirectoryA;

BOOL (__stdcall * Real_CreateDirectoryExA)(LPCSTR a0,
                                           LPCSTR a1,
                                           LPSECURITY_ATTRIBUTES a2)
    = CreateDirectoryExA;

BOOL (__stdcall * Real_CreateDirectoryExW)(LPCWSTR a0,
                                           LPCWSTR a1,
                                           LPSECURITY_ATTRIBUTES a2)
    = CreateDirectoryExW;

BOOL (__stdcall * Real_CreateDirectoryW)(LPCWSTR a0,
                                         LPSECURITY_ATTRIBUTES a1)
    = CreateDirectoryW;

HBITMAP (__stdcall * Real_CreateDiscardableBitmap)(HDC a0,
                                                   int a1,
                                                   int a2)
    = CreateDiscardableBitmap;

HRGN (__stdcall * Real_CreateEllipticRgn)(int a0,
                                          int a1,
                                          int a2,
                                          int a3)
    = CreateEllipticRgn;

HRGN (__stdcall * Real_CreateEllipticRgnIndirect)(CONST RECT* a0)
    = CreateEllipticRgnIndirect;

HDC (__stdcall * Real_CreateEnhMetaFileA)(HDC a0,
                                          LPCSTR a1,
                                          CONST RECT* a2,
                                          LPCSTR a3)
    = CreateEnhMetaFileA;

HDC (__stdcall * Real_CreateEnhMetaFileW)(HDC a0,
                                          LPCWSTR a1,
                                          CONST RECT* a2,
                                          LPCWSTR a3)
    = CreateEnhMetaFileW;

HANDLE (__stdcall * Real_CreateEventA)(LPSECURITY_ATTRIBUTES a0,
                                       BOOL a1,
                                       BOOL a2,
                                       LPCSTR a3)
    = CreateEventA;

HANDLE (__stdcall * Real_CreateEventW)(LPSECURITY_ATTRIBUTES a0,
                                       BOOL a1,
                                       BOOL a2,
                                       LPCWSTR a3)
    = CreateEventW;

LPVOID (__stdcall * Real_CreateFiber)(ULONG_PTR a0,
                                      LPFIBER_START_ROUTINE a1,
                                      LPVOID a2)
    = CreateFiber;

HANDLE (__stdcall * Real_CreateFileA)(LPCSTR a0,
                                      DWORD a1,
                                      DWORD a2,
                                      LPSECURITY_ATTRIBUTES a3,
                                      DWORD a4,
                                      DWORD a5,
                                      HANDLE a6)
    = CreateFileA;

HANDLE (__stdcall * Real_CreateFileMappingA)(HANDLE a0,
                                             LPSECURITY_ATTRIBUTES a1,
                                             DWORD a2,
                                             DWORD a3,
                                             DWORD a4,
                                             LPCSTR a5)
    = CreateFileMappingA;

HANDLE (__stdcall * Real_CreateFileMappingW)(HANDLE a0,
                                             LPSECURITY_ATTRIBUTES a1,
                                             DWORD a2,
                                             DWORD a3,
                                             DWORD a4,
                                             LPCWSTR a5)
    = CreateFileMappingW;

HRESULT (__stdcall * Real_CreateFileMoniker)(LPCOLESTR a0,
                                             IMoniker** a1)
    = CreateFileMoniker;

HANDLE (__stdcall * Real_CreateFileW)(LPCWSTR a0,
                                      DWORD a1,
                                      DWORD a2,
                                      LPSECURITY_ATTRIBUTES a3,
                                      DWORD a4,
                                      DWORD a5,
                                      HANDLE a6)
    = CreateFileW;

HFONT (__stdcall * Real_CreateFontA)(int a0,
                                     int a1,
                                     int a2,
                                     int a3,
                                     int a4,
                                     DWORD a5,
                                     DWORD a6,
                                     DWORD a7,
                                     DWORD a8,
                                     DWORD a9,
                                     DWORD a10,
                                     DWORD a11,
                                     DWORD a12,
                                     LPCSTR a13)
    = CreateFontA;

HFONT (__stdcall * Real_CreateFontIndirectA)(CONST LOGFONTA* a0)
    = CreateFontIndirectA;

HFONT (__stdcall * Real_CreateFontIndirectW)(CONST LOGFONTW* a0)
    = CreateFontIndirectW;

HFONT (__stdcall * Real_CreateFontW)(int a0,
                                     int a1,
                                     int a2,
                                     int a3,
                                     int a4,
                                     DWORD a5,
                                     DWORD a6,
                                     DWORD a7,
                                     DWORD a8,
                                     DWORD a9,
                                     DWORD a10,
                                     DWORD a11,
                                     DWORD a12,
                                     LPCWSTR a13)
    = CreateFontW;

HRESULT (__stdcall * Real_CreateGenericComposite)(IMoniker* a0,
                                                  IMoniker* a1,
                                                  IMoniker** a2)
    = CreateGenericComposite;

HPALETTE (__stdcall * Real_CreateHalftonePalette)(HDC a0)
    = CreateHalftonePalette;

HBRUSH (__stdcall * Real_CreateHatchBrush)(int a0,
                                           COLORREF a1)
    = CreateHatchBrush;

HDC (__stdcall * Real_CreateICA)(LPCSTR a0,
                                 LPCSTR a1,
                                 LPCSTR a2,
                                 CONST DEVMODEA* a3)
    = CreateICA;

HDC (__stdcall * Real_CreateICW)(LPCWSTR a0,
                                 LPCWSTR a1,
                                 LPCWSTR a2,
                                 CONST DEVMODEW* a3)
    = CreateICW;

HRESULT (__stdcall * Real_CreateILockBytesOnHGlobal)(HGLOBAL a0,
                                                     BOOL a1,
                                                     ILockBytes** a2)
    = CreateILockBytesOnHGlobal;

HICON (__stdcall * Real_CreateIcon)(HINSTANCE a0,
                                    int a1,
                                    int a2,
                                    BYTE a3,
                                    BYTE a4,
                                    CONST BYTE* a5,
                                    CONST BYTE* a6)
    = CreateIcon;

HICON (__stdcall * Real_CreateIconFromResource)(PBYTE a0,
                                                DWORD a1,
                                                BOOL a2,
                                                DWORD a3)
    = CreateIconFromResource;

HICON (__stdcall * Real_CreateIconFromResourceEx)(PBYTE a0,
                                                  DWORD a1,
                                                  BOOL a2,
                                                  DWORD a3,
                                                  int a4,
                                                  int a5,
                                                  UINT a6)
    = CreateIconFromResourceEx;

HICON (__stdcall * Real_CreateIconIndirect)(PICONINFO a0)
    = CreateIconIndirect;

HANDLE (__stdcall * Real_CreateIoCompletionPort)(HANDLE a0,
                                                 HANDLE a1,
                                                 ULONG_PTR a2,
                                                 DWORD a3)
    = CreateIoCompletionPort;

HRESULT (__stdcall * Real_CreateItemMoniker)(LPCOLESTR a0,
                                             LPCOLESTR a1,
                                             IMoniker** a2)
    = CreateItemMoniker;

#if _MSC_VER < 1300
HWND (__stdcall * Real_CreateMDIWindowA)(LPSTR a0,
                                         LPSTR a1,
                                         DWORD a2,
                                         int a3,
                                         int a4,
                                         int a5,
                                         int a6,
                                         HWND a7,
                                         HINSTANCE a8,
                                         LPARAM a9)
    = CreateMDIWindowA;
#else
HWND (__stdcall * Real_CreateMDIWindowA)(LPCSTR a0,
                                         LPCSTR a1,
                                         DWORD a2,
                                         int a3,
                                         int a4,
                                         int a5,
                                         int a6,
                                         HWND a7,
                                         HINSTANCE a8,
                                         LPARAM a9)
    = CreateMDIWindowA;
#endif

#if _MSC_VER < 1300
HWND (__stdcall * Real_CreateMDIWindowW)(LPWSTR a0,
                                         LPWSTR a1,
                                         DWORD a2,
                                         int a3,
                                         int a4,
                                         int a5,
                                         int a6,
                                         HWND a7,
                                         HINSTANCE a8,
                                         LPARAM a9)
    = CreateMDIWindowW;
#else
HWND (__stdcall * Real_CreateMDIWindowW)(LPCWSTR a0,
                                         LPCWSTR a1,
                                         DWORD a2,
                                         int a3,
                                         int a4,
                                         int a5,
                                         int a6,
                                         HWND a7,
                                         HINSTANCE a8,
                                         LPARAM a9)
    = CreateMDIWindowW;
#endif

HANDLE (__stdcall * Real_CreateMailslotA)(LPCSTR a0,
                                          DWORD a1,
                                          DWORD a2,
                                          LPSECURITY_ATTRIBUTES a3)
    = CreateMailslotA;

HANDLE (__stdcall * Real_CreateMailslotW)(LPCWSTR a0,
                                          DWORD a1,
                                          DWORD a2,
                                          LPSECURITY_ATTRIBUTES a3)
    = CreateMailslotW;

HMENU (__stdcall * Real_CreateMenu)(void)
    = CreateMenu;

HDC (__stdcall * Real_CreateMetaFileA)(LPCSTR a0)
    = CreateMetaFileA;

HDC (__stdcall * Real_CreateMetaFileW)(LPCWSTR a0)
    = CreateMetaFileW;

HANDLE (__stdcall * Real_CreateMutexA)(LPSECURITY_ATTRIBUTES a0,
                                       BOOL a1,
                                       LPCSTR a2)
    = CreateMutexA;

HANDLE (__stdcall * Real_CreateMutexW)(LPSECURITY_ATTRIBUTES a0,
                                       BOOL a1,
                                       LPCWSTR a2)
    = CreateMutexW;

HANDLE (__stdcall * Real_CreateNamedPipeA)(LPCSTR a0,
                                           DWORD a1,
                                           DWORD a2,
                                           DWORD a3,
                                           DWORD a4,
                                           DWORD a5,
                                           DWORD a6,
                                           LPSECURITY_ATTRIBUTES a7)
    = CreateNamedPipeA;

HANDLE (__stdcall * Real_CreateNamedPipeW)(LPCWSTR a0,
                                           DWORD a1,
                                           DWORD a2,
                                           DWORD a3,
                                           DWORD a4,
                                           DWORD a5,
                                           DWORD a6,
                                           LPSECURITY_ATTRIBUTES a7)
    = CreateNamedPipeW;

HRESULT (__stdcall * Real_CreateOleAdviseHolder)(LPOLEADVISEHOLDER* a0)
    = CreateOleAdviseHolder;

HPALETTE (__stdcall * Real_CreatePalette)(CONST LOGPALETTE* a0)
    = CreatePalette;

HBRUSH (__stdcall * Real_CreatePatternBrush)(HBITMAP a0)
    = CreatePatternBrush;

HPEN (__stdcall * Real_CreatePen)(int a0,
                                  int a1,
                                  COLORREF a2)
    = CreatePen;

HPEN (__stdcall * Real_CreatePenIndirect)(CONST LOGPEN* a0)
    = CreatePenIndirect;

BOOL (__stdcall * Real_CreatePipe)(PHANDLE a0,
                                   PHANDLE a1,
                                   LPSECURITY_ATTRIBUTES a2,
                                   DWORD a3)
    = CreatePipe;

HRESULT (__stdcall * Real_CreatePointerMoniker)(LPUNKNOWN a0,
                                                IMoniker** a1)
    = CreatePointerMoniker;

HRGN (__stdcall * Real_CreatePolyPolygonRgn)(CONST POINT* a0,
                                             CONST INT* a1,
                                             int a2,
                                             int a3)
    = CreatePolyPolygonRgn;

HRGN (__stdcall * Real_CreatePolygonRgn)(CONST POINT* a0,
                                         int a1,
                                         int a2)
    = CreatePolygonRgn;

HMENU (__stdcall * Real_CreatePopupMenu)(void)
    = CreatePopupMenu;

BOOL (__stdcall * Real_CreateProcessA)(LPCSTR a0,
                                       LPSTR a1,
                                       LPSECURITY_ATTRIBUTES a2,
                                       LPSECURITY_ATTRIBUTES a3,
                                       BOOL a4,
                                       DWORD a5,
                                       LPVOID a6,
                                       LPCSTR a7,
                                       LPSTARTUPINFOA a8,
                                       LPPROCESS_INFORMATION a9)
    = CreateProcessA;

BOOL (__stdcall * Real_CreateProcessW)(LPCWSTR a0,
                                       LPWSTR a1,
                                       LPSECURITY_ATTRIBUTES a2,
                                       LPSECURITY_ATTRIBUTES a3,
                                       BOOL a4,
                                       DWORD a5,
                                       LPVOID a6,
                                       LPCWSTR a7,
                                       LPSTARTUPINFOW a8,
                                       LPPROCESS_INFORMATION a9)
    = CreateProcessW;

BOOL (__stdcall * Real_CreateProcessAsUserA)(
  __in_opt     HANDLE hToken,
  __in_opt     LPCSTR lpApplicationName,
  __inout_opt  LPSTR lpCommandLine,
  __in_opt     LPSECURITY_ATTRIBUTES lpProcessAttributes,
  __in_opt     LPSECURITY_ATTRIBUTES lpThreadAttributes,
  __in         BOOL bInheritHandles,
  __in         DWORD dwCreationFlags,
  __in_opt     LPVOID lpEnvironment,
  __in_opt     LPCSTR lpCurrentDirectory,
  __in         LPSTARTUPINFOA lpStartupInfo,
  __out        LPPROCESS_INFORMATION lpProcessInformation
                                       ) = CreateProcessAsUserA;

BOOL (__stdcall * Real_CreateProcessAsUserW)(
  __in_opt     HANDLE hToken,
  __in_opt     LPCWSTR lpApplicationName,
  __inout_opt  LPWSTR lpCommandLine,
  __in_opt     LPSECURITY_ATTRIBUTES lpProcessAttributes,
  __in_opt     LPSECURITY_ATTRIBUTES lpThreadAttributes,
  __in         BOOL bInheritHandles,
  __in         DWORD dwCreationFlags,
  __in_opt     LPVOID lpEnvironment,
  __in_opt     LPCWSTR lpCurrentDirectory,
  __in         LPSTARTUPINFOW lpStartupInfo,
  __out        LPPROCESS_INFORMATION lpProcessInformation
) = CreateProcessAsUserW;

#if(_Real_WINNT >= 0x0500)

BOOL (WINAPI * Real_CreateProcessWithLogonW)(
  __in         LPCWSTR lpUsername,
  __in_opt     LPCWSTR lpDomain,
  __in         LPCWSTR lpPassword,
  __in         DWORD dwLogonFlags,
  __in_opt     LPCWSTR lpApplicationName,
  __inout_opt  LPWSTR lpCommandLine,
  __in         DWORD dwCreationFlags,
  __in_opt     LPVOID lpEnvironment,
  __in_opt     LPCWSTR lpCurrentDirectory,
  __in         LPSTARTUPINFOW lpStartupInfo,
  __out        LPPROCESS_INFORMATION lpProcessInfo
) = CreateProcessWithLogonW;

BOOL (WINAPI * Real_CreateProcessWithTokenW)(
  __in         HANDLE hToken,
  __in         DWORD dwLogonFlags,
  __in_opt     LPCWSTR lpApplicationName,
  __inout_opt  LPWSTR lpCommandLine,
  __in         DWORD dwCreationFlags,
  __in_opt     LPVOID lpEnvironment,
  __in_opt     LPCWSTR lpCurrentDirectory,
  __in         LPSTARTUPINFOW lpStartupInfo,
  __out        LPPROCESS_INFORMATION lpProcessInfo
) = CreateProcessWithTokenW;
#endif

HRGN (__stdcall * Real_CreateRectRgn)(int a0,
                                      int a1,
                                      int a2,
                                      int a3)
    = CreateRectRgn;

HRGN (__stdcall * Real_CreateRectRgnIndirect)(CONST RECT* a0)
    = CreateRectRgnIndirect;

HANDLE (__stdcall * Real_CreateRemoteThread)(HANDLE a0,
                                             LPSECURITY_ATTRIBUTES a1,
                                             ULONG_PTR a2,
                                             LPTHREAD_START_ROUTINE a3,
                                             LPVOID a4,
                                             DWORD a5,
                                             LPDWORD a6)
    = CreateRemoteThread;

HRGN (__stdcall * Real_CreateRoundRectRgn)(int a0,
                                           int a1,
                                           int a2,
                                           int a3,
                                           int a4,
                                           int a5)
    = CreateRoundRectRgn;

BOOL (__stdcall * Real_CreateScalableFontResourceA)(DWORD a0,
                                                    LPCSTR a1,
                                                    LPCSTR a2,
                                                    LPCSTR a3)
    = CreateScalableFontResourceA;

BOOL (__stdcall * Real_CreateScalableFontResourceW)(DWORD a0,
                                                    LPCWSTR a1,
                                                    LPCWSTR a2,
                                                    LPCWSTR a3)
    = CreateScalableFontResourceW;

HANDLE (__stdcall * Real_CreateSemaphoreA)(LPSECURITY_ATTRIBUTES a0,
                                           LONG a1,
                                           LONG a2,
                                           LPCSTR a3)
    = CreateSemaphoreA;

HANDLE (__stdcall * Real_CreateSemaphoreW)(LPSECURITY_ATTRIBUTES a0,
                                           LONG a1,
                                           LONG a2,
                                           LPCWSTR a3)
    = CreateSemaphoreW;

HBRUSH (__stdcall * Real_CreateSolidBrush)(COLORREF a0)
    = CreateSolidBrush;

HRESULT (__stdcall * Real_CreateStdProgressIndicator)(HWND a0,
                                                      LPCOLESTR a1,
                                                      IBindStatusCallback* a2,
                                                      IBindStatusCallback** a3)
    = CreateStdProgressIndicator;

HRESULT (__stdcall * Real_CreateStreamOnHGlobal)(HGLOBAL a0,
                                                 BOOL a1,
                                                 LPSTREAM* a2)
    = CreateStreamOnHGlobal;

DWORD (__stdcall * Real_CreateTapePartition)(HANDLE a0,
                                             DWORD a1,
                                             DWORD a2,
                                             DWORD a3)
    = CreateTapePartition;

HANDLE (__stdcall * Real_CreateThread)(LPSECURITY_ATTRIBUTES a0,
                                       ULONG_PTR a1,
                                       LPTHREAD_START_ROUTINE a2,
                                       LPVOID a3,
                                       DWORD a4,
                                       LPDWORD a5)
    = CreateThread;

HANDLE (__stdcall * Real_CreateWaitableTimerA)(LPSECURITY_ATTRIBUTES a0,
                                               BOOL a1,
                                               LPCSTR a2)
    = CreateWaitableTimerA;

HANDLE (__stdcall * Real_CreateWaitableTimerW)(LPSECURITY_ATTRIBUTES a0,
                                               BOOL a1,
                                               LPCWSTR a2)
    = CreateWaitableTimerW;

HWND (__stdcall * Real_CreateWindowExA)(DWORD a0,
                                        LPCSTR a1,
                                        LPCSTR a2,
                                        DWORD a3,
                                        int a4,
                                        int a5,
                                        int a6,
                                        int a7,
                                        HWND a8,
                                        HMENU a9,
                                        HINSTANCE a10,
                                        LPVOID a11)
    = CreateWindowExA;

HWND (__stdcall * Real_CreateWindowExW)(DWORD a0,
                                        LPCWSTR a1,
                                        LPCWSTR a2,
                                        DWORD a3,
                                        int a4,
                                        int a5,
                                        int a6,
                                        int a7,
                                        HWND a8,
                                        HMENU a9,
                                        HINSTANCE a10,
                                        LPVOID a11)
    = CreateWindowExW;

#if _MSC_VER < 1300
HWINSTA (__stdcall * Real_CreateWindowStationA)(LPSTR a0,
                                                DWORD a1,
                                                ACCESS_MASK a2,
                                                LPSECURITY_ATTRIBUTES a3)
    = CreateWindowStationA;
#else
HWINSTA (__stdcall * Real_CreateWindowStationA)(LPCSTR a0,
                                                DWORD a1,
                                                ACCESS_MASK a2,
                                                LPSECURITY_ATTRIBUTES a3)
    = CreateWindowStationA;
#endif

#if _MSC_VER < 1300
HWINSTA (__stdcall * Real_CreateWindowStationW)(LPWSTR a0,
                                                DWORD a1,
                                                ACCESS_MASK a2,
                                                LPSECURITY_ATTRIBUTES a3)
    = CreateWindowStationW;
#else
HWINSTA (__stdcall * Real_CreateWindowStationW)(LPCWSTR a0,
                                                DWORD a1,
                                                ACCESS_MASK a2,
                                                LPSECURITY_ATTRIBUTES a3)
    = CreateWindowStationW;
#endif

BOOL (__stdcall * Real_DPtoLP)(HDC a0,
                               POINT* a1,
                               int a2)
    = DPtoLP;

BOOL (__stdcall * Real_DdeAbandonTransaction)(DWORD a0,
                                              HCONV a1,
                                              DWORD a2)
    = DdeAbandonTransaction;

LPBYTE (__stdcall * Real_DdeAccessData)(HDDEDATA a0,
                                        LPDWORD a1)
    = DdeAccessData;

HDDEDATA (__stdcall * Real_DdeAddData)(HDDEDATA a0,
                                       LPBYTE a1,
                                       DWORD a2,
                                       DWORD a3)
    = DdeAddData;

HDDEDATA (__stdcall * Real_DdeClientTransaction)(LPBYTE a0,
                                                 DWORD a1,
                                                 HCONV a2,
                                                 HSZ a3,
                                                 UINT a4,
                                                 UINT a5,
                                                 DWORD a6,
                                                 LPDWORD a7)
    = DdeClientTransaction;

int (__stdcall * Real_DdeCmpStringHandles)(HSZ a0,
                                           HSZ a1)
    = DdeCmpStringHandles;

HCONV (__stdcall * Real_DdeConnect)(DWORD a0,
                                    HSZ a1,
                                    HSZ a2,
                                    PCONVCONTEXT a3)
    = DdeConnect;

HCONVLIST (__stdcall * Real_DdeConnectList)(DWORD a0,
                                            HSZ a1,
                                            HSZ a2,
                                            HCONVLIST a3,
                                            PCONVCONTEXT a4)
    = DdeConnectList;

HDDEDATA (__stdcall * Real_DdeCreateDataHandle)(DWORD a0,
                                                LPBYTE a1,
                                                DWORD a2,
                                                DWORD a3,
                                                HSZ a4,
                                                UINT a5,
                                                UINT a6)
    = DdeCreateDataHandle;

HSZ (__stdcall * Real_DdeCreateStringHandleA)(DWORD a0,
                                              LPCSTR a1,
                                              int a2)
    = DdeCreateStringHandleA;

HSZ (__stdcall * Real_DdeCreateStringHandleW)(DWORD a0,
                                              LPCWSTR a1,
                                              int a2)
    = DdeCreateStringHandleW;

BOOL (__stdcall * Real_DdeDisconnect)(HCONV a0)
    = DdeDisconnect;

BOOL (__stdcall * Real_DdeDisconnectList)(HCONVLIST a0)
    = DdeDisconnectList;

BOOL (__stdcall * Real_DdeEnableCallback)(DWORD a0,
                                          HCONV a1,
                                          UINT a2)
    = DdeEnableCallback;

BOOL (__stdcall * Real_DdeFreeDataHandle)(HDDEDATA a0)
    = DdeFreeDataHandle;

BOOL (__stdcall * Real_DdeFreeStringHandle)(DWORD a0,
                                            HSZ a1)
    = DdeFreeStringHandle;

DWORD (__stdcall * Real_DdeGetData)(HDDEDATA a0,
                                    LPBYTE a1,
                                    DWORD a2,
                                    DWORD a3)
    = DdeGetData;

UINT (__stdcall * Real_DdeGetLastError)(DWORD a0)
    = DdeGetLastError;

BOOL (__stdcall * Real_DdeImpersonateClient)(HCONV a0)
    = DdeImpersonateClient;

BOOL (__stdcall * Real_DdeKeepStringHandle)(DWORD a0,
                                            HSZ a1)
    = DdeKeepStringHandle;

HDDEDATA (__stdcall * Real_DdeNameService)(DWORD a0,
                                           HSZ a1,
                                           HSZ a2,
                                           UINT a3)
    = DdeNameService;

BOOL (__stdcall * Real_DdePostAdvise)(DWORD a0,
                                      HSZ a1,
                                      HSZ a2)
    = DdePostAdvise;

UINT (__stdcall * Real_DdeQueryConvInfo)(HCONV a0,
                                         DWORD a1,
                                         CONVINFO* a2)
    = DdeQueryConvInfo;

HCONV (__stdcall * Real_DdeQueryNextServer)(HCONVLIST a0,
                                            HCONV a1)
    = DdeQueryNextServer;

DWORD (__stdcall * Real_DdeQueryStringA)(DWORD a0,
                                         HSZ a1,
                                         LPSTR a2,
                                         DWORD a3,
                                         int a4)
    = DdeQueryStringA;

DWORD (__stdcall * Real_DdeQueryStringW)(DWORD a0,
                                         HSZ a1,
                                         LPWSTR a2,
                                         DWORD a3,
                                         int a4)
    = DdeQueryStringW;

HCONV (__stdcall * Real_DdeReconnect)(HCONV a0)
    = DdeReconnect;

BOOL (__stdcall * Real_DdeSetQualityOfService)(HWND a0,
                                               CONST SECURITY_QUALITY_OF_SERVICE* a1,
                                               PSECURITY_QUALITY_OF_SERVICE a2)
    = DdeSetQualityOfService;

BOOL (__stdcall * Real_DdeSetUserHandle)(HCONV a0,
                                         DWORD a1,
                                         ULONG_PTR a2)
    = DdeSetUserHandle;

BOOL (__stdcall * Real_DdeUnaccessData)(HDDEDATA a0)
    = DdeUnaccessData;

BOOL (__stdcall * Real_DdeUninitialize)(DWORD a0)
    = DdeUninitialize;

BOOL (__stdcall * Real_DebugActiveProcess)(DWORD a0)
    = DebugActiveProcess;

BOOL (__stdcall * Real_DebugActiveProcessStop)(DWORD a0)
    = DebugActiveProcessStop;

void (__stdcall * Real_DebugBreak)(void)
    = DebugBreak;

LRESULT (__stdcall * Real_DefDlgProcA)(HWND a0,
                                       UINT a1,
                                       WPARAM a2,
                                       LPARAM a3)
    = DefDlgProcA;

LRESULT (__stdcall * Real_DefDlgProcW)(HWND a0,
                                       UINT a1,
                                       WPARAM a2,
                                       LPARAM a3)
    = DefDlgProcW;

LRESULT (__stdcall * Real_DefFrameProcA)(HWND a0,
                                         HWND a1,
                                         UINT a2,
                                         WPARAM a3,
                                         LPARAM a4)
    = DefFrameProcA;

LRESULT (__stdcall * Real_DefFrameProcW)(HWND a0,
                                         HWND a1,
                                         UINT a2,
                                         WPARAM a3,
                                         LPARAM a4)
    = DefFrameProcW;

LRESULT (__stdcall * Real_DefMDIChildProcA)(HWND a0,
                                            UINT a1,
                                            WPARAM a2,
                                            LPARAM a3)
    = DefMDIChildProcA;

LRESULT (__stdcall * Real_DefMDIChildProcW)(HWND a0,
                                            UINT a1,
                                            WPARAM a2,
                                            LPARAM a3)
    = DefMDIChildProcW;

LRESULT (__stdcall * Real_DefWindowProcA)(HWND a0,
                                          UINT a1,
                                          WPARAM a2,
                                          LPARAM a3)
    = DefWindowProcA;

LRESULT (__stdcall * Real_DefWindowProcW)(HWND a0,
                                          UINT a1,
                                          WPARAM a2,
                                          LPARAM a3)
    = DefWindowProcW;

HDWP (__stdcall * Real_DeferWindowPos)(HDWP a0,
                                       HWND a1,
                                       HWND a2,
                                       int a3,
                                       int a4,
                                       int a5,
                                       int a6,
                                       UINT a7)
    = DeferWindowPos;

BOOL (__stdcall * Real_DefineDosDeviceA)(DWORD a0,
                                         LPCSTR a1,
                                         LPCSTR a2)
    = DefineDosDeviceA;

BOOL (__stdcall * Real_DefineDosDeviceW)(DWORD a0,
                                         LPCWSTR a1,
                                         LPCWSTR a2)
    = DefineDosDeviceW;

ATOM (__stdcall * Real_DeleteAtom)(ATOM a0)
    = DeleteAtom;

BOOL (__stdcall * Real_DeleteColorSpace)(HCOLORSPACE a0)
    = DeleteColorSpace;

BOOL (__stdcall * Real_DeleteDC)(HDC a0)
    = DeleteDC;

BOOL (__stdcall * Real_DeleteEnhMetaFile)(HENHMETAFILE a0)
    = DeleteEnhMetaFile;

void (__stdcall * Real_DeleteFiber)(LPVOID a0)
    = DeleteFiber;

BOOL (__stdcall * Real_DeleteFileA)(LPCSTR a0)
    = DeleteFileA;

BOOL (__stdcall * Real_DeleteFileW)(LPCWSTR a0)
    = DeleteFileW;

BOOL (__stdcall * Real_DeleteMenu)(HMENU a0,
                                   UINT a1,
                                   UINT a2)
    = DeleteMenu;

BOOL (__stdcall * Real_DeleteMetaFile)(HMETAFILE a0)
    = DeleteMetaFile;

BOOL (__stdcall * Real_DeleteObject)(HGDIOBJ a0)
    = DeleteObject;

int (__stdcall * Real_DescribePixelFormat)(HDC a0,
                                           int a1,
                                           UINT a2,
                                           PIXELFORMATDESCRIPTOR* a3)
    = DescribePixelFormat;

BOOL (__stdcall * Real_DestroyAcceleratorTable)(HACCEL a0)
    = DestroyAcceleratorTable;

BOOL (__stdcall * Real_DestroyCaret)(void)
    = DestroyCaret;

BOOL (__stdcall * Real_DestroyCursor)(HCURSOR a0)
    = DestroyCursor;

BOOL (__stdcall * Real_DestroyIcon)(HICON a0)
    = DestroyIcon;

BOOL (__stdcall * Real_DestroyMenu)(HMENU a0)
    = DestroyMenu;

BOOL (__stdcall * Real_DestroyWindow)(HWND a0)
    = DestroyWindow;

BOOL (__stdcall * Real_DeviceIoControl)(HANDLE a0,
                                        DWORD a1,
                                        LPVOID a2,
                                        DWORD a3,
                                        LPVOID a4,
                                        DWORD a5,
                                        LPDWORD a6,
                                        LPOVERLAPPED a7)
    = DeviceIoControl;

INT_PTR (__stdcall * Real_DialogBoxIndirectParamA)(HINSTANCE a0,
                                                   LPCDLGTEMPLATEA a1,
                                                   HWND a2,
                                                   DLGPROC a3,
                                                   LPARAM a4)
    = DialogBoxIndirectParamA;

INT_PTR (__stdcall * Real_DialogBoxIndirectParamW)(HINSTANCE a0,
                                                   LPCDLGTEMPLATEW a1,
                                                   HWND a2,
                                                   DLGPROC a3,
                                                   LPARAM a4)
    = DialogBoxIndirectParamW;

INT_PTR (__stdcall * Real_DialogBoxParamA)(HINSTANCE a0,
                                           LPCSTR a1,
                                           HWND a2,
                                           DLGPROC a3,
                                           LPARAM a4)
    = DialogBoxParamA;

INT_PTR (__stdcall * Real_DialogBoxParamW)(HINSTANCE a0,
                                           LPCWSTR a1,
                                           HWND a2,
                                           DLGPROC a3,
                                           LPARAM a4)
    = DialogBoxParamW;

BOOL (__stdcall * Real_DisableThreadLibraryCalls)(HMODULE a0)
    = DisableThreadLibraryCalls;

BOOL (__stdcall * Real_DisconnectNamedPipe)(HANDLE a0)
    = DisconnectNamedPipe;

LRESULT (__stdcall * Real_DispatchMessageA)(CONST MSG* a0)
    = DispatchMessageA;

LRESULT (__stdcall * Real_DispatchMessageW)(CONST MSG* a0)
    = DispatchMessageW;

int (__stdcall * Real_DlgDirListA)(HWND a0,
                                   LPSTR a1,
                                   int a2,
                                   int a3,
                                   UINT a4)
    = DlgDirListA;

int (__stdcall * Real_DlgDirListComboBoxA)(HWND a0,
                                           LPSTR a1,
                                           int a2,
                                           int a3,
                                           UINT a4)
    = DlgDirListComboBoxA;

int (__stdcall * Real_DlgDirListComboBoxW)(HWND a0,
                                           LPWSTR a1,
                                           int a2,
                                           int a3,
                                           UINT a4)
    = DlgDirListComboBoxW;

int (__stdcall * Real_DlgDirListW)(HWND a0,
                                   LPWSTR a1,
                                   int a2,
                                   int a3,
                                   UINT a4)
    = DlgDirListW;

BOOL (__stdcall * Real_DlgDirSelectComboBoxExA)(HWND a0,
                                                LPSTR a1,
                                                int a2,
                                                int a3)
    = DlgDirSelectComboBoxExA;

BOOL (__stdcall * Real_DlgDirSelectComboBoxExW)(HWND a0,
                                                LPWSTR a1,
                                                int a2,
                                                int a3)
    = DlgDirSelectComboBoxExW;

BOOL (__stdcall * Real_DlgDirSelectExA)(HWND a0,
                                        LPSTR a1,
                                        int a2,
                                        int a3)
    = DlgDirSelectExA;

BOOL (__stdcall * Real_DlgDirSelectExW)(HWND a0,
                                        LPWSTR a1,
                                        int a2,
                                        int a3)
    = DlgDirSelectExW;

HRESULT (__stdcall * Real_DoDragDrop)(IDataObject* a0,
                                      IDropSource* a1,
                                      DWORD a2,
                                      LPDWORD a3)
    = DoDragDrop;

BOOL (__stdcall * Real_DosDateTimeToFileTime)(WORD a0,
                                              WORD a1,
                                              LPFILETIME a2)
    = DosDateTimeToFileTime;

BOOL (__stdcall * Real_DragDetect)(HWND a0,
                                   POINT a1)
    = DragDetect;

DWORD (__stdcall * Real_DragObject)(HWND a0,
                                    HWND a1,
                                    UINT a2,
                                    ULONG_PTR a3,
                                    HCURSOR a4)
    = DragObject;

BOOL (__stdcall * Real_DrawAnimatedRects)(HWND a0,
                                          int a1,
                                          CONST RECT* a2,
                                          CONST RECT* a3)
    = DrawAnimatedRects;

BOOL (__stdcall * Real_DrawCaption)(HWND a0,
                                    HDC a1,
                                    CONST RECT* a2,
                                    UINT a3)
    = DrawCaption;

BOOL (__stdcall * Real_DrawEdge)(HDC a0,
                                 LPRECT a1,
                                 UINT a2,
                                 UINT a3)
    = DrawEdge;

int (__stdcall * Real_DrawEscape)(HDC a0,
                                  int a1,
                                  int a2,
                                  LPCSTR a3)
    = DrawEscape;

BOOL (__stdcall * Real_DrawFocusRect)(HDC a0,
                                      CONST RECT* a1)
    = DrawFocusRect;

BOOL (__stdcall * Real_DrawFrameControl)(HDC a0,
                                         LPRECT a1,
                                         UINT a2,
                                         UINT a3)
    = DrawFrameControl;

BOOL (__stdcall * Real_DrawIcon)(HDC a0,
                                 int a1,
                                 int a2,
                                 HICON a3)
    = DrawIcon;

BOOL (__stdcall * Real_DrawIconEx)(HDC a0,
                                   int a1,
                                   int a2,
                                   HICON a3,
                                   int a4,
                                   int a5,
                                   UINT a6,
                                   HBRUSH a7,
                                   UINT a8)
    = DrawIconEx;

BOOL (__stdcall * Real_DrawMenuBar)(HWND a0)
    = DrawMenuBar;

BOOL (__stdcall * Real_DrawStateA)(HDC a0,
                                   HBRUSH a1,
                                   DRAWSTATEPROC a2,
                                   LPARAM a3,
                                   WPARAM a4,
                                   int a5,
                                   int a6,
                                   int a7,
                                   int a8,
                                   UINT a9)
    = DrawStateA;

BOOL (__stdcall * Real_DrawStateW)(HDC a0,
                                   HBRUSH a1,
                                   DRAWSTATEPROC a2,
                                   LPARAM a3,
                                   WPARAM a4,
                                   int a5,
                                   int a6,
                                   int a7,
                                   int a8,
                                   UINT a9)
    = DrawStateW;

int (__stdcall * Real_DrawTextA)(HDC a0,
                                 LPCSTR a1,
                                 int a2,
                                 LPRECT a3,
                                 UINT a4)
    = DrawTextA;

int (__stdcall * Real_DrawTextExA)(HDC a0,
                                   LPSTR a1,
                                   int a2,
                                   LPRECT a3,
                                   UINT a4,
                                   LPDRAWTEXTPARAMS a5)
    = DrawTextExA;

int (__stdcall * Real_DrawTextExW)(HDC a0,
                                   LPWSTR a1,
                                   int a2,
                                   LPRECT a3,
                                   UINT a4,
                                   LPDRAWTEXTPARAMS a5)
    = DrawTextExW;

int (__stdcall * Real_DrawTextW)(HDC a0,
                                 LPCWSTR a1,
                                 int a2,
                                 LPRECT a3,
                                 UINT a4)
    = DrawTextW;

BOOL (__stdcall * Real_DuplicateHandle)(HANDLE a0,
                                        HANDLE a1,
                                        HANDLE a2,
                                        LPHANDLE a3,
                                        DWORD a4,
                                        BOOL a5,
                                        DWORD a6)
    = DuplicateHandle;

BOOL (__stdcall * Real_Ellipse)(HDC a0,
                                int a1,
                                int a2,
                                int a3,
                                int a4)
    = Ellipse;

BOOL (__stdcall * Real_EmptyClipboard)(void)
    = EmptyClipboard;

BOOL (__stdcall * Real_EnableMenuItem)(HMENU a0,
                                       UINT a1,
                                       UINT a2)
    = EnableMenuItem;

BOOL (__stdcall * Real_EnableScrollBar)(HWND a0,
                                        UINT a1,
                                        UINT a2)
    = EnableScrollBar;

BOOL (__stdcall * Real_EnableWindow)(HWND a0,
                                     BOOL a1)
    = EnableWindow;

BOOL (__stdcall * Real_EndDeferWindowPos)(HDWP a0)
    = EndDeferWindowPos;

BOOL (__stdcall * Real_EndDialog)(HWND a0,
                                  INT_PTR a1)
    = EndDialog;

int (__stdcall * Real_EndDoc)(HDC a0)
    = EndDoc;

int (__stdcall * Real_EndPage)(HDC a0)
    = EndPage;

BOOL (__stdcall * Real_EndPaint)(HWND a0,
                                 CONST PAINTSTRUCT* a1)
    = EndPaint;

BOOL (__stdcall * Real_EndPath)(HDC a0)
    = EndPath;

BOOL (__stdcall * Real_EndUpdateResourceA)(HANDLE a0,
                                           BOOL a1)
    = EndUpdateResourceA;

BOOL (__stdcall * Real_EndUpdateResourceW)(HANDLE a0,
                                           BOOL a1)
    = EndUpdateResourceW;

BOOL (__stdcall * Real_EnumCalendarInfoA)(CALINFO_ENUMPROCA a0,
                                          LCID a1,
                                          CALID a2,
                                          CALTYPE a3)
    = EnumCalendarInfoA;

BOOL (__stdcall * Real_EnumCalendarInfoW)(CALINFO_ENUMPROCW a0,
                                          LCID a1,
                                          CALID a2,
                                          CALTYPE a3)
    = EnumCalendarInfoW;

BOOL (__stdcall * Real_EnumChildWindows)(HWND a0,
                                         WNDENUMPROC a1,
                                         LPARAM a2)
    = EnumChildWindows;

UINT (__stdcall * Real_EnumClipboardFormats)(UINT a0)
    = EnumClipboardFormats;

BOOL (__stdcall * Real_EnumDateFormatsA)(DATEFMT_ENUMPROCA a0,
                                         LCID a1,
                                         DWORD a2)
    = EnumDateFormatsA;

BOOL (__stdcall * Real_EnumDateFormatsW)(DATEFMT_ENUMPROCW a0,
                                         LCID a1,
                                         DWORD a2)
    = EnumDateFormatsW;

BOOL (__stdcall * Real_EnumDesktopWindows)(HDESK a0,
                                           WNDENUMPROC a1,
                                           LPARAM a2)
    = EnumDesktopWindows;

BOOL (__stdcall * Real_EnumDesktopsA)(HWINSTA a0,
                                      DESKTOPENUMPROCA a1,
                                      LPARAM a2)
    = EnumDesktopsA;

BOOL (__stdcall * Real_EnumDesktopsW)(HWINSTA a0,
                                      DESKTOPENUMPROCW a1,
                                      LPARAM a2)
    = EnumDesktopsW;

BOOL (__stdcall * Real_EnumDisplaySettingsA)(LPCSTR a0,
                                             DWORD a1,
                                             LPDEVMODEA a2)
    = EnumDisplaySettingsA;

BOOL (__stdcall * Real_EnumDisplaySettingsW)(LPCWSTR a0,
                                             DWORD a1,
                                             LPDEVMODEW a2)
    = EnumDisplaySettingsW;

BOOL (__stdcall * Real_EnumEnhMetaFile)(HDC a0,
                                        HENHMETAFILE a1,
                                        ENHMFENUMPROC a2,
                                        LPVOID a3,
                                        CONST RECT* a4)
    = EnumEnhMetaFile;

int (__stdcall * Real_EnumFontFamiliesA)(HDC a0,
                                         LPCSTR a1,
                                         FONTENUMPROCA a2,
                                         LPARAM a3)
    = EnumFontFamiliesA;

int (__stdcall * Real_EnumFontFamiliesExA)(HDC a0,
                                           LPLOGFONTA a1,
                                           FONTENUMPROCA a2,
                                           LPARAM a3,
                                           DWORD a4)
    = EnumFontFamiliesExA;

int (__stdcall * Real_EnumFontFamiliesExW)(HDC a0,
                                           LPLOGFONTW a1,
                                           FONTENUMPROCW a2,
                                           LPARAM a3,
                                           DWORD a4)
    = EnumFontFamiliesExW;

int (__stdcall * Real_EnumFontFamiliesW)(HDC a0,
                                         LPCWSTR a1,
                                         FONTENUMPROCW a2,
                                         LPARAM a3)
    = EnumFontFamiliesW;

int (__stdcall * Real_EnumFontsA)(HDC a0,
                                  LPCSTR a1,
                                  FONTENUMPROCA a2,
                                  LPARAM a3)
    = EnumFontsA;

int (__stdcall * Real_EnumFontsW)(HDC a0,
                                  LPCWSTR a1,
                                  FONTENUMPROCW a2,
                                  LPARAM a3)
    = EnumFontsW;

int (__stdcall * Real_EnumICMProfilesA)(HDC a0,
                                        ICMENUMPROCA a1,
                                        LPARAM a2)
    = EnumICMProfilesA;

int (__stdcall * Real_EnumICMProfilesW)(HDC a0,
                                        ICMENUMPROCW a1,
                                        LPARAM a2)
    = EnumICMProfilesW;

BOOL (__stdcall * Real_EnumMetaFile)(HDC a0,
                                     HMETAFILE a1,
                                     MFENUMPROC a2,
                                     LPARAM a3)
    = EnumMetaFile;

int (__stdcall * Real_EnumObjects)(HDC a0,
                                   int a1,
                                   GOBJENUMPROC a2,
                                   LPARAM a3)
    = EnumObjects;

int (__stdcall * Real_EnumPropsA)(HWND a0,
                                  PROPENUMPROCA a1)
    = EnumPropsA;

int (__stdcall * Real_EnumPropsExA)(HWND a0,
                                    PROPENUMPROCEXA a1,
                                    LPARAM a2)
    = EnumPropsExA;

int (__stdcall * Real_EnumPropsExW)(HWND a0,
                                    PROPENUMPROCEXW a1,
                                    LPARAM a2)
    = EnumPropsExW;

int (__stdcall * Real_EnumPropsW)(HWND a0,
                                  PROPENUMPROCW a1)
    = EnumPropsW;

#if _MSC_VER < 1300
BOOL (__stdcall * Real_EnumResourceLanguagesA)(HMODULE a0,
                                               LPCSTR a1,
                                               LPCSTR a2,
                                               ENUMRESLANGPROC a3,
                                               LONG a4)
    = EnumResourceLanguagesA;
#else
BOOL (__stdcall * Real_EnumResourceLanguagesA)(HMODULE a0,
                                               LPCSTR a1,
                                               LPCSTR a2,
                                               ENUMRESLANGPROCA a3,
                                               LONG_PTR a4)
    = EnumResourceLanguagesA;
#endif

#if _MSC_VER < 1300
BOOL (__stdcall * Real_EnumResourceLanguagesW)(HMODULE a0,
                                               LPCWSTR a1,
                                               LPCWSTR a2,
                                               ENUMRESLANGPROC a3,
                                               LONG a4)
    = EnumResourceLanguagesW;
#else
BOOL (__stdcall * Real_EnumResourceLanguagesW)(HMODULE a0,
                                               LPCWSTR a1,
                                               LPCWSTR a2,
                                               ENUMRESLANGPROCW a3,
                                               LONG_PTR a4)
    = EnumResourceLanguagesW;
#endif

#if _MSC_VER < 1300
BOOL (__stdcall * Real_EnumResourceNamesA)(HMODULE a0,
                                           LPCSTR a1,
                                           ENUMRESNAMEPROC a2,
                                           LONG a3)
    = EnumResourceNamesA;
#else
BOOL (__stdcall * Real_EnumResourceNamesA)(HMODULE a0,
                                           LPCSTR a1,
                                           ENUMRESNAMEPROCA a2,
                                           LONG_PTR a3)
    = EnumResourceNamesA;
#endif

#if _MSC_VER < 1300
BOOL (__stdcall * Real_EnumResourceNamesW)(HMODULE a0,
                                           LPCWSTR a1,
                                           ENUMRESNAMEPROC a2,
                                           LONG a3)
    = EnumResourceNamesW;
#else
BOOL (__stdcall * Real_EnumResourceNamesW)(HMODULE a0,
                                           LPCWSTR a1,
                                           ENUMRESNAMEPROCW a2,
                                           LONG_PTR a3)
    = EnumResourceNamesW;
#endif

#if _MSC_VER < 1300
BOOL (__stdcall * Real_EnumResourceTypesA)(HMODULE a0,
                                           ENUMRESTYPEPROC a1,
                                           LONG a2)
    = EnumResourceTypesA;
#else
BOOL (__stdcall * Real_EnumResourceTypesA)(HMODULE a0,
                                           ENUMRESTYPEPROCA a1,
                                           LONG_PTR a2)
    = EnumResourceTypesA;
#endif

#if _MSC_VER < 1300
BOOL (__stdcall * Real_EnumResourceTypesW)(HMODULE a0,
                                           ENUMRESTYPEPROC a1,
                                           LONG a2)
    = EnumResourceTypesW;
#else
BOOL (__stdcall * Real_EnumResourceTypesW)(HMODULE a0,
                                           ENUMRESTYPEPROCW a1,
                                           LONG_PTR a2)
    = EnumResourceTypesW;
#endif

BOOL (__stdcall * Real_EnumSystemCodePagesA)(CODEPAGE_ENUMPROCA a0,
                                             DWORD a1)
    = EnumSystemCodePagesA;

BOOL (__stdcall * Real_EnumSystemCodePagesW)(CODEPAGE_ENUMPROCW a0,
                                             DWORD a1)
    = EnumSystemCodePagesW;

#if(WINVER >= 0x0500)
BOOL (__stdcall * Real_EnumSystemLocalesA)(LOCALE_ENUMPROCA a0,
                                           DWORD a1)
    = EnumSystemLocalesA;

BOOL (__stdcall * Real_EnumSystemLocalesW)(LOCALE_ENUMPROCW a0,
                                           DWORD a1)
    = EnumSystemLocalesW;
#endif // (WINVER >= 0x0500)

BOOL (__stdcall * Real_EnumThreadWindows)(DWORD a0,
                                          WNDENUMPROC a1,
                                          LPARAM a2)
    = EnumThreadWindows;

BOOL (__stdcall * Real_EnumTimeFormatsA)(TIMEFMT_ENUMPROCA a0,
                                         LCID a1,
                                         DWORD a2)
    = EnumTimeFormatsA;

BOOL (__stdcall * Real_EnumTimeFormatsW)(TIMEFMT_ENUMPROCW a0,
                                         LCID a1,
                                         DWORD a2)
    = EnumTimeFormatsW;

BOOL (__stdcall * Real_EnumWindowStationsA)(WINSTAENUMPROCA a0,
                                            LPARAM a1)
    = EnumWindowStationsA;

BOOL (__stdcall * Real_EnumWindowStationsW)(WINSTAENUMPROCW a0,
                                            LPARAM a1)
    = EnumWindowStationsW;

BOOL (__stdcall * Real_EnumWindows)(WNDENUMPROC a0,
                                    LPARAM a1)
    = EnumWindows;

BOOL (__stdcall * Real_EqualRect)(CONST RECT* a0,
                                  CONST RECT* a1)
    = EqualRect;

BOOL (__stdcall * Real_EqualRgn)(HRGN a0,
                                 HRGN a1)
    = EqualRgn;

DWORD (__stdcall * Real_EraseTape)(HANDLE a0,
                                   DWORD a1,
                                   BOOL a2)
    = EraseTape;

int (__stdcall * Real_Escape)(HDC a0,
                              int a1,
                              int a2,
                              LPCSTR a3,
                              LPVOID a4)
    = Escape;

BOOL (__stdcall * Real_EscapeCommFunction)(HANDLE a0,
                                           DWORD a1)
    = EscapeCommFunction;

int (__stdcall * Real_ExcludeClipRect)(HDC a0,
                                       int a1,
                                       int a2,
                                       int a3,
                                       int a4)
    = ExcludeClipRect;

int (__stdcall * Real_ExcludeUpdateRgn)(HDC a0,
                                        HWND a1)
    = ExcludeUpdateRgn;

void (__stdcall * Real_ExitProcess)(UINT a0)
    = ExitProcess;

void (__stdcall * Real_ExitThread)(DWORD a0)
    = ExitThread;

BOOL (__stdcall * Real_ExitWindowsEx)(UINT a0,
                                      DWORD a1)
    = ExitWindowsEx;

DWORD (__stdcall * Real_ExpandEnvironmentStringsA)(LPCSTR a0,
                                                   LPSTR a1,
                                                   DWORD a2)
    = ExpandEnvironmentStringsA;

DWORD (__stdcall * Real_ExpandEnvironmentStringsW)(LPCWSTR a0,
                                                   LPWSTR a1,
                                                   DWORD a2)
    = ExpandEnvironmentStringsW;

HPEN (__stdcall * Real_ExtCreatePen)(DWORD a0,
                                     DWORD a1,
                                     CONST LOGBRUSH* a2,
                                     DWORD a3,
                                     CONST DWORD* a4)
    = ExtCreatePen;

HRGN (__stdcall * Real_ExtCreateRegion)(CONST XFORM* a0,
                                        DWORD a1,
                                        CONST RGNDATA* a2)
    = ExtCreateRegion;

int (__stdcall * Real_ExtEscape)(HDC a0,
                                 int a1,
                                 int a2,
                                 LPCSTR a3,
                                 int a4,
                                 LPSTR a5)
    = ExtEscape;

BOOL (__stdcall * Real_ExtFloodFill)(HDC a0,
                                     int a1,
                                     int a2,
                                     COLORREF a3,
                                     UINT a4)
    = ExtFloodFill;

int (__stdcall * Real_ExtSelectClipRgn)(HDC a0,
                                        HRGN a1,
                                        int a2)
    = ExtSelectClipRgn;

BOOL (__stdcall * Real_ExtTextOutA)(HDC a0,
                                    int a1,
                                    int a2,
                                    UINT a3,
                                    CONST RECT* a4,
                                    LPCSTR a5,
                                    UINT a6,
                                    CONST INT* a7)
    = ExtTextOutA;

BOOL (__stdcall * Real_ExtTextOutW)(HDC a0,
                                    int a1,
                                    int a2,
                                    UINT a3,
                                    CONST RECT* a4,
                                    LPCWSTR a5,
                                    UINT a6,
                                    CONST INT* a7)
    = ExtTextOutW;

void (__stdcall * Real_FatalAppExitA)(UINT a0,
                                      LPCSTR a1)
    = FatalAppExitA;

void (__stdcall * Real_FatalAppExitW)(UINT a0,
                                      LPCWSTR a1)
    = FatalAppExitW;

void (__stdcall * Real_FatalExit)(int a0)
    = FatalExit;

BOOL (__stdcall * Real_FileTimeToDosDateTime)(CONST FILETIME* a0,
                                              LPWORD a1,
                                              LPWORD a2)
    = FileTimeToDosDateTime;

BOOL (__stdcall * Real_FileTimeToLocalFileTime)(CONST FILETIME* a0,
                                                LPFILETIME a1)
    = FileTimeToLocalFileTime;

BOOL (__stdcall * Real_FileTimeToSystemTime)(CONST FILETIME* a0,
                                             LPSYSTEMTIME a1)
    = FileTimeToSystemTime;

BOOL (__stdcall * Real_FillConsoleOutputAttribute)(HANDLE a0,
                                                   WORD a1,
                                                   DWORD a2,
                                                   COORD a3,
                                                   LPDWORD a4)
    = FillConsoleOutputAttribute;

BOOL (__stdcall * Real_FillConsoleOutputCharacterA)(HANDLE a0,
                                                    CHAR a1,
                                                    DWORD a2,
                                                    COORD a3,
                                                    LPDWORD a4)
    = FillConsoleOutputCharacterA;

BOOL (__stdcall * Real_FillConsoleOutputCharacterW)(HANDLE a0,
                                                    WCHAR a1,
                                                    DWORD a2,
                                                    COORD a3,
                                                    LPDWORD a4)
    = FillConsoleOutputCharacterW;

BOOL (__stdcall * Real_FillPath)(HDC a0)
    = FillPath;

int (__stdcall * Real_FillRect)(HDC a0,
                                CONST RECT* a1,
                                HBRUSH a2)
    = FillRect;

BOOL (__stdcall * Real_FillRgn)(HDC a0,
                                HRGN a1,
                                HBRUSH a2)
    = FillRgn;

ATOM (__stdcall * Real_FindAtomA)(LPCSTR a0)
    = FindAtomA;

ATOM (__stdcall * Real_FindAtomW)(LPCWSTR a0)
    = FindAtomW;

BOOL (__stdcall * Real_FindClose)(HANDLE a0)
    = FindClose;

BOOL (__stdcall * Real_FindCloseChangeNotification)(HANDLE a0)
    = FindCloseChangeNotification;

HANDLE (__stdcall * Real_FindFirstChangeNotificationA)(LPCSTR a0,
                                                       BOOL a1,
                                                       DWORD a2)
    = FindFirstChangeNotificationA;

HANDLE (__stdcall * Real_FindFirstChangeNotificationW)(LPCWSTR a0,
                                                       BOOL a1,
                                                       DWORD a2)
    = FindFirstChangeNotificationW;

HANDLE (__stdcall * Real_FindFirstFileA)(LPCSTR a0,
                                         LPReal_FIND_DATAA a1)
    = FindFirstFileA;

HANDLE (__stdcall * Real_FindFirstFileExA)(LPCSTR a0,
                                           FINDEX_INFO_LEVELS a1,
                                           LPVOID a2,
                                           FINDEX_SEARCH_OPS a3,
                                           LPVOID a4,
                                           DWORD a5)
    = FindFirstFileExA;

HANDLE (__stdcall * Real_FindFirstFileExW)(LPCWSTR a0,
                                           FINDEX_INFO_LEVELS a1,
                                           LPVOID a2,
                                           FINDEX_SEARCH_OPS a3,
                                           LPVOID a4,
                                           DWORD a5)
    = FindFirstFileExW;

HANDLE (__stdcall * Real_FindFirstFileW)(LPCWSTR a0,
                                         LPReal_FIND_DATAW a1)
    = FindFirstFileW;

BOOL (__stdcall * Real_FindNextChangeNotification)(HANDLE a0)
    = FindNextChangeNotification;

BOOL (__stdcall * Real_FindNextFileA)(HANDLE a0,
                                      LPReal_FIND_DATAA a1)
    = FindNextFileA;

BOOL (__stdcall * Real_FindNextFileW)(HANDLE a0,
                                      LPReal_FIND_DATAW a1)
    = FindNextFileW;

HRSRC (__stdcall * Real_FindResourceA)(HMODULE a0,
                                       LPCSTR a1,
                                       LPCSTR a2)
    = FindResourceA;

HRSRC (__stdcall * Real_FindResourceExA)(HMODULE a0,
                                         LPCSTR a1,
                                         LPCSTR a2,
                                         WORD a3)
    = FindResourceExA;

HRSRC (__stdcall * Real_FindResourceExW)(HMODULE a0,
                                         LPCWSTR a1,
                                         LPCWSTR a2,
                                         WORD a3)
    = FindResourceExW;

HRSRC (__stdcall * Real_FindResourceW)(HMODULE a0,
                                       LPCWSTR a1,
                                       LPCWSTR a2)
    = FindResourceW;

HWND (__stdcall * Real_FindWindowA)(LPCSTR a0,
                                    LPCSTR a1)
    = FindWindowA;

HWND (__stdcall * Real_FindWindowExA)(HWND a0,
                                      HWND a1,
                                      LPCSTR a2,
                                      LPCSTR a3)
    = FindWindowExA;

HWND (__stdcall * Real_FindWindowExW)(HWND a0,
                                      HWND a1,
                                      LPCWSTR a2,
                                      LPCWSTR a3)
    = FindWindowExW;

HWND (__stdcall * Real_FindWindowW)(LPCWSTR a0,
                                    LPCWSTR a1)
    = FindWindowW;

BOOL (__stdcall * Real_FixBrushOrgEx)(HDC a0,
                                      int a1,
                                      int a2,
                                      POINT* a3)
    = FixBrushOrgEx;

BOOL (__stdcall * Real_FlashWindow)(HWND a0,
                                    BOOL a1)
    = FlashWindow;

BOOL (__stdcall * Real_FlattenPath)(HDC a0)
    = FlattenPath;

BOOL (__stdcall * Real_FloodFill)(HDC a0,
                                  int a1,
                                  int a2,
                                  COLORREF a3)
    = FloodFill;

BOOL (__stdcall * Real_FlushConsoleInputBuffer)(HANDLE a0)
    = FlushConsoleInputBuffer;

BOOL (__stdcall * Real_FlushFileBuffers)(HANDLE a0)
    = FlushFileBuffers;

BOOL (__stdcall * Real_FlushViewOfFile)(LPCVOID a0,
                                        SIZE_T a1)
    = FlushViewOfFile;

HRESULT (__stdcall * Real_FmtIdToPropStgName)(CONST FMTID* a0,
                                              LPOLESTR a1)
    = FmtIdToPropStgName;

int (__stdcall * Real_FoldStringA)(DWORD a0,
                                   LPCSTR a1,
                                   int a2,
                                   LPSTR a3,
                                   int a4)
    = FoldStringA;

int (__stdcall * Real_FoldStringW)(DWORD a0,
                                   LPCWSTR a1,
                                   int a2,
                                   LPWSTR a3,
                                   int a4)
    = FoldStringW;

DWORD (__stdcall * Real_FormatMessageA)(DWORD a0,
                                        LPCVOID a1,
                                        DWORD a2,
                                        DWORD a3,
                                        LPSTR a4,
                                        DWORD a5,
                                        va_list* a6)
    = FormatMessageA;

DWORD (__stdcall * Real_FormatMessageW)(DWORD a0,
                                        LPCVOID a1,
                                        DWORD a2,
                                        DWORD a3,
                                        LPWSTR a4,
                                        DWORD a5,
                                        va_list* a6)
    = FormatMessageW;

int (__stdcall * Real_FrameRect)(HDC a0,
                                 CONST RECT* a1,
                                 HBRUSH a2)
    = FrameRect;

BOOL (__stdcall * Real_FrameRgn)(HDC a0,
                                 HRGN a1,
                                 HBRUSH a2,
                                 int a3,
                                 int a4)
    = FrameRgn;

BOOL (__stdcall * Real_FreeConsole)(void)
    = FreeConsole;

BOOL (__stdcall * Real_FreeDDElParam)(UINT a0,
                                      LPARAM a1)
    = FreeDDElParam;

BOOL (__stdcall * Real_FreeEnvironmentStringsA)(LPSTR a0)
    = FreeEnvironmentStringsA;

BOOL (__stdcall * Real_FreeEnvironmentStringsW)(LPWSTR a0)
    = FreeEnvironmentStringsW;

BOOL (__stdcall * Real_FreeLibrary)(HMODULE a0)
    = FreeLibrary;

void (__stdcall * Real_FreeLibraryAndExitThread)(HMODULE a0,
                                                 DWORD a1)
    = FreeLibraryAndExitThread;

HRESULT (__stdcall * Real_FreePropVariantArray)(ULONG a0,
                                                PROPVARIANT* a1)
    = FreePropVariantArray;

BOOL (__stdcall * Real_FreeResource)(HGLOBAL a0)
    = FreeResource;

BOOL (__stdcall * Real_GdiComment)(HDC a0,
                                   UINT a1,
                                   CONST BYTE* a2)
    = GdiComment;

BOOL (__stdcall * Real_GdiFlush)(void)
    = GdiFlush;

DWORD (__stdcall * Real_GdiGetBatchLimit)(void)
    = GdiGetBatchLimit;

DWORD (__stdcall * Real_GdiSetBatchLimit)(DWORD a0)
    = GdiSetBatchLimit;

BOOL (__stdcall * Real_GenerateConsoleCtrlEvent)(DWORD a0,
                                                 DWORD a1)
    = GenerateConsoleCtrlEvent;

UINT (__stdcall * Real_GetACP)(void)
    = GetACP;

HWND (__stdcall * Real_GetActiveWindow)(void)
    = GetActiveWindow;

int (__stdcall * Real_GetArcDirection)(HDC a0)
    = GetArcDirection;

BOOL (__stdcall * Real_GetAspectRatioFilterEx)(HDC a0,
                                               SIZE* a1)
    = GetAspectRatioFilterEx;

SHORT (__stdcall * Real_GetAsyncKeyState)(int a0)
    = GetAsyncKeyState;

UINT (__stdcall * Real_GetAtomNameA)(ATOM a0,
                                     LPSTR a1,
                                     int a2)
    = GetAtomNameA;

UINT (__stdcall * Real_GetAtomNameW)(ATOM a0,
                                     LPWSTR a1,
                                     int a2)
    = GetAtomNameW;

BOOL (__stdcall * Real_GetBinaryTypeA)(LPCSTR a0,
                                       LPDWORD a1)
    = GetBinaryTypeA;

BOOL (__stdcall * Real_GetBinaryTypeW)(LPCWSTR a0,
                                       LPDWORD a1)
    = GetBinaryTypeW;

LONG (__stdcall * Real_GetBitmapBits)(HBITMAP a0,
                                      LONG a1,
                                      LPVOID a2)
    = GetBitmapBits;

BOOL (__stdcall * Real_GetBitmapDimensionEx)(HBITMAP a0,
                                             SIZE* a1)
    = GetBitmapDimensionEx;

COLORREF (__stdcall * Real_GetBkColor)(HDC a0)
    = GetBkColor;

int (__stdcall * Real_GetBkMode)(HDC a0)
    = GetBkMode;

UINT (__stdcall * Real_GetBoundsRect)(HDC a0,
                                      LPRECT a1,
                                      UINT a2)
    = GetBoundsRect;

BOOL (__stdcall * Real_GetBrushOrgEx)(HDC a0,
                                      POINT* a1)
    = GetBrushOrgEx;

BOOL (__stdcall * Real_GetCPInfo)(UINT a0,
                                  LPCPINFO a1)
    = GetCPInfo;

HWND (__stdcall * Real_GetCapture)(void)
    = GetCapture;

UINT (__stdcall * Real_GetCaretBlinkTime)(void)
    = GetCaretBlinkTime;

BOOL (__stdcall * Real_GetCaretPos)(POINT* a0)
    = GetCaretPos;

BOOL (__stdcall * Real_GetCharABCWidthsA)(HDC a0,
                                          UINT a1,
                                          UINT a2,
                                          LPABC a3)
    = GetCharABCWidthsA;

BOOL (__stdcall * Real_GetCharABCWidthsFloatA)(HDC a0,
                                               UINT a1,
                                               UINT a2,
                                               LPABCFLOAT a3)
    = GetCharABCWidthsFloatA;

BOOL (__stdcall * Real_GetCharABCWidthsFloatW)(HDC a0,
                                               UINT a1,
                                               UINT a2,
                                               LPABCFLOAT a3)
    = GetCharABCWidthsFloatW;

BOOL (__stdcall * Real_GetCharABCWidthsW)(HDC a0,
                                          UINT a1,
                                          UINT a2,
                                          LPABC a3)
    = GetCharABCWidthsW;

BOOL (__stdcall * Real_GetCharWidth32A)(HDC a0,
                                        UINT a1,
                                        UINT a2,
                                        LPINT a3)
    = GetCharWidth32A;

BOOL (__stdcall * Real_GetCharWidth32W)(HDC a0,
                                        UINT a1,
                                        UINT a2,
                                        LPINT a3)
    = GetCharWidth32W;

BOOL (__stdcall * Real_GetCharWidthA)(HDC a0,
                                      UINT a1,
                                      UINT a2,
                                      LPINT a3)
    = GetCharWidthA;

BOOL (__stdcall * Real_GetCharWidthFloatA)(HDC a0,
                                           UINT a1,
                                           UINT a2,
                                           PFLOAT a3)
    = GetCharWidthFloatA;

BOOL (__stdcall * Real_GetCharWidthFloatW)(HDC a0,
                                           UINT a1,
                                           UINT a2,
                                           PFLOAT a3)
    = GetCharWidthFloatW;

BOOL (__stdcall * Real_GetCharWidthW)(HDC a0,
                                      UINT a1,
                                      UINT a2,
                                      LPINT a3)
    = GetCharWidthW;

DWORD (__stdcall * Real_GetCharacterPlacementA)(HDC a0,
                                                LPCSTR a1,
                                                int a2,
                                                int a3,
                                                LPGCP_RESULTSA a4,
                                                DWORD a5)
    = GetCharacterPlacementA;

DWORD (__stdcall * Real_GetCharacterPlacementW)(HDC a0,
                                                LPCWSTR a1,
                                                int a2,
                                                int a3,
                                                LPGCP_RESULTSW a4,
                                                DWORD a5)
    = GetCharacterPlacementW;

HRESULT (__stdcall * Real_GetClassFile)(LPCOLESTR a0,
                                        CLSID* a1)
    = GetClassFile;

BOOL (__stdcall * Real_GetClassInfoA)(HINSTANCE a0,
                                      LPCSTR a1,
                                      LPWNDCLASSA a2)
    = GetClassInfoA;

BOOL (__stdcall * Real_GetClassInfoExA)(HINSTANCE a0,
                                        LPCSTR a1,
                                        LPWNDCLASSEXA a2)
    = GetClassInfoExA;

BOOL (__stdcall * Real_GetClassInfoExW)(HINSTANCE a0,
                                        LPCWSTR a1,
                                        LPWNDCLASSEXW a2)
    = GetClassInfoExW;

BOOL (__stdcall * Real_GetClassInfoW)(HINSTANCE a0,
                                      LPCWSTR a1,
                                      LPWNDCLASSW a2)
    = GetClassInfoW;

DWORD (__stdcall * Real_GetClassLongA)(HWND a0,
                                       int a1)
    = GetClassLongA;

DWORD (__stdcall * Real_GetClassLongW)(HWND a0,
                                       int a1)
    = GetClassLongW;

int (__stdcall * Real_GetClassNameA)(HWND a0,
                                     LPSTR a1,
                                     int a2)
    = GetClassNameA;

int (__stdcall * Real_GetClassNameW)(HWND a0,
                                     LPWSTR a1,
                                     int a2)
    = GetClassNameW;

WORD (__stdcall * Real_GetClassWord)(HWND a0,
                                     int a1)
    = GetClassWord;

BOOL (__stdcall * Real_GetClientRect)(HWND a0,
                                      LPRECT a1)
    = GetClientRect;

int (__stdcall * Real_GetClipBox)(HDC a0,
                                  LPRECT a1)
    = GetClipBox;

BOOL (__stdcall * Real_GetClipCursor)(LPRECT a0)
    = GetClipCursor;

int (__stdcall * Real_GetClipRgn)(HDC a0,
                                  HRGN a1)
    = GetClipRgn;

HANDLE (__stdcall * Real_GetClipboardData)(UINT a0)
    = GetClipboardData;

int (__stdcall * Real_GetClipboardFormatNameA)(UINT a0,
                                               LPSTR a1,
                                               int a2)
    = GetClipboardFormatNameA;

int (__stdcall * Real_GetClipboardFormatNameW)(UINT a0,
                                               LPWSTR a1,
                                               int a2)
    = GetClipboardFormatNameW;

HWND (__stdcall * Real_GetClipboardOwner)(void)
    = GetClipboardOwner;

HWND (__stdcall * Real_GetClipboardViewer)(void)
    = GetClipboardViewer;

BOOL (__stdcall * Real_GetColorAdjustment)(HDC a0,
                                           LPCOLORADJUSTMENT a1)
    = GetColorAdjustment;

HCOLORSPACE (__stdcall * Real_GetColorSpace)(HDC a0)
    = GetColorSpace;

BOOL (__stdcall * Real_GetCommConfig)(HANDLE a0,
                                      LPCOMMCONFIG a1,
                                      LPDWORD a2)
    = GetCommConfig;

BOOL (__stdcall * Real_GetCommMask)(HANDLE a0,
                                    LPDWORD a1)
    = GetCommMask;

BOOL (__stdcall * Real_GetCommModemStatus)(HANDLE a0,
                                           LPDWORD a1)
    = GetCommModemStatus;

BOOL (__stdcall * Real_GetCommProperties)(HANDLE a0,
                                          LPCOMMPROP a1)
    = GetCommProperties;

BOOL (__stdcall * Real_GetCommState)(HANDLE a0,
                                     LPDCB a1)
    = GetCommState;

BOOL (__stdcall * Real_GetCommTimeouts)(HANDLE a0,
                                        LPCOMMTIMEOUTS a1)
    = GetCommTimeouts;

LPSTR (__stdcall * Real_GetCommandLineA)(void)
    = GetCommandLineA;

LPWSTR (__stdcall * Real_GetCommandLineW)(void)
    = GetCommandLineW;

BOOL (__stdcall * Real_GetComputerNameA)(LPSTR a0,
                                         LPDWORD a1)
    = GetComputerNameA;

BOOL (__stdcall * Real_GetComputerNameW)(LPWSTR a0,
                                         LPDWORD a1)
    = GetComputerNameW;

UINT (__stdcall * Real_GetConsoleCP)(void)
    = GetConsoleCP;

BOOL (__stdcall * Real_GetConsoleCursorInfo)(HANDLE a0,
                                             PCONSOLE_CURSOR_INFO a1)
    = GetConsoleCursorInfo;

BOOL (__stdcall * Real_GetConsoleMode)(HANDLE a0,
                                       LPDWORD a1)
    = GetConsoleMode;

UINT (__stdcall * Real_GetConsoleOutputCP)(void)
    = GetConsoleOutputCP;

BOOL (__stdcall * Real_GetConsoleScreenBufferInfo)(HANDLE a0,
                                                   PCONSOLE_SCREEN_BUFFER_INFO a1)
    = GetConsoleScreenBufferInfo;

DWORD (__stdcall * Real_GetConsoleTitleA)(LPSTR a0,
                                          DWORD a1)
    = GetConsoleTitleA;

DWORD (__stdcall * Real_GetConsoleTitleW)(LPWSTR a0,
                                          DWORD a1)
    = GetConsoleTitleW;

HRESULT (__stdcall * Real_GetConvertStg)(IStorage* a0)
    = GetConvertStg;

int (__stdcall * Real_GetCurrencyFormatA)(LCID a0,
                                          DWORD a1,
                                          LPCSTR a2,
                                          CONST CURRENCYFMTA* a3,
                                          LPSTR a4,
                                          int a5)
    = GetCurrencyFormatA;

int (__stdcall * Real_GetCurrencyFormatW)(LCID a0,
                                          DWORD a1,
                                          LPCWSTR a2,
                                          CONST CURRENCYFMTW* a3,
                                          LPWSTR a4,
                                          int a5)
    = GetCurrencyFormatW;

DWORD (__stdcall * Real_GetCurrentDirectoryA)(DWORD a0,
                                              LPSTR a1)
    = GetCurrentDirectoryA;

DWORD (__stdcall * Real_GetCurrentDirectoryW)(DWORD a0,
                                              LPWSTR a1)
    = GetCurrentDirectoryW;

HGDIOBJ (__stdcall * Real_GetCurrentObject)(HDC a0,
                                            UINT a1)
    = GetCurrentObject;

BOOL (__stdcall * Real_GetCurrentPositionEx)(HDC a0,
                                             POINT* a1)
    = GetCurrentPositionEx;

HANDLE (__stdcall * Real_GetCurrentProcess)(void)
    = GetCurrentProcess;

DWORD (__stdcall * Real_GetCurrentProcessId)(void)
    = GetCurrentProcessId;

DWORD (__stdcall * Real_GetCurrentThreadId)(void)
    = GetCurrentThreadId;

HCURSOR (__stdcall * Real_GetCursor)(void)
    = GetCursor;

BOOL (__stdcall * Real_GetCursorPos)(POINT* a0)
    = GetCursorPos;

HDC (__stdcall * Real_GetDC)(HWND a0)
    = GetDC;

HDC (__stdcall * Real_GetDCEx)(HWND a0,
                               HRGN a1,
                               DWORD a2)
    = GetDCEx;

BOOL (__stdcall * Real_GetDCOrgEx)(HDC a0,
                                   POINT* a1)
    = GetDCOrgEx;

UINT (__stdcall * Real_GetDIBColorTable)(HDC a0,
                                         UINT a1,
                                         UINT a2,
                                         RGBQUAD* a3)
    = GetDIBColorTable;

int (__stdcall * Real_GetDIBits)(HDC a0,
                                 HBITMAP a1,
                                 UINT a2,
                                 UINT a3,
                                 LPVOID a4,
                                 LPBITMAPINFO a5,
                                 UINT a6)
    = GetDIBits;

int (__stdcall * Real_GetDateFormatA)(LCID a0,
                                      DWORD a1,
                                      CONST SYSTEMTIME* a2,
                                      LPCSTR a3,
                                      LPSTR a4,
                                      int a5)
    = GetDateFormatA;

int (__stdcall * Real_GetDateFormatW)(LCID a0,
                                      DWORD a1,
                                      CONST SYSTEMTIME* a2,
                                      LPCWSTR a3,
                                      LPWSTR a4,
                                      int a5)
    = GetDateFormatW;

BOOL (__stdcall * Real_GetDefaultCommConfigA)(LPCSTR a0,
                                              LPCOMMCONFIG a1,
                                              LPDWORD a2)
    = GetDefaultCommConfigA;

BOOL (__stdcall * Real_GetDefaultCommConfigW)(LPCWSTR a0,
                                              LPCOMMCONFIG a1,
                                              LPDWORD a2)
    = GetDefaultCommConfigW;

HWND (__stdcall * Real_GetDesktopWindow)(void)
    = GetDesktopWindow;

int (__stdcall * Real_GetDeviceCaps)(HDC a0,
                                     int a1)
    = GetDeviceCaps;

BOOL (__stdcall * Real_GetDeviceGammaRamp)(HDC a0,
                                           LPVOID a1)
    = GetDeviceGammaRamp;

long (__stdcall * Real_GetDialogBaseUnits)(void)
    = GetDialogBaseUnits;

BOOL (__stdcall * Real_GetDiskFreeSpaceA)(LPCSTR a0,
                                          LPDWORD a1,
                                          LPDWORD a2,
                                          LPDWORD a3,
                                          LPDWORD a4)
    = GetDiskFreeSpaceA;

BOOL (__stdcall * Real_GetDiskFreeSpaceExA)(LPCSTR a0,
                                            union _ULARGE_INTEGER* a1,
                                            union _ULARGE_INTEGER* a2,
                                            union _ULARGE_INTEGER* a3)
    = GetDiskFreeSpaceExA;

BOOL (__stdcall * Real_GetDiskFreeSpaceExW)(LPCWSTR a0,
                                            union _ULARGE_INTEGER* a1,
                                            union _ULARGE_INTEGER* a2,
                                            union _ULARGE_INTEGER* a3)
    = GetDiskFreeSpaceExW;

BOOL (__stdcall * Real_GetDiskFreeSpaceW)(LPCWSTR a0,
                                          LPDWORD a1,
                                          LPDWORD a2,
                                          LPDWORD a3,
                                          LPDWORD a4)
    = GetDiskFreeSpaceW;

int (__stdcall * Real_GetDlgCtrlID)(HWND a0)
    = GetDlgCtrlID;

HWND (__stdcall * Real_GetDlgItem)(HWND a0,
                                   int a1)
    = GetDlgItem;

UINT (__stdcall * Real_GetDlgItemInt)(HWND a0,
                                      int a1,
                                      BOOL* a2,
                                      BOOL a3)
    = GetDlgItemInt;

UINT (__stdcall * Real_GetDlgItemTextA)(HWND a0,
                                        int a1,
                                        LPSTR a2,
                                        int a3)
    = GetDlgItemTextA;

UINT (__stdcall * Real_GetDlgItemTextW)(HWND a0,
                                        int a1,
                                        LPWSTR a2,
                                        int a3)
    = GetDlgItemTextW;

UINT (__stdcall * Real_GetDoubleClickTime)(void)
    = GetDoubleClickTime;

UINT (__stdcall * Real_GetDriveTypeA)(LPCSTR a0)
    = GetDriveTypeA;

UINT (__stdcall * Real_GetDriveTypeW)(LPCWSTR a0)
    = GetDriveTypeW;

HENHMETAFILE (__stdcall * Real_GetEnhMetaFileA)(LPCSTR a0)
    = GetEnhMetaFileA;

UINT (__stdcall * Real_GetEnhMetaFileBits)(HENHMETAFILE a0,
                                           UINT a1,
                                           LPBYTE a2)
    = GetEnhMetaFileBits;

UINT (__stdcall * Real_GetEnhMetaFileDescriptionA)(HENHMETAFILE a0,
                                                   UINT a1,
                                                   LPSTR a2)
    = GetEnhMetaFileDescriptionA;

UINT (__stdcall * Real_GetEnhMetaFileDescriptionW)(HENHMETAFILE a0,
                                                   UINT a1,
                                                   LPWSTR a2)
    = GetEnhMetaFileDescriptionW;

UINT (__stdcall * Real_GetEnhMetaFileHeader)(HENHMETAFILE a0,
                                             UINT a1,
                                             ENHMETAHEADER* a2)
    = GetEnhMetaFileHeader;

UINT (__stdcall * Real_GetEnhMetaFilePaletteEntries)(HENHMETAFILE a0,
                                                     UINT a1,
                                                     PALETTEENTRY* a2)
    = GetEnhMetaFilePaletteEntries;

UINT (__stdcall * Real_GetEnhMetaFilePixelFormat)(HENHMETAFILE a0,
                                                  UINT a1,
                                                  PIXELFORMATDESCRIPTOR* a2)
    = GetEnhMetaFilePixelFormat;

HENHMETAFILE (__stdcall * Real_GetEnhMetaFileW)(LPCWSTR a0)
    = GetEnhMetaFileW;

#undef GetEnvironmentStrings

LPSTR (__stdcall * Real_GetEnvironmentStrings)(void)
    = GetEnvironmentStrings;

LPWSTR (__stdcall * Real_GetEnvironmentStringsW)(void)
    = GetEnvironmentStringsW;

DWORD (__stdcall * Real_GetEnvironmentVariableA)(LPCSTR a0,
                                                 LPSTR a1,
                                                 DWORD a2)
    = GetEnvironmentVariableA;

DWORD (__stdcall * Real_GetEnvironmentVariableW)(LPCWSTR a0,
                                                 LPWSTR a1,
                                                 DWORD a2)
    = GetEnvironmentVariableW;

BOOL (__stdcall * Real_GetExitCodeProcess)(HANDLE a0,
                                           LPDWORD a1)
    = GetExitCodeProcess;

BOOL (__stdcall * Real_GetExitCodeThread)(HANDLE a0,
                                          LPDWORD a1)
    = GetExitCodeThread;

DWORD (__stdcall * Real_GetFileAttributesA)(LPCSTR a0)
    = GetFileAttributesA;

BOOL (__stdcall * Real_GetFileAttributesExA)(LPCSTR a0,
                                             enum _GET_FILEEX_INFO_LEVELS a1,
                                             LPVOID a2)
    = GetFileAttributesExA;

BOOL (__stdcall * Real_GetFileAttributesExW)(LPCWSTR a0,
                                             enum _GET_FILEEX_INFO_LEVELS a1,
                                             LPVOID a2)
    = GetFileAttributesExW;

DWORD (__stdcall * Real_GetFileAttributesW)(LPCWSTR a0)
    = GetFileAttributesW;

BOOL (__stdcall * Real_GetFileInformationByHandle)(HANDLE a0,
                                                   LPBY_HANDLE_FILE_INFORMATION a1)
    = GetFileInformationByHandle;

DWORD (__stdcall * Real_GetFileSize)(HANDLE a0,
                                     LPDWORD a1)
    = GetFileSize;

BOOL (__stdcall * Real_GetFileTime)(HANDLE a0,
                                    LPFILETIME a1,
                                    LPFILETIME a2,
                                    LPFILETIME a3)
    = GetFileTime;

DWORD (__stdcall * Real_GetFileType)(HANDLE a0)
    = GetFileType;

HWND (__stdcall * Real_GetFocus)(void)
    = GetFocus;

DWORD (__stdcall * Real_GetFontData)(HDC a0,
                                     DWORD a1,
                                     DWORD a2,
                                     LPVOID a3,
                                     DWORD a4)
    = GetFontData;

DWORD (__stdcall * Real_GetFontLanguageInfo)(HDC a0)
    = GetFontLanguageInfo;

HWND (__stdcall * Real_GetForegroundWindow)(void)
    = GetForegroundWindow;

DWORD (__stdcall * Real_GetFullPathNameA)(LPCSTR a0,
                                          DWORD a1,
                                          LPSTR a2,
                                          LPSTR* a3)
    = GetFullPathNameA;

DWORD (__stdcall * Real_GetFullPathNameW)(LPCWSTR a0,
                                          DWORD a1,
                                          LPWSTR a2,
                                          LPWSTR* a3)
    = GetFullPathNameW;

DWORD (__stdcall * Real_GetGlyphOutlineA)(HDC a0,
                                          UINT a1,
                                          UINT a2,
                                          LPGLYPHMETRICS a3,
                                          DWORD a4,
                                          LPVOID a5,
                                          CONST MAT2* a6)
    = GetGlyphOutlineA;

DWORD (__stdcall * Real_GetGlyphOutlineW)(HDC a0,
                                          UINT a1,
                                          UINT a2,
                                          LPGLYPHMETRICS a3,
                                          DWORD a4,
                                          LPVOID a5,
                                          CONST MAT2* a6)
    = GetGlyphOutlineW;

int (__stdcall * Real_GetGraphicsMode)(HDC a0)
    = GetGraphicsMode;

HRESULT (__stdcall * Real_GetHGlobalFromILockBytes)(ILockBytes* a0,
                                                    HGLOBAL* a1)
    = GetHGlobalFromILockBytes;

HRESULT (__stdcall * Real_GetHGlobalFromStream)(LPSTREAM a0,
                                                HGLOBAL* a1)
    = GetHGlobalFromStream;

BOOL (__stdcall * Real_GetHandleInformation)(HANDLE a0,
                                             LPDWORD a1)
    = GetHandleInformation;

BOOL (__stdcall * Real_GetICMProfileA)(HDC a0,
                                       LPDWORD a1,
                                       LPSTR a2)
    = GetICMProfileA;

BOOL (__stdcall * Real_GetICMProfileW)(HDC a0,
                                       LPDWORD a1,
                                       LPWSTR a2)
    = GetICMProfileW;

BOOL (__stdcall * Real_GetIconInfo)(HICON a0,
                                    PICONINFO a1)
    = GetIconInfo;

BOOL (__stdcall * Real_GetInputState)(void)
    = GetInputState;

UINT (__stdcall * Real_GetKBCodePage)(void)
    = GetKBCodePage;

DWORD (__stdcall * Real_GetKerningPairsA)(HDC a0,
                                          DWORD a1,
                                          KERNINGPAIR* a2)
    = GetKerningPairsA;

DWORD (__stdcall * Real_GetKerningPairsW)(HDC a0,
                                          DWORD a1,
                                          KERNINGPAIR* a2)
    = GetKerningPairsW;

int (__stdcall * Real_GetKeyNameTextA)(LONG a0,
                                       LPSTR a1,
                                       int a2)
    = GetKeyNameTextA;

int (__stdcall * Real_GetKeyNameTextW)(LONG a0,
                                       LPWSTR a1,
                                       int a2)
    = GetKeyNameTextW;

SHORT (__stdcall * Real_GetKeyState)(int a0)
    = GetKeyState;

HKL (__stdcall * Real_GetKeyboardLayout)(DWORD a0)
    = GetKeyboardLayout;

int (__stdcall * Real_GetKeyboardLayoutList)(int a0,
                                             HKL* a1)
    = GetKeyboardLayoutList;

BOOL (__stdcall * Real_GetKeyboardLayoutNameA)(LPSTR a0)
    = GetKeyboardLayoutNameA;

BOOL (__stdcall * Real_GetKeyboardLayoutNameW)(LPWSTR a0)
    = GetKeyboardLayoutNameW;

BOOL (__stdcall * Real_GetKeyboardState)(PBYTE a0)
    = GetKeyboardState;

int (__stdcall * Real_GetKeyboardType)(int a0)
    = GetKeyboardType;

HWND (__stdcall * Real_GetLastActivePopup)(HWND a0)
    = GetLastActivePopup;

void (__stdcall * Real_GetLocalTime)(LPSYSTEMTIME a0)
    = GetLocalTime;

int (__stdcall * Real_GetLocaleInfoA)(LCID a0,
                                      LCTYPE a1,
                                      LPSTR a2,
                                      int a3)
    = GetLocaleInfoA;

int (__stdcall * Real_GetLocaleInfoW)(LCID a0,
                                      LCTYPE a1,
                                      LPWSTR a2,
                                      int a3)
    = GetLocaleInfoW;

BOOL (__stdcall * Real_GetLogColorSpaceA)(HCOLORSPACE a0,
                                          LOGCOLORSPACEA* a1,
                                          DWORD a2)
    = GetLogColorSpaceA;

BOOL (__stdcall * Real_GetLogColorSpaceW)(HCOLORSPACE a0,
                                          LOGCOLORSPACEW* a1,
                                          DWORD a2)
    = GetLogColorSpaceW;

DWORD (__stdcall * Real_GetLogicalDriveStringsA)(DWORD a0,
                                                 LPSTR a1)
    = GetLogicalDriveStringsA;

DWORD (__stdcall * Real_GetLogicalDriveStringsW)(DWORD a0,
                                                 LPWSTR a1)
    = GetLogicalDriveStringsW;

DWORD (__stdcall * Real_GetLogicalDrives)(void)
    = GetLogicalDrives;

BOOL (__stdcall * Real_GetMailslotInfo)(HANDLE a0,
                                        LPDWORD a1,
                                        LPDWORD a2,
                                        LPDWORD a3,
                                        LPDWORD a4)
    = GetMailslotInfo;

int (__stdcall * Real_GetMapMode)(HDC a0)
    = GetMapMode;

HMENU (__stdcall * Real_GetMenu)(HWND a0)
    = GetMenu;

LONG (__stdcall * Real_GetMenuCheckMarkDimensions)(void)
    = GetMenuCheckMarkDimensions;

DWORD (__stdcall * Real_GetMenuContextHelpId)(HMENU a0)
    = GetMenuContextHelpId;

UINT (__stdcall * Real_GetMenuDefaultItem)(HMENU a0,
                                           UINT a1,
                                           UINT a2)
    = GetMenuDefaultItem;

int (__stdcall * Real_GetMenuItemCount)(HMENU a0)
    = GetMenuItemCount;

UINT (__stdcall * Real_GetMenuItemID)(HMENU a0,
                                      int a1)
    = GetMenuItemID;

BOOL (__stdcall * Real_GetMenuItemInfoA)(HMENU a0,
                                         UINT a1,
                                         BOOL a2,
                                         LPMENUITEMINFOA a3)
    = GetMenuItemInfoA;

BOOL (__stdcall * Real_GetMenuItemInfoW)(HMENU a0,
                                         UINT a1,
                                         BOOL a2,
                                         LPMENUITEMINFOW a3)
    = GetMenuItemInfoW;

BOOL (__stdcall * Real_GetMenuItemRect)(HWND a0,
                                        HMENU a1,
                                        UINT a2,
                                        LPRECT a3)
    = GetMenuItemRect;

UINT (__stdcall * Real_GetMenuState)(HMENU a0,
                                     UINT a1,
                                     UINT a2)
    = GetMenuState;

int (__stdcall * Real_GetMenuStringA)(HMENU a0,
                                      UINT a1,
                                      LPSTR a2,
                                      int a3,
                                      UINT a4)
    = GetMenuStringA;

int (__stdcall * Real_GetMenuStringW)(HMENU a0,
                                      UINT a1,
                                      LPWSTR a2,
                                      int a3,
                                      UINT a4)
    = GetMenuStringW;

BOOL (__stdcall * Real_GetMessageA)(LPMSG a0,
                                    HWND a1,
                                    UINT a2,
                                    UINT a3)
    = GetMessageA;

LPARAM (__stdcall * Real_GetMessageExtraInfo)(void)
    = GetMessageExtraInfo;

DWORD (__stdcall * Real_GetMessagePos)(void)
    = GetMessagePos;

LONG (__stdcall * Real_GetMessageTime)(void)
    = GetMessageTime;

BOOL (__stdcall * Real_GetMessageW)(LPMSG a0,
                                    HWND a1,
                                    UINT a2,
                                    UINT a3)
    = GetMessageW;

HMETAFILE (__stdcall * Real_GetMetaFileA)(LPCSTR a0)
    = GetMetaFileA;

UINT (__stdcall * Real_GetMetaFileBitsEx)(HMETAFILE a0,
                                          UINT a1,
                                          LPVOID a2)
    = GetMetaFileBitsEx;

HMETAFILE (__stdcall * Real_GetMetaFileW)(LPCWSTR a0)
    = GetMetaFileW;

int (__stdcall * Real_GetMetaRgn)(HDC a0,
                                  HRGN a1)
    = GetMetaRgn;

BOOL (__stdcall * Real_GetMiterLimit)(HDC a0,
                                      PFLOAT a1)
    = GetMiterLimit;

DWORD (__stdcall * Real_GetModuleFileNameA)(HMODULE a0,
                                            LPSTR a1,
                                            DWORD a2)
    = GetModuleFileNameA;

DWORD (__stdcall * Real_GetModuleFileNameW)(HMODULE a0,
                                            LPWSTR a1,
                                            DWORD a2)
    = GetModuleFileNameW;

HMODULE (__stdcall * Real_GetModuleHandleA)(LPCSTR a0)
    = GetModuleHandleA;

HMODULE (__stdcall * Real_GetModuleHandleW)(LPCWSTR a0)
    = GetModuleHandleW;

BOOL (__stdcall * Real_GetNamedPipeHandleStateA)(HANDLE a0,
                                                 LPDWORD a1,
                                                 LPDWORD a2,
                                                 LPDWORD a3,
                                                 LPDWORD a4,
                                                 LPSTR a5,
                                                 DWORD a6)
    = GetNamedPipeHandleStateA;

BOOL (__stdcall * Real_GetNamedPipeHandleStateW)(HANDLE a0,
                                                 LPDWORD a1,
                                                 LPDWORD a2,
                                                 LPDWORD a3,
                                                 LPDWORD a4,
                                                 LPWSTR a5,
                                                 DWORD a6)
    = GetNamedPipeHandleStateW;

BOOL (__stdcall * Real_GetNamedPipeInfo)(HANDLE a0,
                                         LPDWORD a1,
                                         LPDWORD a2,
                                         LPDWORD a3,
                                         LPDWORD a4)
    = GetNamedPipeInfo;

COLORREF (__stdcall * Real_GetNearestColor)(HDC a0,
                                            COLORREF a1)
    = GetNearestColor;

UINT (__stdcall * Real_GetNearestPaletteIndex)(HPALETTE a0,
                                               COLORREF a1)
    = GetNearestPaletteIndex;

HWND (__stdcall * Real_GetNextDlgGroupItem)(HWND a0,
                                            HWND a1,
                                            BOOL a2)
    = GetNextDlgGroupItem;

HWND (__stdcall * Real_GetNextDlgTabItem)(HWND a0,
                                          HWND a1,
                                          BOOL a2)
    = GetNextDlgTabItem;

int (__stdcall * Real_GetNumberFormatA)(LCID a0,
                                        DWORD a1,
                                        LPCSTR a2,
                                        CONST NUMBERFMTA* a3,
                                        LPSTR a4,
                                        int a5)
    = GetNumberFormatA;

int (__stdcall * Real_GetNumberFormatW)(LCID a0,
                                        DWORD a1,
                                        LPCWSTR a2,
                                        CONST NUMBERFMTW* a3,
                                        LPWSTR a4,
                                        int a5)
    = GetNumberFormatW;

BOOL (__stdcall * Real_GetNumberOfConsoleInputEvents)(HANDLE a0,
                                                      LPDWORD a1)
    = GetNumberOfConsoleInputEvents;

BOOL (__stdcall * Real_GetNumberOfConsoleMouseButtons)(LPDWORD a0)
    = GetNumberOfConsoleMouseButtons;

UINT (__stdcall * Real_GetOEMCP)(void)
    = GetOEMCP;

int (__stdcall * Real_GetObjectA)(HGDIOBJ a0,
                                  int a1,
                                  LPVOID a2)
    = GetObjectA;

DWORD (__stdcall * Real_GetObjectType)(HGDIOBJ a0)
    = GetObjectType;

int (__stdcall * Real_GetObjectW)(HGDIOBJ a0,
                                  int a1,
                                  LPVOID a2)
    = GetObjectW;

HWND (__stdcall * Real_GetOpenClipboardWindow)(void)
    = GetOpenClipboardWindow;

UINT (__stdcall * Real_GetOutlineTextMetricsA)(HDC a0,
                                               UINT a1,
                                               LPOUTLINETEXTMETRICA a2)
    = GetOutlineTextMetricsA;

UINT (__stdcall * Real_GetOutlineTextMetricsW)(HDC a0,
                                               UINT a1,
                                               LPOUTLINETEXTMETRICW a2)
    = GetOutlineTextMetricsW;

BOOL (__stdcall * Real_GetOverlappedResult)(HANDLE a0,
                                            LPOVERLAPPED a1,
                                            LPDWORD a2,
                                            BOOL a3)
    = GetOverlappedResult;

UINT (__stdcall * Real_GetPaletteEntries)(HPALETTE a0,
                                          UINT a1,
                                          UINT a2,
                                          PALETTEENTRY* a3)
    = GetPaletteEntries;

HWND (__stdcall * Real_GetParent)(HWND a0)
    = GetParent;

int (__stdcall * Real_GetPath)(HDC a0,
                               POINT* a1,
                               LPBYTE a2,
                               int a3)
    = GetPath;

COLORREF (__stdcall * Real_GetPixel)(HDC a0,
                                     int a1,
                                     int a2)
    = GetPixel;

int (__stdcall * Real_GetPixelFormat)(HDC a0)
    = GetPixelFormat;

int (__stdcall * Real_GetPolyFillMode)(HDC a0)
    = GetPolyFillMode;

DWORD (__stdcall * Real_GetPriorityClass)(HANDLE a0)
    = GetPriorityClass;

int (__stdcall * Real_GetPriorityClipboardFormat)(UINT* a0,
                                                  int a1)
    = GetPriorityClipboardFormat;

UINT (__stdcall * Real_GetPrivateProfileIntA)(LPCSTR a0,
                                              LPCSTR a1,
                                              INT a2,
                                              LPCSTR a3)
    = GetPrivateProfileIntA;

UINT (__stdcall * Real_GetPrivateProfileIntW)(LPCWSTR a0,
                                              LPCWSTR a1,
                                              INT a2,
                                              LPCWSTR a3)
    = GetPrivateProfileIntW;

DWORD (__stdcall * Real_GetPrivateProfileSectionA)(LPCSTR a0,
                                                   LPSTR a1,
                                                   DWORD a2,
                                                   LPCSTR a3)
    = GetPrivateProfileSectionA;

DWORD (__stdcall * Real_GetPrivateProfileSectionNamesA)(LPSTR a0,
                                                        DWORD a1,
                                                        LPCSTR a2)
    = GetPrivateProfileSectionNamesA;

DWORD (__stdcall * Real_GetPrivateProfileSectionNamesW)(LPWSTR a0,
                                                        DWORD a1,
                                                        LPCWSTR a2)
    = GetPrivateProfileSectionNamesW;

DWORD (__stdcall * Real_GetPrivateProfileSectionW)(LPCWSTR a0,
                                                   LPWSTR a1,
                                                   DWORD a2,
                                                   LPCWSTR a3)
    = GetPrivateProfileSectionW;

DWORD (__stdcall * Real_GetPrivateProfileStringA)(LPCSTR a0,
                                                  LPCSTR a1,
                                                  LPCSTR a2,
                                                  LPSTR a3,
                                                  DWORD a4,
                                                  LPCSTR a5)
    = GetPrivateProfileStringA;

DWORD (__stdcall * Real_GetPrivateProfileStringW)(LPCWSTR a0,
                                                  LPCWSTR a1,
                                                  LPCWSTR a2,
                                                  LPWSTR a3,
                                                  DWORD a4,
                                                  LPCWSTR a5)
    = GetPrivateProfileStringW;

BOOL (__stdcall * Real_GetPrivateProfileStructA)(LPCSTR a0,
                                                 LPCSTR a1,
                                                 LPVOID a2,
                                                 UINT a3,
                                                 LPCSTR a4)
    = GetPrivateProfileStructA;

BOOL (__stdcall * Real_GetPrivateProfileStructW)(LPCWSTR a0,
                                                 LPCWSTR a1,
                                                 LPVOID a2,
                                                 UINT a3,
                                                 LPCWSTR a4)
    = GetPrivateProfileStructW;

FARPROC (__stdcall * Real_GetProcAddress)(HMODULE a0,
                                          LPCSTR a1)
    = GetProcAddress;

BOOL (__stdcall * Real_GetProcessAffinityMask)(HANDLE a0,
                                               PDWORD_PTR a1,
                                               PDWORD_PTR a2)
    = GetProcessAffinityMask;

DWORD (__stdcall * Real_GetProcessHeaps)(DWORD a0,
                                         PHANDLE a1)
    = GetProcessHeaps;

BOOL (__stdcall * Real_GetProcessShutdownParameters)(LPDWORD a0,
                                                     LPDWORD a1)
    = GetProcessShutdownParameters;

BOOL (__stdcall * Real_GetProcessTimes)(HANDLE a0,
                                        LPFILETIME a1,
                                        LPFILETIME a2,
                                        LPFILETIME a3,
                                        LPFILETIME a4)
    = GetProcessTimes;

DWORD (__stdcall * Real_GetProcessVersion)(DWORD a0)
    = GetProcessVersion;

HWINSTA (__stdcall * Real_GetProcessWindowStation)(void)
    = GetProcessWindowStation;

BOOL (__stdcall * Real_GetProcessWorkingSetSize)(HANDLE a0,
                                                 PSIZE_T a1,
                                                 PSIZE_T a2)
    = GetProcessWorkingSetSize;

UINT (__stdcall * Real_GetProfileIntA)(LPCSTR a0,
                                       LPCSTR a1,
                                       INT a2)
    = GetProfileIntA;

UINT (__stdcall * Real_GetProfileIntW)(LPCWSTR a0,
                                       LPCWSTR a1,
                                       INT a2)
    = GetProfileIntW;

DWORD (__stdcall * Real_GetProfileSectionA)(LPCSTR a0,
                                            LPSTR a1,
                                            DWORD a2)
    = GetProfileSectionA;

DWORD (__stdcall * Real_GetProfileSectionW)(LPCWSTR a0,
                                            LPWSTR a1,
                                            DWORD a2)
    = GetProfileSectionW;

DWORD (__stdcall * Real_GetProfileStringA)(LPCSTR a0,
                                           LPCSTR a1,
                                           LPCSTR a2,
                                           LPSTR a3,
                                           DWORD a4)
    = GetProfileStringA;

DWORD (__stdcall * Real_GetProfileStringW)(LPCWSTR a0,
                                           LPCWSTR a1,
                                           LPCWSTR a2,
                                           LPWSTR a3,
                                           DWORD a4)
    = GetProfileStringW;

HANDLE (__stdcall * Real_GetPropA)(HWND a0,
                                   LPCSTR a1)
    = GetPropA;

HANDLE (__stdcall * Real_GetPropW)(HWND a0,
                                   LPCWSTR a1)
    = GetPropW;

DWORD (__stdcall * Real_GetQueueStatus)(UINT a0)
    = GetQueueStatus;

BOOL (__stdcall * Real_GetQueuedCompletionStatus)(HANDLE a0,
                                                  LPDWORD a1,
                                                  PULONG_PTR a2,
                                                  LPOVERLAPPED* a3,
                                                  DWORD a4)
    = GetQueuedCompletionStatus;

int (__stdcall * Real_GetROP2)(HDC a0)
    = GetROP2;

BOOL (__stdcall * Real_GetRasterizerCaps)(LPRASTERIZER_STATUS a0,
                                          UINT a1)
    = GetRasterizerCaps;

DWORD (__stdcall * Real_GetRegionData)(HRGN a0,
                                       DWORD a1,
                                       LPRGNDATA a2)
    = GetRegionData;

int (__stdcall * Real_GetRgnBox)(HRGN a0,
                                 LPRECT a1)
    = GetRgnBox;

HRESULT (__stdcall * Real_GetRunningObjectTable)(DWORD a0,
                                                 LPRUNNINGOBJECTTABLE* a1)
    = GetRunningObjectTable;

BOOL (__stdcall * Real_GetScrollInfo)(HWND a0,
                                      int a1,
                                      LPSCROLLINFO a2)
    = GetScrollInfo;

int (__stdcall * Real_GetScrollPos)(HWND a0,
                                    int a1)
    = GetScrollPos;

BOOL (__stdcall * Real_GetScrollRange)(HWND a0,
                                       int a1,
                                       LPINT a2,
                                       LPINT a3)
    = GetScrollRange;

DWORD (__stdcall * Real_GetShortPathNameA)(LPCSTR a0,
                                           LPSTR a1,
                                           DWORD a2)
    = GetShortPathNameA;

DWORD (__stdcall * Real_GetShortPathNameW)(LPCWSTR a0,
                                           LPWSTR a1,
                                           DWORD a2)
    = GetShortPathNameW;

void (__stdcall * Real_GetStartupInfoA)(LPSTARTUPINFOA a0)
    = GetStartupInfoA;

void (__stdcall * Real_GetStartupInfoW)(LPSTARTUPINFOW a0)
    = GetStartupInfoW;

HANDLE (__stdcall * Real_GetStdHandle)(DWORD a0)
    = GetStdHandle;

HGDIOBJ (__stdcall * Real_GetStockObject)(int a0)
    = GetStockObject;

int (__stdcall * Real_GetStretchBltMode)(HDC a0)
    = GetStretchBltMode;

HMENU (__stdcall * Real_GetSubMenu)(HMENU a0,
                                    int a1)
    = GetSubMenu;

DWORD (__stdcall * Real_GetSysColor)(int a0)
    = GetSysColor;

HBRUSH (__stdcall * Real_GetSysColorBrush)(int a0)
    = GetSysColorBrush;

LCID (__stdcall * Real_GetSystemDefaultLCID)(void)
    = GetSystemDefaultLCID;

LANGID (__stdcall * Real_GetSystemDefaultLangID)(void)
    = GetSystemDefaultLangID;

UINT (__stdcall * Real_GetSystemDirectoryA)(LPSTR a0,
                                            UINT a1)
    = GetSystemDirectoryA;

UINT (__stdcall * Real_GetSystemDirectoryW)(LPWSTR a0,
                                            UINT a1)
    = GetSystemDirectoryW;

void (__stdcall * Real_GetSystemInfo)(LPSYSTEM_INFO a0)
    = GetSystemInfo;

HMENU (__stdcall * Real_GetSystemMenu)(HWND a0,
                                       BOOL a1)
    = GetSystemMenu;

int (__stdcall * Real_GetSystemMetrics)(int a0)
    = GetSystemMetrics;

UINT (__stdcall * Real_GetSystemPaletteEntries)(HDC a0,
                                                UINT a1,
                                                UINT a2,
                                                PALETTEENTRY* a3)
    = GetSystemPaletteEntries;

UINT (__stdcall * Real_GetSystemPaletteUse)(HDC a0)
    = GetSystemPaletteUse;

BOOL (__stdcall * Real_GetSystemPowerStatus)(LPSYSTEM_POWER_STATUS a0)
    = GetSystemPowerStatus;

void (__stdcall * Real_GetSystemTime)(LPSYSTEMTIME a0)
    = GetSystemTime;

BOOL (__stdcall * Real_GetSystemTimeAdjustment)(PDWORD a0,
                                                PDWORD a1,
                                                PBOOL a2)
    = GetSystemTimeAdjustment;

void (__stdcall * Real_GetSystemTimeAsFileTime)(LPFILETIME a0)
    = GetSystemTimeAsFileTime;

#if _MSC_VER < 1300
DWORD (__stdcall * Real_GetTabbedTextExtentA)(HDC a0,
                                              LPCSTR a1,
                                              int a2,
                                              int a3,
                                              INT* a4)
    = GetTabbedTextExtentA;
#else
DWORD (__stdcall * Real_GetTabbedTextExtentA)(HDC a0,
                                              LPCSTR a1,
                                              int a2,
                                              int a3,
                                              CONST INT* a4)
    = GetTabbedTextExtentA;
#endif

#if _MSC_VER < 1300
DWORD (__stdcall * Real_GetTabbedTextExtentW)(HDC a0,
                                              LPCWSTR a1,
                                              int a2,
                                              int a3,
                                              INT* a4)
    = GetTabbedTextExtentW;
#else
DWORD (__stdcall * Real_GetTabbedTextExtentW)(HDC a0,
                                              LPCWSTR a1,
                                              int a2,
                                              int a3,
                                              CONST INT* a4)
    = GetTabbedTextExtentW;
#endif

DWORD (__stdcall * Real_GetTapeParameters)(HANDLE a0,
                                           DWORD a1,
                                           LPDWORD a2,
                                           LPVOID a3)
    = GetTapeParameters;

DWORD (__stdcall * Real_GetTapePosition)(HANDLE a0,
                                         DWORD a1,
                                         LPDWORD a2,
                                         LPDWORD a3,
                                         LPDWORD a4)
    = GetTapePosition;

DWORD (__stdcall * Real_GetTapeStatus)(HANDLE a0)
    = GetTapeStatus;

UINT (__stdcall * Real_GetTempFileNameA)(LPCSTR a0,
                                         LPCSTR a1,
                                         UINT a2,
                                         LPSTR a3)
    = GetTempFileNameA;

UINT (__stdcall * Real_GetTempFileNameW)(LPCWSTR a0,
                                         LPCWSTR a1,
                                         UINT a2,
                                         LPWSTR a3)
    = GetTempFileNameW;

DWORD (__stdcall * Real_GetTempPathA)(DWORD a0,
                                      LPSTR a1)
    = GetTempPathA;

DWORD (__stdcall * Real_GetTempPathW)(DWORD a0,
                                      LPWSTR a1)
    = GetTempPathW;

UINT (__stdcall * Real_GetTextAlign)(HDC a0)
    = GetTextAlign;

int (__stdcall * Real_GetTextCharacterExtra)(HDC a0)
    = GetTextCharacterExtra;

int (__stdcall * Real_GetTextCharset)(HDC a0)
    = GetTextCharset;

int (__stdcall * Real_GetTextCharsetInfo)(HDC a0,
                                          LPFONTSIGNATURE a1,
                                          DWORD a2)
    = GetTextCharsetInfo;

COLORREF (__stdcall * Real_GetTextColor)(HDC a0)
    = GetTextColor;

BOOL (__stdcall * Real_GetTextExtentExPointA)(HDC a0,
                                              LPCSTR a1,
                                              int a2,
                                              int a3,
                                              LPINT a4,
                                              LPINT a5,
                                              SIZE* a6)
    = GetTextExtentExPointA;

BOOL (__stdcall * Real_GetTextExtentExPointW)(HDC a0,
                                              LPCWSTR a1,
                                              int a2,
                                              int a3,
                                              LPINT a4,
                                              LPINT a5,
                                              SIZE* a6)
    = GetTextExtentExPointW;

BOOL (__stdcall * Real_GetTextExtentPoint32A)(HDC a0,
                                              LPCSTR a1,
                                              int a2,
                                              SIZE* a3)
    = GetTextExtentPoint32A;

BOOL (__stdcall * Real_GetTextExtentPoint32W)(HDC a0,
                                              LPCWSTR a1,
                                              int a2,
                                              SIZE* a3)
    = GetTextExtentPoint32W;

BOOL (__stdcall * Real_GetTextExtentPointA)(HDC a0,
                                            LPCSTR a1,
                                            int a2,
                                            SIZE* a3)
    = GetTextExtentPointA;

BOOL (__stdcall * Real_GetTextExtentPointW)(HDC a0,
                                            LPCWSTR a1,
                                            int a2,
                                            SIZE* a3)
    = GetTextExtentPointW;

int (__stdcall * Real_GetTextFaceA)(HDC a0,
                                    int a1,
                                    LPSTR a2)
    = GetTextFaceA;

int (__stdcall * Real_GetTextFaceW)(HDC a0,
                                    int a1,
                                    LPWSTR a2)
    = GetTextFaceW;

BOOL (__stdcall * Real_GetTextMetricsA)(HDC a0,
                                        LPTEXTMETRICA a1)
    = GetTextMetricsA;

BOOL (__stdcall * Real_GetTextMetricsW)(HDC a0,
                                        LPTEXTMETRICW a1)
    = GetTextMetricsW;

BOOL (__stdcall * Real_GetThreadContext)(HANDLE a0,
                                         LPCONTEXT a1)
    = GetThreadContext;

HDESK (__stdcall * Real_GetThreadDesktop)(DWORD a0)
    = GetThreadDesktop;

#if(WINVER >= 0x0500)
LCID (__stdcall * Real_GetThreadLocale)(void)
    = GetThreadLocale;
#endif // (WINVER >= 0x0500)

int (__stdcall * Real_GetThreadPriority)(HANDLE a0)
    = GetThreadPriority;

BOOL (__stdcall * Real_GetThreadPriorityBoost)(HANDLE a0,
                                               PBOOL a1)
    = GetThreadPriorityBoost;

BOOL (__stdcall * Real_GetThreadSelectorEntry)(HANDLE a0,
                                               DWORD a1,
                                               LPLDT_ENTRY a2)
    = GetThreadSelectorEntry;

BOOL (__stdcall * Real_GetThreadTimes)(HANDLE a0,
                                       LPFILETIME a1,
                                       LPFILETIME a2,
                                       LPFILETIME a3,
                                       LPFILETIME a4)
    = GetThreadTimes;

DWORD (__stdcall * Real_GetTickCount)(void)
    = GetTickCount;

int (__stdcall * Real_GetTimeFormatA)(LCID a0,
                                      DWORD a1,
                                      CONST SYSTEMTIME* a2,
                                      LPCSTR a3,
                                      LPSTR a4,
                                      int a5)
    = GetTimeFormatA;

int (__stdcall * Real_GetTimeFormatW)(LCID a0,
                                      DWORD a1,
                                      CONST SYSTEMTIME* a2,
                                      LPCWSTR a3,
                                      LPWSTR a4,
                                      int a5)
    = GetTimeFormatW;

DWORD (__stdcall * Real_GetTimeZoneInformation)(LPTIME_ZONE_INFORMATION a0)
    = GetTimeZoneInformation;

HWND (__stdcall * Real_GetTopWindow)(HWND a0)
    = GetTopWindow;

BOOL (__stdcall * Real_GetUpdateRect)(HWND a0,
                                      LPRECT a1,
                                      BOOL a2)
    = GetUpdateRect;

int (__stdcall * Real_GetUpdateRgn)(HWND a0,
                                    HRGN a1,
                                    BOOL a2)
    = GetUpdateRgn;

LCID (__stdcall * Real_GetUserDefaultLCID)(void)
    = GetUserDefaultLCID;

LANGID (__stdcall * Real_GetUserDefaultLangID)(void)
    = GetUserDefaultLangID;

BOOL (__stdcall * Real_GetUserObjectInformationA)(HANDLE a0,
                                                  int a1,
                                                  PVOID a2,
                                                  DWORD a3,
                                                  LPDWORD a4)
    = GetUserObjectInformationA;

BOOL (__stdcall * Real_GetUserObjectInformationW)(HANDLE a0,
                                                  int a1,
                                                  PVOID a2,
                                                  DWORD a3,
                                                  LPDWORD a4)
    = GetUserObjectInformationW;

BOOL (__stdcall * Real_GetUserObjectSecurity)(HANDLE a0,
                                              PSECURITY_INFORMATION a1,
                                              PSECURITY_DESCRIPTOR a2,
                                              DWORD a3,
                                              LPDWORD a4)
    = GetUserObjectSecurity;

DWORD (__stdcall * Real_GetVersion)(void)
    = GetVersion;

BOOL (__stdcall * Real_GetVersionExA)(LPOSVERSIONINFOA a0)
    = GetVersionExA;

BOOL (__stdcall * Real_GetVersionExW)(LPOSVERSIONINFOW a0)
    = GetVersionExW;

BOOL (__stdcall * Real_GetViewportExtEx)(HDC a0,
                                         SIZE* a1)
    = GetViewportExtEx;

BOOL (__stdcall * Real_GetViewportOrgEx)(HDC a0,
                                         POINT* a1)
    = GetViewportOrgEx;

BOOL (__stdcall * Real_GetVolumeInformationA)(LPCSTR a0,
                                              LPSTR a1,
                                              DWORD a2,
                                              LPDWORD a3,
                                              LPDWORD a4,
                                              LPDWORD a5,
                                              LPSTR a6,
                                              DWORD a7)
    = GetVolumeInformationA;

BOOL (__stdcall * Real_GetVolumeInformationW)(LPCWSTR a0,
                                              LPWSTR a1,
                                              DWORD a2,
                                              LPDWORD a3,
                                              LPDWORD a4,
                                              LPDWORD a5,
                                              LPWSTR a6,
                                              DWORD a7)
    = GetVolumeInformationW;

UINT (__stdcall * Real_GetWinMetaFileBits)(HENHMETAFILE a0,
                                           UINT a1,
                                           LPBYTE a2,
                                           INT a3,
                                           HDC a4)
    = GetWinMetaFileBits;

HWND (__stdcall * Real_GetWindow)(HWND a0,
                                  UINT a1)
    = GetWindow;

DWORD (__stdcall * Real_GetWindowContextHelpId)(HWND a0)
    = GetWindowContextHelpId;

HDC (__stdcall * Real_GetWindowDC)(HWND a0)
    = GetWindowDC;

BOOL (__stdcall * Real_GetWindowExtEx)(HDC a0,
                                       SIZE* a1)
    = GetWindowExtEx;

LONG (__stdcall * Real_GetWindowLongA)(HWND a0,
                                       int a1)
    = GetWindowLongA;

LONG (__stdcall * Real_GetWindowLongW)(HWND a0,
                                       int a1)
    = GetWindowLongW;

BOOL (__stdcall * Real_GetWindowOrgEx)(HDC a0,
                                       POINT* a1)
    = GetWindowOrgEx;

BOOL (__stdcall * Real_GetWindowPlacement)(HWND a0,
                                           WINDOWPLACEMENT* a1)
    = GetWindowPlacement;

BOOL (__stdcall * Real_GetWindowRect)(HWND a0,
                                      LPRECT a1)
    = GetWindowRect;

int (__stdcall * Real_GetWindowRgn)(HWND a0,
                                    HRGN a1)
    = GetWindowRgn;

int (__stdcall * Real_GetWindowTextA)(HWND a0,
                                      LPSTR a1,
                                      int a2)
    = GetWindowTextA;

int (__stdcall * Real_GetWindowTextLengthA)(HWND a0)
    = GetWindowTextLengthA;

int (__stdcall * Real_GetWindowTextLengthW)(HWND a0)
    = GetWindowTextLengthW;

int (__stdcall * Real_GetWindowTextW)(HWND a0,
                                      LPWSTR a1,
                                      int a2)
    = GetWindowTextW;

DWORD (__stdcall * Real_GetWindowThreadProcessId)(HWND a0,
                                                  LPDWORD a1)
    = GetWindowThreadProcessId;

WORD (__stdcall * Real_GetWindowWord)(HWND a0,
                                      int a1)
    = GetWindowWord;

UINT (__stdcall * Real_GetWindowsDirectoryA)(LPSTR a0,
                                             UINT a1)
    = GetWindowsDirectoryA;

UINT (__stdcall * Real_GetWindowsDirectoryW)(LPWSTR a0,
                                             UINT a1)
    = GetWindowsDirectoryW;

BOOL (__stdcall * Real_GetWorldTransform)(HDC a0,
                                          XFORM* a1)
    = GetWorldTransform;

ATOM (__stdcall * Real_GlobalAddAtomA)(LPCSTR a0)
    = GlobalAddAtomA;

ATOM (__stdcall * Real_GlobalAddAtomW)(LPCWSTR a0)
    = GlobalAddAtomW;

HGLOBAL (__stdcall * Real_GlobalAlloc)(UINT a0,
                                       SIZE_T a1)
    = GlobalAlloc;

#if _MSC_VER < 1300
UINT (__stdcall * Real_GlobalCompact)(DWORD a0)
    = GlobalCompact;
#else
SIZE_T (__stdcall * Real_GlobalCompact)(DWORD a0)
    = GlobalCompact;
#endif

ATOM (__stdcall * Real_GlobalDeleteAtom)(ATOM a0)
    = GlobalDeleteAtom;

ATOM (__stdcall * Real_GlobalFindAtomA)(LPCSTR a0)
    = GlobalFindAtomA;

ATOM (__stdcall * Real_GlobalFindAtomW)(LPCWSTR a0)
    = GlobalFindAtomW;

void (__stdcall * Real_GlobalFix)(HGLOBAL a0)
    = GlobalFix;

UINT (__stdcall * Real_GlobalFlags)(HGLOBAL a0)
    = GlobalFlags;

HGLOBAL (__stdcall * Real_GlobalFree)(HGLOBAL a0)
    = GlobalFree;

UINT (__stdcall * Real_GlobalGetAtomNameA)(ATOM a0,
                                           LPSTR a1,
                                           int a2)
    = GlobalGetAtomNameA;

UINT (__stdcall * Real_GlobalGetAtomNameW)(ATOM a0,
                                           LPWSTR a1,
                                           int a2)
    = GlobalGetAtomNameW;

HGLOBAL (__stdcall * Real_GlobalHandle)(LPCVOID a0)
    = GlobalHandle;

LPVOID (__stdcall * Real_GlobalLock)(HGLOBAL a0)
    = GlobalLock;

void (__stdcall * Real_GlobalMemoryStatus)(LPMEMORYSTATUS a0)
    = GlobalMemoryStatus;

HGLOBAL (__stdcall * Real_GlobalReAlloc)(HGLOBAL a0,
                                         SIZE_T a1,
                                         UINT a2)
    = GlobalReAlloc;

SIZE_T (__stdcall * Real_GlobalSize)(HGLOBAL a0)
    = GlobalSize;

BOOL (__stdcall * Real_GlobalUnWire)(HGLOBAL a0)
    = GlobalUnWire;

void (__stdcall * Real_GlobalUnfix)(HGLOBAL a0)
    = GlobalUnfix;

BOOL (__stdcall * Real_GlobalUnlock)(HGLOBAL a0)
    = GlobalUnlock;

LPVOID (__stdcall * Real_GlobalWire)(HGLOBAL a0)
    = GlobalWire;

BOOL (__stdcall * Real_GrayStringA)(HDC a0,
                                    HBRUSH a1,
                                    GRAYSTRINGPROC a2,
                                    LPARAM a3,
                                    int a4,
                                    int a5,
                                    int a6,
                                    int a7,
                                    int a8)
    = GrayStringA;

BOOL (__stdcall * Real_GrayStringW)(HDC a0,
                                    HBRUSH a1,
                                    GRAYSTRINGPROC a2,
                                    LPARAM a3,
                                    int a4,
                                    int a5,
                                    int a6,
                                    int a7,
                                    int a8)
    = GrayStringW;

BOOL (__stdcall * Real_HeapLock)(HANDLE a0)
    = HeapLock;

BOOL (__stdcall * Real_HeapUnlock)(HANDLE a0)
    = HeapUnlock;

BOOL (__stdcall * Real_HideCaret)(HWND a0)
    = HideCaret;

BOOL (__stdcall * Real_HiliteMenuItem)(HWND a0,
                                       HMENU a1,
                                       UINT a2,
                                       UINT a3)
    = HiliteMenuItem;

#ifdef _Real_WINNT_WIN7
HRESULT (__stdcall * Real_IIDFromString)(LPCOLESTR a0,
                                         LPGUID a1)
    = IIDFromString;
#else
HRESULT (__stdcall * Real_IIDFromString)(LPOLESTR a0,
                                         LPGUID a1)
    = IIDFromString;
#endif

BOOL (__stdcall * Real_ImpersonateDdeClientWindow)(HWND a0,
                                                   HWND a1)
    = ImpersonateDdeClientWindow;

BOOL (__stdcall * Real_InSendMessage)(void)
    = InSendMessage;

BOOL (__stdcall * Real_InflateRect)(LPRECT a0,
                                    int a1,
                                    int a2)
    = InflateRect;

BOOL (__stdcall * Real_InitAtomTable)(DWORD a0)
    = InitAtomTable;

BOOL (__stdcall * Real_InsertMenuA)(HMENU a0,
                                    UINT a1,
                                    UINT a2,
                                    UINT_PTR a3,
                                    LPCSTR a4)
    = InsertMenuA;

BOOL (__stdcall * Real_InsertMenuW)(HMENU a0,
                                    UINT a1,
                                    UINT a2,
                                    UINT_PTR a3,
                                    LPCWSTR a4)
    = InsertMenuW;

BOOL (__stdcall * Real_InsertMenuItemA)(HMENU a0,
                                        UINT a1,
                                        BOOL a2,
                                        LPCMENUITEMINFOA a3)
    = InsertMenuItemA;

BOOL (__stdcall * Real_InsertMenuItemW)(HMENU a0,
                                        UINT a1,
                                        BOOL a2,
                                        LPCMENUITEMINFOW a3)
    = InsertMenuItemW;

int (__stdcall * Real_IntersectClipRect)(HDC a0,
                                         int a1,
                                         int a2,
                                         int a3,
                                         int a4)
    = IntersectClipRect;

BOOL (__stdcall * Real_IntersectRect)(LPRECT a0,
                                      CONST RECT* a1,
                                      CONST RECT* a2)
    = IntersectRect;

BOOL (__stdcall * Real_InvalidateRect)(HWND a0,
                                       CONST RECT* a1,
                                       BOOL a2)
    = InvalidateRect;

BOOL (__stdcall * Real_InvalidateRgn)(HWND a0,
                                      HRGN a1,
                                      BOOL a2)
    = InvalidateRgn;

BOOL (__stdcall * Real_InvertRect)(HDC a0,
                                   CONST RECT* a1)
    = InvertRect;

BOOL (__stdcall * Real_InvertRgn)(HDC a0,
                                  HRGN a1)
    = InvertRgn;

BOOL (__stdcall * Real_IsAccelerator)(HACCEL a0,
                                      int a1,
                                      LPMSG a2,
                                      WORD* a3)
    = IsAccelerator;

BOOL (__stdcall * Real_IsBadCodePtr)(FARPROC a0)
    = IsBadCodePtr;

BOOL (__stdcall * Real_IsBadHugeReadPtr)(CONST void* a0,
                                         UINT_PTR a1)
    = IsBadHugeReadPtr;

BOOL (__stdcall * Real_IsBadHugeWritePtr)(LPVOID a0,
                                          UINT_PTR a1)
    = IsBadHugeWritePtr;

BOOL (__stdcall * Real_IsBadReadPtr)(CONST void* a0,
                                     UINT_PTR a1)
    = IsBadReadPtr;

BOOL (__stdcall * Real_IsBadStringPtrA)(LPCSTR a0,
                                        UINT_PTR a1)
    = IsBadStringPtrA;

BOOL (__stdcall * Real_IsBadStringPtrW)(LPCWSTR a0,
                                        UINT_PTR a1)
    = IsBadStringPtrW;

BOOL (__stdcall * Real_IsBadWritePtr)(LPVOID a0,
                                      UINT_PTR a1)
    = IsBadWritePtr;

BOOL (__stdcall * Real_IsCharAlphaA)(CHAR a0)
    = IsCharAlphaA;

BOOL (__stdcall * Real_IsCharAlphaNumericA)(CHAR a0)
    = IsCharAlphaNumericA;

BOOL (__stdcall * Real_IsCharAlphaNumericW)(WCHAR a0)
    = IsCharAlphaNumericW;

BOOL (__stdcall * Real_IsCharAlphaW)(WCHAR a0)
    = IsCharAlphaW;

BOOL (__stdcall * Real_IsCharLowerA)(CHAR a0)
    = IsCharLowerA;

BOOL (__stdcall * Real_IsCharLowerW)(WCHAR a0)
    = IsCharLowerW;

BOOL (__stdcall * Real_IsCharUpperA)(CHAR a0)
    = IsCharUpperA;

BOOL (__stdcall * Real_IsCharUpperW)(WCHAR a0)
    = IsCharUpperW;

BOOL (__stdcall * Real_IsChild)(HWND a0,
                                HWND a1)
    = IsChild;

BOOL (__stdcall * Real_IsClipboardFormatAvailable)(UINT a0)
    = IsClipboardFormatAvailable;

BOOL (__stdcall * Real_IsDBCSLeadByte)(BYTE a0)
    = IsDBCSLeadByte;

BOOL (__stdcall * Real_IsDBCSLeadByteEx)(UINT a0,
                                         BYTE a1)
    = IsDBCSLeadByteEx;

BOOL (__stdcall * Real_IsDebuggerPresent)(void)
    = IsDebuggerPresent;

BOOL (__stdcall * Real_IsDialogMessageA)(HWND a0,
                                         LPMSG a1)
    = IsDialogMessageA;

BOOL (__stdcall * Real_IsDialogMessageW)(HWND a0,
                                         LPMSG a1)
    = IsDialogMessageW;

UINT (__stdcall * Real_IsDlgButtonChecked)(HWND a0,
                                           int a1)
    = IsDlgButtonChecked;

BOOL (__stdcall * Real_IsIconic)(HWND a0)
    = IsIconic;

BOOL (__stdcall * Real_IsMenu)(HMENU a0)
    = IsMenu;

BOOL (__stdcall * Real_IsProcessorFeaturePresent)(DWORD a0)
    = IsProcessorFeaturePresent;

BOOL (__stdcall * Real_IsRectEmpty)(CONST RECT* a0)
    = IsRectEmpty;

BOOL (__stdcall * Real_IsValidCodePage)(UINT a0)
    = IsValidCodePage;

BOOL (__stdcall * Real_IsValidLocale)(LCID a0,
                                      DWORD a1)
    = IsValidLocale;

BOOL (__stdcall * Real_IsWindow)(HWND a0)
    = IsWindow;

BOOL (__stdcall * Real_IsWindowEnabled)(HWND a0)
    = IsWindowEnabled;

BOOL (__stdcall * Real_IsWindowUnicode)(HWND a0)
    = IsWindowUnicode;

BOOL (__stdcall * Real_IsWindowVisible)(HWND a0)
    = IsWindowVisible;

BOOL (__stdcall * Real_IsZoomed)(HWND a0)
    = IsZoomed;

BOOL (__stdcall * Real_KillTimer)(HWND a0,
                                  UINT_PTR a1)
    = KillTimer;

BOOL (__stdcall * Real_LPtoDP)(HDC a0,
                               POINT* a1,
                               int a2)
    = LPtoDP;

BOOL (__stdcall * Real_LineDDA)(int a0,
                                int a1,
                                int a2,
                                int a3,
                                LINEDDAPROC a4,
                                LPARAM a5)
    = LineDDA;

BOOL (__stdcall * Real_LineTo)(HDC a0,
                               int a1,
                               int a2)
    = LineTo;

HACCEL (__stdcall * Real_LoadAcceleratorsA)(HINSTANCE a0,
                                            LPCSTR a1)
    = LoadAcceleratorsA;

HACCEL (__stdcall * Real_LoadAcceleratorsW)(HINSTANCE a0,
                                            LPCWSTR a1)
    = LoadAcceleratorsW;

HBITMAP (__stdcall * Real_LoadBitmapA)(HINSTANCE a0,
                                       LPCSTR a1)
    = LoadBitmapA;

HBITMAP (__stdcall * Real_LoadBitmapW)(HINSTANCE a0,
                                       LPCWSTR a1)
    = LoadBitmapW;

HCURSOR (__stdcall * Real_LoadCursorA)(HINSTANCE a0,
                                       LPCSTR a1)
    = LoadCursorA;

HCURSOR (__stdcall * Real_LoadCursorFromFileA)(LPCSTR a0)
    = LoadCursorFromFileA;

HCURSOR (__stdcall * Real_LoadCursorFromFileW)(LPCWSTR a0)
    = LoadCursorFromFileW;

HCURSOR (__stdcall * Real_LoadCursorW)(HINSTANCE a0,
                                       LPCWSTR a1)
    = LoadCursorW;

HICON (__stdcall * Real_LoadIconA)(HINSTANCE a0,
                                   LPCSTR a1)
    = LoadIconA;

HICON (__stdcall * Real_LoadIconW)(HINSTANCE a0,
                                   LPCWSTR a1)
    = LoadIconW;

HANDLE (__stdcall * Real_LoadImageA)(HINSTANCE a0,
                                     LPCSTR a1,
                                     UINT a2,
                                     int a3,
                                     int a4,
                                     UINT a5)
    = LoadImageA;

HANDLE (__stdcall * Real_LoadImageW)(HINSTANCE a0,
                                     LPCWSTR a1,
                                     UINT a2,
                                     int a3,
                                     int a4,
                                     UINT a5)
    = LoadImageW;

HKL (__stdcall * Real_LoadKeyboardLayoutA)(LPCSTR a0,
                                           UINT a1)
    = LoadKeyboardLayoutA;

HKL (__stdcall * Real_LoadKeyboardLayoutW)(LPCWSTR a0,
                                           UINT a1)
    = LoadKeyboardLayoutW;

HMODULE (__stdcall * Real_LoadLibraryA)(LPCSTR a0)
    = LoadLibraryA;

HMODULE (__stdcall * Real_LoadLibraryExA)(LPCSTR a0,
                                          HANDLE a1,
                                          DWORD a2)
    = LoadLibraryExA;

HMODULE (__stdcall * Real_LoadLibraryExW)(LPCWSTR a0,
                                          HANDLE a1,
                                          DWORD a2)
    = LoadLibraryExW;

HMODULE (__stdcall * Real_LoadLibraryW)(LPCWSTR a0)
    = LoadLibraryW;

HMENU (__stdcall * Real_LoadMenuA)(HINSTANCE a0,
                                   LPCSTR a1)
    = LoadMenuA;

HMENU (__stdcall * Real_LoadMenuIndirectA)(CONST MENUTEMPLATEA* a0)
    = LoadMenuIndirectA;

HMENU (__stdcall * Real_LoadMenuIndirectW)(CONST MENUTEMPLATEW* a0)
    = LoadMenuIndirectW;

HMENU (__stdcall * Real_LoadMenuW)(HINSTANCE a0,
                                   LPCWSTR a1)
    = LoadMenuW;

DWORD (__stdcall * Real_LoadModule)(LPCSTR a0,
                                    LPVOID a1)
    = LoadModule;

HGLOBAL (__stdcall * Real_LoadResource)(HMODULE a0,
                                        HRSRC a1)
    = LoadResource;

int (__stdcall * Real_LoadStringA)(HINSTANCE a0,
                                   UINT a1,
                                   LPSTR a2,
                                   int a3)
    = LoadStringA;

int (__stdcall * Real_LoadStringW)(HINSTANCE a0,
                                   UINT a1,
                                   LPWSTR a2,
                                   int a3)
    = LoadStringW;

#if _MSC_VER < 1300
HLOCAL (__stdcall * Real_LocalAlloc)(UINT a0,
                                     UINT a1)
    = LocalAlloc;
#else
HLOCAL (__stdcall * Real_LocalAlloc)(UINT a0,
                                     SIZE_T a1)
    = LocalAlloc;
#endif

#if _MSC_VER < 1300
UINT (__stdcall * Real_LocalCompact)(UINT a0)
    = LocalCompact;
#else
SIZE_T (__stdcall * Real_LocalCompact)(UINT a0)
    = LocalCompact;
#endif

BOOL (__stdcall * Real_LocalFileTimeToFileTime)(CONST FILETIME* a0,
                                                LPFILETIME a1)
    = LocalFileTimeToFileTime;

UINT (__stdcall * Real_LocalFlags)(HLOCAL a0)
    = LocalFlags;

HLOCAL (__stdcall * Real_LocalFree)(HLOCAL a0)
    = LocalFree;

HLOCAL (__stdcall * Real_LocalHandle)(LPCVOID a0)
    = LocalHandle;

LPVOID (__stdcall * Real_LocalLock)(HLOCAL a0)
    = LocalLock;

#if _MSC_VER < 1300
HLOCAL (__stdcall * Real_LocalReAlloc)(HLOCAL a0,
                                       UINT a1,
                                       UINT a2)
    = LocalReAlloc;
#else
HLOCAL (__stdcall * Real_LocalReAlloc)(HLOCAL a0,
                                       SIZE_T a1,
                                       UINT a2)
    = LocalReAlloc;
#endif

#if _MSC_VER < 1300
UINT (__stdcall * Real_LocalShrink)(HLOCAL a0,
                                      UINT a1)
    = LocalShrink;
#else
SIZE_T (__stdcall * Real_LocalShrink)(HLOCAL a0,
                                      UINT a1)
    = LocalShrink;
#endif

#if _MSC_VER < 1300
UINT (__stdcall * Real_LocalSize)(HLOCAL a0)
    = LocalSize;
#else
SIZE_T (__stdcall * Real_LocalSize)(HLOCAL a0)
    = LocalSize;
#endif

BOOL (__stdcall * Real_LocalUnlock)(HLOCAL a0)
    = LocalUnlock;

BOOL (__stdcall * Real_LockFile)(HANDLE a0,
                                 DWORD a1,
                                 DWORD a2,
                                 DWORD a3,
                                 DWORD a4)
    = LockFile;

BOOL (__stdcall * Real_LockFileEx)(HANDLE a0,
                                   DWORD a1,
                                   DWORD a2,
                                   DWORD a3,
                                   DWORD a4,
                                   LPOVERLAPPED a5)
    = LockFileEx;

LPVOID (__stdcall * Real_LockResource)(HGLOBAL a0)
    = LockResource;

BOOL (__stdcall * Real_LockWindowUpdate)(HWND a0)
    = LockWindowUpdate;

int (__stdcall * Real_LookupIconIdFromDirectory)(PBYTE a0,
                                                 BOOL a1)
    = LookupIconIdFromDirectory;

int (__stdcall * Real_LookupIconIdFromDirectoryEx)(PBYTE a0,
                                                   BOOL a1,
                                                   int a2,
                                                   int a3,
                                                   UINT a4)
    = LookupIconIdFromDirectoryEx;

BOOL (__stdcall * Real_MapDialogRect)(HWND a0,
                                      LPRECT a1)
    = MapDialogRect;

LPVOID (__stdcall * Real_MapViewOfFile)(HANDLE a0,
                                        DWORD a1,
                                        DWORD a2,
                                        DWORD a3,
                                        SIZE_T a4)
    = MapViewOfFile;

LPVOID (__stdcall * Real_MapViewOfFileEx)(HANDLE a0,
                                          DWORD a1,
                                          DWORD a2,
                                          DWORD a3,
                                          SIZE_T a4,
                                          LPVOID a5)
    = MapViewOfFileEx;

UINT (__stdcall * Real_MapVirtualKeyA)(UINT a0,
                                       UINT a1)
    = MapVirtualKeyA;

UINT (__stdcall * Real_MapVirtualKeyExA)(UINT a0,
                                         UINT a1,
                                         HKL a2)
    = MapVirtualKeyExA;

UINT (__stdcall * Real_MapVirtualKeyExW)(UINT a0,
                                         UINT a1,
                                         HKL a2)
    = MapVirtualKeyExW;

UINT (__stdcall * Real_MapVirtualKeyW)(UINT a0,
                                       UINT a1)
    = MapVirtualKeyW;

int (__stdcall * Real_MapWindowPoints)(HWND a0,
                                       HWND a1,
                                       POINT* a2,
                                       UINT a3)
    = MapWindowPoints;

BOOL (__stdcall * Real_MaskBlt)(HDC a0,
                                int a1,
                                int a2,
                                int a3,
                                int a4,
                                HDC a5,
                                int a6,
                                int a7,
                                HBITMAP a8,
                                int a9,
                                int a10,
                                DWORD a11)
    = MaskBlt;

int (__stdcall * Real_MenuItemFromPoint)(HWND a0,
                                         HMENU a1,
                                         POINT a2)
    = MenuItemFromPoint;

BOOL (__stdcall * Real_MessageBeep)(UINT a0)
    = MessageBeep;

int (__stdcall * Real_MessageBoxA)(HWND a0,
                                   LPCSTR a1,
                                   LPCSTR a2,
                                   UINT a3)
    = MessageBoxA;

int (__stdcall * Real_MessageBoxExA)(HWND a0,
                                     LPCSTR a1,
                                     LPCSTR a2,
                                     UINT a3,
                                     WORD a4)
    = MessageBoxExA;

int (__stdcall * Real_MessageBoxExW)(HWND a0,
                                     LPCWSTR a1,
                                     LPCWSTR a2,
                                     UINT a3,
                                     WORD a4)
    = MessageBoxExW;

#if _MSC_VER < 1300
int (__stdcall * Real_MessageBoxIndirectA)(MSGBOXPARAMSA* a0)
    = MessageBoxIndirectA;
#else
int (__stdcall * Real_MessageBoxIndirectA)(CONST MSGBOXPARAMSA* a0)
    = MessageBoxIndirectA;
#endif

#if _MSC_VER < 1300
int (__stdcall * Real_MessageBoxIndirectW)(MSGBOXPARAMSW* a0)
    = MessageBoxIndirectW;
#else
int (__stdcall * Real_MessageBoxIndirectW)(CONST MSGBOXPARAMSW* a0)
    = MessageBoxIndirectW;
#endif

int (__stdcall * Real_MessageBoxW)(HWND a0,
                                   LPCWSTR a1,
                                   LPCWSTR a2,
                                   UINT a3)
    = MessageBoxW;

HRESULT (__stdcall * Real_MkParseDisplayName)(IBindCtx* a0,
                                              LPCOLESTR a1,
                                              ULONG* a2,
                                              IMoniker** a3)
    = MkParseDisplayName;

BOOL (__stdcall * Real_ModifyMenuA)(HMENU a0,
                                    UINT a1,
                                    UINT a2,
                                    UINT_PTR a3,
                                    LPCSTR a4)
    = ModifyMenuA;

BOOL (__stdcall * Real_ModifyMenuW)(HMENU a0,
                                    UINT a1,
                                    UINT a2,
                                    UINT_PTR a3,
                                    LPCWSTR a4)
    = ModifyMenuW;

BOOL (__stdcall * Real_ModifyWorldTransform)(HDC a0,
                                             CONST XFORM* a1,
                                             DWORD a2)
    = ModifyWorldTransform;

HRESULT (__stdcall * Real_MonikerCommonPrefixWith)(IMoniker* a0,
                                                   IMoniker* a1,
                                                   IMoniker** a2)
    = MonikerCommonPrefixWith;

HRESULT (__stdcall * Real_MonikerRelativePathTo)(IMoniker* a0,
                                                 IMoniker* a1,
                                                 IMoniker** a2,
                                                 BOOL a3)
    = MonikerRelativePathTo;

BOOL (__stdcall * Real_MoveFileA)(LPCSTR a0,
                                  LPCSTR a1)
    = MoveFileA;

BOOL (__stdcall * Real_MoveFileExA)(LPCSTR a0,
                                    LPCSTR a1,
                                    DWORD a2)
    = MoveFileExA;

BOOL (__stdcall * Real_MoveFileExW)(LPCWSTR a0,
                                    LPCWSTR a1,
                                    DWORD a2)
    = MoveFileExW;

BOOL (__stdcall * Real_MoveFileW)(LPCWSTR a0,
                                  LPCWSTR a1)
    = MoveFileW;

BOOL (__stdcall * Real_MoveToEx)(HDC a0,
                                 int a1,
                                 int a2,
                                 POINT* a3)
    = MoveToEx;

BOOL (__stdcall * Real_MoveWindow)(HWND a0,
                                   int a1,
                                   int a2,
                                   int a3,
                                   int a4,
                                   BOOL a5)
    = MoveWindow;

#if _MSC_VER < 1300
DWORD (__stdcall * Real_MsgWaitForMultipleObjects)(DWORD a0,
                                                   HANDLE * a1,
                                                   BOOL a2,
                                                   DWORD a3,
                                                   DWORD a4)
    = MsgWaitForMultipleObjects;
#else
DWORD (__stdcall * Real_MsgWaitForMultipleObjects)(DWORD a0,
                                                   CONST HANDLE * a1,
                                                   BOOL a2,
                                                   DWORD a3,
                                                   DWORD a4)
    = MsgWaitForMultipleObjects;
#endif

#if _MSC_VER < 1300
DWORD (__stdcall * Real_MsgWaitForMultipleObjectsEx)(DWORD a0,
                                                     HANDLE * a1,
                                                     DWORD a2,
                                                     DWORD a3,
                                                     DWORD a4)
    = MsgWaitForMultipleObjectsEx;
#else
DWORD (__stdcall * Real_MsgWaitForMultipleObjectsEx)(DWORD a0,
                                                     CONST HANDLE * a1,
                                                     DWORD a2,
                                                     DWORD a3,
                                                     DWORD a4)
    = MsgWaitForMultipleObjectsEx;
#endif

int (__stdcall * Real_MulDiv)(int a0,
                              int a1,
                              int a2)
    = MulDiv;

DWORD (__stdcall * Real_OemKeyScan)(WORD a0)
    = OemKeyScan;

BOOL (__stdcall * Real_OemToCharA)(LPCSTR a0,
                                   LPSTR a1)
    = OemToCharA;

BOOL (__stdcall * Real_OemToCharBuffA)(LPCSTR a0,
                                       LPSTR a1,
                                       DWORD a2)
    = OemToCharBuffA;

BOOL (__stdcall * Real_OemToCharBuffW)(LPCSTR a0,
                                       LPWSTR a1,
                                       DWORD a2)
    = OemToCharBuffW;

BOOL (__stdcall * Real_OemToCharW)(LPCSTR a0,
                                   LPWSTR a1)
    = OemToCharW;

int (__stdcall * Real_OffsetClipRgn)(HDC a0,
                                     int a1,
                                     int a2)
    = OffsetClipRgn;

BOOL (__stdcall * Real_OffsetRect)(LPRECT a0,
                                   int a1,
                                   int a2)
    = OffsetRect;

int (__stdcall * Real_OffsetRgn)(HRGN a0,
                                 int a1,
                                 int a2)
    = OffsetRgn;

BOOL (__stdcall * Real_OffsetViewportOrgEx)(HDC a0,
                                            int a1,
                                            int a2,
                                            POINT* a3)
    = OffsetViewportOrgEx;

BOOL (__stdcall * Real_OffsetWindowOrgEx)(HDC a0,
                                          int a1,
                                          int a2,
                                          POINT* a3)
    = OffsetWindowOrgEx;

DWORD (__stdcall * Real_OleBuildVersion)(void)
    = OleBuildVersion;

HRESULT (__stdcall * Real_OleConvertIStorageToOLESTREAM)(IStorage* a0,
                                                         LPOLESTREAM a1)
    = OleConvertIStorageToOLESTREAM;

HRESULT (__stdcall * Real_OleConvertIStorageToOLESTREAMEx)(IStorage* a0,
                                                           CLIPFORMAT a1,
                                                           LONG a2,
                                                           LONG a3,
                                                           DWORD a4,
                                                           LPSTGMEDIUM a5,
                                                           LPOLESTREAM a6)
    = OleConvertIStorageToOLESTREAMEx;

HRESULT (__stdcall * Real_OleConvertOLESTREAMToIStorage)(LPOLESTREAM a0,
                                                         LPSTORAGE a1,
                                                         CONST DVTARGETDEVICE* a2)
    = OleConvertOLESTREAMToIStorage;

HRESULT (__stdcall * Real_OleConvertOLESTREAMToIStorageEx)(LPOLESTREAM a0,
                                                           LPSTORAGE a1,
                                                           CLIPFORMAT* a2,
                                                           LONG* a3,
                                                           LONG* a4,
                                                           DWORD* a5,
                                                           LPSTGMEDIUM a6)
    = OleConvertOLESTREAMToIStorageEx;

HRESULT (__stdcall * Real_OleCreate)(CONST IID& a0,
                                     CONST IID& a1,
                                     DWORD a2,
                                     LPFORMATETC a3,
                                     LPOLECLIENTSITE a4,
                                     LPSTORAGE a5,
                                     LPVOID* a6)
    = OleCreate;

HRESULT (__stdcall * Real_OleCreateDefaultHandler)(CONST IID& a0,
                                                   LPUNKNOWN a1,
                                                   CONST IID& a2,
                                                   LPVOID* a3)
    = OleCreateDefaultHandler;

HRESULT (__stdcall * Real_OleCreateEmbeddingHelper)(CONST IID& a0,
                                                    LPUNKNOWN a1,
                                                    DWORD a2,
                                                    LPCLASSFACTORY a3,
                                                    CONST IID& a4,
                                                    LPVOID* a5)
    = OleCreateEmbeddingHelper;

HRESULT (__stdcall * Real_OleCreateEx)(CONST IID& a0,
                                       CONST IID& a1,
                                       DWORD a2,
                                       DWORD a3,
                                       ULONG a4,
                                       DWORD* a5,
                                       LPFORMATETC a6,
                                       IAdviseSink* a7,
                                       DWORD* a8,
                                       LPOLECLIENTSITE a9,
                                       LPSTORAGE a10,
                                       LPVOID* a11)
    = OleCreateEx;

HRESULT (__stdcall * Real_OleCreateFromData)(IDataObject* a0,
                                             CONST IID& a1,
                                             DWORD a2,
                                             LPFORMATETC a3,
                                             LPOLECLIENTSITE a4,
                                             LPSTORAGE a5,
                                             LPVOID* a6)
    = OleCreateFromData;

HRESULT (__stdcall * Real_OleCreateFromDataEx)(IDataObject* a0,
                                               CONST IID& a1,
                                               DWORD a2,
                                               DWORD a3,
                                               ULONG a4,
                                               DWORD* a5,
                                               LPFORMATETC a6,
                                               IAdviseSink* a7,
                                               DWORD* a8,
                                               LPOLECLIENTSITE a9,
                                               LPSTORAGE a10,
                                               LPVOID* a11)
    = OleCreateFromDataEx;

HRESULT (__stdcall * Real_OleCreateFromFile)(CONST IID& a0,
                                             LPCOLESTR a1,
                                             CONST IID& a2,
                                             DWORD a3,
                                             LPFORMATETC a4,
                                             LPOLECLIENTSITE a5,
                                             LPSTORAGE a6,
                                             LPVOID* a7)
    = OleCreateFromFile;

HRESULT (__stdcall * Real_OleCreateFromFileEx)(CONST IID& a0,
                                               LPCOLESTR a1,
                                               CONST IID& a2,
                                               DWORD a3,
                                               DWORD a4,
                                               ULONG a5,
                                               DWORD* a6,
                                               LPFORMATETC a7,
                                               IAdviseSink* a8,
                                               DWORD* a9,
                                               LPOLECLIENTSITE a10,
                                               LPSTORAGE a11,
                                               LPVOID* a12)
    = OleCreateFromFileEx;

HRESULT (__stdcall * Real_OleCreateLink)(IMoniker* a0,
                                         CONST IID& a1,
                                         DWORD a2,
                                         LPFORMATETC a3,
                                         LPOLECLIENTSITE a4,
                                         LPSTORAGE a5,
                                         LPVOID* a6)
    = OleCreateLink;

HRESULT (__stdcall * Real_OleCreateLinkEx)(IMoniker* a0,
                                           CONST IID& a1,
                                           DWORD a2,
                                           DWORD a3,
                                           ULONG a4,
                                           DWORD* a5,
                                           LPFORMATETC a6,
                                           IAdviseSink* a7,
                                           DWORD* a8,
                                           LPOLECLIENTSITE a9,
                                           LPSTORAGE a10,
                                           LPVOID* a11)
    = OleCreateLinkEx;

HRESULT (__stdcall * Real_OleCreateLinkFromData)(IDataObject* a0,
                                                 CONST IID& a1,
                                                 DWORD a2,
                                                 LPFORMATETC a3,
                                                 LPOLECLIENTSITE a4,
                                                 LPSTORAGE a5,
                                                 LPVOID* a6)
    = OleCreateLinkFromData;

HRESULT (__stdcall * Real_OleCreateLinkFromDataEx)(IDataObject* a0,
                                                   CONST IID& a1,
                                                   DWORD a2,
                                                   DWORD a3,
                                                   ULONG a4,
                                                   DWORD* a5,
                                                   LPFORMATETC a6,
                                                   IAdviseSink* a7,
                                                   DWORD* a8,
                                                   LPOLECLIENTSITE a9,
                                                   LPSTORAGE a10,
                                                   LPVOID* a11)
    = OleCreateLinkFromDataEx;

HRESULT (__stdcall * Real_OleCreateLinkToFile)(LPCOLESTR a0,
                                               CONST IID& a1,
                                               DWORD a2,
                                               LPFORMATETC a3,
                                               LPOLECLIENTSITE a4,
                                               LPSTORAGE a5,
                                               LPVOID* a6)
    = OleCreateLinkToFile;

HRESULT (__stdcall * Real_OleCreateLinkToFileEx)(LPCOLESTR a0,
                                                 CONST IID& a1,
                                                 DWORD a2,
                                                 DWORD a3,
                                                 ULONG a4,
                                                 DWORD* a5,
                                                 LPFORMATETC a6,
                                                 IAdviseSink* a7,
                                                 DWORD* a8,
                                                 LPOLECLIENTSITE a9,
                                                 LPSTORAGE a10,
                                                 LPVOID* a11)
    = OleCreateLinkToFileEx;

HOLEMENU (__stdcall * Real_OleCreateMenuDescriptor)(HMENU a0,
                                                    LPOLEMENUGROUPWIDTHS a1)
    = OleCreateMenuDescriptor;

HRESULT (__stdcall * Real_OleCreateStaticFromData)(IDataObject* a0,
                                                   CONST IID& a1,
                                                   DWORD a2,
                                                   LPFORMATETC a3,
                                                   LPOLECLIENTSITE a4,
                                                   LPSTORAGE a5,
                                                   LPVOID* a6)
    = OleCreateStaticFromData;

HRESULT (__stdcall * Real_OleDestroyMenuDescriptor)(HOLEMENU a0)
    = OleDestroyMenuDescriptor;

HRESULT (__stdcall * Real_OleDoAutoConvert)(LPSTORAGE a0,
                                            LPGUID a1)
    = OleDoAutoConvert;

HRESULT (__stdcall * Real_OleDraw)(LPUNKNOWN a0,
                                   DWORD a1,
                                   HDC a2,
                                   LPCRECT a3)
    = OleDraw;

HANDLE (__stdcall * Real_OleDuplicateData)(HANDLE a0,
                                           CLIPFORMAT a1,
                                           UINT a2)
    = OleDuplicateData;

HRESULT (__stdcall * Real_OleFlushClipboard)(void)
    = OleFlushClipboard;

HRESULT (__stdcall * Real_OleGetAutoConvert)(CONST IID& a0,
                                             LPGUID a1)
    = OleGetAutoConvert;

HRESULT (__stdcall * Real_OleGetClipboard)(IDataObject** a0)
    = OleGetClipboard;

HGLOBAL (__stdcall * Real_OleGetIconOfClass)(CONST IID& a0,
                                             LPOLESTR a1,
                                             BOOL a2)
    = OleGetIconOfClass;

HGLOBAL (__stdcall * Real_OleGetIconOfFile)(LPOLESTR a0,
                                            BOOL a1)
    = OleGetIconOfFile;

HRESULT (__stdcall * Real_OleInitialize)(LPVOID a0)
    = OleInitialize;

HRESULT (__stdcall * Real_OleIsCurrentClipboard)(IDataObject* a0)
    = OleIsCurrentClipboard;

BOOL (__stdcall * Real_OleIsRunning)(IOleObject* a0)
    = OleIsRunning;

HRESULT (__stdcall * Real_OleLoad)(LPSTORAGE a0,
                                   CONST IID& a1,
                                   LPOLECLIENTSITE a2,
                                   LPVOID* a3)
    = OleLoad;

HRESULT (__stdcall * Real_OleLoadFromStream)(LPSTREAM a0,
                                             CONST IID& a1,
                                             LPVOID* a2)
    = OleLoadFromStream;

HRESULT (__stdcall * Real_OleLockRunning)(LPUNKNOWN a0,
                                          BOOL a1,
                                          BOOL a2)
    = OleLockRunning;

HGLOBAL (__stdcall * Real_OleMetafilePictFromIconAndLabel)(HICON a0,
                                                           LPOLESTR a1,
                                                           LPOLESTR a2,
                                                           UINT a3)
    = OleMetafilePictFromIconAndLabel;

HRESULT (__stdcall * Real_OleNoteObjectVisible)(LPUNKNOWN a0,
                                                BOOL a1)
    = OleNoteObjectVisible;

HRESULT (__stdcall * Real_OleQueryCreateFromData)(IDataObject* a0)
    = OleQueryCreateFromData;

HRESULT (__stdcall * Real_OleQueryLinkFromData)(IDataObject* a0)
    = OleQueryLinkFromData;

HRESULT (__stdcall * Real_OleRegEnumFormatEtc)(CONST IID& a0,
                                               DWORD a1,
                                               IEnumFORMATETC** a2)
    = OleRegEnumFormatEtc;

HRESULT (__stdcall * Real_OleRegEnumVerbs)(CONST IID& a0,
                                           IEnumOLEVERB** a1)
    = OleRegEnumVerbs;

HRESULT (__stdcall * Real_OleRegGetMiscStatus)(CONST IID& a0,
                                               DWORD a1,
                                               DWORD* a2)
    = OleRegGetMiscStatus;

HRESULT (__stdcall * Real_OleRegGetUserType)(CONST IID& a0,
                                             DWORD a1,
                                             LPOLESTR* a2)
    = OleRegGetUserType;

HRESULT (__stdcall * Real_OleRun)(LPUNKNOWN a0)
    = OleRun;

HRESULT (__stdcall * Real_OleSave)(LPPERSISTSTORAGE a0,
                                   LPSTORAGE a1,
                                   BOOL a2)
    = OleSave;

HRESULT (__stdcall * Real_OleSaveToStream)(IPersistStream* a0,
                                           LPSTREAM a1)
    = OleSaveToStream;

HRESULT (__stdcall * Real_OleSetAutoConvert)(CONST IID& a0,
                                             CONST IID& a1)
    = OleSetAutoConvert;

HRESULT (__stdcall * Real_OleSetClipboard)(IDataObject* a0)
    = OleSetClipboard;

HRESULT (__stdcall * Real_OleSetContainedObject)(LPUNKNOWN a0,
                                                 BOOL a1)
    = OleSetContainedObject;

HRESULT (__stdcall * Real_OleSetMenuDescriptor)(HOLEMENU a0,
                                                HWND a1,
                                                HWND a2,
                                                LPOLEINPLACEFRAME a3,
                                                LPOLEINPLACEACTIVEOBJECT a4)
    = OleSetMenuDescriptor;

HRESULT (__stdcall * Real_OleTranslateAccelerator)(LPOLEINPLACEFRAME a0,
                                                   LPOLEINPLACEFRAMEINFO a1,
                                                   LPMSG a2)
    = OleTranslateAccelerator;

void (__stdcall * Real_OleUninitialize)(void)
    = OleUninitialize;

BOOL (__stdcall * Real_OpenClipboard)(HWND a0)
    = OpenClipboard;

#if _MSC_VER < 1300
HDESK (__stdcall * Real_OpenDesktopA)(LPSTR a0,
                                      DWORD a1,
                                      BOOL a2,
                                      ACCESS_MASK a3)
    = OpenDesktopA;
#else
HDESK (__stdcall * Real_OpenDesktopA)(LPCSTR a0,
                                      DWORD a1,
                                      BOOL a2,
                                      ACCESS_MASK a3)
    = OpenDesktopA;
#endif

#if _MSC_VER < 1300
HDESK (__stdcall * Real_OpenDesktopW)(LPWSTR a0,
                                      DWORD a1,
                                      BOOL a2,
                                      ACCESS_MASK a3)
    = OpenDesktopW;
#else
HDESK (__stdcall * Real_OpenDesktopW)(LPCWSTR a0,
                                      DWORD a1,
                                      BOOL a2,
                                      ACCESS_MASK a3)
    = OpenDesktopW;
#endif

HANDLE (__stdcall * Real_OpenEventA)(DWORD a0,
                                     BOOL a1,
                                     LPCSTR a2)
    = OpenEventA;

HANDLE (__stdcall * Real_OpenEventW)(DWORD a0,
                                     BOOL a1,
                                     LPCWSTR a2)
    = OpenEventW;

HFILE (__stdcall * Real_OpenFile)(LPCSTR a0,
                                  LPOFSTRUCT a1,
                                  UINT a2)
    = OpenFile;

HANDLE (__stdcall * Real_OpenFileMappingA)(DWORD a0,
                                           BOOL a1,
                                           LPCSTR a2)
    = OpenFileMappingA;

HANDLE (__stdcall * Real_OpenFileMappingW)(DWORD a0,
                                           BOOL a1,
                                           LPCWSTR a2)
    = OpenFileMappingW;

BOOL (__stdcall * Real_OpenIcon)(HWND a0)
    = OpenIcon;

HDESK (__stdcall * Real_OpenInputDesktop)(DWORD a0,
                                          BOOL a1,
                                          ACCESS_MASK a2)
    = OpenInputDesktop;

HANDLE (__stdcall * Real_OpenMutexA)(DWORD a0,
                                     BOOL a1,
                                     LPCSTR a2)
    = OpenMutexA;

HANDLE (__stdcall * Real_OpenMutexW)(DWORD a0,
                                     BOOL a1,
                                     LPCWSTR a2)
    = OpenMutexW;

HANDLE (__stdcall * Real_OpenProcess)(DWORD a0,
                                      BOOL a1,
                                      DWORD pid)
    = OpenProcess;

BOOL (__stdcall * Real_OpenProcessToken)(__in   HANDLE ProcessHandle,
                                         __in   DWORD DesiredAccess,
                                         __out  PHANDLE TokenHandle
                                        )
    = OpenProcessToken;

HANDLE (__stdcall * Real_OpenSemaphoreA)(DWORD a0,
                                         BOOL a1,
                                         LPCSTR a2)
    = OpenSemaphoreA;

HANDLE (__stdcall * Real_OpenSemaphoreW)(DWORD a0,
                                         BOOL a1,
                                         LPCWSTR a2)
    = OpenSemaphoreW;

HANDLE (__stdcall * Real_OpenWaitableTimerA)(DWORD a0,
                                             BOOL a1,
                                             LPCSTR a2)
    = OpenWaitableTimerA;

HANDLE (__stdcall * Real_OpenWaitableTimerW)(DWORD a0,
                                             BOOL a1,
                                             LPCWSTR a2)
    = OpenWaitableTimerW;

#if _MSC_VER < 1300
HWINSTA (__stdcall * Real_OpenWindowStationA)(LPSTR a0,
                                              BOOL a1,
                                              ACCESS_MASK a2)
    = OpenWindowStationA;
#else
HWINSTA (__stdcall * Real_OpenWindowStationA)(LPCSTR a0,
                                              BOOL a1,
                                              ACCESS_MASK a2)
    = OpenWindowStationA;
#endif

#if _MSC_VER < 1300
HWINSTA (__stdcall * Real_OpenWindowStationW)(LPWSTR a0,
                                              BOOL a1,
                                              ACCESS_MASK a2)
    = OpenWindowStationW;
#else
HWINSTA (__stdcall * Real_OpenWindowStationW)(LPCWSTR a0,
                                              BOOL a1,
                                              ACCESS_MASK a2)
    = OpenWindowStationW;
#endif

void (__stdcall * Real_OutputDebugStringA)(LPCSTR a0)
    = OutputDebugStringA;

void (__stdcall * Real_OutputDebugStringW)(LPCWSTR a0)
    = OutputDebugStringW;

LPARAM (__stdcall * Real_PackDDElParam)(UINT a0,
                                        UINT_PTR a1,
                                        UINT_PTR a2)
    = PackDDElParam;

BOOL (__stdcall * Real_PaintDesktop)(HDC a0)
    = PaintDesktop;

BOOL (__stdcall * Real_PaintRgn)(HDC a0,
                                 HRGN a1)
    = PaintRgn;

BOOL (__stdcall * Real_PatBlt)(HDC a0,
                               int a1,
                               int a2,
                               int a3,
                               int a4,
                               DWORD a5)
    = PatBlt;

HRGN (__stdcall * Real_PathToRegion)(HDC a0)
    = PathToRegion;

BOOL (__stdcall * Real_PeekConsoleInputA)(HANDLE a0,
                                          PINPUT_RECORD a1,
                                          DWORD a2,
                                          LPDWORD a3)
    = PeekConsoleInputA;

BOOL (__stdcall * Real_PeekConsoleInputW)(HANDLE a0,
                                          PINPUT_RECORD a1,
                                          DWORD a2,
                                          LPDWORD a3)
    = PeekConsoleInputW;

BOOL (__stdcall * Real_PeekMessageA)(LPMSG a0,
                                     HWND a1,
                                     UINT a2,
                                     UINT a3,
                                     UINT a4)
    = PeekMessageA;

BOOL (__stdcall * Real_PeekMessageW)(LPMSG a0,
                                     HWND a1,
                                     UINT a2,
                                     UINT a3,
                                     UINT a4)
    = PeekMessageW;

BOOL (__stdcall * Real_PeekNamedPipe)(HANDLE a0,
                                      LPVOID a1,
                                      DWORD a2,
                                      LPDWORD a3,
                                      LPDWORD a4,
                                      LPDWORD a5)
    = PeekNamedPipe;

BOOL (__stdcall * Real_Pie)(HDC a0,
                            int a1,
                            int a2,
                            int a3,
                            int a4,
                            int a5,
                            int a6,
                            int a7,
                            int a8)
    = Pie;

BOOL (__stdcall * Real_PlayEnhMetaFile)(HDC a0,
                                        HENHMETAFILE a1,
                                        CONST RECT* a2)
    = PlayEnhMetaFile;

BOOL (__stdcall * Real_PlayEnhMetaFileRecord)(HDC a0,
                                              LPHANDLETABLE a1,
                                              CONST ENHMETARECORD* a2,
                                              UINT a3)
    = PlayEnhMetaFileRecord;

BOOL (__stdcall * Real_PlayMetaFile)(HDC a0,
                                     HMETAFILE a1)
    = PlayMetaFile;

BOOL (__stdcall * Real_PlayMetaFileRecord)(HDC a0,
                                           LPHANDLETABLE a1,
                                           LPMETARECORD a2,
                                           UINT a3)
    = PlayMetaFileRecord;

BOOL (__stdcall * Real_PlgBlt)(HDC a0,
                               CONST POINT* a1,
                               HDC a2,
                               int a3,
                               int a4,
                               int a5,
                               int a6,
                               HBITMAP a7,
                               int a8,
                               int a9)
    = PlgBlt;

BOOL (__stdcall * Real_PolyBezier)(HDC a0,
                                   CONST POINT* a1,
                                   DWORD a2)
    = PolyBezier;

BOOL (__stdcall * Real_PolyBezierTo)(HDC a0,
                                     CONST POINT* a1,
                                     DWORD a2)
    = PolyBezierTo;

BOOL (__stdcall * Real_PolyDraw)(HDC a0,
                                 CONST POINT* a1,
                                 CONST BYTE* a2,
                                 int a3)
    = PolyDraw;

BOOL (__stdcall * Real_PolyPolygon)(HDC a0,
                                    CONST POINT* a1,
                                    CONST INT* a2,
                                    int a3)
    = PolyPolygon;

BOOL (__stdcall * Real_PolyPolyline)(HDC a0,
                                     CONST POINT* a1,
                                     CONST DWORD* a2,
                                     DWORD a3)
    = PolyPolyline;

BOOL (__stdcall * Real_PolyTextOutA)(HDC a0,
                                     CONST POLYTEXTA* a1,
                                     int a2)
    = PolyTextOutA;

BOOL (__stdcall * Real_PolyTextOutW)(HDC a0,
                                     CONST POLYTEXTW* a1,
                                     int a2)
    = PolyTextOutW;

BOOL (__stdcall * Real_Polygon)(HDC a0,
                                CONST POINT* a1,
                                int a2)
    = Polygon;

BOOL (__stdcall * Real_Polyline)(HDC a0,
                                 CONST POINT* a1,
                                 int a2)
    = Polyline;

BOOL (__stdcall * Real_PolylineTo)(HDC a0,
                                   CONST POINT* a1,
                                   DWORD a2)
    = PolylineTo;

BOOL (__stdcall * Real_PostMessageA)(HWND a0,
                                     UINT a1,
                                     WPARAM a2,
                                     LPARAM a3)
    = PostMessageA;

BOOL (__stdcall * Real_PostMessageW)(HWND a0,
                                     UINT a1,
                                     WPARAM a2,
                                     LPARAM a3)
    = PostMessageW;

BOOL (__stdcall * Real_PostQueuedCompletionStatus)(HANDLE a0,
                                                   DWORD a1,
                                                   ULONG_PTR a2,
                                                   LPOVERLAPPED a3)
    = PostQueuedCompletionStatus;

void (__stdcall * Real_PostQuitMessage)(int a0)
    = PostQuitMessage;

BOOL (__stdcall * Real_PostThreadMessageA)(DWORD a0,
                                           UINT a1,
                                           WPARAM a2,
                                           LPARAM a3)
    = PostThreadMessageA;

BOOL (__stdcall * Real_PostThreadMessageW)(DWORD a0,
                                           UINT a1,
                                           WPARAM a2,
                                           LPARAM a3)
    = PostThreadMessageW;

DWORD (__stdcall * Real_PrepareTape)(HANDLE a0,
                                     DWORD a1,
                                     BOOL a2)
    = PrepareTape;

HRESULT (__stdcall * Real_ProgIDFromCLSID)(CONST IID& a0,
                                           LPOLESTR* a1)
    = ProgIDFromCLSID;

HRESULT (__stdcall * Real_PropVariantClear)(PROPVARIANT* a0)
    = PropVariantClear;

HRESULT (__stdcall * Real_PropVariantCopy)(PROPVARIANT* a0,
                                           CONST PROPVARIANT* a1)
    = PropVariantCopy;

BOOL (__stdcall * Real_PtInRect)(CONST RECT* a0,
                                 POINT a1)
    = PtInRect;

BOOL (__stdcall * Real_PtInRegion)(HRGN a0,
                                   int a1,
                                   int a2)
    = PtInRegion;

BOOL (__stdcall * Real_PtVisible)(HDC a0,
                                  int a1,
                                  int a2)
    = PtVisible;

BOOL (__stdcall * Real_PulseEvent)(HANDLE a0)
    = PulseEvent;

BOOL (__stdcall * Real_PurgeComm)(HANDLE a0,
                                  DWORD a1)
    = PurgeComm;

DWORD (__stdcall * Real_QueryDosDeviceA)(LPCSTR a0,
                                         LPSTR a1,
                                         DWORD a2)
    = QueryDosDeviceA;

DWORD (__stdcall * Real_QueryDosDeviceW)(LPCWSTR a0,
                                         LPWSTR a1,
                                         DWORD a2)
    = QueryDosDeviceW;

BOOL (__stdcall * Real_QueryPerformanceCounter)(LARGE_INTEGER* a0)
    = QueryPerformanceCounter;

BOOL (__stdcall * Real_QueryPerformanceFrequency)(LARGE_INTEGER* a0)
    = QueryPerformanceFrequency;

DWORD (__stdcall * Real_QueueUserAPC)(PAPCFUNC a0,
                                      HANDLE a1,
                                      ULONG_PTR a2)
    = QueueUserAPC;

void (__stdcall * Real_RaiseException)(DWORD a0,
                                       DWORD a1,
                                       DWORD a2,
                                       CONST ULONG_PTR* a3)
    = RaiseException;

HRESULT (__stdcall * Real_ReadClassStg)(LPSTORAGE a0,
                                        CLSID* a1)
    = ReadClassStg;

HRESULT (__stdcall * Real_ReadClassStm)(LPSTREAM a0,
                                        CLSID* a1)
    = ReadClassStm;

BOOL (__stdcall * Real_ReadConsoleA)(
                                     HANDLE a0,
                                     LPVOID a1,
                                     DWORD a2,
                                     LPDWORD a3,
#ifdef ENABLE_INSERT_MODE
                                     PCONSOLE_READCONSOLE_CONTROL a4
#else
                                     LPVOID a4
#endif
                                    )
    = ReadConsoleA;

BOOL (__stdcall * Real_ReadConsoleInputA)(HANDLE a0,
                                          PINPUT_RECORD a1,
                                          DWORD a2,
                                          LPDWORD a3)
    = ReadConsoleInputA;

BOOL (__stdcall * Real_ReadConsoleInputW)(HANDLE a0,
                                          PINPUT_RECORD a1,
                                          DWORD a2,
                                          LPDWORD a3)
    = ReadConsoleInputW;

BOOL (__stdcall * Real_ReadConsoleOutputA)(HANDLE a0,
                                           PCHAR_INFO a1,
                                           COORD a2,
                                           COORD a3,
                                           PSMALL_RECT a4)
    = ReadConsoleOutputA;

BOOL (__stdcall * Real_ReadConsoleOutputAttribute)(HANDLE a0,
                                                   LPWORD a1,
                                                   DWORD a2,
                                                   COORD a3,
                                                   LPDWORD a4)
    = ReadConsoleOutputAttribute;

BOOL (__stdcall * Real_ReadConsoleOutputCharacterA)(HANDLE a0,
                                                    LPSTR a1,
                                                    DWORD a2,
                                                    COORD a3,
                                                    LPDWORD a4)
    = ReadConsoleOutputCharacterA;

BOOL (__stdcall * Real_ReadConsoleOutputCharacterW)(HANDLE a0,
                                                    LPWSTR a1,
                                                    DWORD a2,
                                                    COORD a3,
                                                    LPDWORD a4)
    = ReadConsoleOutputCharacterW;

BOOL (__stdcall * Real_ReadConsoleOutputW)(HANDLE a0,
                                           PCHAR_INFO a1,
                                           COORD a2,
                                           COORD a3,
                                           PSMALL_RECT a4)
    = ReadConsoleOutputW;

BOOL (__stdcall * Real_ReadConsoleW)(
                                     HANDLE a0,
                                     LPVOID a1,
                                     DWORD a2,
                                     LPDWORD a3,
#ifdef ENABLE_INSERT_MODE
                                     PCONSOLE_READCONSOLE_CONTROL a4
#else
                                     LPVOID a4
#endif
                                    )
    = ReadConsoleW;

BOOL (__stdcall * Real_ReadDirectoryChangesW)(HANDLE a0,
                                              LPVOID a1,
                                              DWORD a2,
                                              BOOL a3,
                                              DWORD a4,
                                              LPDWORD a5,
                                              LPOVERLAPPED a6,
                                              LPOVERLAPPED_COMPLETION_ROUTINE a7)
    = ReadDirectoryChangesW;

BOOL (__stdcall * Real_ReadFile)(HANDLE a0,
                                 LPVOID a1,
                                 DWORD a2,
                                 LPDWORD a3,
                                 LPOVERLAPPED a4)
    = ReadFile;

BOOL (__stdcall * Real_ReadFileEx)(HANDLE a0,
                                   LPVOID a1,
                                   DWORD a2,
                                   LPOVERLAPPED a3,
                                   LPOVERLAPPED_COMPLETION_ROUTINE a4)
    = ReadFileEx;

HRESULT (__stdcall * Real_ReadFmtUserTypeStg)(LPSTORAGE a0,
                                              CLIPFORMAT* a1,
                                              LPOLESTR* a2)
    = ReadFmtUserTypeStg;

BOOL (__stdcall * Real_ReadProcessMemory)(HANDLE a0,
                                          LPCVOID a1,
                                          LPVOID a2,
                                          DWORD_PTR a3,
                                          PDWORD_PTR a4)
    = ReadProcessMemory;

UINT (__stdcall * Real_RealizePalette)(HDC a0)
    = RealizePalette;

BOOL (__stdcall * Real_RectInRegion)(HRGN a0,
                                     CONST RECT* a1)
    = RectInRegion;

BOOL (__stdcall * Real_RectVisible)(HDC a0,
                                    CONST RECT* a1)
    = RectVisible;

BOOL (__stdcall * Real_Rectangle)(HDC a0,
                                  int a1,
                                  int a2,
                                  int a3,
                                  int a4)
    = Rectangle;

BOOL (__stdcall * Real_RedrawWindow)(HWND a0,
                                     CONST RECT* a1,
                                     HRGN a2,
                                     UINT a3)
    = RedrawWindow;

ATOM (__stdcall * Real_RegisterClassA)(CONST WNDCLASSA* a0)
    = RegisterClassA;

ATOM (__stdcall * Real_RegisterClassExA)(CONST WNDCLASSEXA* a0)
    = RegisterClassExA;

ATOM (__stdcall * Real_RegisterClassExW)(CONST WNDCLASSEXW* a0)
    = RegisterClassExW;

ATOM (__stdcall * Real_RegisterClassW)(CONST WNDCLASSW* a0)
    = RegisterClassW;

UINT (__stdcall * Real_RegisterClipboardFormatA)(LPCSTR a0)
    = RegisterClipboardFormatA;

UINT (__stdcall * Real_RegisterClipboardFormatW)(LPCWSTR a0)
    = RegisterClipboardFormatW;

HRESULT (__stdcall * Real_RegisterDragDrop)(HWND a0,
                                            IDropTarget* a1)
    = RegisterDragDrop;

BOOL (__stdcall * Real_RegisterHotKey)(HWND a0,
                                       int a1,
                                       UINT a2,
                                       UINT a3)
    = RegisterHotKey;

UINT (__stdcall * Real_RegisterWindowMessageA)(LPCSTR a0)
    = RegisterWindowMessageA;

UINT (__stdcall * Real_RegisterWindowMessageW)(LPCWSTR a0)
    = RegisterWindowMessageW;

BOOL (__stdcall * Real_ReleaseCapture)(void)
    = ReleaseCapture;

int (__stdcall * Real_ReleaseDC)(HWND a0,
                                 HDC a1)
    = ReleaseDC;

BOOL (__stdcall * Real_ReleaseMutex)(HANDLE a0)
    = ReleaseMutex;

BOOL (__stdcall * Real_ReleaseSemaphore)(HANDLE a0,
                                         LONG a1,
                                         LPLONG a2)
    = ReleaseSemaphore;

void (__stdcall * Real_ReleaseStgMedium)(LPSTGMEDIUM a0)
    = ReleaseStgMedium;

BOOL (__stdcall * Real_RemoveDirectoryA)(LPCSTR a0)
    = RemoveDirectoryA;

BOOL (__stdcall * Real_RemoveDirectoryW)(LPCWSTR a0)
    = RemoveDirectoryW;

BOOL (__stdcall * Real_RemoveFontResourceA)(LPCSTR a0)
    = RemoveFontResourceA;

BOOL (__stdcall * Real_RemoveFontResourceW)(LPCWSTR a0)
    = RemoveFontResourceW;

BOOL (__stdcall * Real_RemoveMenu)(HMENU a0,
                                   UINT a1,
                                   UINT a2)
    = RemoveMenu;

HANDLE (__stdcall * Real_RemovePropA)(HWND a0,
                                      LPCSTR a1)
    = RemovePropA;

HANDLE (__stdcall * Real_RemovePropW)(HWND a0,
                                      LPCWSTR a1)
    = RemovePropW;

BOOL (__stdcall * Real_ReplyMessage)(LRESULT a0)
    = ReplyMessage;

HDC (__stdcall * Real_ResetDCA)(HDC a0,
                                CONST DEVMODEA* a1)
    = ResetDCA;

HDC (__stdcall * Real_ResetDCW)(HDC a0,
                                CONST DEVMODEW* a1)
    = ResetDCW;

BOOL (__stdcall * Real_ResetEvent)(HANDLE a0)
    = ResetEvent;

BOOL (__stdcall * Real_ResizePalette)(HPALETTE a0,
                                      UINT a1)
    = ResizePalette;

BOOL (__stdcall * Real_RestoreDC)(HDC a0,
                                  int a1)
    = RestoreDC;

DWORD (__stdcall * Real_ResumeThread)(HANDLE a0)
    = ResumeThread;

LPARAM (__stdcall * Real_ReuseDDElParam)(LPARAM a0,
                                         UINT a1,
                                         UINT a2,
                                         UINT_PTR a3,
                                         UINT_PTR a4)
    = ReuseDDElParam;

HRESULT (__stdcall * Real_RevokeDragDrop)(HWND a0)
    = RevokeDragDrop;

BOOL (__stdcall * Real_RoundRect)(HDC a0,
                                  int a1,
                                  int a2,
                                  int a3,
                                  int a4,
                                  int a5,
                                  int a6)
    = RoundRect;

int (__stdcall * Real_SaveDC)(HDC a0)
    = SaveDC;

BOOL (__stdcall * Real_ScaleViewportExtEx)(HDC a0,
                                           int a1,
                                           int a2,
                                           int a3,
                                           int a4,
                                           SIZE* a5)
    = ScaleViewportExtEx;

BOOL (__stdcall * Real_ScaleWindowExtEx)(HDC a0,
                                         int a1,
                                         int a2,
                                         int a3,
                                         int a4,
                                         SIZE* a5)
    = ScaleWindowExtEx;

BOOL (__stdcall * Real_ScreenToClient)(HWND a0,
                                       POINT* a1)
    = ScreenToClient;

BOOL (__stdcall * Real_ScrollConsoleScreenBufferA)(HANDLE a0,
                                                   CONST SMALL_RECT* a1,
                                                   CONST SMALL_RECT* a2,
                                                   COORD a3,
                                                   CONST CHAR_INFO* a4)
    = ScrollConsoleScreenBufferA;

BOOL (__stdcall * Real_ScrollConsoleScreenBufferW)(HANDLE a0,
                                                   CONST SMALL_RECT* a1,
                                                   CONST SMALL_RECT* a2,
                                                   COORD a3,
                                                   CONST CHAR_INFO* a4)
    = ScrollConsoleScreenBufferW;

BOOL (__stdcall * Real_ScrollDC)(HDC a0,
                                 int a1,
                                 int a2,
                                 CONST RECT* a3,
                                 CONST RECT* a4,
                                 HRGN a5,
                                 LPRECT a6)
    = ScrollDC;

BOOL (__stdcall * Real_ScrollWindow)(HWND a0,
                                     int a1,
                                     int a2,
                                     CONST RECT* a3,
                                     CONST RECT* a4)
    = ScrollWindow;

int (__stdcall * Real_ScrollWindowEx)(HWND a0,
                                      int a1,
                                      int a2,
                                      CONST RECT* a3,
                                      CONST RECT* a4,
                                      HRGN a5,
                                      LPRECT a6,
                                      UINT a7)
    = ScrollWindowEx;

DWORD (__stdcall * Real_SearchPathA)(LPCSTR a0,
                                     LPCSTR a1,
                                     LPCSTR a2,
                                     DWORD a3,
                                     LPSTR a4,
                                     LPSTR* a5)
    = SearchPathA;

DWORD (__stdcall * Real_SearchPathW)(LPCWSTR a0,
                                     LPCWSTR a1,
                                     LPCWSTR a2,
                                     DWORD a3,
                                     LPWSTR a4,
                                     LPWSTR* a5)
    = SearchPathW;

BOOL (__stdcall * Real_SelectClipPath)(HDC a0,
                                       int a1)
    = SelectClipPath;

int (__stdcall * Real_SelectClipRgn)(HDC a0,
                                     HRGN a1)
    = SelectClipRgn;

HGDIOBJ (__stdcall * Real_SelectObject)(HDC a0,
                                        HGDIOBJ a1)
    = SelectObject;

HPALETTE (__stdcall * Real_SelectPalette)(HDC a0,
                                          HPALETTE a1,
                                          BOOL a2)
    = SelectPalette;

LRESULT (__stdcall * Real_SendDlgItemMessageA)(HWND a0,
                                               int a1,
                                               UINT a2,
                                               WPARAM a3,
                                               LPARAM a4)
    = SendDlgItemMessageA;

LRESULT (__stdcall * Real_SendDlgItemMessageW)(HWND a0,
                                               int a1,
                                               UINT a2,
                                               WPARAM a3,
                                               LPARAM a4)
    = SendDlgItemMessageW;

LRESULT (__stdcall * Real_SendMessageA)(HWND a0,
                                        UINT a1,
                                        WPARAM a2,
                                        LPARAM a3)
    = SendMessageA;

BOOL (__stdcall * Real_SendMessageCallbackA)(HWND a0,
                                             UINT a1,
                                             WPARAM a2,
                                             LPARAM a3,
                                             SENDASYNCPROC a4,
                                             ULONG_PTR a5)
    = SendMessageCallbackA;

BOOL (__stdcall * Real_SendMessageCallbackW)(HWND a0,
                                             UINT a1,
                                             WPARAM a2,
                                             LPARAM a3,
                                             SENDASYNCPROC a4,
                                             ULONG_PTR a5)
    = SendMessageCallbackW;

LRESULT (__stdcall * Real_SendMessageTimeoutA)(HWND a0,
                                               UINT a1,
                                               WPARAM a2,
                                               LPARAM a3,
                                               UINT a4,
                                               UINT a5,
                                               PULONG_PTR a6)
    = SendMessageTimeoutA;

LRESULT (__stdcall * Real_SendMessageTimeoutW)(HWND a0,
                                               UINT a1,
                                               WPARAM a2,
                                               LPARAM a3,
                                               UINT a4,
                                               UINT a5,
                                               PULONG_PTR a6)
    = SendMessageTimeoutW;

LRESULT (__stdcall * Real_SendMessageW)(HWND a0,
                                        UINT a1,
                                        WPARAM a2,
                                        LPARAM a3)
    = SendMessageW;

BOOL (__stdcall * Real_SendNotifyMessageA)(HWND a0,
                                           UINT a1,
                                           WPARAM a2,
                                           LPARAM a3)
    = SendNotifyMessageA;

BOOL (__stdcall * Real_SendNotifyMessageW)(HWND a0,
                                           UINT a1,
                                           WPARAM a2,
                                           LPARAM a3)
    = SendNotifyMessageW;

int (__stdcall * Real_SetAbortProc)(HDC a0,
                                    ABORTPROC a1)
    = SetAbortProc;

HWND (__stdcall * Real_SetActiveWindow)(HWND a0)
    = SetActiveWindow;

int (__stdcall * Real_SetArcDirection)(HDC a0,
                                       int a1)
    = SetArcDirection;

LONG (__stdcall * Real_SetBitmapBits)(HBITMAP a0,
                                      DWORD a1,
                                      CONST void* a2)
    = SetBitmapBits;

BOOL (__stdcall * Real_SetBitmapDimensionEx)(HBITMAP a0,
                                             int a1,
                                             int a2,
                                             SIZE* a3)
    = SetBitmapDimensionEx;

COLORREF (__stdcall * Real_SetBkColor)(HDC a0,
                                       COLORREF a1)
    = SetBkColor;

int (__stdcall * Real_SetBkMode)(HDC a0,
                                 int a1)
    = SetBkMode;

UINT (__stdcall * Real_SetBoundsRect)(HDC a0,
                                      CONST RECT* a1,
                                      UINT a2)
    = SetBoundsRect;

BOOL (__stdcall * Real_SetBrushOrgEx)(HDC a0,
                                      int a1,
                                      int a2,
                                      POINT* a3)
    = SetBrushOrgEx;

HWND (__stdcall * Real_SetCapture)(HWND a0)
    = SetCapture;

BOOL (__stdcall * Real_SetCaretBlinkTime)(UINT a0)
    = SetCaretBlinkTime;

BOOL (__stdcall * Real_SetCaretPos)(int a0,
                                    int a1)
    = SetCaretPos;

DWORD (__stdcall * Real_SetClassLongA)(HWND a0,
                                       int a1,
                                       LONG a2)
    = SetClassLongA;

DWORD (__stdcall * Real_SetClassLongW)(HWND a0,
                                       int a1,
                                       LONG a2)
    = SetClassLongW;

WORD (__stdcall * Real_SetClassWord)(HWND a0,
                                     int a1,
                                     WORD a2)
    = SetClassWord;

HANDLE (__stdcall * Real_SetClipboardData)(UINT a0,
                                           HANDLE a1)
    = SetClipboardData;

HWND (__stdcall * Real_SetClipboardViewer)(HWND a0)
    = SetClipboardViewer;

BOOL (__stdcall * Real_SetColorAdjustment)(HDC a0,
                                           CONST COLORADJUSTMENT* a1)
    = SetColorAdjustment;

HCOLORSPACE (__stdcall * Real_SetColorSpace)(HDC a0,
                                             HCOLORSPACE a1)
    = SetColorSpace;

BOOL (__stdcall * Real_SetCommBreak)(HANDLE a0)
    = SetCommBreak;

BOOL (__stdcall * Real_SetCommConfig)(HANDLE a0,
                                      LPCOMMCONFIG a1,
                                      DWORD a2)
    = SetCommConfig;

BOOL (__stdcall * Real_SetCommMask)(HANDLE a0,
                                    DWORD a1)
    = SetCommMask;

BOOL (__stdcall * Real_SetCommState)(HANDLE a0,
                                     LPDCB a1)
    = SetCommState;

BOOL (__stdcall * Real_SetCommTimeouts)(HANDLE a0,
                                        LPCOMMTIMEOUTS a1)
    = SetCommTimeouts;

BOOL (__stdcall * Real_SetComputerNameA)(LPCSTR a0)
    = SetComputerNameA;

BOOL (__stdcall * Real_SetComputerNameW)(LPCWSTR a0)
    = SetComputerNameW;

BOOL (__stdcall * Real_SetConsoleActiveScreenBuffer)(HANDLE a0)
    = SetConsoleActiveScreenBuffer;

BOOL (__stdcall * Real_SetConsoleCP)(UINT a0)
    = SetConsoleCP;

BOOL (__stdcall * Real_SetConsoleCtrlHandler)(PHANDLER_ROUTINE a0,
                                              BOOL a1)
    = SetConsoleCtrlHandler;

BOOL (__stdcall * Real_SetConsoleCursorInfo)(HANDLE a0,
                                             CONST CONSOLE_CURSOR_INFO* a1)
    = SetConsoleCursorInfo;

BOOL (__stdcall * Real_SetConsoleCursorPosition)(HANDLE a0,
                                                 COORD a1)
    = SetConsoleCursorPosition;

BOOL (__stdcall * Real_SetConsoleMode)(HANDLE a0,
                                       DWORD a1)
    = SetConsoleMode;

BOOL (__stdcall * Real_SetConsoleOutputCP)(UINT a0)
    = SetConsoleOutputCP;

BOOL (__stdcall * Real_SetConsoleScreenBufferSize)(HANDLE a0,
                                                   COORD a1)
    = SetConsoleScreenBufferSize;

BOOL (__stdcall * Real_SetConsoleTextAttribute)(HANDLE a0,
                                                WORD a1)
    = SetConsoleTextAttribute;

BOOL (__stdcall * Real_SetConsoleTitleA)(LPCSTR a0)
    = SetConsoleTitleA;

BOOL (__stdcall * Real_SetConsoleTitleW)(LPCWSTR a0)
    = SetConsoleTitleW;

BOOL (__stdcall * Real_SetConsoleWindowInfo)(HANDLE a0,
                                             BOOL a1,
                                             CONST SMALL_RECT* a2)
    = SetConsoleWindowInfo;

HRESULT (__stdcall * Real_SetConvertStg)(LPSTORAGE a0,
                                         BOOL a1)
    = SetConvertStg;

BOOL (__stdcall * Real_SetCurrentDirectoryA)(LPCSTR a0)
    = SetCurrentDirectoryA;

BOOL (__stdcall * Real_SetCurrentDirectoryW)(LPCWSTR a0)
    = SetCurrentDirectoryW;

HCURSOR (__stdcall * Real_SetCursor)(HCURSOR a0)
    = SetCursor;

BOOL (__stdcall * Real_SetCursorPos)(int a0,
                                     int a1)
    = SetCursorPos;

UINT (__stdcall * Real_SetDIBColorTable)(HDC a0,
                                         UINT a1,
                                         UINT a2,
                                         CONST RGBQUAD* a3)
    = SetDIBColorTable;

int (__stdcall * Real_SetDIBits)(HDC a0,
                                 HBITMAP a1,
                                 UINT a2,
                                 UINT a3,
                                 CONST void* a4,
                                 CONST BITMAPINFO* a5,
                                 UINT a6)
    = SetDIBits;

int (__stdcall * Real_SetDIBitsToDevice)(HDC a0,
                                         int a1,
                                         int a2,
                                         DWORD a3,
                                         DWORD a4,
                                         int a5,
                                         int a6,
                                         UINT a7,
                                         UINT a8,
                                         CONST void* a9,
                                         CONST BITMAPINFO* a10,
                                         UINT a11)
    = SetDIBitsToDevice;

BOOL (__stdcall * Real_SetDefaultCommConfigA)(LPCSTR a0,
                                              LPCOMMCONFIG a1,
                                              DWORD a2)
    = SetDefaultCommConfigA;

BOOL (__stdcall * Real_SetDefaultCommConfigW)(LPCWSTR a0,
                                              LPCOMMCONFIG a1,
                                              DWORD a2)
    = SetDefaultCommConfigW;

BOOL (__stdcall * Real_SetDeviceGammaRamp)(HDC a0,
                                           LPVOID a1)
    = SetDeviceGammaRamp;

BOOL (__stdcall * Real_SetDlgItemInt)(HWND a0,
                                      int a1,
                                      UINT a2,
                                      BOOL a3)
    = SetDlgItemInt;

BOOL (__stdcall * Real_SetDlgItemTextA)(HWND a0,
                                        int a1,
                                        LPCSTR a2)
    = SetDlgItemTextA;

BOOL (__stdcall * Real_SetDlgItemTextW)(HWND a0,
                                        int a1,
                                        LPCWSTR a2)
    = SetDlgItemTextW;

BOOL (__stdcall * Real_SetDoubleClickTime)(UINT a0)
    = SetDoubleClickTime;

BOOL (__stdcall * Real_SetEndOfFile)(HANDLE a0)
    = SetEndOfFile;

HENHMETAFILE (__stdcall * Real_SetEnhMetaFileBits)(UINT a0,
                                                   CONST BYTE* a1)
    = SetEnhMetaFileBits;

BOOL (__stdcall * Real_SetEnvironmentVariableA)(LPCSTR a0,
                                                LPCSTR a1)
    = SetEnvironmentVariableA;

BOOL (__stdcall * Real_SetEnvironmentVariableW)(LPCWSTR a0,
                                                LPCWSTR a1)
    = SetEnvironmentVariableW;

UINT (__stdcall * Real_SetErrorMode)(UINT a0)
    = SetErrorMode;

BOOL (__stdcall * Real_SetEvent)(HANDLE a0)
    = SetEvent;

void (__stdcall * Real_SetFileApisToANSI)(void)
    = SetFileApisToANSI;

void (__stdcall * Real_SetFileApisToOEM)(void)
    = SetFileApisToOEM;

BOOL (__stdcall * Real_SetFileAttributesA)(LPCSTR a0,
                                           DWORD a1)
    = SetFileAttributesA;

BOOL (__stdcall * Real_SetFileAttributesW)(LPCWSTR a0,
                                           DWORD a1)
    = SetFileAttributesW;

DWORD (__stdcall * Real_SetFilePointer)(HANDLE a0,
                                        LONG a1,
                                        PLONG a2,
                                        DWORD a3)
    = SetFilePointer;

BOOL (__stdcall * Real_SetFileTime)(HANDLE a0,
                                    CONST FILETIME* a1,
                                    CONST FILETIME* a2,
                                    CONST FILETIME* a3)
    = SetFileTime;

HWND (__stdcall * Real_SetFocus)(HWND a0)
    = SetFocus;

BOOL (__stdcall * Real_SetForegroundWindow)(HWND a0)
    = SetForegroundWindow;

int (__stdcall * Real_SetGraphicsMode)(HDC a0,
                                       int a1)
    = SetGraphicsMode;

UINT (__stdcall * Real_SetHandleCount)(UINT a0)
    = SetHandleCount;

BOOL (__stdcall * Real_SetHandleInformation)(HANDLE a0,
                                             DWORD a1,
                                             DWORD a2)
    = SetHandleInformation;

int (__stdcall * Real_SetICMMode)(HDC a0,
                                  int a1)
    = SetICMMode;

BOOL (__stdcall * Real_SetICMProfileA)(HDC a0,
                                       LPSTR a1)
    = SetICMProfileA;

BOOL (__stdcall * Real_SetICMProfileW)(HDC a0,
                                       LPWSTR a1)
    = SetICMProfileW;

BOOL (__stdcall * Real_SetKeyboardState)(LPBYTE a0)
    = SetKeyboardState;

BOOL (__stdcall * Real_SetLocalTime)(CONST SYSTEMTIME* a0)
    = SetLocalTime;

BOOL (__stdcall * Real_SetLocaleInfoA)(LCID a0,
                                       LCTYPE a1,
                                       LPCSTR a2)
    = SetLocaleInfoA;

BOOL (__stdcall * Real_SetLocaleInfoW)(LCID a0,
                                       LCTYPE a1,
                                       LPCWSTR a2)
    = SetLocaleInfoW;

BOOL (__stdcall * Real_SetMailslotInfo)(HANDLE a0,
                                        DWORD a1)
    = SetMailslotInfo;

int (__stdcall * Real_SetMapMode)(HDC a0,
                                  int a1)
    = SetMapMode;

DWORD (__stdcall * Real_SetMapperFlags)(HDC a0,
                                        DWORD a1)
    = SetMapperFlags;

BOOL (__stdcall * Real_SetMenu)(HWND a0,
                                HMENU a1)
    = SetMenu;

BOOL (__stdcall * Real_SetMenuContextHelpId)(HMENU a0,
                                             DWORD a1)
    = SetMenuContextHelpId;

BOOL (__stdcall * Real_SetMenuDefaultItem)(HMENU a0,
                                           UINT a1,
                                           UINT a2)
    = SetMenuDefaultItem;

BOOL (__stdcall * Real_SetMenuItemBitmaps)(HMENU a0,
                                           UINT a1,
                                           UINT a2,
                                           HBITMAP a3,
                                           HBITMAP a4)
    = SetMenuItemBitmaps;

BOOL (__stdcall * Real_SetMenuItemInfoA)(HMENU a0,
                                         UINT a1,
                                         BOOL a2,
                                         CONST MENUITEMINFOA* a3)
    = SetMenuItemInfoA;

BOOL (__stdcall * Real_SetMenuItemInfoW)(HMENU a0,
                                         UINT a1,
                                         BOOL a2,
                                         CONST MENUITEMINFOW* a3)
    = SetMenuItemInfoW;

LPARAM (__stdcall * Real_SetMessageExtraInfo)(LPARAM a0)
    = SetMessageExtraInfo;

BOOL (__stdcall * Real_SetMessageQueue)(int a0)
    = SetMessageQueue;

HMETAFILE (__stdcall * Real_SetMetaFileBitsEx)(UINT a0,
                                               CONST BYTE* a1)
    = SetMetaFileBitsEx;

int (__stdcall * Real_SetMetaRgn)(HDC a0)
    = SetMetaRgn;

BOOL (__stdcall * Real_SetMiterLimit)(HDC a0,
                                      FLOAT a1,
                                      PFLOAT a2)
    = SetMiterLimit;

BOOL (__stdcall * Real_SetNamedPipeHandleState)(HANDLE a0,
                                                LPDWORD a1,
                                                LPDWORD a2,
                                                LPDWORD a3)
    = SetNamedPipeHandleState;

UINT (__stdcall * Real_SetPaletteEntries)(HPALETTE a0,
                                          UINT a1,
                                          UINT a2,
                                          CONST PALETTEENTRY* a3)
    = SetPaletteEntries;

HWND (__stdcall * Real_SetParent)(HWND a0,
                                  HWND a1)
    = SetParent;

COLORREF (__stdcall * Real_SetPixel)(HDC a0,
                                     int a1,
                                     int a2,
                                     COLORREF a3)
    = SetPixel;

BOOL (__stdcall * Real_SetPixelFormat)(HDC a0,
                                       int a1,
                                       CONST PIXELFORMATDESCRIPTOR* a2)
    = SetPixelFormat;

BOOL (__stdcall * Real_SetPixelV)(HDC a0,
                                  int a1,
                                  int a2,
                                  COLORREF a3)
    = SetPixelV;

int (__stdcall * Real_SetPolyFillMode)(HDC a0,
                                       int a1)
    = SetPolyFillMode;

BOOL (__stdcall * Real_SetPriorityClass)(HANDLE a0,
                                         DWORD a1)
    = SetPriorityClass;

BOOL (__stdcall * Real_SetProcessAffinityMask)(HANDLE a0,
                                               DWORD_PTR a1)
    = SetProcessAffinityMask;

BOOL (__stdcall * Real_SetProcessShutdownParameters)(DWORD a0,
                                                     DWORD a1)
    = SetProcessShutdownParameters;

BOOL (__stdcall * Real_SetProcessWindowStation)(HWINSTA a0)
    = SetProcessWindowStation;

BOOL (__stdcall * Real_SetProcessWorkingSetSize)(HANDLE a0,
                                                 SIZE_T a1,
                                                 SIZE_T a2)
    = SetProcessWorkingSetSize;

BOOL (__stdcall * Real_SetPropA)(HWND a0,
                                 LPCSTR a1,
                                 HANDLE a2)
    = SetPropA;

BOOL (__stdcall * Real_SetPropW)(HWND a0,
                                 LPCWSTR a1,
                                 HANDLE a2)
    = SetPropW;

int (__stdcall * Real_SetROP2)(HDC a0,
                               int a1)
    = SetROP2;

BOOL (__stdcall * Real_SetRect)(LPRECT a0,
                                int a1,
                                int a2,
                                int a3,
                                int a4)
    = SetRect;

BOOL (__stdcall * Real_SetRectEmpty)(LPRECT a0)
    = SetRectEmpty;

BOOL (__stdcall * Real_SetRectRgn)(HRGN a0,
                                   int a1,
                                   int a2,
                                   int a3,
                                   int a4)
    = SetRectRgn;

int (__stdcall * Real_SetScrollInfo)(HWND a0,
                                     int a1,
                                     LPCSCROLLINFO a2,
                                     BOOL a3)
    = SetScrollInfo;

int (__stdcall * Real_SetScrollPos)(HWND a0,
                                    int a1,
                                    int a2,
                                    BOOL a3)
    = SetScrollPos;

BOOL (__stdcall * Real_SetScrollRange)(HWND a0,
                                       int a1,
                                       int a2,
                                       int a3,
                                       BOOL a4)
    = SetScrollRange;

BOOL (__stdcall * Real_SetStdHandle)(DWORD a0,
                                     HANDLE a1)
    = SetStdHandle;

int (__stdcall * Real_SetStretchBltMode)(HDC a0,
                                         int a1)
    = SetStretchBltMode;

BOOL (__stdcall * Real_SetSysColors)(int a0,
                                     CONST INT* a1,
                                     CONST COLORREF* a2)
    = SetSysColors;

BOOL (__stdcall * Real_SetSystemCursor)(HCURSOR a0,
                                        DWORD a1)
    = SetSystemCursor;

UINT (__stdcall * Real_SetSystemPaletteUse)(HDC a0,
                                            UINT a1)
    = SetSystemPaletteUse;

BOOL (__stdcall * Real_SetSystemPowerState)(BOOL a0,
                                            BOOL a1)
    = SetSystemPowerState;

BOOL (__stdcall * Real_SetSystemTime)(CONST SYSTEMTIME* a0)
    = SetSystemTime;

BOOL (__stdcall * Real_SetSystemTimeAdjustment)(DWORD a0,
                                                BOOL a1)
    = SetSystemTimeAdjustment;

DWORD (__stdcall * Real_SetTapeParameters)(HANDLE a0,
                                           DWORD a1,
                                           LPVOID a2)
    = SetTapeParameters;

DWORD (__stdcall * Real_SetTapePosition)(HANDLE a0,
                                         DWORD a1,
                                         DWORD a2,
                                         DWORD a3,
                                         DWORD a4,
                                         BOOL a5)
    = SetTapePosition;

UINT (__stdcall * Real_SetTextAlign)(HDC a0,
                                     UINT a1)
    = SetTextAlign;

int (__stdcall * Real_SetTextCharacterExtra)(HDC a0,
                                             int a1)
    = SetTextCharacterExtra;

COLORREF (__stdcall * Real_SetTextColor)(HDC a0,
                                         COLORREF a1)
    = SetTextColor;

BOOL (__stdcall * Real_SetTextJustification)(HDC a0,
                                             int a1,
                                             int a2)
    = SetTextJustification;

DWORD_PTR (__stdcall * Real_SetThreadAffinityMask)(HANDLE a0,
                                               DWORD_PTR a1)
    = SetThreadAffinityMask;

BOOL (__stdcall * Real_SetThreadContext)(HANDLE a0,
                                         CONST CONTEXT* a1)
    = SetThreadContext;

BOOL (__stdcall * Real_SetThreadDesktop)(HDESK a0)
    = SetThreadDesktop;

DWORD (__stdcall * Real_SetThreadIdealProcessor)(HANDLE a0,
                                                 DWORD a1)
    = SetThreadIdealProcessor;

#if(WINVER >= 0x0500)
BOOL (__stdcall * Real_SetThreadLocale)(LCID a0)
    = SetThreadLocale;
#endif // (WINVER >= 0x0500)

BOOL (__stdcall * Real_SetThreadPriority)(HANDLE a0,
                                          int a1)
    = SetThreadPriority;

BOOL (__stdcall * Real_SetThreadPriorityBoost)(HANDLE a0,
                                               BOOL a1)
    = SetThreadPriorityBoost;

BOOL (__stdcall * Real_SetTimeZoneInformation)(CONST TIME_ZONE_INFORMATION* a0)
    = SetTimeZoneInformation;

UINT_PTR (__stdcall * Real_SetTimer)(HWND a0,
                                     UINT_PTR a1,
                                     UINT a2,
                                     TIMERPROC a3)
    = SetTimer;

LPTOP_LEVEL_EXCEPTION_FILTER (__stdcall * Real_SetUnhandledExceptionFilter)(LPTOP_LEVEL_EXCEPTION_FILTER a0)
    = SetUnhandledExceptionFilter;

BOOL (__stdcall * Real_SetUserObjectInformationA)(HANDLE a0,
                                                  int a1,
                                                  PVOID a2,
                                                  DWORD a3)
    = SetUserObjectInformationA;

BOOL (__stdcall * Real_SetUserObjectInformationW)(HANDLE a0,
                                                  int a1,
                                                  PVOID a2,
                                                  DWORD a3)
    = SetUserObjectInformationW;

BOOL (__stdcall * Real_SetUserObjectSecurity)(HANDLE a0,
                                              PSECURITY_INFORMATION a1,
                                              PSECURITY_DESCRIPTOR a2)
    = SetUserObjectSecurity;

BOOL (__stdcall * Real_SetViewportExtEx)(HDC a0,
                                         int a1,
                                         int a2,
                                         SIZE* a3)
    = SetViewportExtEx;

BOOL (__stdcall * Real_SetViewportOrgEx)(HDC a0,
                                         int a1,
                                         int a2,
                                         POINT* a3)
    = SetViewportOrgEx;

BOOL (__stdcall * Real_SetVolumeLabelA)(LPCSTR a0,
                                        LPCSTR a1)
    = SetVolumeLabelA;

BOOL (__stdcall * Real_SetVolumeLabelW)(LPCWSTR a0,
                                        LPCWSTR a1)
    = SetVolumeLabelW;

BOOL (__stdcall * Real_SetWaitableTimer)(HANDLE a0,
                                         CONST LARGE_INTEGER* a1,
                                         LONG a2,
                                         PTIMERAPCROUTINE a3,
                                         LPVOID a4,
                                         BOOL a5)
    = SetWaitableTimer;

HENHMETAFILE (__stdcall * Real_SetWinMetaFileBits)(UINT a0,
                                                   CONST BYTE* a1,
                                                   HDC a2,
                                                   CONST METAFILEPICT* a3)
    = SetWinMetaFileBits;

BOOL (__stdcall * Real_SetWindowContextHelpId)(HWND a0,
                                               DWORD a1)
    = SetWindowContextHelpId;

BOOL (__stdcall * Real_SetWindowExtEx)(HDC a0,
                                       int a1,
                                       int a2,
                                       SIZE* a3)
    = SetWindowExtEx;

LONG (__stdcall * Real_SetWindowLongA)(HWND a0,
                                       int a1,
                                       LONG a2)
    = SetWindowLongA;

LONG (__stdcall * Real_SetWindowLongW)(HWND a0,
                                       int a1,
                                       LONG a2)
    = SetWindowLongW;

BOOL (__stdcall * Real_SetWindowOrgEx)(HDC a0,
                                       int a1,
                                       int a2,
                                       POINT* a3)
    = SetWindowOrgEx;

BOOL (__stdcall * Real_SetWindowPlacement)(HWND a0,
                                           CONST WINDOWPLACEMENT* a1)
    = SetWindowPlacement;

BOOL (__stdcall * Real_SetWindowPos)(HWND a0,
                                     HWND a1,
                                     int a2,
                                     int a3,
                                     int a4,
                                     int a5,
                                     UINT a6)
    = SetWindowPos;

int (__stdcall * Real_SetWindowRgn)(HWND a0,
                                    HRGN a1,
                                    BOOL a2)
    = SetWindowRgn;

BOOL (__stdcall * Real_SetWindowTextA)(HWND a0,
                                       LPCSTR a1)
    = SetWindowTextA;

BOOL (__stdcall * Real_SetWindowTextW)(HWND a0,
                                       LPCWSTR a1)
    = SetWindowTextW;

WORD (__stdcall * Real_SetWindowWord)(HWND a0,
                                      int a1,
                                      WORD a2)
    = SetWindowWord;

HHOOK (__stdcall * Real_SetWindowsHookA)(int a0,
                                         HOOKPROC a1)
    = SetWindowsHookA;

HHOOK (__stdcall * Real_SetWindowsHookExA)(int a0,
                                           HOOKPROC a1,
                                           HINSTANCE a2,
                                           DWORD a3)
    = SetWindowsHookExA;

HHOOK (__stdcall * Real_SetWindowsHookExW)(int a0,
                                           HOOKPROC a1,
                                           HINSTANCE a2,
                                           DWORD a3)
    = SetWindowsHookExW;

HHOOK (__stdcall * Real_SetWindowsHookW)(int a0,
                                         HOOKPROC a1)
    = SetWindowsHookW;

BOOL (__stdcall * Real_SetWorldTransform)(HDC a0,
                                          CONST XFORM* a1)
    = SetWorldTransform;

BOOL (__stdcall * Real_SetupComm)(HANDLE a0,
                                  DWORD a1,
                                  DWORD a2)
    = SetupComm;

BOOL (__stdcall * Real_ShowCaret)(HWND a0)
    = ShowCaret;

int (__stdcall * Real_ShowCursor)(BOOL a0)
    = ShowCursor;

BOOL (__stdcall * Real_ShowOwnedPopups)(HWND a0,
                                        BOOL a1)
    = ShowOwnedPopups;

BOOL (__stdcall * Real_ShowScrollBar)(HWND a0,
                                      int a1,
                                      BOOL a2)
    = ShowScrollBar;

BOOL (__stdcall * Real_ShowWindow)(HWND a0,
                                   int a1)
    = ShowWindow;

BOOL (__stdcall * Real_ShowWindowAsync)(HWND a0,
                                        int a1)
    = ShowWindowAsync;

DWORD (__stdcall * Real_SignalObjectAndWait)(HANDLE a0,
                                             HANDLE a1,
                                             DWORD a2,
                                             BOOL a3)
    = SignalObjectAndWait;

DWORD (__stdcall * Real_SizeofResource)(HMODULE a0,
                                        HRSRC a1)
    = SizeofResource;

DWORD (__stdcall * Real_SleepEx)(DWORD a0,
                                 BOOL a1)
    = SleepEx;

int (__stdcall * Real_StartDocA)(HDC a0,
                                 CONST DOCINFOA* a1)
    = StartDocA;

int (__stdcall * Real_StartDocW)(HDC a0,
                                 CONST DOCINFOW* a1)
    = StartDocW;

int (__stdcall * Real_StartPage)(HDC a0)
    = StartPage;

HRESULT (__stdcall * Real_StgCreateDocfile)(CONST OLECHAR* a0,
                                            DWORD a1,
                                            DWORD a2,
                                            IStorage** a3)
    = StgCreateDocfile;

HRESULT (__stdcall * Real_StgCreateDocfileOnILockBytes)(ILockBytes* a0,
                                                        DWORD a1,
                                                        DWORD a2,
                                                        IStorage** a3)
    = StgCreateDocfileOnILockBytes;

HRESULT (__stdcall * Real_StgCreatePropSetStg)(IStorage* a0,
                                               DWORD a1,
                                               IPropertySetStorage** a2)
    = StgCreatePropSetStg;

HRESULT (__stdcall * Real_StgCreatePropStg)(IUnknown* a0,
                                            CONST IID& a1,
                                            CONST CLSID* a2,
                                            DWORD a3,
                                            DWORD a4,
                                            IPropertyStorage** a5)
    = StgCreatePropStg;

#if _MSC_VER < 1300
HRESULT (__stdcall * Real_StgCreateStorageEx)(CONST WCHAR* a0,
                                              DWORD a1,
                                              DWORD a2,
                                              DWORD a3,
                                              void* a4,
                                              void* a5,
                                              CONST IID& a6,
                                              void** a7)
    = StgCreateStorageEx;
#else
HRESULT (__stdcall * Real_StgCreateStorageEx)(CONST WCHAR* a0,
                                              DWORD a1,
                                              DWORD a2,
                                              DWORD a3,
                                              STGOPTIONS* a4,
                                              void* a5,
                                              CONST IID& a6,
                                              void** a7)
    = StgCreateStorageEx;
#endif

HRESULT (__stdcall * Real_StgGetIFillLockBytesOnFile)(CONST OLECHAR* a0,
                                                      IFillLockBytes** a1)
    = StgGetIFillLockBytesOnFile;

HRESULT (__stdcall * Real_StgGetIFillLockBytesOnILockBytes)(ILockBytes* a0,
                                                            IFillLockBytes** a1)
    = StgGetIFillLockBytesOnILockBytes;

HRESULT (__stdcall * Real_StgIsStorageFile)(CONST OLECHAR* a0)
    = StgIsStorageFile;

HRESULT (__stdcall * Real_StgIsStorageILockBytes)(ILockBytes* a0)
    = StgIsStorageILockBytes;

HRESULT (__stdcall * Real_StgOpenAsyncDocfileOnIFillLockBytes)(IFillLockBytes* a0,
                                                               DWORD a1,
                                                               DWORD a2,
                                                               IStorage** a3)
    = StgOpenAsyncDocfileOnIFillLockBytes;

HRESULT (__stdcall * Real_StgOpenPropStg)(IUnknown* a0,
                                          CONST IID& a1,
                                          DWORD a2,
                                          DWORD a3,
                                          IPropertyStorage** a4)
    = StgOpenPropStg;

HRESULT (__stdcall * Real_StgOpenStorage)(CONST OLECHAR* a0,
                                          IStorage* a1,
                                          DWORD a2,
                                          SNB a3,
                                          DWORD a4,
                                          IStorage** a5)
    = StgOpenStorage;

#if _MSC_VER < 1300
HRESULT (__stdcall * Real_StgOpenStorageEx)(CONST WCHAR* a0,
                                            DWORD a1,
                                            DWORD a2,
                                            DWORD a3,
                                            void* a4,
                                            void* a5,
                                            CONST IID& a6,
                                            void** a7)
    = StgOpenStorageEx;
#else
HRESULT (__stdcall * Real_StgOpenStorageEx)(CONST WCHAR* a0,
                                            DWORD a1,
                                            DWORD a2,
                                            DWORD a3,
                                            STGOPTIONS* a4,
                                            void* a5,
                                            CONST IID& a6,
                                            void** a7)
    = StgOpenStorageEx;
#endif

HRESULT (__stdcall * Real_StgOpenStorageOnILockBytes)(ILockBytes* a0,
                                                      IStorage* a1,
                                                      DWORD a2,
                                                      SNB a3,
                                                      DWORD a4,
                                                      IStorage** a5)
    = StgOpenStorageOnILockBytes;

HRESULT (__stdcall * Real_StgSetTimes)(CONST OLECHAR* a0,
                                       CONST FILETIME* a1,
                                       CONST FILETIME* a2,
                                       CONST FILETIME* a3)
    = StgSetTimes;

BOOL (__stdcall * Real_StretchBlt)(HDC a0,
                                   int a1,
                                   int a2,
                                   int a3,
                                   int a4,
                                   HDC a5,
                                   int a6,
                                   int a7,
                                   int a8,
                                   int a9,
                                   DWORD a10)
    = StretchBlt;

int (__stdcall * Real_StretchDIBits)(HDC a0,
                                     int a1,
                                     int a2,
                                     int a3,
                                     int a4,
                                     int a5,
                                     int a6,
                                     int a7,
                                     int a8,
                                     CONST void* a9,
                                     CONST BITMAPINFO* a10,
                                     UINT a11,
                                     DWORD a12)
    = StretchDIBits;

HRESULT (__stdcall * Real_StringFromCLSID)(CONST IID& a0,
                                           LPOLESTR* a1)
    = StringFromCLSID;

int (__stdcall * Real_StringFromGUID2)(CONST GUID& a0,
                                       LPOLESTR a1,
                                       int a2)
    = StringFromGUID2;

HRESULT (__stdcall * Real_StringFromIID)(CONST IID& a0,
                                         LPOLESTR* a1)
    = StringFromIID;

BOOL (__stdcall * Real_StrokeAndFillPath)(HDC a0)
    = StrokeAndFillPath;

BOOL (__stdcall * Real_StrokePath)(HDC a0)
    = StrokePath;

BOOL (__stdcall * Real_SubtractRect)(LPRECT a0,
                                     CONST RECT* a1,
                                     CONST RECT* a2)
    = SubtractRect;

DWORD (__stdcall * Real_SuspendThread)(HANDLE a0)
    = SuspendThread;

BOOL (__stdcall * Real_SwapBuffers)(HDC a0)
    = SwapBuffers;

BOOL (__stdcall * Real_SwapMouseButton)(BOOL a0)
    = SwapMouseButton;

BOOL (__stdcall * Real_SwitchDesktop)(HDESK a0)
    = SwitchDesktop;

void (__stdcall * Real_SwitchToFiber)(LPVOID a0)
    = SwitchToFiber;

BOOL (__stdcall * Real_SwitchToThread)(void)
    = SwitchToThread;

BOOL (__stdcall * Real_SystemParametersInfoA)(UINT a0,
                                              UINT a1,
                                              PVOID a2,
                                              UINT a3)
    = SystemParametersInfoA;

BOOL (__stdcall * Real_SystemParametersInfoW)(UINT a0,
                                              UINT a1,
                                              PVOID a2,
                                              UINT a3)
    = SystemParametersInfoW;

BOOL (__stdcall * Real_SystemTimeToFileTime)(CONST SYSTEMTIME* a0,
                                             LPFILETIME a1)
    = SystemTimeToFileTime;

BOOL (__stdcall * Real_SystemTimeToTzSpecificLocalTime)(
#ifdef PIPE_ACCEPT_REMOTE_CLIENTS
                                                        const TIME_ZONE_INFORMATION *a0,
                                                        const SYSTEMTIME *a1,
#else
                                                        LPTIME_ZONE_INFORMATION a0,
                                                        LPSYSTEMTIME a1,
#endif
                                                        LPSYSTEMTIME a2
                                                       )
    = SystemTimeToTzSpecificLocalTime;

#if _MSC_VER < 1300
LONG (__stdcall * Real_TabbedTextOutA)(HDC a0,
                                       int a1,
                                       int a2,
                                       LPCSTR a3,
                                       int a4,
                                       int a5,
                                       INT* a6,
                                       int a7)
    = TabbedTextOutA;
#else
LONG (__stdcall * Real_TabbedTextOutA)(HDC a0,
                                       int a1,
                                       int a2,
                                       LPCSTR a3,
                                       int a4,
                                       int a5,
                                       CONST INT* a6,
                                       int a7)
    = TabbedTextOutA;
#endif

#if _MSC_VER < 1300
LONG (__stdcall * Real_TabbedTextOutW)(HDC a0,
                                       int a1,
                                       int a2,
                                       LPCWSTR a3,
                                       int a4,
                                       int a5,
                                       INT* a6,
                                       int a7)
    = TabbedTextOutW;
#else
LONG (__stdcall * Real_TabbedTextOutW)(HDC a0,
                                       int a1,
                                       int a2,
                                       LPCWSTR a3,
                                       int a4,
                                       int a5,
                                       CONST INT* a6,
                                       int a7)
    = TabbedTextOutW;
#endif

BOOL (__stdcall * Real_TerminateProcess)(HANDLE a0,
                                         UINT a1)
    = TerminateProcess;

BOOL (__stdcall * Real_TerminateThread)(HANDLE a0,
                                        DWORD a1)
    = TerminateThread;

BOOL (__stdcall * Real_TextOutA)(HDC a0,
                                 int a1,
                                 int a2,
                                 LPCSTR a3,
                                 int a4)
    = TextOutA;

BOOL (__stdcall * Real_TextOutW)(HDC a0,
                                 int a1,
                                 int a2,
                                 LPCWSTR a3,
                                 int a4)
    = TextOutW;

WORD (__stdcall * Real_TileWindows)(HWND a0,
                                    UINT a1,
                                    CONST RECT* a2,
                                    UINT a3,
                                    CONST HWND * a4)
    = TileWindows;

#if _MSC_VER < 1300
int (__stdcall * Real_ToAscii)(UINT a0,
                               UINT a1,
                               BYTE* a2,
                               LPWORD a3,
                               UINT a4)
    = ToAscii;
#else
int (__stdcall * Real_ToAscii)(UINT a0,
                               UINT a1,
                               CONST BYTE* a2,
                               LPWORD a3,
                               UINT a4)
    = ToAscii;
#endif

#if _MSC_VER < 1300
int (__stdcall * Real_ToAsciiEx)(UINT a0,
                                 UINT a1,
                                 BYTE* a2,
                                 LPWORD a3,
                                 UINT a4,
                                 HKL a5)
    = ToAsciiEx;
#else
int (__stdcall * Real_ToAsciiEx)(UINT a0,
                                 UINT a1,
                                 CONST BYTE* a2,
                                 LPWORD a3,
                                 UINT a4,
                                 HKL a5)
    = ToAsciiEx;
#endif

#if _MSC_VER < 1300
int (__stdcall * Real_ToUnicode)(UINT a0,
                                 UINT a1,
                                 BYTE* a2,
                                 LPWSTR a3,
                                 int a4,
                                 UINT a5)
    = ToUnicode;
#else
int (__stdcall * Real_ToUnicode)(UINT a0,
                                 UINT a1,
                                 CONST BYTE* a2,
                                 LPWSTR a3,
                                 int a4,
                                 UINT a5)
    = ToUnicode;
#endif

#if _MSC_VER < 1300
int (__stdcall * Real_ToUnicodeEx)(UINT a0,
                                   UINT a1,
                                   BYTE* a2,
                                   LPWSTR a3,
                                   int a4,
                                   UINT a5,
                                   HKL a6)
    = ToUnicodeEx;
#else
int (__stdcall * Real_ToUnicodeEx)(UINT a0,
                                   UINT a1,
                                   CONST BYTE* a2,
                                   LPWSTR a3,
                                   int a4,
                                   UINT a5,
                                   HKL a6)
    = ToUnicodeEx;
#endif

BOOL (__stdcall * Real_TrackMouseEvent)(TRACKMOUSEEVENT* a0)
    = TrackMouseEvent;

BOOL (__stdcall * Real_TrackPopupMenu)(HMENU a0,
                                       UINT a1,
                                       int a2,
                                       int a3,
                                       int a4,
                                       HWND a5,
                                       CONST RECT* a6)
    = TrackPopupMenu;

BOOL (__stdcall * Real_TrackPopupMenuEx)(HMENU a0,
                                         UINT a1,
                                         int a2,
                                         int a3,
                                         HWND a4,
                                         LPTPMPARAMS a5)
    = TrackPopupMenuEx;

BOOL (__stdcall * Real_TransactNamedPipe)(HANDLE a0,
                                          LPVOID a1,
                                          DWORD a2,
                                          LPVOID a3,
                                          DWORD a4,
                                          LPDWORD a5,
                                          LPOVERLAPPED a6)
    = TransactNamedPipe;

int (__stdcall * Real_TranslateAcceleratorA)(HWND a0,
                                             HACCEL a1,
                                             LPMSG a2)
    = TranslateAcceleratorA;

int (__stdcall * Real_TranslateAcceleratorW)(HWND a0,
                                             HACCEL a1,
                                             LPMSG a2)
    = TranslateAcceleratorW;

BOOL (__stdcall * Real_TranslateCharsetInfo)(DWORD* a0,
                                             CHARSETINFO* a1,
                                             DWORD a2)
    = TranslateCharsetInfo;

BOOL (__stdcall * Real_TranslateMDISysAccel)(HWND a0,
                                             LPMSG a1)
    = TranslateMDISysAccel;

BOOL (__stdcall * Real_TranslateMessage)(CONST MSG* a0)
    = TranslateMessage;

BOOL (__stdcall * Real_TransmitCommChar)(HANDLE a0,
                                         char a1)
    = TransmitCommChar;

#if DETOUR_TryEnterCriticalSection_EVEN_IF_IT_EXERCISES_A_BUG_IN_RtlLookupFunctionTable
BOOL (__stdcall * Real_TryEnterCriticalSection)(LPCRITICAL_SECTION a0)
    = TryEnterCriticalSection;
#endif

BOOL (__stdcall * Real_UnhookWindowsHook)(int a0,
                                          HOOKPROC a1)
    = UnhookWindowsHook;

BOOL (__stdcall * Real_UnhookWindowsHookEx)(HHOOK a0)
    = UnhookWindowsHookEx;

BOOL (__stdcall * Real_UnionRect)(LPRECT a0,
                                  CONST RECT* a1,
                                  CONST RECT* a2)
    = UnionRect;

BOOL (__stdcall * Real_UnloadKeyboardLayout)(HKL a0)
    = UnloadKeyboardLayout;

BOOL (__stdcall * Real_UnlockFile)(HANDLE a0,
                                   DWORD a1,
                                   DWORD a2,
                                   DWORD a3,
                                   DWORD a4)
    = UnlockFile;

BOOL (__stdcall * Real_UnlockFileEx)(HANDLE a0,
                                     DWORD a1,
                                     DWORD a2,
                                     DWORD a3,
                                     LPOVERLAPPED a4)
    = UnlockFileEx;

BOOL (__stdcall * Real_UnmapViewOfFile)(LPCVOID a0)
    = UnmapViewOfFile;

BOOL (__stdcall * Real_UnpackDDElParam)(UINT a0,
                                        LPARAM a1,
                                        PUINT_PTR a2,
                                        PUINT_PTR a3)
    = UnpackDDElParam;

BOOL (__stdcall * Real_UnrealizeObject)(HGDIOBJ a0)
    = UnrealizeObject;

BOOL (__stdcall * Real_UnregisterClassA)(LPCSTR a0,
                                         HINSTANCE a1)
    = UnregisterClassA;

BOOL (__stdcall * Real_UnregisterClassW)(LPCWSTR a0,
                                         HINSTANCE a1)
    = UnregisterClassW;

BOOL (__stdcall * Real_UnregisterHotKey)(HWND a0,
                                         int a1)
    = UnregisterHotKey;

BOOL (__stdcall * Real_UpdateColors)(HDC a0)
    = UpdateColors;

BOOL (__stdcall * Real_UpdateResourceA)(HANDLE a0,
                                        LPCSTR a1,
                                        LPCSTR a2,
                                        WORD a3,
                                        LPVOID a4,
                                        DWORD a5)
    = UpdateResourceA;

BOOL (__stdcall * Real_UpdateResourceW)(HANDLE a0,
                                        LPCWSTR a1,
                                        LPCWSTR a2,
                                        WORD a3,
                                        LPVOID a4,
                                        DWORD a5)
    = UpdateResourceW;

BOOL (__stdcall * Real_UpdateWindow)(HWND a0)
    = UpdateWindow;

BOOL (__stdcall * Real_ValidateRect)(HWND a0,
                                     CONST RECT* a1)
    = ValidateRect;

BOOL (__stdcall * Real_ValidateRgn)(HWND a0,
                                    HRGN a1)
    = ValidateRgn;

DWORD (__stdcall * Real_VerLanguageNameA)(DWORD a0,
                                          LPSTR a1,
                                          DWORD a2)
    = VerLanguageNameA;

DWORD (__stdcall * Real_VerLanguageNameW)(DWORD a0,
                                          LPWSTR a1,
                                          DWORD a2)
    = VerLanguageNameW;

LPVOID (__stdcall * Real_VirtualAllocEx)(HANDLE a0,
                                         LPVOID a1,
                                         SIZE_T a2,
                                         DWORD a3,
                                         DWORD a4)
    = VirtualAllocEx;

BOOL (__stdcall * Real_VirtualFreeEx)(HANDLE a0,
                                      LPVOID a1,
                                      SIZE_T a2,
                                      DWORD a3)
    = VirtualFreeEx;

BOOL (__stdcall * Real_VirtualProtectEx)(HANDLE a0,
                                         LPVOID a1,
                                         SIZE_T a2,
                                         DWORD a3,
                                         PDWORD a4)
    = VirtualProtectEx;

DWORD_PTR (__stdcall * Real_VirtualQueryEx)(HANDLE a0,
                                        LPCVOID a1,
                                        PMEMORY_BASIC_INFORMATION a2,
                                        DWORD_PTR a3)
    = VirtualQueryEx;

SHORT (__stdcall * Real_VkKeyScanA)(CHAR a0)
    = VkKeyScanA;

SHORT (__stdcall * Real_VkKeyScanExA)(CHAR a0,
                                      HKL a1)
    = VkKeyScanExA;

SHORT (__stdcall * Real_VkKeyScanExW)(WCHAR a0,
                                      HKL a1)
    = VkKeyScanExW;

SHORT (__stdcall * Real_VkKeyScanW)(WCHAR a0)
    = VkKeyScanW;

SOCKET (__stdcall * Real_WSAAccept)(SOCKET a0,
                                    sockaddr* a1,
                                    LPINT a2,
                                    LPCONDITIONPROC a3,
                                    DWORD_PTR a4)
    = WSAAccept;

INT (__stdcall * Real_WSAAddressToStringA)(LPSOCKADDR a0,
                                           DWORD a1,
                                           LPWSAPROTOCOL_INFOA a2,
                                           LPSTR a3,
                                           LPDWORD a4)
    = WSAAddressToStringA;

INT (__stdcall * Real_WSAAddressToStringW)(LPSOCKADDR a0,
                                           DWORD a1,
                                           LPWSAPROTOCOL_INFOW a2,
                                           LPWSTR a3,
                                           LPDWORD a4)
    = WSAAddressToStringW;

HANDLE (__stdcall * Real_WSAAsyncGetHostByAddr)(HWND a0,
                                                u_int a1,
                                                CONST char* a2,
                                                int a3,
                                                int a4,
                                                char* a5,
                                                int a6)
    = WSAAsyncGetHostByAddr;

HANDLE (__stdcall * Real_WSAAsyncGetHostByName)(HWND a0,
                                                u_int a1,
                                                CONST char* a2,
                                                char* a3,
                                                int a4)
    = WSAAsyncGetHostByName;

HANDLE (__stdcall * Real_WSAAsyncGetProtoByName)(HWND a0,
                                                 u_int a1,
                                                 CONST char* a2,
                                                 char* a3,
                                                 int a4)
    = WSAAsyncGetProtoByName;

HANDLE (__stdcall * Real_WSAAsyncGetProtoByNumber)(HWND a0,
                                                   u_int a1,
                                                   int a2,
                                                   char* a3,
                                                   int a4)
    = WSAAsyncGetProtoByNumber;

HANDLE (__stdcall * Real_WSAAsyncGetServByName)(HWND a0,
                                                u_int a1,
                                                CONST char* a2,
                                                CONST char* a3,
                                                char* a4,
                                                int a5)
    = WSAAsyncGetServByName;

HANDLE (__stdcall * Real_WSAAsyncGetServByPort)(HWND a0,
                                                u_int a1,
                                                int a2,
                                                CONST char* a3,
                                                char* a4,
                                                int a5)
    = WSAAsyncGetServByPort;

int (__stdcall * Real_WSAAsyncSelect)(SOCKET a0,
                                      HWND a1,
                                      u_int a2,
                                      long a3)
    = WSAAsyncSelect;

int (__stdcall * Real_WSACancelAsyncRequest)(HANDLE a0)
    = WSACancelAsyncRequest;

int (__stdcall * Real_WSACancelBlockingCall)(void)
    = WSACancelBlockingCall;

int (__stdcall * Real_WSACleanup)(void)
    = WSACleanup;

BOOL (__stdcall * Real_WSACloseEvent)(HANDLE a0)
    = WSACloseEvent;

int (__stdcall * Real_WSAConnect)(SOCKET a0,
                                  CONST sockaddr* a1,
                                  int a2,
                                  LPWSABUF a3,
                                  LPWSABUF a4,
                                  LPQOS a5,
                                  LPQOS a6)
    = WSAConnect;

HANDLE (__stdcall * Real_WSACreateEvent)(void)
    = WSACreateEvent;

int (__stdcall * Real_WSADuplicateSocketA)(SOCKET a0,
                                           DWORD a1,
                                           LPWSAPROTOCOL_INFOA a2)
    = WSADuplicateSocketA;

int (__stdcall * Real_WSADuplicateSocketW)(SOCKET a0,
                                           DWORD a1,
                                           LPWSAPROTOCOL_INFOW a2)
    = WSADuplicateSocketW;

INT (__stdcall * Real_WSAEnumNameSpaceProvidersA)(LPDWORD a0,
                                                  LPWSANAMESPACE_INFOA a1)
    = WSAEnumNameSpaceProvidersA;

INT (__stdcall * Real_WSAEnumNameSpaceProvidersW)(LPDWORD a0,
                                                  LPWSANAMESPACE_INFOW a1)
    = WSAEnumNameSpaceProvidersW;

int (__stdcall * Real_WSAEnumNetworkEvents)(SOCKET a0,
                                            HANDLE a1,
                                            LPWSANETWORKEVENTS a2)
    = WSAEnumNetworkEvents;

int (__stdcall * Real_WSAEnumProtocolsA)(LPINT a0,
                                         LPWSAPROTOCOL_INFOA a1,
                                         LPDWORD a2)
    = WSAEnumProtocolsA;

int (__stdcall * Real_WSAEnumProtocolsW)(LPINT a0,
                                         LPWSAPROTOCOL_INFOW a1,
                                         LPDWORD a2)
    = WSAEnumProtocolsW;

int (__stdcall * Real_WSAEventSelect)(SOCKET a0,
                                      HANDLE a1,
                                      long a2)
    = WSAEventSelect;

BOOL (__stdcall * Real_WSAGetOverlappedResult)(SOCKET a0,
                                               LPWSAOVERLAPPED a1,
                                               LPDWORD a2,
                                               BOOL a3,
                                               LPDWORD a4)
    = WSAGetOverlappedResult;

BOOL (__stdcall * Real_WSAGetQOSByName)(SOCKET a0,
                                        LPWSABUF a1,
                                        LPQOS a2)
    = WSAGetQOSByName;

INT (__stdcall * Real_WSAGetServiceClassInfoA)(LPGUID a0,
                                               LPGUID a1,
                                               LPDWORD a2,
                                               LPWSASERVICECLASSINFOA a3)
    = WSAGetServiceClassInfoA;

INT (__stdcall * Real_WSAGetServiceClassInfoW)(LPGUID a0,
                                               LPGUID a1,
                                               LPDWORD a2,
                                               LPWSASERVICECLASSINFOW a3)
    = WSAGetServiceClassInfoW;

INT (__stdcall * Real_WSAGetServiceClassNameByClassIdA)(LPGUID a0,
                                                        LPSTR a1,
                                                        LPDWORD a2)
    = WSAGetServiceClassNameByClassIdA;

INT (__stdcall * Real_WSAGetServiceClassNameByClassIdW)(LPGUID a0,
                                                        LPWSTR a1,
                                                        LPDWORD a2)
    = WSAGetServiceClassNameByClassIdW;

int (__stdcall * Real_WSAHtonl)(SOCKET a0,
                                u_long a1,
                                u_long* a2)
    = WSAHtonl;

int (__stdcall * Real_WSAHtons)(SOCKET a0,
                                u_short a1,
                                u_short* a2)
    = WSAHtons;

INT (__stdcall * Real_WSAInstallServiceClassA)(LPWSASERVICECLASSINFOA a0)
    = WSAInstallServiceClassA;

INT (__stdcall * Real_WSAInstallServiceClassW)(LPWSASERVICECLASSINFOW a0)
    = WSAInstallServiceClassW;

int (__stdcall * Real_WSAIoctl)(SOCKET a0,
                                DWORD a1,
                                LPVOID a2,
                                DWORD a3,
                                LPVOID a4,
                                DWORD a5,
                                LPDWORD a6,
                                LPWSAOVERLAPPED a7,
                                LPWSAOVERLAPPED_COMPLETION_ROUTINE a8)
    = WSAIoctl;

BOOL (__stdcall * Real_WSAIsBlocking)(void)
    = WSAIsBlocking;

SOCKET (__stdcall * Real_WSAJoinLeaf)(SOCKET a0,
                                      CONST sockaddr* a1,
                                      int a2,
                                      LPWSABUF a3,
                                      LPWSABUF a4,
                                      LPQOS a5,
                                      LPQOS a6,
                                      DWORD a7)
    = WSAJoinLeaf;

INT (__stdcall * Real_WSALookupServiceBeginA)(LPWSAQUERYSETA a0,
                                              DWORD a1,
                                              LPHANDLE a2)
    = WSALookupServiceBeginA;

INT (__stdcall * Real_WSALookupServiceBeginW)(LPWSAQUERYSETW a0,
                                              DWORD a1,
                                              LPHANDLE a2)
    = WSALookupServiceBeginW;

INT (__stdcall * Real_WSALookupServiceEnd)(HANDLE a0)
    = WSALookupServiceEnd;

INT (__stdcall * Real_WSALookupServiceNextA)(HANDLE a0,
                                             DWORD a1,
                                             LPDWORD a2,
                                             LPWSAQUERYSETA a3)
    = WSALookupServiceNextA;

INT (__stdcall * Real_WSALookupServiceNextW)(HANDLE a0,
                                             DWORD a1,
                                             LPDWORD a2,
                                             LPWSAQUERYSETW a3)
    = WSALookupServiceNextW;

int (__stdcall * Real_WSANtohl)(SOCKET a0,
                                u_long a1,
                                u_long* a2)
    = WSANtohl;

int (__stdcall * Real_WSANtohs)(SOCKET a0,
                                u_short a1,
                                u_short* a2)
    = WSANtohs;

INT (__stdcall * Real_WSAProviderConfigChange)(LPHANDLE a0,
                                               LPWSAOVERLAPPED a1,
                                               LPWSAOVERLAPPED_COMPLETION_ROUTINE a2)
    = WSAProviderConfigChange;

int (__stdcall * Real_WSARecv)(SOCKET a0,
                               LPWSABUF a1,
                               DWORD a2,
                               LPDWORD a3,
                               LPDWORD a4,
                               LPWSAOVERLAPPED a5,
                               LPWSAOVERLAPPED_COMPLETION_ROUTINE a6)
    = WSARecv;

int (__stdcall * Real_WSARecvDisconnect)(SOCKET a0,
                                         LPWSABUF a1)
    = WSARecvDisconnect;

int (__stdcall * Real_WSARecvFrom)(SOCKET a0,
                                   LPWSABUF a1,
                                   DWORD a2,
                                   LPDWORD a3,
                                   LPDWORD a4,
                                   sockaddr* a5,
                                   LPINT a6,
                                   LPWSAOVERLAPPED a7,
                                   LPWSAOVERLAPPED_COMPLETION_ROUTINE a8)
    = WSARecvFrom;

INT (__stdcall * Real_WSARemoveServiceClass)(LPGUID a0)
    = WSARemoveServiceClass;

BOOL (__stdcall * Real_WSAResetEvent)(HANDLE a0)
    = WSAResetEvent;

int (__stdcall * Real_WSASend)(SOCKET a0,
                               LPWSABUF a1,
                               DWORD a2,
                               LPDWORD a3,
                               DWORD a4,
                               LPWSAOVERLAPPED a5,
                               LPWSAOVERLAPPED_COMPLETION_ROUTINE a6)
    = WSASend;

int (__stdcall * Real_WSASendDisconnect)(SOCKET a0,
                                         LPWSABUF a1)
    = WSASendDisconnect;

int (__stdcall * Real_WSASendTo)(SOCKET a0,
                                 LPWSABUF a1,
                                 DWORD a2,
                                 LPDWORD a3,
                                 DWORD a4,
                                 CONST sockaddr* a5,
                                 int a6,
                                 LPWSAOVERLAPPED a7,
                                 LPWSAOVERLAPPED_COMPLETION_ROUTINE a8)
    = WSASendTo;

FARPROC (__stdcall * Real_WSASetBlockingHook)(FARPROC a0)
    = WSASetBlockingHook;

BOOL (__stdcall * Real_WSASetEvent)(HANDLE a0)
    = WSASetEvent;

INT (__stdcall * Real_WSASetServiceA)(LPWSAQUERYSETA a0,
                                      WSAESETSERVICEOP a1,
                                      DWORD a2)
    = WSASetServiceA;

INT (__stdcall * Real_WSASetServiceW)(LPWSAQUERYSETW a0,
                                      WSAESETSERVICEOP a1,
                                      DWORD a2)
    = WSASetServiceW;

SOCKET (__stdcall * Real_WSASocketA)(int a0,
                                     int a1,
                                     int a2,
                                     LPWSAPROTOCOL_INFOA a3,
                                     GROUP a4,
                                     DWORD a5)
    = WSASocketA;

SOCKET (__stdcall * Real_WSASocketW)(int a0,
                                     int a1,
                                     int a2,
                                     LPWSAPROTOCOL_INFOW a3,
                                     GROUP a4,
                                     DWORD a5)
    = WSASocketW;

int (__stdcall * Real_WSAStartup)(WORD a0,
                                  LPWSADATA a1)
    = WSAStartup;

INT (__stdcall * Real_WSAStringToAddressA)(LPSTR a0,
                                           INT a1,
                                           LPWSAPROTOCOL_INFOA a2,
                                           LPSOCKADDR a3,
                                           LPINT a4)
    = WSAStringToAddressA;

INT (__stdcall * Real_WSAStringToAddressW)(LPWSTR a0,
                                           INT a1,
                                           LPWSAPROTOCOL_INFOW a2,
                                           LPSOCKADDR a3,
                                           LPINT a4)
    = WSAStringToAddressW;

int (__stdcall * Real_WSAUnhookBlockingHook)(void)
    = WSAUnhookBlockingHook;

DWORD (__stdcall * Real_WSAWaitForMultipleEvents)(DWORD a0,
                                                  CONST HANDLE * a1,
                                                  BOOL a2,
                                                  DWORD a3,
                                                  BOOL a4)
    = WSAWaitForMultipleEvents;

BOOL (__stdcall * Real_WaitCommEvent)(HANDLE a0,
                                      LPDWORD a1,
                                      LPOVERLAPPED a2)
    = WaitCommEvent;

BOOL (__stdcall * Real_WaitForDebugEvent)(LPDEBUG_EVENT a0,
                                          DWORD a1)
    = WaitForDebugEvent;

DWORD (__stdcall * Real_WaitForInputIdle)(HANDLE a0,
                                          DWORD a1)
    = WaitForInputIdle;

DWORD (__stdcall * Real_WaitForMultipleObjects)(DWORD a0,
                                                CONST HANDLE * a1,
                                                BOOL a2,
                                                DWORD a3)
    = WaitForMultipleObjects;

DWORD (__stdcall * Real_WaitForMultipleObjectsEx)(DWORD a0,
                                                  CONST HANDLE * a1,
                                                  BOOL a2,
                                                  DWORD a3,
                                                  BOOL a4)
    = WaitForMultipleObjectsEx;

DWORD (__stdcall * Real_WaitForSingleObject)(HANDLE a0,
                                             DWORD a1)
    = WaitForSingleObject;

DWORD (__stdcall * Real_WaitForSingleObjectEx)(HANDLE a0,
                                               DWORD a1,
                                               BOOL a2)
    = WaitForSingleObjectEx;

BOOL (__stdcall * Real_WaitMessage)(void)
    = WaitMessage;

BOOL (__stdcall * Real_WaitNamedPipeA)(LPCSTR a0,
                                       DWORD a1)
    = WaitNamedPipeA;

BOOL (__stdcall * Real_WaitNamedPipeW)(LPCWSTR a0,
                                       DWORD a1)
    = WaitNamedPipeW;

BOOL (__stdcall * Real_WidenPath)(HDC a0)
    = WidenPath;

UINT (__stdcall * Real_WinExec)(LPCSTR a0,
                                UINT a1)
    = WinExec;

BOOL (__stdcall * Real_WinHelpA)(HWND a0,
                                 LPCSTR a1,
                                 UINT a2,
                                 ULONG_PTR a3)
    = WinHelpA;

BOOL (__stdcall * Real_WinHelpW)(HWND a0,
                                 LPCWSTR a1,
                                 UINT a2,
                                 ULONG_PTR a3)
    = WinHelpW;

HWND (__stdcall * Real_WindowFromDC)(HDC a0)
    = WindowFromDC;

HWND (__stdcall * Real_WindowFromPoint)(POINT a0)
    = WindowFromPoint;

HRESULT (__stdcall * Real_WriteClassStg)(LPSTORAGE a0,
                                         CONST IID& a1)
    = WriteClassStg;

HRESULT (__stdcall * Real_WriteClassStm)(LPSTREAM a0,
                                         CONST IID& a1)
    = WriteClassStm;

BOOL (__stdcall * Real_WriteConsoleA)(HANDLE a0,
                                      CONST void* a1,
                                      DWORD a2,
                                      LPDWORD a3,
                                      LPVOID a4)
    = WriteConsoleA;

BOOL (__stdcall * Real_WriteConsoleInputA)(HANDLE a0,
                                           CONST INPUT_RECORD* a1,
                                           DWORD a2,
                                           LPDWORD a3)
    = WriteConsoleInputA;

BOOL (__stdcall * Real_WriteConsoleInputW)(HANDLE a0,
                                           CONST INPUT_RECORD* a1,
                                           DWORD a2,
                                           LPDWORD a3)
    = WriteConsoleInputW;

BOOL (__stdcall * Real_WriteConsoleOutputA)(HANDLE a0,
                                            CONST CHAR_INFO* a1,
                                            COORD a2,
                                            COORD a3,
                                            PSMALL_RECT a4)
    = WriteConsoleOutputA;

BOOL (__stdcall * Real_WriteConsoleOutputAttribute)(HANDLE a0,
                                                    CONST WORD* a1,
                                                    DWORD a2,
                                                    COORD a3,
                                                    LPDWORD a4)
    = WriteConsoleOutputAttribute;

BOOL (__stdcall * Real_WriteConsoleOutputCharacterA)(HANDLE a0,
                                                     LPCSTR a1,
                                                     DWORD a2,
                                                     COORD a3,
                                                     LPDWORD a4)
    = WriteConsoleOutputCharacterA;

BOOL (__stdcall * Real_WriteConsoleOutputCharacterW)(HANDLE a0,
                                                     LPCWSTR a1,
                                                     DWORD a2,
                                                     COORD a3,
                                                     LPDWORD a4)
    = WriteConsoleOutputCharacterW;

BOOL (__stdcall * Real_WriteConsoleOutputW)(HANDLE a0,
                                            CONST CHAR_INFO* a1,
                                            COORD a2,
                                            COORD a3,
                                            PSMALL_RECT a4)
    = WriteConsoleOutputW;

BOOL (__stdcall * Real_WriteConsoleW)(HANDLE a0,
                                      CONST void* a1,
                                      DWORD a2,
                                      LPDWORD a3,
                                      LPVOID a4)
    = WriteConsoleW;

BOOL (__stdcall * Real_WriteFile)(HANDLE a0,
                                  LPCVOID a1,
                                  DWORD a2,
                                  LPDWORD a3,
                                  LPOVERLAPPED a4)
    = WriteFile;

BOOL (__stdcall * Real_WriteFileEx)(HANDLE a0,
                                    LPCVOID a1,
                                    DWORD a2,
                                    LPOVERLAPPED a3,
                                    LPOVERLAPPED_COMPLETION_ROUTINE a4)
    = WriteFileEx;

HRESULT (__stdcall * Real_WriteFmtUserTypeStg)(LPSTORAGE a0,
                                               CLIPFORMAT a1,
                                               LPOLESTR a2)
    = WriteFmtUserTypeStg;

BOOL (__stdcall * Real_WritePrivateProfileSectionA)(LPCSTR a0,
                                                    LPCSTR a1,
                                                    LPCSTR a2)
    = WritePrivateProfileSectionA;

BOOL (__stdcall * Real_WritePrivateProfileSectionW)(LPCWSTR a0,
                                                    LPCWSTR a1,
                                                    LPCWSTR a2)
    = WritePrivateProfileSectionW;

BOOL (__stdcall * Real_WritePrivateProfileStringA)(LPCSTR a0,
                                                   LPCSTR a1,
                                                   LPCSTR a2,
                                                   LPCSTR a3)
    = WritePrivateProfileStringA;

BOOL (__stdcall * Real_WritePrivateProfileStringW)(LPCWSTR a0,
                                                   LPCWSTR a1,
                                                   LPCWSTR a2,
                                                   LPCWSTR a3)
    = WritePrivateProfileStringW;

BOOL (__stdcall * Real_WritePrivateProfileStructA)(LPCSTR a0,
                                                   LPCSTR a1,
                                                   LPVOID a2,
                                                   UINT a3,
                                                   LPCSTR a4)
    = WritePrivateProfileStructA;

BOOL (__stdcall * Real_WritePrivateProfileStructW)(LPCWSTR a0,
                                                   LPCWSTR a1,
                                                   LPVOID a2,
                                                   UINT a3,
                                                   LPCWSTR a4)
    = WritePrivateProfileStructW;

#if _MSC_VER < 1300
BOOL (__stdcall * Real_WriteProcessMemory)(HANDLE a0,
                                           LPVOID a1,
                                           LPVOID a2,
                                           DWORD_PTR a3,
                                           PDWORD_PTR a4)
    = WriteProcessMemory;
#else
BOOL (__stdcall * Real_WriteProcessMemory)(HANDLE a0,
                                           LPVOID a1,
                                           LPCVOID a2,
                                           DWORD_PTR a3,
                                           PDWORD_PTR a4)
    = WriteProcessMemory;
#endif

BOOL (__stdcall * Real_WriteProfileSectionA)(LPCSTR a0,
                                             LPCSTR a1)
    = WriteProfileSectionA;

BOOL (__stdcall * Real_WriteProfileSectionW)(LPCWSTR a0,
                                             LPCWSTR a1)
    = WriteProfileSectionW;

BOOL (__stdcall * Real_WriteProfileStringA)(LPCSTR a0,
                                            LPCSTR a1,
                                            LPCSTR a2)
    = WriteProfileStringA;

BOOL (__stdcall * Real_WriteProfileStringW)(LPCWSTR a0,
                                            LPCWSTR a1,
                                            LPCWSTR a2)
    = WriteProfileStringW;

DWORD (__stdcall * Real_WriteTapemark)(HANDLE a0,
                                       DWORD a1,
                                       DWORD a2,
                                       BOOL a3)
    = WriteTapemark;

int (__stdcall * Real___WSAFDIsSet)(SOCKET a0,
                                    fd_set* a1)
    = __WSAFDIsSet;

long (__stdcall * Real__hread)(HFILE a0,
                               LPVOID a1,
                               long a2)
    = _hread;

long (__stdcall * Real__hwrite)(HFILE a0,
                                LPCSTR a1,
                                long a2)
    = _hwrite;

HFILE (__stdcall * Real__lclose)(HFILE a0)
    = _lclose;

HFILE (__stdcall * Real__lcreat)(LPCSTR a0,
                                 int a1)
    = _lcreat;

LONG (__stdcall * Real__llseek)(HFILE a0,
                                LONG a1,
                                int a2)
    = _llseek;

HFILE (__stdcall * Real__lopen)(LPCSTR a0,
                                int a1)
    = _lopen;

UINT (__stdcall * Real__lread)(HFILE a0,
                               LPVOID a1,
                               UINT a2)
    = _lread;

UINT (__stdcall * Real__lwrite)(HFILE a0,
                                LPCSTR a1,
                                UINT a2)
    = _lwrite;

SOCKET (__stdcall * Real_accept)(SOCKET a0,
                                 sockaddr* a1,
                                 int* a2)
    = accept;

int (__stdcall * Real_bind)(SOCKET a0,
                            CONST sockaddr* a1,
                            int a2)
    = bind;

int (__stdcall * Real_closesocket)(SOCKET a0)
    = closesocket;

int (__stdcall * Real_connect)(SOCKET a0,
                               CONST sockaddr* a1,
                               int a2)
    = connect;

hostent * (__stdcall * Real_gethostbyaddr)(CONST char* a0,
                                          int a1,
                                          int a2)
    = gethostbyaddr;

hostent * (__stdcall * Real_gethostbyname)(CONST char* a0)
    = gethostbyname;

int (__stdcall * Real_gethostname)(char* a0,
                                   int a1)
    = gethostname;

int (__stdcall * Real_getpeername)(SOCKET a0,
                                   sockaddr* a1,
                                   int* a2)
    = getpeername;

protoent * (__stdcall * Real_getprotobyname)(CONST char* a0)
    = getprotobyname;

protoent * (__stdcall * Real_getprotobynumber)(int a0)
    = getprotobynumber;

servent * (__stdcall * Real_getservbyname)(CONST char* a0,
                                           CONST char* a1)
    = getservbyname;

servent * (__stdcall * Real_getservbyport)(int a0,
                                           CONST char* a1)
    = getservbyport;

int (__stdcall * Real_getsockname)(SOCKET a0,
                                   sockaddr* a1,
                                   int* a2)
    = getsockname;

int (__stdcall * Real_getsockopt)(SOCKET a0,
                                  int a1,
                                  int a2,
                                  char* a3,
                                  int* a4)
    = getsockopt;

u_long (__stdcall * Real_htonl)(u_long a0)
    = htonl;

u_short (__stdcall * Real_htons)(u_short a0)
    = htons;

unsigned long (__stdcall * Real_inet_addr)(CONST char* a0)
    = inet_addr;

char * (__stdcall * Real_inet_ntoa)(in_addr a0)
    = inet_ntoa;

int (__stdcall * Real_ioctlsocket)(SOCKET a0,
                                   long a1,
                                   u_long* a2)
    = ioctlsocket;

void (__stdcall * Real_keybd_event)(BYTE a0,
                                    BYTE a1,
                                    DWORD a2,
                                    ULONG_PTR a3)
    = keybd_event;

int (__stdcall * Real_listen)(SOCKET a0,
                              int a1)
    = listen;

void (__stdcall * Real_mouse_event)(DWORD a0,
                                    DWORD a1,
                                    DWORD a2,
                                    DWORD a3,
                                    ULONG_PTR a4)
    = mouse_event;

u_long (__stdcall * Real_ntohl)(u_long a0)
    = ntohl;

u_short (__stdcall * Real_ntohs)(u_short a0)
    = ntohs;

int (__stdcall * Real_recv)(SOCKET a0,
                            char* a1,
                            int a2,
                            int a3)
    = recv;

int (__stdcall * Real_recvfrom)(SOCKET a0,
                                char* a1,
                                int a2,
                                int a3,
                                sockaddr* a4,
                                int* a5)
    = recvfrom;

int (__stdcall * Real_select)(int a0,
                              fd_set* a1,
                              fd_set* a2,
                              fd_set* a3,
                              CONST timeval* a4)
    = select;

int (__stdcall * Real_send)(SOCKET a0,
                            CONST char* a1,
                            int a2,
                            int a3)
    = send;

int (__stdcall * Real_sendto)(SOCKET a0,
                              CONST char* a1,
                              int a2,
                              int a3,
                              CONST sockaddr* a4,
                              int a5)
    = sendto;

int (__stdcall * Real_setsockopt)(SOCKET a0,
                                  int a1,
                                  int a2,
                                  CONST char* a3,
                                  int a4)
    = setsockopt;

int (__stdcall * Real_shutdown)(SOCKET a0,
                                int a1)
    = shutdown;

SOCKET (__stdcall * Real_socket)(int a0,
                                 int a1,
                                 int a2)
    = socket;

#endif // !WIN32DEF_HPP
