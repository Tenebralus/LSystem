#pragma once
#include "stdafx.h"
#include "clsMathOperation.h"

namespace GenerationLib
{
	class clsMathOperationValue : public clsMathOperation
	{
	public:
		clsMathOperationValue( float* Value, clsMathOperation* Next );
		clsMathOperationValue( );
		~clsMathOperationValue( );
		void Calculate( );
	private:

	public:

	private:

	};
}
