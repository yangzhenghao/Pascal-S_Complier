//	File Name	: Syntax_Tree.h
//	Notes		: Define the Syntax Tree DataStructure
//	Version		: 0.6
//	Author		: Yun Zhang
//				: Boxi Cao

#pragma once

#include <vector>
#include <string>
using namespace std;

// class declare
class Programstruct;
class Program_Body;
class Const_Declarations;
class Var_Declarations;
class SubProgram_Declarations;
class Statement_List;
class Common;
class Procedure;
class Function;
class Statement;
class Parameter_List;
class Variable;
class Procedure_Call;
class Function_Call;
class Expression;
class Simple_Expression;
class Term;
class Factor;
class Not;
class Uminus;
class Type;
class Const_Value;
class Assignop;
class If_Then_Else;
class For;
class Parameter;
class Relop;
class Addop;
class Mulop;
class Id;
class Id_List;
class Period;
class Expression_List;

//�����඼���ڳ����﷨����û�е��࣬Ϊʵ�ַ��뷽��������
class Program_Head;
class Compound_Statement;
class Const_Declaration;
class Var_Declaration;
class SubProgram_Declaration;
class Subprogram;
class Subprogram_Head;
class Subprogram_Body;
class Formal_Parameter;
class Var_Parameter;
class Value_Parameter;


// class define

//	Program Total node
//	The Root of Syntax Tree
class Programstruct {
public:
	Programstruct();
	~Programstruct();

	string func_codeGeneration();

private:
	Id * mp_Id;
	Id_List			*mp_Id_List;
	Program_Body	*mp_Program_Body;
};

//	Define the Main Program body
//	include four parts
class Program_Body {
public:
	Program_Body();
	~Program_Body();

	string func_codeGeneration();

private:
	Const_Declarations * mp_Const_Declarations;
	Var_Declarations		*mp_Var_Declarations;
	SubProgram_Declarations	*mp_SubProgram_Declarations;
	Statement_List			*mp_Statement_List;
};

//	Const Block
class Const_Declarations {
public:
	Const_Declarations();
	~Const_Declarations();

	string func_codeGeneration();

private:
	vector<pair<Id*, Const_Value*>>mv_Const;
};

//	Var Block
class Var_Declarations {
public:
	Var_Declarations();
	~Var_Declarations();

	string func_codeGeneration();

private:
	vector<pair<Id_List*, Type*>>mv_Var;
};

//	SubProgram Block
//	Define the functions and procedures
class SubProgram_Declarations {
public:
	SubProgram_Declarations();
	~SubProgram_Declarations();

	string	func_codeGeneration();
	void	func_checkType();

private:
	vector<Common*>mv_Common;

};

//	contain many statements
class Statement_List {
public:
	Statement_List();
	~Statement_List();

	string func_codeGeneration();

private:

	vector<Statement*>mv_Statement;
};

class Common {
public:
	Common();
	virtual ~Common() = 0;

	virtual string func_codeGeneration() = 0;
	virtual int func_get_Common_Type() {
		return -1;
	}

private:

};

//	the Precodure block
class Procedure : public Common {
public:
	Procedure();
	~Procedure();

	string	func_codeGeneration();
	int		func_get_Common_Type() {
		return COMMON_PROCEDURE;
	}


private:
	int m_lineno;

	Id					*mp_Id;
	Parameter_List		*mp_Parameter_List;
	Const_Declarations	*mp_Const_Declarations;
	Var_Declarations	*mp_Var_Declarations;
	Statement_List		*mp_Statement_List;
};

//	the Function block
class Function : public Common {
public:
	Function();
	~Function();

	string	func_codeGeneration();
	int 	func_checkReturnType() {
		return m_returnType;
	}
	int		func_get_Common_Type() {
		return COMMON_FUNCTION;
	}


private:
	int m_returnType;
	int m_lineno;

	Id					*mp_Id;
	Parameter_List		*mp_Parameter_List;
	Const_Declarations	*mp_Const_Declarations;
	Var_Declarations	*mp_Var_Declarations;
	Statement_List		*mp_Statement_List;
};

//	the statements are partitioned by ';'
class Statement {
public:
	Statement();
	~Statement();

	string	func_codeGeneration();
	void	func_checkType();



private:
	int m_stateType;
	int m_lineno;

	Assignop		*mp_Assignop;
	Procedure_Call	*mp_Procedure_call;		// which include the read and write call
	Statement_List	*mp_Statement_List;		// which refered to begin-block-end
	If_Then_Else	*mp_If_Then_Else;
	For				*mp_For;
};

