#pragma once
#include "stdafx.h"
#include "clsLSystemCommand.h"

namespace GenerationLib
{
	class clsMathOperation
	{
	public:
		clsMathOperation( float* Value, clsMathOperation* Next );
		clsMathOperation( );
		~clsMathOperation( );
		void Destroy( );

		virtual void Calculate( ) = 0;
	protected:
		float DoCalculate( );
	private:

	public:
		clsMathOperation * m_Next;
		clsMathOperation* m_Left;
		clsMathOperation* m_Right;
		clsMathOperation* m_Previous;
		int m_RecursiveDepth;
		float* m_PValue;

	protected:
		float m_NValue;

	};
}
