#include <Windows.h>
#include <stdio.h>
#include <AccCtrl.h>
#include <AclAPI.h>

// Below function is copy paste from https://docs.microsoft.com/en-

us/windows/win32/secauthz/modifying-the-acls-of-an-object-in-c--

DWORD AddAceToObjectsSecurityDescriptor (
    LPTSTR pszObjName,          // sethc.exe 
    SE_OBJECT_TYPE ObjectType,  // need to check
    LPTSTR pszTrustee,          // fac.exe
    TRUSTEE_FORM TrusteeForm,   // need to check 
    DWORD dwAccessRights,       // Read write need to check this format 
    ACCESS_MODE AccessMode,     // write 
    DWORD dwInheritance         // inheritance flags for new ACE
) 
{
    DWORD dwRes = 0;
    PACL pOldDACL = NULL, pNewDACL = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;
    EXPLICIT_ACCESS ea;

    if (NULL == pszObjName) 
        return ERROR_INVALID_PARAMETER;

    // Get a pointer to the existing DACL.

    dwRes = GetNamedSecurityInfo(pszObjName, ObjectType, 
          DACL_SECURITY_INFORMATION,
          NULL, NULL, &pOldDACL, NULL, &pSD);
    if (ERROR_SUCCESS != dwRes) {
        printf( "GetNamedSecurityInfo Error %u\n", dwRes );
        goto Cleanup; 
    }  

    // Initialize an EXPLICIT_ACCESS structure for the new ACE. 
    //memset(ea, 0, sizeof(ea));

    ZeroMemory(&ea, sizeof(ea));

    ea.grfAccessPermissions = dwAccessRights;
    ea.grfAccessMode = AccessMode;
    ea.grfInheritance= dwInheritance;
    ea.Trustee.TrusteeForm = TrusteeForm;
    ea.Trustee.ptstrName = pszTrustee;

    // Create a new ACL that merges the new ACE
    // into the existing DACL.

    dwRes = SetEntriesInAcl(1, &ea, pOldDACL, &pNewDACL);
    if (ERROR_SUCCESS != dwRes)  {
        printf( "SetEntriesInAcl Error %u\n", dwRes );
        goto Cleanup; 
    }  

    // Attach the new ACL as the object's DACL.

    dwRes = SetNamedSecurityInfo(pszObjName, ObjectType, 
          DACL_SECURITY_INFORMATION,
          NULL, NULL, pNewDACL, NULL);
    if (ERROR_SUCCESS != dwRes)  {
        printf( "SetNamedSecurityInfo Error %u\n", dwRes );
        goto Cleanup; 
    }  

    Cleanup:

        if(pSD != NULL) 
            LocalFree((HLOCAL) pSD); 
        if(pNewDACL != NULL) 
            LocalFree((HLOCAL) pNewDACL); 

        return dwRes;
}

//End of copy paste 

int main (int argc, TCHAR *argv[])
{
    LPTSTR Acl_obj, Acl_Trustee;
    SE_OBJECT_TYPE Acl_obj_type ;
    TRUSTEE_FORM tForm ;
   // DWORD Acl_INH, Acl_Acc_Right;
    ACCESS_MODE Acl_Acc_Mod ;

    Acl_obj = argv[1] ;
    Acl_Trustee = "test.bat";
    /*Acl_obj_type = SE_FILE_OBJECT; 
    tForm = TRUSTEE_IS_NAME;
    Acl_Acc_Mod = GRANT_ACCESS;*/

    if(AddAceToObjectsSecurityDescriptor(Acl_obj,SE_FILE_OBJECT,Acl_Trustee,TRUSTEE_IS_OBJECTS_AND_NAME,GENERIC_ALL,SET_ACCESS,SUB_CONTAINERS_AND_OBJECTS_INHERIT) == NULL)
printf ("Error %d", GetLastError());

    return 0;
}