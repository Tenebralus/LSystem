#pragma once
#include "stdafx.h"

namespace GenerationLib
{
	class clsLSystem
	{
	public:
		clsLSystem( );
		~clsLSystem( );
		void Destroy( );
		std::vector<clsPoint> GetJoints( );
		void SetupLSystem( );
		void SetAngle( float Angle );
		void SetAmountOfIterations( int AmountOfIterations );
		void SetAxiom( std::string Axiom );
		void SetDefine( char Name, float Value );
		void SetRule( char Name, std::string Successor, std::string Parameters = { } );
	private:
		void CalculateIteration( );
		void TranslateStringToTurtle( );
		void ConvertRuleToObject( char RuleName, std::string Parameters, std::string Successor );
		void ConvertSuccessorCommandToObject( clsLSystemPredecessorCommand* CurrPredecessorCommand, std::vector<clsMathOperation*>* MathOperationsParameter, std::map<char, int>* ParameterMapFromPredecessor, std::string Command, enumLSystemCommands::enumLSystemCommands CommandType );
		void ConvertAxiomToObjects( std::string Axiom );
		void ConvertAxiomCommandToObject( std::string Command, enumLSystemCommands::enumLSystemCommands CommandType );
		void ParseCharToPriority( char Char, char PreviousChar, int *r_CurrParenthesesPriority, std::string *r_PriorityString );
		void OrderPriorityString( clsLSystemPredecessorCommand* CurrPredecessorCommand, std::vector<clsMathOperation*>* MathOperationsParameter, std::map<char, int>* ParameterMapFromPredecessor, clsLSystemSuccessorCommand* a_CurrSuccessorCommand, std::string *r_CommandString, std::string *r_PriorityString );
		void OrderRecursive( clsLSystemPredecessorCommand* CurrPredecessorCommand, std::vector<clsMathOperation*>* MathOperationsParameter, std::map<char, int>* ParameterMapFromPredecessor, std::string CommandString, std::string PriorityString, int DepthLevel, int CommandIndex, clsMathOperation* &r_MathOperation, clsMathOperation* PrevMathOperation );
		void SetMathOperationPointerValue( clsLSystemPredecessorCommand* CurrPredecessorCommand, std::map<char, int>* ParameterMapFromPredecessor, char ParamName, char PosNegSign, clsMathOperation* MathOperation );
		void SetPreviousMathOperation( clsMathOperation** CurrMathOperation, clsMathOperation** PrevMathOperation );
		void MoveForward( clsPoint& r_Point, glm::vec3 Distance, float Width );
		void SetYaw( std::vector<float> Parameters, int PosNegValue ); // PosNegValue: 1 = Pos, -1 = Neg
		void SetPitch( std::vector<float> Parameters, int PosNegValue ); // PosNegValue: 1 = Pos, -1 = Neg
		void SetRoll( std::vector<float> Parameters, int PosNegValue ); // PosNegValue: 1 = Pos, -1 = Neg
		void SetBranchingPoint( std::vector<clsPoint>& VisitedBranchingPoints );
		void SetReturnPoint( std::vector<clsPoint>& r_VisitedBranchingPoints, clsPoint& r_Point, glm::quat PrevBranchingPointRot, glm::vec3 PrevBranchingPointPos );
		void SetWidth( std::vector<float> Parameters, float &r_Width );
		void SetTurtleRightVectorToHorizontal( );
	public:

	private:
		int m_AmountOfIterations;
		std::string m_Axiom;
		float m_Angle;
		OpenGLLib::clsObject* m_Turtle;
		std::vector<clsPoint> m_TreePoints;
		std::map<std::pair<char, std::string>, std::string> m_UnparsedRules;
		std::vector<clsLSystemPredecessorCommand> m_Rules;
		std::map<char, float> m_Defines;
		std::vector<clsLSystemSuccessorCommand> m_Result;
		std::map<char, std::vector<float>> m_ParameterMapFromAxiom;
		clsMathOperation** m_MathOperation;
	};
}
