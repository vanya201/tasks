
#include "stdAfx.h"
#include "DbHostAppServices.h"
#include "DbIdMapping.h"
#include "DbDatabase.h"
#include "DbBlockTable.h"

#include <ctime>
#include <string>

void _SaveDedicatedObjToFile_func(OdEdCommandContext* pCmdCtx)
{
    OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

    OdEdUserIO* uIO = pCmdCtx->userIO();
    OdDbDatabasePtr pDb = pDbCmdCtx->database();
    OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
    OdDbHostAppServices* pSvs = pDb->appServices();


    OdString fname = pIO->getString(OD_T("Enter file name :"));

    OdDbDatabasePtr pnDb;
    
    try 
    {
        pnDb = pDb->appServices()->readFile(fname);
    }
    catch (const OdError& er) 
    {
        pnDb = pSvs->createDatabase();

        uIO->putString("File will greate" + er.code() + er.description());
    }

    OdDbSelectionSetPtr pSSet = pIO->select(L"Select objects:", OdEd::kSelAllowEmpty);
    OdDbObjectIdArray arraysId = pSSet->objectIdArray();

    if (arraysId.empty()) 
    {
        uIO->putString("No select objects");
        return;
    }
  
    OdDbIdMappingPtr pMap = OdDbIdMapping::createObject();
    pMap->setDestDb(pnDb);

    pDb->wblockCloneObjects(arraysId, pnDb->getModelSpaceId(), *pMap, OdDb::kDrcReplace);

    OdDb::SaveType fileType = OdDb::kDwg;
    OdDb::DwgVersion outVer = OdDb::vAC24;

    try
    {
        pnDb->writeFile(fname, fileType, outVer, true);
        uIO->putString("Saved");
    }
    catch (const OdError& er)
    {
        uIO->putString("NO Saved");
    }

    pnDb.release();

}

void _ChangeColorNoSelect_func(OdEdCommandContext* pCmdCtx)
{

    clock_t start_time = clock();

    OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);

    OdEdUserIO* uIO = pCmdCtx->userIO();
    OdDbDatabasePtr pDb = pDbCmdCtx->database();
    OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

    OdDbSelectionSetPtr pSSet = pIO->select(L"Select objects:", OdEd::kSelAllowEmpty);
    OdDbObjectIdArray arraysId = pSSet->objectIdArray();

    OdDbBlockTableRecordPtr pModelSpace = pDb->getModelSpaceId().safeOpenObject(OdDb::kForRead);

    OdDbObjectIteratorPtr pEntIter = pModelSpace->newIterator();

    OdCmColor color = pIO->getColor("Enter color");

    for (; pEntIter->done(); pEntIter->step()) 
    {
        OdDbEntityPtr pEn = pEntIter->entity(OdDb::kForWrite);

        if (!arraysId.contains(pEntIter->objectId()))
        {
            pEn->setColor(color);
        }
    }
    clock_t end_time = clock();

    double elapsed_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;

    uIO->putString(std::to_string(elapsed_time).c_str());
}