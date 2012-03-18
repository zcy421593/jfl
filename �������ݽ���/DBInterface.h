#ifndef DBINTERFACE_H
#define  DBINTERFACE_H

#include <vector>
#include "CommonDataStruct.h"
#include "afxcmn.h"
#include "Common.h"
#import "C:\Program Files\common files\system\ado\msado15.dll"	no_namespace rename("EOF","adoEOF")
class CDBInterface
{
	// æ≤Ã¨±‰¡ø
public:
	static void InitDBInterface(CString strServIP,CString strDatabaseName,CString strUID,CString strPWD);
	static void ReleaseDBInterface(){delete m_pDBInterface;};
	static  CDBInterface* GetDBInterface(){return m_pDBInterface;};
	~CDBInterface(void);

public:
	BOOL AddStockInfo(CString strStockName,CString strDllPath,std::vector<CLineInfo> &vecLines,CString& strErr);
	BOOL GetAllStocks(std::vector<CStockInfo>& vecStocks);
	BOOL GetStockLines(CString strStockName,std::vector<CLineInfo>&vecLines);
	BOOL DeleteStock(CString strStockName);
private:
	CDBInterface::CDBInterface(CString strServIP,CString strDatabaseName,CString strUID,CString strPWD);
	static CDBInterface* m_pDBInterface;
	_ConnectionPtr m_pConnection;

};

#endif