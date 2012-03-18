CString ReadRegisteTable(CString root,CString path, CString key)
{
  HKEY hAppKey;   
  LPCTSTR WINDS_SERVICE_REGISTRY_KEY=path;
  LPCTSTR DATA_FILE_SUB_KEY=key;
  TCHAR szDataFile[80];
 if(root=="HKEY_LOCAL_MACHINE")
 {
  if (ERROR_SUCCESS == RegOpenKeyEx (
    HKEY_LOCAL_MACHINE,
    WINDS_SERVICE_REGISTRY_KEY,
    0,
    KEY_READ,
    &hAppKey))
  {      
    ULONG cbSize = MAX_PATH*sizeof(TCHAR);   
    DWORD dwFlag = RegQueryValueEx (
     hAppKey,
     DATA_FILE_SUB_KEY,
     NULL,
     NULL,
     (LPBYTE)szDataFile,&cbSize);
    RegCloseKey (hAppKey);   
    if (ERROR_SUCCESS == dwFlag)  
   {       
     CString strDate=szDataFile; //MessageBox(strDate);
     if(strDate.GetLength()>0) return strDate;   
     else return _T("");
   }
    return _T("");
  }  
 }
 if(root=="HKEY_CURRENT_USER")
 {
  if (ERROR_SUCCESS == RegOpenKeyEx (
    HKEY_CURRENT_USER,
    WINDS_SERVICE_REGISTRY_KEY,
    0,
    KEY_READ,
    &hAppKey))
  {      
    ULONG cbSize = MAX_PATH*sizeof(TCHAR);   
    DWORD dwFlag = RegQueryValueEx (
     hAppKey,
     DATA_FILE_SUB_KEY,
     NULL,
     NULL,
     (LPBYTE)szDataFile,&cbSize);
    RegCloseKey (hAppKey);   
    if (ERROR_SUCCESS == dwFlag)  
   {       
     CString strDate=szDataFile; //MessageBox(strDate);
     if(strDate.GetLength()>0) return strDate;   
     else return _T("");;
   }
    return _T("");;
  }  
 }
  return _T("");;
}
