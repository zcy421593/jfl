#include "stdafx.h"
#include "DBInterface.h"
CDBInterface* CDBInterface::m_pDBInterface=0;
CDBInterface::CDBInterface(CString strServIP,CString strDatabaseName,CString strUID,CString strPWD)
{
		TCHAR szPath[260]={0};
		GetModuleFileName(NULL,szPath,260);
		CString strModulePath=szPath;
		CString strPath=strModulePath.Left(strModulePath.ReverseFind(_T('\\')))+"\\data.s3db";
	CString strConnectionString;
	CoInitialize(NULL);
	m_pConnection.CreateInstance(_uuidof(Connection)); //初始化Connection指针
	//m_pRecordset.CreateInstance(_uuidof(Recordset));//初始化Recordset指针
	strConnectionString.AppendFormat("Provider=MSDASQL.1;Persist Security Info=False;Driver={SQLite3 ODBC Driver};Database=%s",strPath);
	
	const char* szConnStr=strConnectionString.GetBuffer();
	try{
		m_pConnection->ConnectionTimeout=5;
		m_pConnection->Open(szConnStr,"","",0);
		strConnectionString.ReleaseBuffer();
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		exit(0);
	}
	//SysFreeString(bstrText);
}


CDBInterface::~CDBInterface(void)
{
	m_pConnection->Close();
}

void CDBInterface::InitDBInterface(CString strServIP,CString strDatabaseName,CString strUID,CString strPWD)
{
	CDBInterface::m_pDBInterface=new CDBInterface( strServIP, strDatabaseName, strUID, strPWD);
}

BOOL CDBInterface::GetAllStocks( std::vector<CStockInfo>& vecStocks )
{
	_RecordsetPtr pRecordSet;
	pRecordSet.CreateInstance(_uuidof(Recordset));
	try{
	pRecordSet->Open("select * from stock",m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(...)
	{
		return FALSE;
	}
	vecStocks.clear();
	while(!pRecordSet->adoEOF)
	{
		CStockInfo stockInfo;
		stockInfo.m_strStockName=pRecordSet->GetCollect("stockName");
		stockInfo.m_strDescribtion=pRecordSet->GetCollect("describtion");		
		stockInfo.m_strDllPath=pRecordSet->GetCollect("dllPath");
		stockInfo.m_bIsLoad=pRecordSet->GetCollect("isLoad");
		vecStocks.push_back(stockInfo);
		pRecordSet->MoveNext();
	}
	pRecordSet->Close();
	return TRUE;
}

BOOL CDBInterface::AddStockInfo(CString strStockName,CString strDllPath,std::vector<CLineInfo> &vecLines,CString& strErr)
{
	CString strSql;
	BOOL bRes=TRUE;
	try{
	m_pConnection->BeginTrans();
	strSql.Format("insert into stock(stockName,tableName,dllPath,isLoad,describtion) values(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')"
							,strStockName,strStockName,strDllPath,"false","");
	m_pConnection->Execute(strSql.GetBuffer(),NULL,adCmdText);
	strSql.ReleaseBuffer();


	strSql.Format("create table %s(",strStockName);
	for(int i=0;i<vecLines.size();i++)
	{
		strSql+=vecLines[i].m_strName+" decimal,";
	}
	strSql.Delete(strSql.GetLength()-1,1);
	strSql+=")";
	m_pConnection->Execute(strSql.GetBuffer(),NULL,adCmdText);
	strSql.ReleaseBuffer();

	for(int i=0;i<vecLines.size();i++)
	{
		strSql.Format("insert into lineInfo(stockName,lineName,color,lineWidth) values(\'%s\',\'%s\',\'%d\',\'%d\')",
								strStockName,vecLines[i].m_strName,vecLines[i].m_dwColor,vecLines[i].m_iWidth);
		m_pConnection->Execute(strSql.GetBuffer(),NULL,adCmdText);
		strSql.ReleaseBuffer();
	}


	
 	m_pConnection->CommitTrans();
	}
	catch(_com_error err)
	{
 		m_pConnection->RollbackTrans();
		AfxMessageBox(err.Description());
		bRes=FALSE;
 	}
	return TRUE;

}

BOOL CDBInterface::GetStockLines( CString strStockName,std::vector<CLineInfo>&vecLines )
{
	_RecordsetPtr pRecordSet;
	pRecordSet.CreateInstance(_uuidof(Recordset));
	CString strSql;
	strSql.Format("select * from lineInfo where stockName=\'%s\'",strStockName);
	try{
		pRecordSet->Open(strSql.GetBuffer(),m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(...)
	{

		return FALSE;
	}
	vecLines.clear();
	while(!pRecordSet->adoEOF)
	{
		CLineInfo lineInfo;
		lineInfo.m_strName=pRecordSet->GetCollect("lineName");
		lineInfo.m_dwColor=pRecordSet->GetCollect("color");		
		lineInfo.m_iWidth=pRecordSet->GetCollect("lineWidth");
		vecLines.push_back(lineInfo);
		pRecordSet->MoveNext();
	}
	pRecordSet->Close();
	return TRUE;
}

BOOL CDBInterface::DeleteStock( CString strStockName )
{
	CString strSql;
	BOOL bRes=TRUE;
	_RecordsetPtr pRecordSet;
	pRecordSet.CreateInstance(_uuidof(Recordset));
	try{
		m_pConnection->BeginTrans();
		strSql.Format("select * from stock where stockName=\'%s\'",strStockName);
		pRecordSet->Open(strSql.GetBuffer(),m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		strSql.ReleaseBuffer();
		if(!pRecordSet->GetRecordCount())
		{
			pRecordSet->Close();
			return TRUE;
		}

				
		CString strTableName=pRecordSet->GetCollect(L"tableName");
		strStockName.ReleaseBuffer();

		strSql.Format("delete from stock where stockName=\'%s\'",strStockName);
		m_pConnection->Execute(strSql.GetBuffer(),NULL,adCmdText);
		strSql.ReleaseBuffer();


		strSql.Format("delete from lineInfo where stockName=\'%s\'",strStockName);
		m_pConnection->Execute(strSql.GetBuffer(),NULL,adCmdText);
		strSql.ReleaseBuffer();

		strSql.Format("drop table [%s]",strTableName);
		m_pConnection->Execute(strSql.GetBuffer(),NULL,adCmdText);
		strSql.ReleaseBuffer();

		m_pConnection->CommitTrans();
	}
	catch(...)
	{
		m_pConnection->RollbackTrans();
		bRes=FALSE;
	}
	return TRUE;

}
