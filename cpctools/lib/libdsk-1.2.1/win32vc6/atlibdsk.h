/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Jan 23 19:21:15 2008
 */
/* Compiler settings for J:\C\LibDsk\win32vc6\atlibdsk.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __atlibdsk_h__
#define __atlibdsk_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IGeometry_FWD_DEFINED__
#define __IGeometry_FWD_DEFINED__
typedef interface IGeometry IGeometry;
#endif 	/* __IGeometry_FWD_DEFINED__ */


#ifndef __IDisk_FWD_DEFINED__
#define __IDisk_FWD_DEFINED__
typedef interface IDisk IDisk;
#endif 	/* __IDisk_FWD_DEFINED__ */


#ifndef __IReporter_FWD_DEFINED__
#define __IReporter_FWD_DEFINED__
typedef interface IReporter IReporter;
#endif 	/* __IReporter_FWD_DEFINED__ */


#ifndef __ILibrary_FWD_DEFINED__
#define __ILibrary_FWD_DEFINED__
typedef interface ILibrary ILibrary;
#endif 	/* __ILibrary_FWD_DEFINED__ */


#ifndef __Library_FWD_DEFINED__
#define __Library_FWD_DEFINED__

#ifdef __cplusplus
typedef class Library Library;
#else
typedef struct Library Library;
#endif /* __cplusplus */

#endif 	/* __Library_FWD_DEFINED__ */


#ifndef __Disk_FWD_DEFINED__
#define __Disk_FWD_DEFINED__

#ifdef __cplusplus
typedef class Disk Disk;
#else
typedef struct Disk Disk;
#endif /* __cplusplus */

#endif 	/* __Disk_FWD_DEFINED__ */


#ifndef __Geometry_FWD_DEFINED__
#define __Geometry_FWD_DEFINED__

#ifdef __cplusplus
typedef class Geometry Geometry;
#else
typedef struct Geometry Geometry;
#endif /* __cplusplus */

#endif 	/* __Geometry_FWD_DEFINED__ */


#ifndef __Reporter_FWD_DEFINED__
#define __Reporter_FWD_DEFINED__

#ifdef __cplusplus
typedef class Reporter Reporter;
#else
typedef struct Reporter Reporter;
#endif /* __cplusplus */

#endif 	/* __Reporter_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_atlibdsk_0000 */
/* [local] */ 

typedef /* [public][public][public] */ 
enum __MIDL___MIDL_itf_atlibdsk_0000_0001
    {	ALT	= 0,
	OUTBACK	= ALT + 1,
	OUTOUT	= OUTBACK + 1
    }	Sidedness;

typedef /* [public][public][public] */ 
enum __MIDL___MIDL_itf_atlibdsk_0000_0002
    {	RateHD	= 0,
	RateDD	= RateHD + 1,
	RateSD	= RateDD + 1,
	RateED	= RateSD + 1
    }	DataRate;



extern RPC_IF_HANDLE __MIDL_itf_atlibdsk_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_atlibdsk_0000_v0_0_s_ifspec;

#ifndef __IGeometry_INTERFACE_DEFINED__
#define __IGeometry_INTERFACE_DEFINED__

