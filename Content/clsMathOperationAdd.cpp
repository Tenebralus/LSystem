#pragma once
#include "stdafx.h"
#include "clsMathOperationAdd.h"

namespace GenerationLib
{
	clsMathOperationAdd::clsMathOperationAdd( float* a_Value, clsMathOperation* a_Next ) : clsMathOperation( a_Value, a_Next )
	{

	}

	clsMathOperationAdd::clsMathOperationAdd( ) : clsMathOperation( )
	{

	}

	clsMathOperationAdd::~clsMathOperationAdd( )
	{

	}

	void clsMathOperationAdd::Calculate( )
	{
		if (m_Left != nullptr && m_Left->m_PValue != nullptr && m_Right != nullptr && m_Right->m_PValue != nullptr)
		{
			m_NValue = *m_Left->m_PValue + *m_Right->m_PValue;
			m_PValue = &m_NValue;
		}
	}
}