// only used for function or procedure define
class Parameter_List {
public:
	Parameter_List();
	~Parameter_List();

	string func_codeGeneration();

private:
	int m_lineno;

	vector<Parameter*>mv_Patameter;
};

//	define the variable type
class Variable {
public:
	Variable();
	~Variable();

	string func_codeGeneration();



private:
	bool	m_isArray;
	int		m_lineno;

	Id				*mp_Id;

	//	default to be null
	//
	//	if m_isArray == true
	//	this pointer point to the index of array(multi-dimension)
	//		partition by ','
	Expression_List	*mp_Expression_List;
};

//	belong to statement
class Procedure_Call {
public:
	Procedure_Call();
	~Procedure_Call();

	string func_codeGeneration();



private:
	int	m_proCall_Tpye;

	int m_expNum;
	int m_lineno;

	Id				*mp_Id;
	Expression_List	*mp_Expression_List;
};

class Function_Call {
public:
	Function_Call();
	~Function_Call();

	string func_codeGeneration();



private:
	int m_expNum;
	int m_lineno;

	Id				*mp_Id;
	Expression_List	*mp_Expression_List;
};

// the expression can be calculate to a value
class Expression {
public:
	Expression();
	~Expression();

	string func_codeGeneration();



private:
	int m_lineno;

	//  the below two pointers
	//	only one can be assign to non-NULL
	//	the other must be NULL
	Relop				*mp_Relop;
	Simple_Expression	*mp_Simple_Expression;
};

// belong to expression
class Simple_Expression {
public:
	Simple_Expression();
	~Simple_Expression();

	string func_codeGeneration();



private:
	int m_lineno;

	//  the below two pointers
	//	only one can be assign to non-NULL
	//	the other must be NULL
	Addop	*mp_Addop;
	Term	*mp_Term;
};

class Term {
public:
	Term();
	~Term();

	string func_codeGeneration();



private:
	int m_lineno;

	//  the below two pointers
	//	only one can be assign to non-NULL
	//	the other must be NULL
	Mulop	*mp_Mulop;
	Factor	*mp_Factor;
};

// the smallest value unit
class Factor {
public:
	Factor();
	~Factor();

	string func_codeGeneration();
	void	func_checkFactorType();



private:
	int m_num;
	int m_factorType;
	int m_lineno;

	//	the below pointers
	//	only one can be assigned to non-NULL
	//	the others must be null
	Variable		*mp_Variable;
	Function_Call	*mp_Function_Call;
	Expression		*mp_Expression;
	Not				*mp_Not;
	Uminus			*mp_Uminus;
};

//	the operator 'not'
class Not {
public:
	Not();
	~Not();

	string func_codeGeneration();



private:
	int m_lineno;

	Factor	*mp_Factor;
};

//	the operator + -
//	as for:
//		-1 +2 -i +j
class Uminus {
public:
	Uminus();
	~Uminus();

	string func_codeGeneration();



private:
	int m_lineno;

	Factor	*mp_Factor;
};

// only used for var block
class Type {
public:
	Type();
	~Type();

	string	func_codeGeneration();
	string  func_code_getperiod() {
		return mp_Period->func_codeGeneration();
	}
	int		func_checkSimpleType() {
		return m_simpleType;
	}
	bool    func_check_isArray() {
		return m_isArray;
	}
	vector<pair<int, int>> func_get_Period() {
		return mp_Period->func_get_Range();
	}


private:
	int		m_simpleType;
	bool	m_isArray;
	int		m_lineno;

	//	if m_isArray == true
	//	this pointer will point to the period(multi-dimension)
	Period	*mp_Period;
};

//	used for const block
class Const_Value {
public:
	Const_Value();
	~Const_Value();

	string	func_codeGeneration();
	int		func_checkValueType() {
		return m_valueType;
	}


private:
	int		m_postNeg;
	int		m_valueType;

	// the below value are all unsigned
	int		m_int;
	float	m_real;
	char	m_char;

	int		m_lineno;
	bool	m_isId;

	//	if m_isId == true
	//	this pointer point to the refered ID
	Id	*mp_Id;
};

class Assignop {
public:
	Assignop();
	~Assignop();

	string func_codeGeneration();



private:
	int m_lineno;

	Variable	*mp_Variable;
	Expression	*mp_Expression;
};

class If_Then_Else {
public:
	If_Then_Else();
	~If_Then_Else();

	string func_codeGeneration();



private:
	int m_lineno;

	Expression	*mp_Expression;
	Statement	*mp_Statement_1;
	Statement	*mp_Statement_2;
};

class For {
public:
	For();
	~For();

