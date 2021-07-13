#pragma once
#include "stdafx.h"
#include "clsMathOperationValue.h"

namespace GenerationLib
{
	clsMathOperationValue::clsMathOperationValue( float* a_Value, clsMathOperation* a_Next ) : clsMathOperation( a_Value, a_Next )
	{

	}

	clsMathOperationValue::clsMathOperationValue( ) : clsMathOperation( )
	{

	}

	clsMathOperationValue::~clsMathOperationValue( )
	{

	}

	void clsMathOperationValue::Calculate( )
	{

	}
}
