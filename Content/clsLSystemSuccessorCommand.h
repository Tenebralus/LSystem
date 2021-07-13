#pragma once
#include "stdafx.h"
#include "clsLSystemCommand.h"

namespace GenerationLib
{
	class clsLSystemSuccessorCommand : public clsLSystemCommand
	{
	public:
		clsLSystemSuccessorCommand( );
		~clsLSystemSuccessorCommand( );
		void Destroy( );
		void AddSuccessorParameter( float Parameter );
		void AddAxiomParameter( float Parameter );
		void SetAndSortCurrentMathOperationsParameter( std::vector<clsMathOperation*>* MathOperationsParameter );
		std::vector<float> GetSuccessorParameters( );
		std::vector<float> GetAxiomParameters( );
		std::vector<std::vector<clsMathOperation*>> GetMathOperationsParameters( );
		void SetAxiomParameters( std::vector<float> Parameters );
		void SetSuccessorParameterAt( int Index, float Parameter );
		void Calculate( );
		void SetRoutine( char Routine );
		char GetRoutine( );
	protected:

	public:

	protected:
		std::vector<float> m_SuccessorParams;
		std::vector<float> m_AxiomParams;
		std::vector<std::vector<clsMathOperation*>> m_MathOperationsParameters;
		char m_Routine;
	};
}
