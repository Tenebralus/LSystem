#pragma once
#include "stdafx.h"
#include "clsMathOperation.h"

namespace GenerationLib
{
	class clsMathOperationDivide : public clsMathOperation
	{
	public:
		clsMathOperationDivide( float* Value, clsMathOperation* Next );
		clsMathOperationDivide( );
		~clsMathOperationDivide( );
		void Calculate( );
	private:

	public:

	private:

	};
}
