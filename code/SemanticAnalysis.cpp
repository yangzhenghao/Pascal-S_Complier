#include "SemanticAnalysis.h"
#include <iostream>

//ע�� Programstruct�Ĵ����⣬�������ر�����
bool Programstruct::error_detect(string symbol_sheet_name)
{
	bool flag=true;
	if (this->mp_Program_Body)
		flag = mp_Program_Body->error_detect(symbol_sheet_name);
	return flag;
}

//ע�� һ�����,������������û���
bool Program_Body::error_detect(string symbol_sheet_name)
{
	bool flag = true;
	if (mp_Statement_List)
		flag = mp_Statement_List->error_detect(symbol_sheet_name);
	return flag;                                  //�˴���ҪΪflag&&��һ��boolֵ
}

// Statement_List�����������
bool Statement_List::error_detect(string symbol_sheet_name)
{
	bool flag = true;
	for (auto i : mv_Statement)
	{
		flag = flag && i->error_detect(symbol_sheet_name);
		
	}
	return flag;
}


bool Statement::error_detect(string symbol_sheet_name)
{
	bool flag = true;
	if (mp_Assignop)
		flag = mp_Assignop->error_detect(symbol_sheet_name);
	else if (mp_Procedure_call)
		flag = mp_Procedure_call->error_detect(symbol_sheet_name);
	else if (mp_Statement_List)
		flag = mp_Statement_List->error_detect(symbol_sheet_name);
	else if (mp_If_Then_Else)
		flag = mp_If_Then_Else->error_detect(symbol_sheet_name);
	else
		flag = mp_For->error_detect(symbol_sheet_name);
	return flag;
}

bool Assignop::error_detect(string symbol_sheet_name)
{
	bool flag;
	if (mp_Variable)
		flag =flag&&mp_Variable->error_detect(symbol_sheet_name);
	if (mp_Expression)
		flag = flag&&mp_Variable->error_detect(symbol_sheet_name);
	if (!mp_Variable || !mp_Expression)
	{
		flag = false;
		std::cout << "��" << m_lineno << ": �﷨������" << endl;
	}
	return flag;
}
bool If_Then_Else::error_detect(string symbol_sheet_name)
{
	bool flag = true;
	if (mp_Expression)
		flag = mp_Expression->error_detect(symbol_sheet_name);
	else
	{
		cout << "��" << m_lineno << ": �﷨������" << endl;
		flag = false;
	}
	if (mp_Statement_1)
		flag = flag&&mp_Statement_1;
	if (mp_Statement_1&&mp_Statement_2)
		flag = flag&&mp_Statement_2;
	else if (!mp_Statement_1&&mp_Statement_2)
	{
		cout << "��" << m_lineno << ": �﷨������" << endl;
		flag = false;
	}
	return flag;
}

bool For::error_detect(string symbol_sheet_name)
{
	if (mp_Id&&mp_Expression_1&&mp_Expression_2)            //�ж��﷨���Ƿ����
	{
		if (lookup_symbol(symbol_sheet_name, mp_Id->func_getName()))
		{
			bool flag = true;
			int type1 = get_symbol_type(symbol_sheet_name, mp_Id->func_getName);
			flag = flag && mp_Expression_1->error_detect(symbol_sheet_name);
			flag = flag && mp_Expression_2->error_detect(symbol_sheet_name);
			int type2 = mp_Expression_1->getType();
			int type3 = mp_Expression_2->getType();
			if ((type1 == TYPE_INTERGER || type1 == TYPE_CHAR) && type1 == type2&&type1 == type3)
				flag = flag;
			else
			{
				flag = false;
				std::cout << "��" << m_lineno << ": ѭ������������" << endl;

			}
			flag = flag&&mp_Statment->error_detect(symbol_sheet_name);
			return flag;
		}
		else
		{
			std::cout << "��" << m_lineno << ": δ��������" << endl;
			return false;
		}
	}
	else
	{
		std::cout << "��" << mp_Id->m_lineno << "�﷨������" << endl;
		return false;
	}
}