/* interface IGeometry */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IGeometry;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("343FE9B1-6FFE-4708-9A83-227D7203193C")
    IGeometry : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_sidedness( 
            /* [retval][out] */ Sidedness __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_sidedness( 
            /* [in] */ Sidedness newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_cylinders( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_cylinders( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_heads( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_heads( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_sectors( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_sectors( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_secbase( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_secbase( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_datarate( 
            /* [retval][out] */ DataRate __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_datarate( 
            /* [in] */ DataRate newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_secsize( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_secsize( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_rwgap( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_rwgap( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_fmtgap( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_fmtgap( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_fm( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_fm( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_nomulti( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_nomulti( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_noskip( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_noskip( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ls2ps( 
            /* [in] */ long lsect,
            /* [out] */ long __RPC_FAR *cyl,
            /* [out] */ short __RPC_FAR *head,
            /* [out] */ short __RPC_FAR *sector) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE lt2pt( 
            /* [in] */ long ltrack,
            /* [out] */ long __RPC_FAR *cyl,
            /* [out] */ short __RPC_FAR *head) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ps2ls( 
            /* [in] */ long cyl,
            /* [in] */ short head,
            /* [in] */ short sector,
            /* [retval][out] */ long __RPC_FAR *lsect) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE pt2lt( 
            /* [in] */ long cyl,
            /* [in] */ short head,
            /* [retval][out] */ long __RPC_FAR *lsect) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE stdformat( 
            /* [in] */ int formatid,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [out] */ BSTR __RPC_FAR *description) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGeometryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IGeometry __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IGeometry __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IGeometry __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_sidedness )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ Sidedness __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_sidedness )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ Sidedness newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_cylinders )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_cylinders )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_heads )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_heads )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_sectors )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_sectors )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_secbase )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_secbase )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_datarate )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ DataRate __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_datarate )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ DataRate newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_secsize )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_secsize )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_rwgap )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_rwgap )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_fmtgap )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_fmtgap )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_fm )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_fm )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_nomulti )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_nomulti )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_noskip )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_noskip )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ls2ps )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ long lsect,
            /* [out] */ long __RPC_FAR *cyl,
            /* [out] */ short __RPC_FAR *head,
            /* [out] */ short __RPC_FAR *sector);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *lt2pt )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ long ltrack,
            /* [out] */ long __RPC_FAR *cyl,
            /* [out] */ short __RPC_FAR *head);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ps2ls )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ long cyl,
            /* [in] */ short head,
            /* [in] */ short sector,
            /* [retval][out] */ long __RPC_FAR *lsect);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *pt2lt )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ long cyl,
            /* [in] */ short head,
            /* [retval][out] */ long __RPC_FAR *lsect);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *stdformat )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ int formatid,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [out] */ BSTR __RPC_FAR *description);
        
        END_INTERFACE
    } IGeometryVtbl;

    interface IGeometry
    {
        CONST_VTBL struct IGeometryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGeometry_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGeometry_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGeometry_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGeometry_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IGeometry_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IGeometry_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IGeometry_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IGeometry_get_sidedness(This,pVal)	\
    (This)->lpVtbl -> get_sidedness(This,pVal)

#define IGeometry_put_sidedness(This,newVal)	\
    (This)->lpVtbl -> put_sidedness(This,newVal)

#define IGeometry_get_cylinders(This,pVal)	\
    (This)->lpVtbl -> get_cylinders(This,pVal)

#define IGeometry_put_cylinders(This,newVal)	\
    (This)->lpVtbl -> put_cylinders(This,newVal)

#define IGeometry_get_heads(This,pVal)	\
    (This)->lpVtbl -> get_heads(This,pVal)

#define IGeometry_put_heads(This,newVal)	\
    (This)->lpVtbl -> put_heads(This,newVal)

#define IGeometry_get_sectors(This,pVal)	\
    (This)->lpVtbl -> get_sectors(This,pVal)

#define IGeometry_put_sectors(This,newVal)	\
    (This)->lpVtbl -> put_sectors(This,newVal)

#define IGeometry_get_secbase(This,pVal)	\
    (This)->lpVtbl -> get_secbase(This,pVal)

#define IGeometry_put_secbase(This,newVal)	\
    (This)->lpVtbl -> put_secbase(This,newVal)

#define IGeometry_get_datarate(This,pVal)	\
    (This)->lpVtbl -> get_datarate(This,pVal)

#define IGeometry_put_datarate(This,newVal)	\
    (This)->lpVtbl -> put_datarate(This,newVal)

#define IGeometry_get_secsize(This,pVal)	\
    (This)->lpVtbl -> get_secsize(This,pVal)

#define IGeometry_put_secsize(This,newVal)	\
    (This)->lpVtbl -> put_secsize(This,newVal)

#define IGeometry_get_rwgap(This,pVal)	\
    (This)->lpVtbl -> get_rwgap(This,pVal)

#define IGeometry_put_rwgap(This,newVal)	\
    (This)->lpVtbl -> put_rwgap(This,newVal)

#define IGeometry_get_fmtgap(This,pVal)	\
    (This)->lpVtbl -> get_fmtgap(This,pVal)

#define IGeometry_put_fmtgap(This,newVal)	\
    (This)->lpVtbl -> put_fmtgap(This,newVal)

#define IGeometry_get_fm(This,pVal)	\
    (This)->lpVtbl -> get_fm(This,pVal)

#define IGeometry_put_fm(This,newVal)	\
    (This)->lpVtbl -> put_fm(This,newVal)

#define IGeometry_get_nomulti(This,pVal)	\
    (This)->lpVtbl -> get_nomulti(This,pVal)

#define IGeometry_put_nomulti(This,newVal)	\
    (This)->lpVtbl -> put_nomulti(This,newVal)

#define IGeometry_get_noskip(This,pVal)	\
    (This)->lpVtbl -> get_noskip(This,pVal)

#define IGeometry_put_noskip(This,newVal)	\
    (This)->lpVtbl -> put_noskip(This,newVal)

#define IGeometry_ls2ps(This,lsect,cyl,head,sector)	\
    (This)->lpVtbl -> ls2ps(This,lsect,cyl,head,sector)

#define IGeometry_lt2pt(This,ltrack,cyl,head)	\
    (This)->lpVtbl -> lt2pt(This,ltrack,cyl,head)

#define IGeometry_ps2ls(This,cyl,head,sector,lsect)	\
    (This)->lpVtbl -> ps2ls(This,cyl,head,sector,lsect)

#define IGeometry_pt2lt(This,cyl,head,lsect)	\
    (This)->lpVtbl -> pt2lt(This,cyl,head,lsect)

#define IGeometry_stdformat(This,formatid,name,description)	\
    (This)->lpVtbl -> stdformat(This,formatid,name,description)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_sidedness_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ Sidedness __RPC_FAR *pVal);


void __RPC_STUB IGeometry_get_sidedness_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGeometry_put_sidedness_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ Sidedness newVal);


void __RPC_STUB IGeometry_put_sidedness_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_cylinders_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IGeometry_get_cylinders_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGeometry_put_cylinders_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IGeometry_put_cylinders_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_heads_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGeometry_get_heads_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGeometry_put_heads_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGeometry_put_heads_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_sectors_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGeometry_get_sectors_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGeometry_put_sectors_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGeometry_put_sectors_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_secbase_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGeometry_get_secbase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGeometry_put_secbase_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGeometry_put_secbase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_datarate_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ DataRate __RPC_FAR *pVal);


void __RPC_STUB IGeometry_get_datarate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGeometry_put_datarate_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ DataRate newVal);


