#pragma once
#include "stdafx.h"
#include "clsMathOperation.h"

namespace GenerationLib
{
	class clsMathOperationSubtract : public clsMathOperation
	{
	public:
		clsMathOperationSubtract( float* Value, clsMathOperation* Next );
		clsMathOperationSubtract( );
		~clsMathOperationSubtract( );
		void Calculate( );
	private:

	public:

	private:
	};
}