//ע�⣬������Ҫ���±�Խ��Ĵ���, �жϷ�Χ��û����
bool Variable::error_detect(string symbol_sheet_name)
{
	if (mp_Id)
	{
		if (lookup_symbol(symbol_sheet_name, mp_Id->func_getName()))
		{
			int type1 = get_symbol_type(symbol_sheet_name, mp_Id->func_getName);
			bool flag = true;
			if (mp_Expression_List&&type1!=5)
			{
				m_isArray = false;
				std::cout << "��" << m_lineno << ": ����������" << endl;
				flag = false;
			}
			else if (mp_Expression_List&&
				mp_Expression_List->func_get_mv_exp().size()
				!= get_symbol_range(symbol_sheet_name, mp_Id->func_getName).size())
			{
				m_isArray = true;
				std::cout << "��" << m_lineno << ": ����ά������" << endl;
				flag = false;
			}
			else if (mp_Expression_List)
			{
				flag = flag&&mp_Expression_List->error_detect();
				this->type = get_array_type(symbol_sheet_name, mp_Id->func_getName);
				vector<int> rangeVal1 = mp_Expression_List->rangeVal;
				vector<bool> rangeValid1 = mp_Expression_List->rangeValid;
				vector<pair<int, int> > rangeTemp = get_symbol_range(symbol_sheet_name, mp_Id->func_getName());
				for (int i = 0; i < rangeTemp.size(); i++)
				{
					if (rangeValid1[i])
					{
						if (rangeVal1[i]<rangeTemp[i].first&&rangeVal1[i]>rangeTemp[i].second)
						{
							std::cout << "��" << m_lineno << ": ����Խ��" << endl;
							flag = false;
						}

					}
				}
			}
			else
				m_isArray = false;
			return flag;
		}
		else
		{
			std::cout << "��" << m_lineno << ": δ��������"<<endl;
			return false;
		}
	}
	else
	{
		std::cout << "��" << mp_Id->m_lineno << "�﷨������" << endl;
		return false;
	}
	
}




//ע�⣬
bool Function_Call::error_detect(string symbol_sheet_name)
{
	bool flag1 = lookup_func(mp_Id->func_getName());
	if (!flag1)
	{
		std::cout << "��" << m_lineno << ": �Ǻ���" << endl;
		return false;
	}
	int nrgs = get_symbol_narg(symbol_sheet_name,mp_Id->func_getName());
	if (nrgs != mp_Expression_List->func_get_mv_type().size())
	{
		std::cout << "��" << m_lineno << ": ʵ���β�������ƥ��" << endl;
		return false;
	}
	vector<int> types = mp_Expression_List->func_get_mv_type();
	vector<int> arg_types = get_symbol_narg_type(symbol_sheet_name, mp_Id->func_getName());
	for (int i=0;i<types.size();i++)
		if (types[i] != arg_types[i])
		{
			std::cout << "��" << m_lineno << ": ��"<<i<<"��ʵ���ββ�ƥ��" << endl;
			return false;
		}
	return true;

}


bool Procedure_Call::error_detect(string symbol_sheet_name)
{
	bool flag1 = lookup_procedure(mp_Id->func_getName());
	if (!flag1)
	{
		std::cout << "��" << m_lineno << ": �ǹ���" << endl;
		return false;
	}
	int nrgs = get_symbol_narg(symbol_sheet_name, mp_Id->func_getName());
	if (nrgs != mp_Expression_List->func_get_mv_type().size())
	{
		std::cout << "��" << m_lineno << ": ʵ���β�������ƥ��" << endl;
		return false;
	}
	vector<int> types = mp_Expression_List->func_get_mv_type();
	vector<int> arg_types = get_symbol_narg_type(symbol_sheet_name, mp_Id->func_getName());
	for (int i = 0; i<types.size(); i++)
		if (types[i] != arg_types[i])
		{
			std::cout << "��" << m_lineno << ": ��" << i << "��ʵ���ββ�ƥ��" << endl;
			return false;
		}
	return true;
}
bool Expression::error_detect(string symbol_sheet_name)
{
	if (mp_Relop)
	{
		bool flag = mp_Relop->error_detect(symbol_sheet_name);
		this->setType(mp_Relop->getType());
		rangeVal = -1;
		rangeValid = false;
		return flag;
	}
	else if(mp_Simple_Expression)
	{
		bool flag = mp_Simple_Expression->error_detect(symbol_sheet_name);
		setType(mp_Simple_Expression->getType());
		rangeVal = mp_Simple_Expression->getRangeVal();
		rangeValid = mp_Simple_Expression->getRangeValid();
		return flag;
	}
	else
	{
		rangeVal = -1;
		rangeValid = false;
		std::cout << "��" << m_lineno << "�﷨������" << endl;
		return false;
	}

}


