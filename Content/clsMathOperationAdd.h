#pragma once
#include "stdafx.h"
#include "clsMathOperation.h"

namespace GenerationLib
{
	class clsMathOperationAdd : public clsMathOperation
	{
	public:
		clsMathOperationAdd( float* Value, clsMathOperation* Next );
		clsMathOperationAdd( );
		~clsMathOperationAdd( );
		void Calculate( );
	private:

	public:

	private:

	};
}