void __RPC_STUB IGeometry_put_datarate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_secsize_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IGeometry_get_secsize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGeometry_put_secsize_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IGeometry_put_secsize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_rwgap_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGeometry_get_rwgap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGeometry_put_rwgap_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGeometry_put_rwgap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_fmtgap_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGeometry_get_fmtgap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGeometry_put_fmtgap_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGeometry_put_fmtgap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_fm_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGeometry_get_fm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGeometry_put_fm_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGeometry_put_fm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_nomulti_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGeometry_get_nomulti_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGeometry_put_nomulti_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGeometry_put_nomulti_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_noskip_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IGeometry_get_noskip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IGeometry_put_noskip_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IGeometry_put_noskip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGeometry_ls2ps_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ long lsect,
    /* [out] */ long __RPC_FAR *cyl,
    /* [out] */ short __RPC_FAR *head,
    /* [out] */ short __RPC_FAR *sector);


void __RPC_STUB IGeometry_ls2ps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGeometry_lt2pt_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ long ltrack,
    /* [out] */ long __RPC_FAR *cyl,
    /* [out] */ short __RPC_FAR *head);


void __RPC_STUB IGeometry_lt2pt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGeometry_ps2ls_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ long cyl,
    /* [in] */ short head,
    /* [in] */ short sector,
    /* [retval][out] */ long __RPC_FAR *lsect);


void __RPC_STUB IGeometry_ps2ls_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGeometry_pt2lt_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ long cyl,
    /* [in] */ short head,
    /* [retval][out] */ long __RPC_FAR *lsect);


void __RPC_STUB IGeometry_pt2lt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGeometry_stdformat_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ int formatid,
    /* [out] */ BSTR __RPC_FAR *name,
    /* [out] */ BSTR __RPC_FAR *description);


void __RPC_STUB IGeometry_stdformat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IGeometry_INTERFACE_DEFINED__ */


#ifndef __IDisk_INTERFACE_DEFINED__
#define __IDisk_INTERFACE_DEFINED__