bool Expression_List::error_detect(string symbol_sheet_name)
{
	bool flag = true;
	for (int i = 0; i < mv_Expression.size(); i++)
	{
		flag = flag&&mv_Expression[i]->error_detect(symbol_sheet_name);
		mv_Type.push_back(mv_Expression[i]->getType());
		rangeVal.push_back(mv_Expression[i]->getRangeVal());
		rangeValid.push_back(mv_Expression[i]->getRangeValid());
	}
	return flag;
}

bool Relop::error_detect(string symbol_sheet_name)
{
	if (mp_Simple_Expression_1&&mp_Simple_Expression_2)
	{
		bool flag;
		int type1 = mp_Simple_Expression_1->getType();
		int type2 = mp_Simple_Expression_2->getType();
		flag = (type1 == type2 || type1 == TYPE_INTERGER&&type2 == TYPE_REAL ||
			type2 == TYPE_INTERGER&&type1 == TYPE_REAL);
		setType(TYPE_BOOLEAN);
		return flag;
	}
	else
	{
		std::cout << "��" << m_lineno << "�﷨������" << endl;
		return false;
	}
}

bool Simple_Expression::error_detect(string symbol_sheet_name)
{
	if (mp_Addop)
	{
		bool flag = mp_Addop->error_detect(symbol_sheet_name);
		setType(mp_Addop->func_checkAddopType());
		rangeVal = -1;
		rangeValid = false;
		return flag;
	}
	else if (mp_Term)
	{
		bool flag = mp_Term->error_detect(symbol_sheet_name);
		setType(mp_Term->getType());
		rangeVal = mp_Term->getRangeVal();
		rangeValid = mp_Term->getRangeValid();
		return flag;
	}
	else
	{
		rangeVal = -1;
		rangeValid = false;
		std::cout << "��" << m_lineno << "�﷨������" << endl;
		return false;
	}
}

//�ж�Addop����ȷ��
bool Addop::error_detect(string symbol_sheet_name)
{
	bool flag1 = mp_Simple_Expression->error_detect(symbol_sheet_name);
	bool flag2 = mp_Term->error_detect(symbol_sheet_name);
	bool flag3 = true;
	int type1 = mp_Simple_Expression->getType();
	int type2 = mp_Term->getType();
	if (m_addopType == ADDOP_ADD || m_addopType == ADDOP_SUB)
	{
		flag3 = ((type1 == TYPE_INTERGER || type1 == TYPE_REAL) && (type2 == TYPE_INTERGER || type2 == TYPE_REAL));
		//��ʹ���Ͳ���Ҳ�ḽһ����ȷ���ͣ������Ժ󲻻��ټ���ִ�д��뷭��
		if (type1 == TYPE_REAL || type2 == TYPE_REAL)
			setType(TYPE_REAL);
		else
			setType(TYPE_INTERGER);
	}
	else
	{
		flag3 = (type1 == type2&&type1==TYPE_BOOLEAN);
		setType(TYPE_BOOLEAN);
	}
	return flag1&&flag1&&flag3;
}

bool Term::error_detect(string symbol_sheet_name)
{
	if (mp_Mulop)
	{
		bool flag = mp_Mulop->error_detect(symbol_sheet_name);
		setType(mp_Mulop->func_checkMulopType());
		rangeVal = -1;
		rangeValid = false;
		return flag;
	}
	else if (mp_Factor)
	{
		bool flag = mp_Factor->error_detect(symbol_sheet_name);
		setType(mp_Factor->func_checkFactorType());
		rangeVal = mp_Factor->getRangeVal();
		rangeValid = mp_Factor->getRangeValid();
		return flag;
	}
	else
	{
		rangeVal = -1;
		rangeValid = false;
		std::cout << "��" << m_lineno << "�﷨������" << endl;
		return false;
	}
}

bool Mulop::error_detect(string symbol_sheet_name)
{
	if (mp_Term&&mp_Factor)
	{
		bool flag1 = mp_Term->error_detect(symbol_sheet_name);
		bool flag2 = mp_Factor->error_detect(symbol_sheet_name);
		int type1 = mp_Term->getType();
		int type2 = mp_Factor->getType();
		int opType = this->func_checkMulopType();
		bool flag3 = true;
		if (opType == MULOP_MULTIPLY || opType == MULOP_INT_DIV || opType == MULOP_READ_DIV)
		{
			flag3 = ((type1 == TYPE_INTERGER || type1 == TYPE_REAL) && (type2 == TYPE_INTERGER || type2 == TYPE_REAL));
			if (type1 == TYPE_REAL || type2 == TYPE_REAL)
				setType(TYPE_REAL);
			else
				setType(TYPE_INTERGER);
		}
		else if (opType == MULOP_MOD)
		{
			flag3 = (type1 == type2)&&(type1 = TYPE_INTERGER);
			setType(TYPE_INTERGER);
		}
		else
		{
			flag3 = (type1 == type2&&type1 == TYPE_BOOLEAN);
			setType(TYPE_BOOLEAN);
		}
		if (!(flag1&&flag2&&flag3))
		{
			std::cout << "��" << m_lineno << ": ���Ͳ�ƥ��." << endl;
			return false;
		}
		return true;
		
	}
	else
	{
		std::cout << "��" << m_lineno << "�﷨������" << endl;
		return false;
	}
}

