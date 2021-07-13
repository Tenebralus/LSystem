#pragma once
#include "stdafx.h"
#include "clsMathOperation.h"

namespace GenerationLib
{
	class clsMathOperationMultiply : public clsMathOperation
	{
	public:
		clsMathOperationMultiply( float* Value, clsMathOperation* Next );
		clsMathOperationMultiply( );
		~clsMathOperationMultiply( );
		void Calculate( );
	private:

	public:

	private:
	};
}
