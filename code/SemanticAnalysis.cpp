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

//ע�⣬������Ҫ���±�Խ��Ĵ���, �жϷ�Χ��û����
bool Variable::error_detect(string symbol_sheet_name)
{
	if (mp_Id)
	{
		if (lookup_symbol(symbol_sheet_name, mp_Id->func_getName()))
		{
			string type = get_symbol_type(symbol_sheet_name, mp_Id->func_getName);
			if (mp_Expression_List&&type.find("array") == string::npos)
			{
				m_isArray = false;
				std::cout << "��" << m_lineno << ": ����������" << endl;
			}
			else if (mp_Expression_List&&
				mp_Expression_List->func_get_mv_exp().size()
				!= get_symbol_range(symbol_sheet_name, mp_Id->func_getName).size())
			{
				m_isArray = true;
				std::cout << "��" << m_lineno << ": ����ά������" << endl;
			}
			else if (mp_Expression_List)
			{

			}
			else
				m_isArray = false;
		}
		else
		{
			std::cout << "��" << m_lineno << ": δ��������"<<endl;
		}
	}
	else
	{
		std::cout << "��" << mp_Id->m_lineno << "�﷨������" << endl;
	}
	
}



