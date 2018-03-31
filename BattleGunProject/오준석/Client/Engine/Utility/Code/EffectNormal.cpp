#include "EffectNormal.h"

USING(Engine)

CEffectNormal::CEffectNormal(MYGDI * pMyGDI, const wstring & wstrFileName)
	:CEffectBasic(pMyGDI, wstrFileName)
{

}
CEffectNormal::CEffectNormal(const CEffectNormal& rhs)
	: Engine::CEffectBasic(rhs)
{
}

CEffectNormal::~CEffectNormal(void)
{
}


HRESULT CEffectNormal::Ready_Effect(void)
{
	m_fxTech = m_pFX->GetTechniqueByName("Default");

	return NOERROR;
}

CEffectNormal * CEffectNormal::Create(MYGDI * pMyGDI, const wstring & wstrFileName)
{
	CEffectNormal*		pInstance = new CEffectNormal(pMyGDI, wstrFileName);
	if (FAILED(pInstance->Ready_Effect()))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

CEffectBasic * CEffectNormal::Clone(void)
{
	return new CEffectNormal(*this);
}

DWORD Engine::CEffectNormal::Release(void)
{
	return 0;
}