/* interface IDisk */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDisk;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2F7EE90E-646D-4AB0-8BA5-CC13DDD43C39")
    IDisk : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get_geometry( 
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *g) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE close( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE drive_status( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ short head,
            /* [retval][out] */ short __RPC_FAR *status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE pread( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ short sector,
            /* [retval][out] */ VARIANT __RPC_FAR *buf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE lread( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long lsect,
            /* [retval][out] */ VARIANT __RPC_FAR *buf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE xread( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ long cyl_expected,
            /* [in] */ short head_expected,
            /* [in] */ short sector,
            /* [in] */ long sector_len,
            /* [out][in] */ short __RPC_FAR *deleted,
            /* [retval][out] */ VARIANT __RPC_FAR *buf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE pwrite( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ VARIANT __RPC_FAR *buf,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ short sector) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE lwrite( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ VARIANT __RPC_FAR *buf,
            /* [in] */ long lsect) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE xwrite( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ VARIANT __RPC_FAR *buf,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ long cyl_expected,
            /* [in] */ short head_expected,
            /* [in] */ short sector,
            /* [in] */ long sector_len,
            /* [in] */ short deleted) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE pcheck( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ VARIANT __RPC_FAR *buf,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ short sector) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE lcheck( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ VARIANT __RPC_FAR *buf,
            /* [in] */ long lsect) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE xcheck( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ VARIANT __RPC_FAR *buf,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ long cyl_expected,
            /* [in] */ short head_expected,
            /* [in] */ short sector,
            /* [in] */ long sector_len) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE pformat( 
            /* [in] */ IGeometry __RPC_FAR *geom,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ VARIANT __RPC_FAR *format,
            /* [in] */ short filler) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE lformat( 
            /* [in] */ IGeometry __RPC_FAR *geom,
            /* [in] */ long track,
            /* [in] */ VARIANT __RPC_FAR *format,
            /* [in] */ short filler) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE apform( 
            /* [in] */ IGeometry __RPC_FAR *geom,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ short filler) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE alform( 
            /* [in] */ IGeometry __RPC_FAR *geom,
            /* [in] */ long track,
            /* [in] */ short filler) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ptread( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [retval][out] */ VARIANT __RPC_FAR *buf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ltread( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long ltrack,
            /* [retval][out] */ VARIANT __RPC_FAR *buf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE xtread( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ long cyl_expected,
            /* [in] */ short head_expected,
            /* [retval][out] */ VARIANT __RPC_FAR *buf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE psecid( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [out] */ VARIANT __RPC_FAR *buffer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE lsecid( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long track,
            /* [out] */ VARIANT __RPC_FAR *buffer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE lseek( 
            /* [in] */ IGeometry __RPC_FAR *geom,
            /* [in] */ long track) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE pseek( 
            /* [in] */ IGeometry __RPC_FAR *geom,
            /* [in] */ long cylinder,
            /* [in] */ short sector) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_comment( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_comment( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_option( 
            /* [in] */ BSTR name,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_option( 
            /* [in] */ BSTR name,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE option_enum( 
            /* [in] */ long idx,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [retval][out] */ BOOL __RPC_FAR *valid) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_retries( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_retries( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_drvname( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_drvdesc( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_compname( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_compdesc( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_dirty( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ptrkid( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [out] */ short __RPC_FAR *count,
            /* [out] */ VARIANT __RPC_FAR *buffer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ltrkid( 
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long track,
            /* [out] */ short __RPC_FAR *count,
            /* [out] */ VARIANT __RPC_FAR *buffer) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDiskVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDisk __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDisk __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDisk __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDisk __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDisk __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDisk __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDisk __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_geometry )( 
            IDisk __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *g);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *close )( 
            IDisk __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *drive_status )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ short head,
            /* [retval][out] */ short __RPC_FAR *status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *pread )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ short sector,
            /* [retval][out] */ VARIANT __RPC_FAR *buf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *lread )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long lsect,
            /* [retval][out] */ VARIANT __RPC_FAR *buf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *xread )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ long cyl_expected,
            /* [in] */ short head_expected,
            /* [in] */ short sector,
            /* [in] */ long sector_len,
            /* [out][in] */ short __RPC_FAR *deleted,
            /* [retval][out] */ VARIANT __RPC_FAR *buf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *pwrite )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ VARIANT __RPC_FAR *buf,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ short sector);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *lwrite )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ VARIANT __RPC_FAR *buf,
            /* [in] */ long lsect);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *xwrite )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ VARIANT __RPC_FAR *buf,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ long cyl_expected,
            /* [in] */ short head_expected,
            /* [in] */ short sector,
            /* [in] */ long sector_len,
            /* [in] */ short deleted);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *pcheck )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ VARIANT __RPC_FAR *buf,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ short sector);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *lcheck )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ VARIANT __RPC_FAR *buf,
            /* [in] */ long lsect);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *xcheck )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ VARIANT __RPC_FAR *buf,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ long cyl_expected,
            /* [in] */ short head_expected,
            /* [in] */ short sector,
            /* [in] */ long sector_len);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *pformat )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *geom,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ VARIANT __RPC_FAR *format,
            /* [in] */ short filler);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *lformat )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *geom,
            /* [in] */ long track,
            /* [in] */ VARIANT __RPC_FAR *format,
            /* [in] */ short filler);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *apform )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *geom,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ short filler);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *alform )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *geom,
            /* [in] */ long track,
            /* [in] */ short filler);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ptread )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [retval][out] */ VARIANT __RPC_FAR *buf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ltread )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long ltrack,
            /* [retval][out] */ VARIANT __RPC_FAR *buf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *xtread )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [in] */ long cyl_expected,
            /* [in] */ short head_expected,
            /* [retval][out] */ VARIANT __RPC_FAR *buf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *psecid )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [out] */ VARIANT __RPC_FAR *buffer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *lsecid )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long track,
            /* [out] */ VARIANT __RPC_FAR *buffer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *lseek )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *geom,
            /* [in] */ long track);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *pseek )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *geom,
            /* [in] */ long cylinder,
            /* [in] */ short sector);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_comment )( 
            IDisk __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_comment )( 
            IDisk __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_option )( 
            IDisk __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_option )( 
            IDisk __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *option_enum )( 
            IDisk __RPC_FAR * This,
            /* [in] */ long idx,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [retval][out] */ BOOL __RPC_FAR *valid);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_retries )( 
            IDisk __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_retries )( 
            IDisk __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_drvname )( 
            IDisk __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_drvdesc )( 
            IDisk __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_compname )( 
            IDisk __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_compdesc )( 
            IDisk __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_dirty )( 
            IDisk __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ptrkid )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long cylinder,
            /* [in] */ short head,
            /* [out] */ short __RPC_FAR *count,
            /* [out] */ VARIANT __RPC_FAR *buffer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ltrkid )( 
            IDisk __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *g,
            /* [in] */ long track,
            /* [out] */ short __RPC_FAR *count,
            /* [out] */ VARIANT __RPC_FAR *buffer);
        
        END_INTERFACE
    } IDiskVtbl;

    interface IDisk
    {
        CONST_VTBL struct IDiskVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDisk_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDisk_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDisk_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDisk_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDisk_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDisk_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDisk_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDisk_get_geometry(This,g)	\
    (This)->lpVtbl -> get_geometry(This,g)

#define IDisk_close(This)	\
    (This)->lpVtbl -> close(This)

#define IDisk_drive_status(This,g,head,status)	\
    (This)->lpVtbl -> drive_status(This,g,head,status)

#define IDisk_pread(This,g,cylinder,head,sector,buf)	\
    (This)->lpVtbl -> pread(This,g,cylinder,head,sector,buf)

#define IDisk_lread(This,g,lsect,buf)	\
    (This)->lpVtbl -> lread(This,g,lsect,buf)

#define IDisk_xread(This,g,cylinder,head,cyl_expected,head_expected,sector,sector_len,deleted,buf)	\
    (This)->lpVtbl -> xread(This,g,cylinder,head,cyl_expected,head_expected,sector,sector_len,deleted,buf)

#define IDisk_pwrite(This,g,buf,cylinder,head,sector)	\
    (This)->lpVtbl -> pwrite(This,g,buf,cylinder,head,sector)

#define IDisk_lwrite(This,g,buf,lsect)	\
    (This)->lpVtbl -> lwrite(This,g,buf,lsect)

#define IDisk_xwrite(This,g,buf,cylinder,head,cyl_expected,head_expected,sector,sector_len,deleted)	\
    (This)->lpVtbl -> xwrite(This,g,buf,cylinder,head,cyl_expected,head_expected,sector,sector_len,deleted)

#define IDisk_pcheck(This,g,buf,cylinder,head,sector)	\
    (This)->lpVtbl -> pcheck(This,g,buf,cylinder,head,sector)

#define IDisk_lcheck(This,g,buf,lsect)	\
    (This)->lpVtbl -> lcheck(This,g,buf,lsect)

#define IDisk_xcheck(This,g,buf,cylinder,head,cyl_expected,head_expected,sector,sector_len)	\
    (This)->lpVtbl -> xcheck(This,g,buf,cylinder,head,cyl_expected,head_expected,sector,sector_len)

#define IDisk_pformat(This,geom,cylinder,head,format,filler)	\
    (This)->lpVtbl -> pformat(This,geom,cylinder,head,format,filler)

#define IDisk_lformat(This,geom,track,format,filler)	\
    (This)->lpVtbl -> lformat(This,geom,track,format,filler)

#define IDisk_apform(This,geom,cylinder,head,filler)	\
    (This)->lpVtbl -> apform(This,geom,cylinder,head,filler)

#define IDisk_alform(This,geom,track,filler)	\
    (This)->lpVtbl -> alform(This,geom,track,filler)

#define IDisk_ptread(This,g,cylinder,head,buf)	\
    (This)->lpVtbl -> ptread(This,g,cylinder,head,buf)

#define IDisk_ltread(This,g,ltrack,buf)	\
    (This)->lpVtbl -> ltread(This,g,ltrack,buf)

#define IDisk_xtread(This,g,cylinder,head,cyl_expected,head_expected,buf)	\
    (This)->lpVtbl -> xtread(This,g,cylinder,head,cyl_expected,head_expected,buf)

#define IDisk_psecid(This,g,cylinder,head,buffer)	\
    (This)->lpVtbl -> psecid(This,g,cylinder,head,buffer)

#define IDisk_lsecid(This,g,track,buffer)	\
    (This)->lpVtbl -> lsecid(This,g,track,buffer)

#define IDisk_lseek(This,geom,track)	\
    (This)->lpVtbl -> lseek(This,geom,track)

#define IDisk_pseek(This,geom,cylinder,sector)	\
    (This)->lpVtbl -> pseek(This,geom,cylinder,sector)

#define IDisk_get_comment(This,pVal)	\
    (This)->lpVtbl -> get_comment(This,pVal)

#define IDisk_put_comment(This,newVal)	\
    (This)->lpVtbl -> put_comment(This,newVal)

#define IDisk_get_option(This,name,pVal)	\
    (This)->lpVtbl -> get_option(This,name,pVal)

#define IDisk_put_option(This,name,newVal)	\
    (This)->lpVtbl -> put_option(This,name,newVal)

#define IDisk_option_enum(This,idx,name,valid)	\
    (This)->lpVtbl -> option_enum(This,idx,name,valid)

#define IDisk_get_retries(This,pVal)	\
    (This)->lpVtbl -> get_retries(This,pVal)

#define IDisk_put_retries(This,newVal)	\
    (This)->lpVtbl -> put_retries(This,newVal)

#define IDisk_get_drvname(This,pVal)	\
    (This)->lpVtbl -> get_drvname(This,pVal)

#define IDisk_get_drvdesc(This,pVal)	\
    (This)->lpVtbl -> get_drvdesc(This,pVal)

#define IDisk_get_compname(This,pVal)	\
    (This)->lpVtbl -> get_compname(This,pVal)

#define IDisk_get_compdesc(This,pVal)	\
    (This)->lpVtbl -> get_compdesc(This,pVal)

#define IDisk_get_dirty(This,pVal)	\
    (This)->lpVtbl -> get_dirty(This,pVal)

#define IDisk_ptrkid(This,g,cylinder,head,count,buffer)	\
    (This)->lpVtbl -> ptrkid(This,g,cylinder,head,count,buffer)

#define IDisk_ltrkid(This,g,track,count,buffer)	\
    (This)->lpVtbl -> ltrkid(This,g,track,count,buffer)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_get_geometry_Proxy( 
    IDisk __RPC_FAR * This,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *g);


void __RPC_STUB IDisk_get_geometry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_close_Proxy( 
    IDisk __RPC_FAR * This);


void __RPC_STUB IDisk_close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_drive_status_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ short head,
    /* [retval][out] */ short __RPC_FAR *status);


