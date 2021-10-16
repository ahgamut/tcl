/*
 * tclTestObj.c --
 *
 *	This file contains C command functions for the additional Tcl commands
 *	that are used for testing implementations of the Tcl object types.
 *	These commands are not normally included in Tcl applications; they're
 *	only used for testing.
 *
 * Copyright © 2021 Nathan Coulter
 *
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

#include "tcl.h"
#include "tclInt.h"

/*
 * Prototypes for functions defined later in this file:
 */
typedef struct indexHex {
    int refCount;
    size_t offset;
} indexHex;


int TcltestObjectInterfaceInit(Tcl_Interp *interp);

int NewTestIndexHex (
    ClientData, Tcl_Interp *interp, int argc, Tcl_Obj *const objv[]);
static void		DupTestIndexHexInternalRep(Tcl_Obj *srcPtr, Tcl_Obj *copyPtr);
static void		FreeTestIndexHexInternalRep(Tcl_Obj *objPtr);
static int		SetTestIndexHexFromAny(Tcl_Interp *interp, Tcl_Obj *objPtr);
static void		UpdateStringOfTestIndexHex(Tcl_Obj *listPtr);


static int indexHexListErrorIndeterminate (Tcl_Interp *interp);
static int indexHexListObjGetElements (tclObjTypeInterfaceArgsListAll);
static int indexHexListObjAppendElement (tclObjTypeInterfaceArgsListAppend);
static int indexHexListObjAppendList (tclObjTypeInterfaceArgsListAppendList);
static int indexHexListObjIndex (tclObjTypeInterfaceArgsListIndex);
static int indexHexListObjIndexEnd (tclObjTypeInterfaceArgsListIndex);
static int indexHexListObjLength (tclObjTypeInterfaceArgsListLength);
static Tcl_Obj* indexHexListObjRange (tclObjTypeInterfaceArgsListRange);
static Tcl_Obj* indexHexListObjRangeEnd (tclObjTypeInterfaceArgsListRangeEnd);
static int indexHexListObjReplace (tclObjTypeInterfaceArgsListReplace);
static int indexHexListObjSetElement (tclObjTypeInterfaceArgsListSet);
static Tcl_Obj * indexHexLsetFlat (tclObjTypeInterfaceArgsListSetList);


ObjInterface IndexHexInterface = {
    1,
    {
	&indexHexListObjGetElements,
	&indexHexListObjAppendElement,
	&indexHexListObjAppendList,
	&indexHexListObjIndex,
	&indexHexListObjIndexEnd,
	&indexHexListObjLength,
	&indexHexListObjRange,
	&indexHexListObjRangeEnd,
	&indexHexListObjReplace,
	&indexHexListObjSetElement,
	&indexHexLsetFlat
    }
};


const ObjectType testIndexHexType = {
    (char *)&TclObjectTypeType0,
    FreeTestIndexHexInternalRep,	/* freeIntRepProc */
    DupTestIndexHexInternalRep,		/* dupIntRepProc */
    UpdateStringOfTestIndexHex,		/* updateStringProc */
    SetTestIndexHexFromAny,		/* setFromAnyProc */
    2,
    "testindexHex",
    (Tcl_ObjInterface *)&IndexHexInterface
};

const Tcl_ObjType *testIndexHexTypePtr = (Tcl_ObjType *)&testIndexHexType;


int TcltestObjectInterfaceInit(Tcl_Interp *interp) {
    Tcl_CreateObjCommand(interp, "testindexhex", NewTestIndexHex, NULL, NULL);
    return TCL_OK;
}


int NewTestIndexHex (
    ClientData, Tcl_Interp *interp, int argc, Tcl_Obj *const objv[])
{
    Tcl_WideInt offset;
    Tcl_ObjIntRep intrep;
    if (argc > 2) {
	if (interp != NULL) {
	    Tcl_SetObjResult(interp, Tcl_NewStringObj("too many arguments", -1));
	    return TCL_ERROR;
	}
    }
    if (argc == 2) {
	if (Tcl_GetWideIntFromObj(interp, objv[1], &offset) != TCL_OK) {
	    return TCL_ERROR;
	}
	if (offset < 0) {
	    Tcl_SetObjResult(interp, Tcl_NewStringObj("bad offset", -1));
	    return TCL_ERROR;
	}
    }  else {
	offset = 0;
    }
    Tcl_Obj *objPtr = Tcl_NewObj();

    indexHex *indexHexPtr = (indexHex *)Tcl_Alloc(sizeof(indexHex));
    indexHexPtr->refCount = 1;
    indexHexPtr->offset = offset;
    intrep.twoPtrValue.ptr1 = indexHexPtr;
    Tcl_StoreIntRep(objPtr, testIndexHexTypePtr, &intrep);
    Tcl_SetObjResult(interp, objPtr);
    return TCL_OK;
}

