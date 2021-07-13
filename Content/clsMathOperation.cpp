#pragma once
#include "stdafx.h"
#include "clsMathOperation.h"

namespace GenerationLib
{
	clsMathOperation::clsMathOperation( float* a_Value, clsMathOperation* a_Next )
	{
		m_PValue = a_Value;
		m_Next = a_Next;

		m_Next = nullptr;
		m_Left = nullptr;
		m_Right = nullptr;
		m_Previous = nullptr;
	}

	clsMathOperation::clsMathOperation( )
	{

	}

	clsMathOperation::~clsMathOperation( )
	{

	}

	void clsMathOperation::Destroy( )
	{

	}

	float clsMathOperation::DoCalculate( )
	{
		Calculate( );
		return 0;
	}
}