void __RPC_STUB IDisk_drive_status_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_pread_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ long cylinder,
    /* [in] */ short head,
    /* [in] */ short sector,
    /* [retval][out] */ VARIANT __RPC_FAR *buf);


void __RPC_STUB IDisk_pread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_lread_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ long lsect,
    /* [retval][out] */ VARIANT __RPC_FAR *buf);


void __RPC_STUB IDisk_lread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_xread_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ long cylinder,
    /* [in] */ short head,
    /* [in] */ long cyl_expected,
    /* [in] */ short head_expected,
    /* [in] */ short sector,
    /* [in] */ long sector_len,
    /* [out][in] */ short __RPC_FAR *deleted,
    /* [retval][out] */ VARIANT __RPC_FAR *buf);


void __RPC_STUB IDisk_xread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_pwrite_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ VARIANT __RPC_FAR *buf,
    /* [in] */ long cylinder,
    /* [in] */ short head,
    /* [in] */ short sector);


void __RPC_STUB IDisk_pwrite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_lwrite_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ VARIANT __RPC_FAR *buf,
    /* [in] */ long lsect);


void __RPC_STUB IDisk_lwrite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_xwrite_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ VARIANT __RPC_FAR *buf,
    /* [in] */ long cylinder,
    /* [in] */ short head,
    /* [in] */ long cyl_expected,
    /* [in] */ short head_expected,
    /* [in] */ short sector,
    /* [in] */ long sector_len,
    /* [in] */ short deleted);