static void
DupTestIndexHexInternalRep(Tcl_Obj *srcPtr, Tcl_Obj *copyPtr)
{
    return;
}

static void
FreeTestIndexHexInternalRep(Tcl_Obj *objPtr)
{
    indexHex *indexHexPtr = (indexHex *)objPtr->internalRep.twoPtrValue.ptr1;
    if (--indexHexPtr->refCount == 0) {
	Tcl_Free(indexHexPtr);
    }
    return;
}

static int
SetTestIndexHexFromAny(Tcl_Interp *interp, Tcl_Obj *objPtr)
{
    if (TclHasIntRep(objPtr, testIndexHexTypePtr)) {
	return TCL_OK;
    } else {
	if (interp != NULL) {
	    Tcl_SetObjResult(interp,
		Tcl_NewStringObj("can not set an existing value to this type", -1));
	}
	return TCL_ERROR;
    }
}

static void
UpdateStringOfTestIndexHex(Tcl_Obj *listPtr)
{
    return;
}

static int indexHexListErrorIndeterminate (Tcl_Interp *interp) {
    Tcl_SetObjResult(interp,
	Tcl_NewStringObj("list length indeterminate", -1));
    Tcl_SetErrorCode(interp, "TCL", "VALUE", "INDEX", "INDETERMINATE", NULL);
    return TCL_ERROR;
}

static int
indexHexListObjGetElements(tclObjTypeInterfaceArgsListAll)
{
    if (interp != NULL) {
	Tcl_SetObjResult(interp,Tcl_NewStringObj("infinite list", -1));
    }
    return TCL_ERROR;
}


static int
indexHexListObjAppendElement(tclObjTypeInterfaceArgsListAppend)
{
    return TCL_ERROR;
}


static int
indexHexListObjAppendList(tclObjTypeInterfaceArgsListAppendList)
{
    return TCL_ERROR;
}


static int
indexHexListObjIndex(tclObjTypeInterfaceArgsListIndex)
{
    Tcl_Obj *resPtr;
    resPtr = Tcl_ObjPrintf("%x", index);
    Tcl_IncrRefCount(resPtr);
    *objPtrPtr = resPtr;
    return TCL_OK;
}


static int
indexHexListObjIndexEnd(tclObjTypeInterfaceArgsListIndexEnd)
{
    return indexHexListErrorIndeterminate(interp);
}


static int
indexHexListObjLength(tclObjTypeInterfaceArgsListLength)
{
    *intPtr = -1;
    return TCL_OK;
}


static Tcl_Obj*
indexHexListObjRange(tclObjTypeInterfaceArgsListRange)
{
    int i;
    Tcl_Obj *itemPtr, *resPtr;
    resPtr = Tcl_NewListObj(0, NULL);
    while (fromIdx <= length && fromIdx <= toIdx) {
	indexHexListObjIndex(interp, listPtr, fromIdx, &itemPtr);
	if (
	    Tcl_ListObjAppendElement(interp, resPtr, itemPtr) != TCL_OK
	) {
	    Tcl_DecrRefCount(resPtr);
	    return TCL_ERROR;
	}
	fromIdx++;
    }
    return resPtr;
}

static Tcl_Obj*
indexHexListObjRangeEnd(tclObjTypeInterfaceArgsListRangeEnd) {
    if (fromAnchor == 1 || toAnchor == 1) {
	indexHexListErrorIndeterminate(interp);
	return NULL;
    }
    return indexHexListObjRange(interp, listPtr, -1, fromIdx, toIdx);
}


static int
indexHexListObjReplace(tclObjTypeInterfaceArgsListReplace)
{
    return TCL_ERROR;
}


static int
indexHexListObjSetElement(tclObjTypeInterfaceArgsListSet)
{
    return TCL_ERROR;
}

static Tcl_Obj * indexHexLsetFlat (tclObjTypeInterfaceArgsListSetList)
{
    return NULL;
}