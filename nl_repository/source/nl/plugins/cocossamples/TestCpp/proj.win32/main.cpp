#include "main.h"
#include "../Classes/AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("Cocos Samples");
    eglView->setFrameSize(480, 320);
    return CCApplication::sharedApplication()->run();
}

#define NL_DLL
#define nlCocosUI_EXPORTS

#if (defined(_WIN32) || defined(_WIN32_WCE)) && defined(NL_DLL)
#if defined(nlCocosUI_EXPORTS)
#define nlCocosUI_API __declspec(dllexport)
#else
#define nlCocosUI_API __declspec(dllimport)	
#endif
#endif


#if !defined(nlCocosUI_API)
#if !defined(NL_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
#define nlCocosUI_API __attribute__ ((visibility ("default")))
#else
#define nlCocosUI_API
#endif
#endif


//-----------------------------------------------------------------------------
// c function declaration define
// define __cdecl for non-Microsoft compilers
#if ( !defined(_MSC_VER) && !defined(__cdecl) )
#  define __cdecl
#endif

#define SL_CRTDECL __cdecl


#if defined(nlCocosUI_EXPORTS)
extern "C" 
{
	nlCocosUI_API void SL_CRTDECL launch();
}
#endif


nlCocosUI_API void SL_CRTDECL launch()	{
	AppDelegate app;
	CCEGLView* eglView = CCEGLView::sharedOpenGLView();
	eglView->setViewName("Cocos Samples");
	eglView->setFrameSize(480 * 2, 320 * 2);
	int returnCode = CCApplication::sharedApplication()->run();
}