void __RPC_STUB IDisk_xwrite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_pcheck_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ VARIANT __RPC_FAR *buf,
    /* [in] */ long cylinder,
    /* [in] */ short head,
    /* [in] */ short sector);


void __RPC_STUB IDisk_pcheck_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_lcheck_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ VARIANT __RPC_FAR *buf,
    /* [in] */ long lsect);


void __RPC_STUB IDisk_lcheck_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_xcheck_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ VARIANT __RPC_FAR *buf,
    /* [in] */ long cylinder,
    /* [in] */ short head,
    /* [in] */ long cyl_expected,
    /* [in] */ short head_expected,
    /* [in] */ short sector,
    /* [in] */ long sector_len);


void __RPC_STUB IDisk_xcheck_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_pformat_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *geom,
    /* [in] */ long cylinder,
    /* [in] */ short head,
    /* [in] */ VARIANT __RPC_FAR *format,
    /* [in] */ short filler);


void __RPC_STUB IDisk_pformat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_lformat_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *geom,
    /* [in] */ long track,
    /* [in] */ VARIANT __RPC_FAR *format,
    /* [in] */ short filler);


void __RPC_STUB IDisk_lformat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_apform_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *geom,
    /* [in] */ long cylinder,
    /* [in] */ short head,
    /* [in] */ short filler);


void __RPC_STUB IDisk_apform_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_alform_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *geom,
    /* [in] */ long track,
    /* [in] */ short filler);


void __RPC_STUB IDisk_alform_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_ptread_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ long cylinder,
    /* [in] */ short head,
    /* [retval][out] */ VARIANT __RPC_FAR *buf);


void __RPC_STUB IDisk_ptread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_ltread_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ long ltrack,
    /* [retval][out] */ VARIANT __RPC_FAR *buf);


void __RPC_STUB IDisk_ltread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_xtread_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ long cylinder,
    /* [in] */ short head,
    /* [in] */ long cyl_expected,
    /* [in] */ short head_expected,
    /* [retval][out] */ VARIANT __RPC_FAR *buf);


void __RPC_STUB IDisk_xtread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_psecid_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ long cylinder,
    /* [in] */ short head,
    /* [out] */ VARIANT __RPC_FAR *buffer);


void __RPC_STUB IDisk_psecid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_lsecid_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ long track,
    /* [out] */ VARIANT __RPC_FAR *buffer);


void __RPC_STUB IDisk_lsecid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_lseek_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *geom,
    /* [in] */ long track);


void __RPC_STUB IDisk_lseek_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_pseek_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *geom,
    /* [in] */ long cylinder,
    /* [in] */ short sector);


void __RPC_STUB IDisk_pseek_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDisk_get_comment_Proxy( 
    IDisk __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IDisk_get_comment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDisk_put_comment_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IDisk_put_comment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDisk_get_option_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IDisk_get_option_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDisk_put_option_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [in] */ long newVal);


void __RPC_STUB IDisk_put_option_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_option_enum_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ long idx,
    /* [out] */ BSTR __RPC_FAR *name,
    /* [retval][out] */ BOOL __RPC_FAR *valid);