	string func_codeGeneration();



private:
	int m_lineno;

	Id			*mp_Id;
	Expression	*mp_Expression_1;
	Expression	*mp_Expression_2;
	Statement	*mp_Statment;
};

class Parameter {
public:
	Parameter();
	~Parameter();

	string	func_codeGeneration();
	bool	func_isVal() {
		return m_isVal;
	}
	vector<Id*> func_get_mv_id() {
		return mp_Id_List->func_get_mv_Id();
	}
	int			func_get_m_type() {
		return m_Type;
	}



private:
	// define whether the parameter is variable element
	bool	m_isVal;
	int		m_lineno;
	int		m_Type;

	Id_List	*mp_Id_List;

};

class Relop {
public:
	Relop();
	~Relop();

	string func_codeGeneration();
	void	func_checkRelopType();



private:
	int m_relopType;
	int m_lineno;

	Simple_Expression *mp_Simple_Expression_1;
	Simple_Expression *mp_Simple_Expression_2;
};

class Addop {
public:
	Addop();
	~Addop();

	string func_codeGeneration();
	void	func_checkAddopType();


private:
	int m_addopType;
	int m_lineno;

	Simple_Expression	*mp_Simple_Expression;
	Term				*mp_Term;
};

class Mulop {
public:
	Mulop();
	~Mulop();

	string func_codeGeneration();
	void	func_checkMulopType();


private:
	int m_mulopType;
	int m_lineno;

	Term	*mp_Term;
	Factor	*mp_Factor;
};

class Id {
public:
	Id();
	~Id();

	string	func_codeGeneration();
	void	func_checkType();
	string	func_getName() {
		return m_name;
	}
	bool	func_isVal() {
		return m_isVal;
	}


private:
	string	m_name;
	int		m_idType;
	int		m_lineno;

	// This note only used in function and procedure 
	// to identify whether the id 
	// is a variable element parameter declaration
	// 
	// default to be false
	//
	bool	m_isVal;
};

class Id_List {
public:
	Id_List();
	~Id_List();

	string			func_codeGeneration();
	vector<Id*>		func_get_mv_Id() {
		return mv_Id;
	}

private:
	int m_lineno;

	vector<Id*>mv_Id;
};

class Period {
public:
	Period();
	~Period();

	string					func_codeGeneration();
	vector<pair<int, int>>	func_get_Range() {
		return mv_dims;
	}


private:
	// record the multi-dimension ranges
	vector<pair<int, int>>mv_dims;

};

class Expression_List {
public:
	Expression_List();
	~Expression_List();
	string func_codeGeneration();

private:

	vector<Expression*>mv_Expression;

};


//	�����඼���ڳ����﷨����û�е���
class Program_Head {
public:
	Program_Head();
	~Program_Head();

private:
	Id * m_Id;
	Id_List *m_Id_List;

};
class Compound_Statement {
public:
	Compound_Statement();
	~Compound_Statement();

private:
	Statement_List * m_Statement_List;

};
class Const_Declaration {
public:
	Const_Declaration();
	~Const_Declaration();

private:
	vector<pair<Id*, Const_Value*>> mv_Const;
};

class Var_Declaration {
public:
	Var_Declaration();
	~Var_Declaration();

private:
	vector<pair<Id_List*, Type*>> mv_Var;

};

class SubProgram_Declaration {
public:
	SubProgram_Declaration();
	~SubProgram_Declaration();

private:
	vector<Subprogram*> mv_Subprogram;

};

class Subprogram {
public:
	Subprogram();
	~Subprogram();

private:
	Subprogram_Head * m_Subprogram_Head;
	Subprogram_Body *m_Subprogram_Body;

};
class Subprogram_Head {
public:
	Subprogram_Head();
	~Subprogram_Head();

private:
	Id * m_ID;
	Formal_Parameter *m_Formal_Parameter;
	int Simple_Type;

};

class Subprogram_Body {
public:
	Subprogram_Body();
	~Subprogram_Body();

private:
	Const_Declarations * m_Const_Declarations;
	Var_Declarations *m_Var_Declarations;
	Compound_Statement *m_Compound_Statement;
};

class Formal_Parameter {
public:
	Formal_Parameter();
	~Formal_Parameter();

private:
	Parameter_List * m_Parameter_List;

};
class Var_Parameter {
public:
	Var_Parameter();
	~Var_Parameter();

private:
	Value_Parameter * m_Value_Parameter;



};
class Value_Parameter {
public:
	Value_Parameter();
	~Value_Parameter();
	int Simple_Type;

private:
	Id_List * m_Id_List;
};