//ע�⣬Factor�Ĵ�����,factor���ֵû�޸�
bool Factor::error_detect(string symbol_sheet_name)
{
	bool flag = true;
	switch (m_factorType)
	{
		case FACTOR_vALUE_BOOL:
		{
			rangeVal = -1;
			rangeValid = false;
			setType(TYPE_BOOLEAN);
			break;
		}
		case FACTOR_VALUE_INT:
		{
			
			rangeVal = m_int;
			rangeValid = true;
			setType(TYPE_INTERGER);
			break;
		}
		case FACTOR_VALUE_CHAR:
		{
			rangeVal = -1;
			rangeValid = false;
			setType(TYPE_CHAR);
			break;
		}
		case FACTOR_VALUE_REAL:
		{
			rangeVal = -1;
			rangeValid = false;
			setType(TYPE_REAL);
			break;
		}
		case FACTOR_VAR: 
		{
			rangeVal = -1;
			rangeValid = false;
			flag = mp_Variable->error_detect(symbol_sheet_name);
			setType(mp_Variable->getType());
			break;
		}
		case FACTOR_FUNCCALL:
		{
			rangeVal = -1;
			rangeValid = false;
			flag = mp_Function_Call->error_detect(symbol_sheet_name);
			setType(get_func_return_type(mp_Function_Call->mp_Id->func_getName()));
			break;
		}
		case FACTOR_BRACKETS:
		{
			rangeVal = -1;
			rangeValid = false;
			flag = mp_Expression->error_detect(symbol_sheet_name);
			setType(mp_Expression->getType());
			break;
		}
		case FACTOR_NOT:
		{
			rangeVal = -1;
			rangeValid = false;
			flag = mp_Not->error_detect(symbol_sheet_name);
			setType(TYPE_BOOLEAN);
			break;
		}
		case FACTOR_UMINUS:
		{
			int typeTemp = mp_Uminus->getType();
			flag = mp_Uminus->error_detect(symbol_sheet_name);
			setType(mp_Uminus->getType());
			if (typeTemp != TYPE_INTERGER)
			{
				rangeVal = -1;
				rangeValid = false;
			}
			else
			{
				rangeVal = mp_Uminus->getRangeVal();
				rangeValid = true;
			}
			break;
		}

		default:
		{
			rangeVal = -1;
			rangeValid = false;
			std::cout << "��" << m_lineno << "�﷨������" << endl;
			return false;
		}
	}
	return flag;
}

bool Not::error_detect(string symbol_sheet_name)
{
	if (mp_Factor) 
	{
		bool flag = true;
		flag = mp_Factor->error_detect(symbol_sheet_name);
		int typeTemp = mp_Factor->getType();
		if (typeTemp != TYPE_BOOLEAN)
		{
			flag = false;
			std::cout << "��" << m_lineno << ": ��boolean���Ͳ���not" << endl;
		}
		return flag;
	}
	else
	{
		std::cout << "��" << m_lineno << "�﷨������" << endl;
		return false;
	}
}

bool Uminus::error_detect(string symbol_sheet_name)
{
	if (mp_Factor)
	{
		bool flag = mp_Factor->error_detect(symbol_sheet_name);
		int typeTemp = mp_Factor->getType();
		if (typeTemp != TYPE_INTERGER&&typeTemp != TYPE_REAL)
		{
			flag = false;
			std::cout << "��" << m_lineno << ": ���Ͳ�ƥ��" << endl;
		}
		rangeVal = mp_Factor->getRangeVal();
		rangeVal = m_unimusType == 0 ? rangeVal : rangeValid;
		rangeValid = mp_Factor->getRangeValid();
		setType(typeTemp);
		return flag;
	}
	else
	{
		setType(TYPE_INTERGER);
		std::cout << "��" << m_lineno << ": �﷨������" << endl;
		return false;
	}
}

