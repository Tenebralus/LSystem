#pragma once
#include "stdafx.h"
#include "clsMathOperationDivide.h"

namespace GenerationLib
{
	clsMathOperationDivide::clsMathOperationDivide( float* a_Value, clsMathOperation* a_Next ) : clsMathOperation( a_Value, a_Next )
	{

	}

	clsMathOperationDivide::clsMathOperationDivide( ) : clsMathOperation( )
	{

	}

	clsMathOperationDivide::~clsMathOperationDivide( )
	{

	}

	void clsMathOperationDivide::Calculate( )
	{
		if (m_Left != nullptr && m_Left->m_PValue != nullptr && m_Right != nullptr && m_Right->m_PValue != nullptr)
		{
			m_NValue = *m_Left->m_PValue / *m_Right->m_PValue;
			m_PValue = &m_NValue;
		}
	}
}
