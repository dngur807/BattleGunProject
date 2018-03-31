#ifndef EffectNormal_h__
#define EffectNormal_h__

#include "EffectBasic.h"

BEGIN(Engine)

class ENGINE_DLL CEffectNormal : public CEffectBasic
{
public:
	explicit CEffectNormal(MYGDI* pMyGDI, const std::wstring& wstrFileName);
	explicit CEffectNormal(const CEffectNormal& rhs);
	virtual ~CEffectNormal(void);

	virtual HRESULT Ready_Effect(void);

public:
	static CEffectNormal* Create(MYGDI*  pMyGDI, const wstring & wstrFileName);
	virtual CEffectBasic* Clone(void);
public:
	DWORD Release(void);

};

END
#endif