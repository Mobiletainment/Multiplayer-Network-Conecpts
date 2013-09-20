#include "MciPlayer.h"

#define WIN_CLASS_NAME        L"CocosDenshionCallbackWnd"
#define BREAK_IF(cond)      if (cond) break;

namespace CocosDenshion {

static HINSTANCE s_hInstance;
static MCIERROR  s_mciError;

static LRESULT WINAPI _SoundPlayProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

MciPlayer::MciPlayer()
: m_hWnd(NULL)
, m_hDev(NULL)
, m_nSoundID(0)
, m_uTimes(0)
, m_bPlaying(false)
{
    if (! s_hInstance)
    {
        s_hInstance = GetModuleHandle( NULL );            // Grab An Instance For Our Window

        WNDCLASS  wc;        // Windows Class Structure

        // Redraw On Size, And Own DC For Window.
        wc.style          = 0;  
        wc.lpfnWndProc    = _SoundPlayProc;                    // WndProc Handles Messages
        wc.cbClsExtra     = 0;                              // No Extra Window Data
        wc.cbWndExtra     = 0;                                // No Extra Window Data
        wc.hInstance      = s_hInstance;                    // Set The Instance
        wc.hIcon          = 0;                                // Load The Default Icon
        wc.hCursor        = LoadCursor( NULL, IDC_ARROW );    // Load The Arrow Pointer
        wc.hbrBackground  = NULL;                           // No Background Required For GL
        wc.lpszMenuName   = NULL;                           // We Don't Want A Menu
        wc.lpszClassName  = WIN_CLASS_NAME;                 // Set The Class Name

        if (! RegisterClass(&wc)
            && 1410 != GetLastError())
        {
            return;
        }
    }

    m_hWnd = CreateWindowEx(
        WS_EX_APPWINDOW,                                    // Extended Style For The Window
        WIN_CLASS_NAME,                                        // Class Name
        NULL,                                        // Window Title
        WS_POPUPWINDOW,/*WS_OVERLAPPEDWINDOW*/               // Defined Window Style
        0, 0,                                                // Window Position
        0,                                                    // Window Width
        0,                                                    // Window Height
        NULL,                                                // No Parent Window
        NULL,                                                // No Menu
        s_hInstance,                                        // Instance
        NULL );
    if (m_hWnd)
    {
        SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
    }
}

MciPlayer::~MciPlayer()
{
    Close();
    DestroyWindow(m_hWnd);
}

void MciPlayer::Open(const char* pFileName, UINT uId)
{
#if (CC_ENABLE_AUDIO_INTEGRATION == 1)
	int len = strlen(pFileName);
	wchar_t uFileName[MAX_PATH];
	// Converts the path to wide characters
	int needed = MultiByteToWideChar(0, 0, pFileName, len + 1, uFileName, len + 1);
//     WCHAR * pBuf = NULL;
    do 
    {
        BREAK_IF(! pFileName || ! m_hWnd);
        int nLen = (int)strlen(pFileName);
        BREAK_IF(! nLen);
//         pBuf = new WCHAR[nLen + 1];
//         BREAK_IF(! pBuf);
//         MultiByteToWideChar(CP_ACP, 0, pFileName, nLen + 1, pBuf, nLen + 1);

        Close();

        MCI_OPEN_PARMS mciOpen = {0};
        MCIERROR mciError;
        mciOpen.lpstrDeviceType = (LPCTSTR)MCI_ALL_DEVICE_ID;
        mciOpen.lpstrElementName = uFileName;

        mciError = mciSendCommand(0,MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&mciOpen);
        BREAK_IF(mciError);

        m_hDev = mciOpen.wDeviceID;
        m_nSoundID = uId;
        m_bPlaying = false;
    } while (0);
#endif // (CC_ENABLE_AUDIO_INTEGRATION == 1)
}

void MciPlayer::Play(UINT uTimes /* = 1 */)
{
#if (CC_ENABLE_AUDIO_INTEGRATION == 1)
    if (! m_hDev)
    {
        return;
    }
    MCI_PLAY_PARMS mciPlay = {0};
    mciPlay.dwCallback = (DWORD_PTR)m_hWnd;
    s_mciError = mciSendCommand(m_hDev,MCI_PLAY, MCI_FROM|MCI_NOTIFY,(DWORD)&mciPlay);
    if (! s_mciError)
    {
        m_bPlaying = true;
        m_uTimes = uTimes;
    }
#endif // (CC_ENABLE_AUDIO_INTEGRATION == 1)
}

void MciPlayer::Close()
{
    if (m_bPlaying)
    {
        Stop();
    }
    if (m_hDev)
    {
         _SendGenericCommand(MCI_CLOSE);
    }
    m_hDev = 0;
    m_bPlaying = false;
}

void MciPlayer::Pause()
{
    _SendGenericCommand(MCI_PAUSE);
}

void MciPlayer::Resume()
{
    _SendGenericCommand(MCI_RESUME);
}

void MciPlayer::Stop()
{
    _SendGenericCommand(MCI_STOP);
    m_bPlaying = false;
}

void MciPlayer::Rewind()
{
#if (CC_ENABLE_AUDIO_INTEGRATION == 1)

    if (! m_hDev)
    {
        return;
    }
    mciSendCommand(m_hDev, MCI_SEEK, MCI_SEEK_TO_START, 0);

    MCI_PLAY_PARMS mciPlay = {0};
    mciPlay.dwCallback = (DWORD)m_hWnd;
    m_bPlaying = mciSendCommand(m_hDev, MCI_PLAY, MCI_NOTIFY,(DWORD)&mciPlay) ? false : true;
#endif // (CC_ENABLE_AUDIO_INTEGRATION == 1)
}

bool MciPlayer::IsPlaying()
{
    return m_bPlaying;
}

UINT MciPlayer::GetSoundID()
{
    return m_nSoundID;
}

//////////////////////////////////////////////////////////////////////////
// private member
//////////////////////////////////////////////////////////////////////////

void MciPlayer::_SendGenericCommand(int nCommand)
{
#if (CC_ENABLE_AUDIO_INTEGRATION == 1)
    if (! m_hDev)
    {
        return;
    }
    mciSendCommand(m_hDev, nCommand, 0, 0);
#endif
}

//////////////////////////////////////////////////////////////////////////
// static function
//////////////////////////////////////////////////////////////////////////

LRESULT WINAPI _SoundPlayProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    MciPlayer * pPlayer = NULL;
    if (MM_MCINOTIFY == Msg 
        && MCI_NOTIFY_SUCCESSFUL == wParam
        &&(pPlayer = (MciPlayer *)GetWindowLong(hWnd, GWL_USERDATA)))
    {
        if (pPlayer->m_uTimes)
        {
            --pPlayer->m_uTimes;
        }

        if (pPlayer->m_uTimes)
        {
#if (CC_ENABLE_AUDIO_INTEGRATION == 1)
            mciSendCommand(lParam, MCI_SEEK, MCI_SEEK_TO_START, 0);

            MCI_PLAY_PARMS mciPlay = {0};
            mciPlay.dwCallback = (DWORD)hWnd;
            mciSendCommand(lParam, MCI_PLAY, MCI_NOTIFY,(DWORD)&mciPlay);
#endif // (CC_ENABLE_AUDIO_INTEGRATION == 1)

        }
        else
        {
            pPlayer->m_bPlaying = false;
        }
        return 0;
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}

} // end of namespace CocosDenshion