void __RPC_STUB IDisk_option_enum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDisk_get_retries_Proxy( 
    IDisk __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IDisk_get_retries_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDisk_put_retries_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IDisk_put_retries_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDisk_get_drvname_Proxy( 
    IDisk __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IDisk_get_drvname_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDisk_get_drvdesc_Proxy( 
    IDisk __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IDisk_get_drvdesc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDisk_get_compname_Proxy( 
    IDisk __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IDisk_get_compname_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDisk_get_compdesc_Proxy( 
    IDisk __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IDisk_get_compdesc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDisk_get_dirty_Proxy( 
    IDisk __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IDisk_get_dirty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_ptrkid_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ long cylinder,
    /* [in] */ short head,
    /* [out] */ short __RPC_FAR *count,
    /* [out] */ VARIANT __RPC_FAR *buffer);


void __RPC_STUB IDisk_ptrkid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDisk_ltrkid_Proxy( 
    IDisk __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *g,
    /* [in] */ long track,
    /* [out] */ short __RPC_FAR *count,
    /* [out] */ VARIANT __RPC_FAR *buffer);


void __RPC_STUB IDisk_ltrkid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDisk_INTERFACE_DEFINED__ */


#ifndef __IReporter_INTERFACE_DEFINED__
#define __IReporter_INTERFACE_DEFINED__

/* interface IReporter */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IReporter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3DED12BB-ABA5-48C3-989A-9D6AA8ED542D")
    IReporter : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE report( 
            /* [string][in] */ BSTR message) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE endreport( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IReporterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IReporter __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IReporter __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IReporter __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IReporter __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IReporter __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IReporter __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IReporter __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *report )( 
            IReporter __RPC_FAR * This,
            /* [string][in] */ BSTR message);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *endreport )( 
            IReporter __RPC_FAR * This);
        
        END_INTERFACE
    } IReporterVtbl;

    interface IReporter
    {
        CONST_VTBL struct IReporterVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IReporter_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IReporter_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IReporter_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IReporter_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IReporter_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IReporter_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IReporter_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IReporter_report(This,message)	\
    (This)->lpVtbl -> report(This,message)

#define IReporter_endreport(This)	\
    (This)->lpVtbl -> endreport(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IReporter_report_Proxy( 
    IReporter __RPC_FAR * This,
    /* [string][in] */ BSTR message);


void __RPC_STUB IReporter_report_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IReporter_endreport_Proxy( 
    IReporter __RPC_FAR * This);


void __RPC_STUB IReporter_endreport_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IReporter_INTERFACE_DEFINED__ */


#ifndef __ILibrary_INTERFACE_DEFINED__
#define __ILibrary_INTERFACE_DEFINED__

/* interface ILibrary */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ILibrary;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("84FC5CD6-91AB-42B3-BCEC-F99BCC20EDF0")
    ILibrary : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE open( 
            /* [in] */ BSTR filename,
            /* [optional][in] */ BSTR driver,
            /* [optional][in] */ BSTR compress,
            /* [retval][out] */ IDisk __RPC_FAR *__RPC_FAR *ppDisk) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE create( 
            /* [in] */ BSTR filename,
            /* [in] */ BSTR driver,
            /* [optional][in] */ BSTR compress,
            /* [retval][out] */ IDisk __RPC_FAR *__RPC_FAR *ppDisk) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE stdformat( 
            /* [in] */ int formatid,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [out] */ BSTR __RPC_FAR *description,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geom) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get_psh( 
            /* [in] */ long sectorsize,
            /* [retval][out] */ short __RPC_FAR *shift) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_stdformat_count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE dosgeom( 
            /* [in] */ VARIANT __RPC_FAR *buffer,
            /* [out] */ IGeometry __RPC_FAR *__RPC_FAR *geom) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE cpm86geom( 
            /* [in] */ VARIANT __RPC_FAR *buffer,
            /* [out] */ IGeometry __RPC_FAR *__RPC_FAR *geom) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE pcwgeom( 
            /* [in] */ VARIANT __RPC_FAR *buffer,
            /* [out] */ IGeometry __RPC_FAR *__RPC_FAR *geom) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE aprigeom( 
            /* [in] */ VARIANT __RPC_FAR *buffer,
            /* [out] */ IGeometry __RPC_FAR *__RPC_FAR *geom) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE type_enum( 
            /* [in] */ long idx,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [retval][out] */ BOOL __RPC_FAR *valid) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE comp_enum( 
            /* [in] */ long idx,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [retval][out] */ BOOL __RPC_FAR *valid) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_reporter( 
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_reporter( 
            /* [in] */ IDispatch __RPC_FAR *newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILibraryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ILibrary __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ILibrary __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ILibrary __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *open )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ BSTR filename,
            /* [optional][in] */ BSTR driver,
            /* [optional][in] */ BSTR compress,
            /* [retval][out] */ IDisk __RPC_FAR *__RPC_FAR *ppDisk);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *create )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ BSTR filename,
            /* [in] */ BSTR driver,
            /* [optional][in] */ BSTR compress,
            /* [retval][out] */ IDisk __RPC_FAR *__RPC_FAR *ppDisk);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *stdformat )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ int formatid,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [out] */ BSTR __RPC_FAR *description,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geom);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_psh )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ long sectorsize,
            /* [retval][out] */ short __RPC_FAR *shift);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_stdformat_count )( 
            ILibrary __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *dosgeom )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ VARIANT __RPC_FAR *buffer,
            /* [out] */ IGeometry __RPC_FAR *__RPC_FAR *geom);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *cpm86geom )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ VARIANT __RPC_FAR *buffer,
            /* [out] */ IGeometry __RPC_FAR *__RPC_FAR *geom);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *pcwgeom )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ VARIANT __RPC_FAR *buffer,
            /* [out] */ IGeometry __RPC_FAR *__RPC_FAR *geom);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *aprigeom )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ VARIANT __RPC_FAR *buffer,
            /* [out] */ IGeometry __RPC_FAR *__RPC_FAR *geom);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *type_enum )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ long idx,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [retval][out] */ BOOL __RPC_FAR *valid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *comp_enum )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ long idx,
            /* [out] */ BSTR __RPC_FAR *name,
            /* [retval][out] */ BOOL __RPC_FAR *valid);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_reporter )( 
            ILibrary __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_reporter )( 
            ILibrary __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *newVal);
        
        END_INTERFACE
    } ILibraryVtbl;

    interface ILibrary
    {
        CONST_VTBL struct ILibraryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILibrary_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILibrary_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILibrary_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILibrary_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ILibrary_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ILibrary_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ILibrary_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ILibrary_open(This,filename,driver,compress,ppDisk)	\
    (This)->lpVtbl -> open(This,filename,driver,compress,ppDisk)

#define ILibrary_create(This,filename,driver,compress,ppDisk)	\
    (This)->lpVtbl -> create(This,filename,driver,compress,ppDisk)

#define ILibrary_stdformat(This,formatid,name,description,geom)	\
    (This)->lpVtbl -> stdformat(This,formatid,name,description,geom)

#define ILibrary_get_psh(This,sectorsize,shift)	\
    (This)->lpVtbl -> get_psh(This,sectorsize,shift)

#define ILibrary_get_stdformat_count(This,pVal)	\
    (This)->lpVtbl -> get_stdformat_count(This,pVal)

#define ILibrary_dosgeom(This,buffer,geom)	\
    (This)->lpVtbl -> dosgeom(This,buffer,geom)

#define ILibrary_cpm86geom(This,buffer,geom)	\
    (This)->lpVtbl -> cpm86geom(This,buffer,geom)

#define ILibrary_pcwgeom(This,buffer,geom)	\
    (This)->lpVtbl -> pcwgeom(This,buffer,geom)

#define ILibrary_aprigeom(This,buffer,geom)	\
    (This)->lpVtbl -> aprigeom(This,buffer,geom)

#define ILibrary_type_enum(This,idx,name,valid)	\
    (This)->lpVtbl -> type_enum(This,idx,name,valid)

#define ILibrary_comp_enum(This,idx,name,valid)	\
    (This)->lpVtbl -> comp_enum(This,idx,name,valid)

#define ILibrary_get_reporter(This,pVal)	\
    (This)->lpVtbl -> get_reporter(This,pVal)

#define ILibrary_put_reporter(This,newVal)	\
    (This)->lpVtbl -> put_reporter(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILibrary_open_Proxy( 
    ILibrary __RPC_FAR * This,
    /* [in] */ BSTR filename,
    /* [optional][in] */ BSTR driver,
    /* [optional][in] */ BSTR compress,
    /* [retval][out] */ IDisk __RPC_FAR *__RPC_FAR *ppDisk);


void __RPC_STUB ILibrary_open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILibrary_create_Proxy( 
    ILibrary __RPC_FAR * This,
    /* [in] */ BSTR filename,
    /* [in] */ BSTR driver,
    /* [optional][in] */ BSTR compress,
    /* [retval][out] */ IDisk __RPC_FAR *__RPC_FAR *ppDisk);


void __RPC_STUB ILibrary_create_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILibrary_stdformat_Proxy( 
    ILibrary __RPC_FAR * This,
    /* [in] */ int formatid,
    /* [out] */ BSTR __RPC_FAR *name,
    /* [out] */ BSTR __RPC_FAR *description,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geom);


void __RPC_STUB ILibrary_stdformat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILibrary_get_psh_Proxy( 
    ILibrary __RPC_FAR * This,
    /* [in] */ long sectorsize,
    /* [retval][out] */ short __RPC_FAR *shift);


void __RPC_STUB ILibrary_get_psh_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILibrary_get_stdformat_count_Proxy( 
    ILibrary __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ILibrary_get_stdformat_count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILibrary_dosgeom_Proxy( 
    ILibrary __RPC_FAR * This,
    /* [in] */ VARIANT __RPC_FAR *buffer,
    /* [out] */ IGeometry __RPC_FAR *__RPC_FAR *geom);


void __RPC_STUB ILibrary_dosgeom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILibrary_cpm86geom_Proxy( 
    ILibrary __RPC_FAR * This,
    /* [in] */ VARIANT __RPC_FAR *buffer,
    /* [out] */ IGeometry __RPC_FAR *__RPC_FAR *geom);


void __RPC_STUB ILibrary_cpm86geom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILibrary_pcwgeom_Proxy( 
    ILibrary __RPC_FAR * This,
    /* [in] */ VARIANT __RPC_FAR *buffer,
    /* [out] */ IGeometry __RPC_FAR *__RPC_FAR *geom);


void __RPC_STUB ILibrary_pcwgeom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILibrary_aprigeom_Proxy( 
    ILibrary __RPC_FAR * This,
    /* [in] */ VARIANT __RPC_FAR *buffer,
    /* [out] */ IGeometry __RPC_FAR *__RPC_FAR *geom);


void __RPC_STUB ILibrary_aprigeom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILibrary_type_enum_Proxy( 
    ILibrary __RPC_FAR * This,
    /* [in] */ long idx,
    /* [out] */ BSTR __RPC_FAR *name,
    /* [retval][out] */ BOOL __RPC_FAR *valid);


void __RPC_STUB ILibrary_type_enum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILibrary_comp_enum_Proxy( 
    ILibrary __RPC_FAR * This,
    /* [in] */ long idx,
    /* [out] */ BSTR __RPC_FAR *name,
    /* [retval][out] */ BOOL __RPC_FAR *valid);


void __RPC_STUB ILibrary_comp_enum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILibrary_get_reporter_Proxy( 
    ILibrary __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB ILibrary_get_reporter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ILibrary_put_reporter_Proxy( 
    ILibrary __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *newVal);


void __RPC_STUB ILibrary_put_reporter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILibrary_INTERFACE_DEFINED__ */



#ifndef __LIBDSK_LIBRARY_DEFINED__
#define __LIBDSK_LIBRARY_DEFINED__

/* library LIBDSK */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_LIBDSK;

EXTERN_C const CLSID CLSID_Library;

#ifdef __cplusplus

class DECLSPEC_UUID("6AAA65C3-2CEA-4F6B-AB2B-FBDE9D2D484E")
Library;
#endif

EXTERN_C const CLSID CLSID_Disk;

#ifdef __cplusplus

class DECLSPEC_UUID("6A6B3263-176B-4D21-81A9-8F5EEF739FA9")
Disk;
#endif

EXTERN_C const CLSID CLSID_Geometry;

#ifdef __cplusplus

class DECLSPEC_UUID("1AFF2A50-EEE7-4233-83EB-EC1C9EBAFA74")
Geometry;
#endif

EXTERN_C const CLSID CLSID_Reporter;

#ifdef __cplusplus

class DECLSPEC_UUID("F011746E-3431-4C26-858A-D04D1454637C")
Reporter;
#endif
#endif /* __LIBDSK_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
