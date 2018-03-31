#ifndef Engine_Macro_h__
#define Engine_Macro_h__

#define MSG_BOX(_message)	MessageBox(NULL, _message, L"System Message", MB_OK)
#define MSGBOX(_message)	MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#define USING(NAMESPACE) using namespace NAMESPACE;
#define TAGMSG_BOX(_tag,_message)	MessageBox(NULL, _message, _tag, MB_OK)

#define NULL_CHECK(_ptr)		{if(_ptr == 0){__asm { int 3}; return;}}

#define NULL_CHECK_RETURN(_ptr, _return) {if(_ptr == 0){__asm{int 3}; return _return;}}

#define NULL_CHECK_MSG(_ptr, _message) {if(_ptr == 0){MessageBox(NULL, _message, L"System Message", MB_OK);__asm { int 3 }; }}

#define NULL_CHECK_RETURN_MSG(_ptr, _return, _message) \
{if(_ptr == 0){MessageBox(NULL, _message, L"System Message", MB_OK); __asm{int 3}; return _return;}}

#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0) \
{MessageBox(NULL, L"Failed", L"System Error", MB_OK); __asm {int 3}; return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )	\
{MessageBox(NULL, L"Failed", L"System Error", MB_OK); __asm {int 3}; return _return;}

#define FAILED_CHECK_MSG(_hr, _message) if( ((HRESULT)(_hr)) < 0)	\
{MessageBox(NULL, _message, L"System_Message", MB_OK); __asm {int 3}; return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG(_hr, _return, _message)	if( ((HRESULT)(_hr)) < 0)	\
{MessageBox(NULL, _message, L"System_Message", MB_OK); __asm {int 3}; return _return;}

#define BEGIN(NAMESPACE) namespace NAMESPACE {
#define END }

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif


#define NO_COPY(CLASSNAME)							\
	private:										\
	CLASSNAME(const CLASSNAME&);					\
	CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)				\
	NO_COPY(CLASSNAME)								\
	private:										\
	static CLASSNAME*	m_pInstance;				\
	public:											\
	static CLASSNAME*	GetInstance( void );		\
	static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
		if(NULL == m_pInstance) {					\
			m_pInstance = new CLASSNAME;			\
		}											\
		return m_pInstance;							\
	}												\
	void CLASSNAME::DestroyInstance( void ) {		\
		if(NULL != m_pInstance)	{					\
			delete m_pInstance;						\
			m_pInstance = NULL;						\
		}											\
	}



//---------------------------------------------------------------------------------------
// Simple d3d error checker for book demos.
//---------------------------------------------------------------------------------------
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)													    \
	{																\
		HRESULT hr = (x);											\
		if(FAILED(hr))												\
		{															\
			LPWSTR output;                                          \
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |				\
						FORMAT_MESSAGE_IGNORE_INSERTS |				\
						FORMAT_MESSAGE_ALLOCATE_BUFFER,				\
						NULL,										\
						hr,											\
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  \
						(LPTSTR) &output,							\
						0,											\
						NULL);										\
			MessageBox(NULL, output, L"Error", MB_OK);				\
		}															\
	}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 



#ifndef D3DX11INLINE
#ifdef _MSC_VER
#if (_MSC_VER >= 1200)
#define D3DX11INLINE __forceinline
#else
#define D3DX11INLINE __inline
#endif
#else
#ifdef __cplusplus
#define D3DX11INLINE inline
#else
#define D3DX11INLINE
#endif
#endif
#endif


//---------------------------------------------------------------------------------------
// Convenience macro for releasing COM objects.
//---------------------------------------------------------------------------------------

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

//---------------------------------------------------------------------------------------
// Convenience macro for deleting objects.
//---------------------------------------------------------------------------------------

#define SafeDelete(x) { delete x; x = 0; }


#endif // !Engine_Macro_h__
