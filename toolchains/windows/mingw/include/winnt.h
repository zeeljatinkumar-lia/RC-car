/*
 * winnt.h
 *
 * Win32-API macros and definitions originating from the WinNT product line.
 *
 * $Id: winnt.h,v faa0e77070d8 2019/06/07 20:53:34 keith $
 *
 * Written by Anders Norlander <anorland@hem2.passagen.se>
 * Copyright (C) 1998-2011, 2016, 2017, 2019, MinGW.org Project
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */
#ifndef _WINNT_H
#pragma GCC system_header

/* For effective self-containment, <winnt.h> requires <windef.h> to have
 * been included, BEFORE defining the _WINNT_H repeat inclusion guard...
 */
#include <windef.h>
/* ...but this has the unfortunate side effect that <windef.h> may then
 * recursively include <winnt.h>; retest the inclusion guard, to break
 * out of any ensuing recursive inclusion loop.
 */
#ifndef _WINNT_H
#define _WINNT_H

#include <winerror.h>

#ifndef RC_INVOKED
#include <string.h>

_BEGIN_C_DECLS

/* FIXME: add more architectures. Is there a way to specify this in GCC? */
#ifdef _X86_
#define UNALIGNED
#else
#define UNALIGNED
#endif

#ifndef DECLSPEC_ALIGN
#ifdef __GNUC__
#define DECLSPEC_ALIGN(x)   __attribute__((__aligned__(x)))
#else
#define DECLSPEC_ALIGN(x)
#endif
#endif

#ifndef DECLSPEC_SELECTANY
#if (__GNUC__ >= 4)
#define DECLSPEC_SELECTANY  __attribute__((__selectany__))
#else
#define DECLSPEC_SELECTANY
#endif
#endif

#ifndef FORCEINLINE
#if (__GNUC__ >= 3)
#define FORCEINLINE	    __inline__ __attribute__((__always_inline__))
#else
#define FORCEINLINE	    __inline__
#endif
#endif

#ifndef C_ASSERT
#define C_ASSERT(expr)  typedef char __C_ASSERT__[(expr)?1:-1]
#endif

#ifndef VOID
#define VOID		void
#endif
typedef char		CHAR;
typedef short		SHORT;
typedef long		LONG;
typedef char		CCHAR,  *PCCHAR;
typedef unsigned char   UCHAR,  *PUCHAR;
typedef unsigned short	USHORT, *PUSHORT;
typedef unsigned long	ULONG,  *PULONG;
typedef char		*PSZ;

typedef void		*PVOID, *LPVOID;

/* FIXME for __WIN64 */
#ifndef __ptr64
#define __ptr64
#endif
typedef void		*__ptr64 PVOID64;

#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
#ifndef _WCHAR_T_
#define _WCHAR_T_
#undef __need_wchar_t
#ifndef __cplusplus
typedef unsigned short wchar_t;
#endif
#endif
#endif

typedef wchar_t WCHAR;
typedef WCHAR *PWCHAR, *LPWCH, *PWCH, *NWPSTR, *LPWSTR, *PWSTR;
typedef CONST WCHAR *LPCWCH, *PCWCH, *LPCWSTR, *PCWSTR;
typedef CHAR *PCHAR, *LPCH, *PCH, *NPSTR, *LPSTR, *PSTR;
typedef CONST CHAR *LPCCH, *PCSTR, *LPCSTR;
#ifndef _TCHAR_DEFINED
#define _TCHAR_DEFINED
#ifdef UNICODE
/*
 * NOTE: This tests UNICODE, which is different from the _UNICODE define
 *       used to differentiate standard C runtime calls.
 */
typedef WCHAR TCHAR;
typedef WCHAR _TCHAR;
#else
typedef CHAR TCHAR;
typedef CHAR _TCHAR;
#endif
#endif
typedef TCHAR TBYTE, *PTCH, *PTBYTE;
typedef TCHAR *LPTCH, *PTSTR, *LPTSTR, *LP, *PTCHAR;
typedef const TCHAR *LPCTSTR;
#ifdef UNICODE
/*
 * __TEXT is a private macro whose specific use is to force the expansion of a
 * macro passed as an argument to the macro TEXT.  DO NOT use this
 * macro within your programs.  It's name and function could change without
 * notice.
 */
#define __TEXT(q) L##q
#else
#define __TEXT(q) q
#endif
/*
 * UNICODE a constant string when UNICODE is defined, else returns the string
 * unmodified.
 * The corresponding macros  _TEXT() and _T() for mapping _UNICODE strings
 * passed to C runtime functions are defined in mingw/tchar.h
 */
#define TEXT(q) 	 __TEXT(q)
typedef SHORT *PSHORT;
typedef LONG *PLONG;
#ifdef STRICT
typedef void *HANDLE;
#define DECLARE_HANDLE(n)  typedef struct n##__{int i;}*n
#else
typedef PVOID HANDLE;
#define DECLARE_HANDLE(n)  typedef HANDLE n
#endif
typedef HANDLE *PHANDLE, *LPHANDLE;
typedef DWORD LCID;
typedef PDWORD PLCID;
typedef WORD LANGID;
#ifdef __GNUC__
#define _HAVE_INT64
#define _INTEGRAL_MAX_BITS  64
#undef __int64
#define __int64 long long
#elif defined(__WATCOMC__) && (defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64)
#define _HAVE_INT64
#endif /* __GNUC__/__WATCOMC */
#if defined(_HAVE_INT64) || (defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64)
typedef __int64 LONGLONG;
typedef unsigned __int64 DWORDLONG;
#else
typedef double LONGLONG, DWORDLONG;
#endif
typedef LONGLONG *PLONGLONG;
typedef DWORDLONG *PDWORDLONG;
typedef DWORDLONG ULONGLONG, *PULONGLONG;
typedef LONGLONG USN;
#ifdef _HAVE_INT64
#define Int32x32To64(a,b)		((LONGLONG)(a)*(LONGLONG)(b))
#define UInt32x32To64(a,b)		((DWORDLONG)(a)*(DWORDLONG)(b))
#define Int64ShllMod32(a,b)		((DWORDLONG)(a)<<(b))
#define Int64ShraMod32(a,b)		((LONGLONG)(a)>>(b))
#define Int64ShrlMod32(a,b)		((DWORDLONG)(a)>>(b))
#endif
#define ANSI_NULL				   '\0'
#define UNICODE_NULL				  L'\0'
typedef BYTE BOOLEAN, *PBOOLEAN;

_END_C_DECLS

#endif	/* ! RC_INVOKED */

typedef BYTE FCHAR;
typedef WORD FSHORT;
typedef DWORD FLONG;

#define NTAPI  __stdcall
#include <basetsd.h>
#define ACE_OBJECT_TYPE_PRESENT 		0x00000001
#define ACE_INHERITED_OBJECT_TYPE_PRESENT	0x00000002
#define APPLICATION_ERROR_MASK			0x20000000
#define ERROR_SEVERITY_SUCCESS			0x00000000
#define ERROR_SEVERITY_INFORMATIONAL		0x40000000
#define ERROR_SEVERITY_WARNING			0x80000000
#define ERROR_SEVERITY_ERROR			0xC0000000
/* also in ddk/ntifs.h */
#define COMPRESSION_FORMAT_NONE 		   (0x0000)
#define COMPRESSION_FORMAT_DEFAULT		   (0x0001)
#define COMPRESSION_FORMAT_LZNT1		   (0x0002)
#define COMPRESSION_ENGINE_STANDARD		   (0x0000)
#define COMPRESSION_ENGINE_MAXIMUM		   (0x0100)
#define COMPRESSION_ENGINE_HIBER		   (0x0200)
#define ACCESS_ALLOWED_ACE_TYPE 		      (0x0)
#define ACCESS_DENIED_ACE_TYPE			      (0x1)
#define SYSTEM_AUDIT_ACE_TYPE			      (0x2)
#define SYSTEM_ALARM_ACE_TYPE			      (0x3)
/*end ntifs.h */
#define ANYSIZE_ARRAY					 1
#define OBJECT_INHERIT_ACE				 1
#define CONTAINER_INHERIT_ACE				 2
#define NO_PROPAGATE_INHERIT_ACE			 4
#define INHERIT_ONLY_ACE				 8
#define INHERITED_ACE					16

#define VALID_INHERIT_FLAGS  \
    OBJECT_INHERIT_ACE | CONTAINER_INHERIT_ACE | NO_PROPAGATE_INHERIT_ACE | \
    INHERIT_ONLY_ACE   | INHERITED_ACE

#define SUCCESSFUL_ACCESS_ACE_FLAG			64
#define FAILED_ACCESS_ACE_FLAG			       128
#define DELETE					0x00010000L
#define READ_CONTROL				   0x20000L
#define WRITE_DAC				   0x40000L
#define WRITE_OWNER				   0x80000L
#define SYNCHRONIZE				  0x100000L
#define STANDARD_RIGHTS_REQUIRED		   0xF0000
#define STANDARD_RIGHTS_READ			   0x20000
#define STANDARD_RIGHTS_WRITE			   0x20000
#define STANDARD_RIGHTS_EXECUTE 		   0x20000
#define STANDARD_RIGHTS_ALL			  0x1F0000
#define SPECIFIC_RIGHTS_ALL			    0xFFFF
#define ACCESS_SYSTEM_SECURITY			 0x1000000

#define MAXIMUM_ALLOWED 			 0x2000000
#define GENERIC_READ				0x80000000
#define GENERIC_WRITE				0x40000000
#define GENERIC_EXECUTE 			0x20000000
#define GENERIC_ALL				0x10000000

#define INVALID_FILE_ATTRIBUTES 		((DWORD)-1)

/* Also in ddk/winddk.h */
#define FILE_LIST_DIRECTORY			0x00000001
#define FILE_READ_DATA				0x00000001
#define FILE_ADD_FILE				0x00000002
#define FILE_WRITE_DATA 			0x00000002
#define FILE_ADD_SUBDIRECTORY			0x00000004
#define FILE_APPEND_DATA			0x00000004
#define FILE_CREATE_PIPE_INSTANCE		0x00000004
#define FILE_READ_EA				0x00000008
#define FILE_READ_PROPERTIES			0x00000008
#define FILE_WRITE_EA				0x00000010
#define FILE_WRITE_PROPERTIES			0x00000010
#define FILE_EXECUTE				0x00000020
#define FILE_TRAVERSE				0x00000020
#define FILE_DELETE_CHILD			0x00000040
#define FILE_READ_ATTRIBUTES			0x00000080
#define FILE_WRITE_ATTRIBUTES			0x00000100

#define FILE_SHARE_READ 			0x00000001
#define FILE_SHARE_WRITE			0x00000002
#define FILE_SHARE_DELETE			0x00000004
#define FILE_SHARE_VALID_FLAGS			0x00000007

#define FILE_ATTRIBUTE_READONLY 		0x00000001
#define FILE_ATTRIBUTE_HIDDEN			0x00000002
#define FILE_ATTRIBUTE_SYSTEM			0x00000004
#define FILE_ATTRIBUTE_DIRECTORY		0x00000010
#define FILE_ATTRIBUTE_ARCHIVE			0x00000020
#define FILE_ATTRIBUTE_DEVICE			0x00000040
#define FILE_ATTRIBUTE_NORMAL			0x00000080
#define FILE_ATTRIBUTE_TEMPORARY		0x00000100
#define FILE_ATTRIBUTE_SPARSE_FILE		0x00000200
#define FILE_ATTRIBUTE_REPARSE_POINT		0x00000400
#define FILE_ATTRIBUTE_COMPRESSED		0x00000800
#define FILE_ATTRIBUTE_OFFLINE			0x00001000
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED	0x00002000
#define FILE_ATTRIBUTE_ENCRYPTED		0x00004000
#define FILE_ATTRIBUTE_VIRTUAL			0x00010000
#define FILE_ATTRIBUTE_VALID_FLAGS		0x00017fb7
#define FILE_ATTRIBUTE_VALID_SET_FLAGS		0x000031a7

#define FILE_COPY_STRUCTURED_STORAGE		0x00000041
#define FILE_STRUCTURED_STORAGE 		0x00000441

#define FILE_VALID_OPTION_FLAGS 		0x00ffffff
#define FILE_VALID_PIPE_OPTION_FLAGS		0x00000032
#define FILE_VALID_MAILSLOT_OPTION_FLAGS	0x00000032
#define FILE_VALID_SET_FLAGS			0x00000036

#define FILE_SUPERSEDE				0x00000000
#define FILE_OPEN				0x00000001
#define FILE_CREATE				0x00000002
#define FILE_OPEN_IF				0x00000003
#define FILE_OVERWRITE				0x00000004
#define FILE_OVERWRITE_IF			0x00000005
#define FILE_MAXIMUM_DISPOSITION		0x00000005

#define FILE_DIRECTORY_FILE			0x00000001
#define FILE_WRITE_THROUGH			0x00000002
#define FILE_SEQUENTIAL_ONLY			0x00000004
#define FILE_NO_INTERMEDIATE_BUFFERING		0x00000008
#define FILE_SYNCHRONOUS_IO_ALERT		0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT		0x00000020
#define FILE_NON_DIRECTORY_FILE 		0x00000040
#define FILE_CREATE_TREE_CONNECTION		0x00000080
#define FILE_COMPLETE_IF_OPLOCKED		0x00000100
#define FILE_NO_EA_KNOWLEDGE			0x00000200
#define FILE_OPEN_FOR_RECOVERY			0x00000400
#define FILE_RANDOM_ACCESS			0x00000800
#define FILE_DELETE_ON_CLOSE			0x00001000
#define FILE_OPEN_BY_FILE_ID			0x00002000
#define FILE_OPEN_FOR_BACKUP_INTENT		0x00004000
#define FILE_NO_COMPRESSION			0x00008000
#define FILE_RESERVE_OPFILTER			0x00100000
#define FILE_OPEN_REPARSE_POINT 		0x00200000
#define FILE_OPEN_NO_RECALL			0x00400000
#define FILE_OPEN_FOR_FREE_SPACE_QUERY		0x00800000

#define FILE_ALL_ACCESS     ( STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x1FF )

#define FILE_GENERIC_EXECUTE	\
  ( STANDARD_RIGHTS_EXECUTE | FILE_READ_ATTRIBUTES | FILE_EXECUTE | SYNCHRONIZE \
  )
#define FILE_GENERIC_READ   \
  ( STANDARD_RIGHTS_READ    | FILE_READ_DATA       | FILE_READ_ATTRIBUTES  | \
    FILE_READ_EA	    | SYNCHRONIZE	   \
  )
#define FILE_GENERIC_WRITE  \
  ( STANDARD_RIGHTS_WRITE   | FILE_WRITE_DATA      | FILE_WRITE_ATTRIBUTES | \
    FILE_WRITE_EA	    | FILE_APPEND_DATA	   | SYNCHRONIZE	   \
  )
/* end winddk.h */
/* also in ddk/ntifs.h */
#define FILE_NOTIFY_CHANGE_FILE_NAME		0x00000001
#define FILE_NOTIFY_CHANGE_DIR_NAME		0x00000002
#define FILE_NOTIFY_CHANGE_NAME 		0x00000003
#define FILE_NOTIFY_CHANGE_ATTRIBUTES		0x00000004
#define FILE_NOTIFY_CHANGE_SIZE 		0x00000008
#define FILE_NOTIFY_CHANGE_LAST_WRITE		0x00000010
#define FILE_NOTIFY_CHANGE_LAST_ACCESS		0x00000020
#define FILE_NOTIFY_CHANGE_CREATION		0x00000040
#define FILE_NOTIFY_CHANGE_EA			0x00000080
#define FILE_NOTIFY_CHANGE_SECURITY		0x00000100
#define FILE_NOTIFY_CHANGE_STREAM_NAME		0x00000200
#define FILE_NOTIFY_CHANGE_STREAM_SIZE		0x00000400
#define FILE_NOTIFY_CHANGE_STREAM_WRITE 	0x00000800
#define FILE_NOTIFY_VALID_MASK			0x00000fff

#define FILE_CASE_SENSITIVE_SEARCH		0x00000001
#define FILE_CASE_PRESERVED_NAMES		0x00000002
#define FILE_UNICODE_ON_DISK			0x00000004
#define FILE_PERSISTENT_ACLS			0x00000008
#define FILE_FILE_COMPRESSION			0x00000010
#define FILE_VOLUME_QUOTAS			0x00000020
#define FILE_SUPPORTS_SPARSE_FILES		0x00000040
#define FILE_SUPPORTS_REPARSE_POINTS		0x00000080
#define FILE_SUPPORTS_REMOTE_STORAGE		0x00000100
#define FS_LFN_APIS				0x00004000
#define FILE_VOLUME_IS_COMPRESSED		0x00008000
#define FILE_SUPPORTS_OBJECT_IDS		0x00010000
#define FILE_SUPPORTS_ENCRYPTION		0x00020000
#define FILE_NAMED_STREAMS			0x00040000
#define FILE_READ_ONLY_VOLUME			0x00080000
#define FILE_SEQUENTIAL_WRITE_ONCE		0x00100000
#define FILE_SUPPORTS_TRANSACTIONS		0x00200000
/* Note: These flags only have a meaning starting with Windows 7/2008 R2.
   Their absence on older OSes does NOT mean that a filesystem is missing
   that property. */
#define FILE_SUPPORTS_HARD_LINKS		0x00400000
#define FILE_SUPPORTS_EXTENDED_ATTRIBUTES	0x00800000
#define FILE_SUPPORTS_OPEN_BY_FILE_ID		0x01000000
#define FILE_SUPPORTS_USN_JOURNAL		0x02000000


#define IO_COMPLETION_QUERY_STATE		    0x0001
#define IO_COMPLETION_MODIFY_STATE		    0x0002
#define IO_COMPLETION_ALL_ACCESS	(STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|0x3)
/* end ntifs.h */

/* also in ddk/winddk.h */
#define DUPLICATE_CLOSE_SOURCE			0x00000001
#define DUPLICATE_SAME_ACCESS			0x00000002
#define DUPLICATE_SAME_ATTRIBUTES		0x00000004
/* end winddk.k */

#define MAILSLOT_NO_MESSAGE			((DWORD)-1)
#define MAILSLOT_WAIT_FOREVER			((DWORD)-1)
#define PROCESS_TERMINATE				 1
#define PROCESS_CREATE_THREAD				 2
#define PROCESS_SET_SESSIONID				 4
#define PROCESS_VM_OPERATION				 8
#define PROCESS_VM_READ 				16
#define PROCESS_VM_WRITE				32
#define PROCESS_DUP_HANDLE				64
#define PROCESS_CREATE_PROCESS			       128
#define PROCESS_SET_QUOTA			       256
#define PROCESS_SUSPEND_RESUME			    0x0800
#define PROCESS_SET_INFORMATION 		       512
#define PROCESS_QUERY_INFORMATION		      1024
#define PROCESS_QUERY_LIMITED_INFORMATION	    0x1000

#define PROCESS_ALL_ACCESS		(STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|0xFFF)
#define THREAD_TERMINATE				 1
#define THREAD_SUSPEND_RESUME				 2
#define THREAD_GET_CONTEXT				 8
#define THREAD_SET_CONTEXT				16
#define THREAD_SET_INFORMATION				32
#define THREAD_QUERY_INFORMATION			64
#define THREAD_SET_THREAD_TOKEN 		       128
#define THREAD_IMPERSONATE			       256
#define THREAD_DIRECT_IMPERSONATION		     0x200
#define THREAD_ALL_ACCESS		(STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|0x3FF)
#define THREAD_BASE_PRIORITY_LOWRT			15
#define THREAD_BASE_PRIORITY_MAX			 2
#define THREAD_BASE_PRIORITY_MIN		       (-2)
#define THREAD_BASE_PRIORITY_IDLE		      (-15)
#define EXCEPTION_NONCONTINUABLE			 1
#define EXCEPTION_MAXIMUM_PARAMETERS			15
#define MUTANT_QUERY_STATE			    0x0001
#define MUTANT_ALL_ACCESS		(STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|MUTANT_QUERY_STATE)
#define TIMER_QUERY_STATE			    0x0001
#define TIMER_MODIFY_STATE			    0x0002
#define TIMER_ALL_ACCESS		(STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|TIMER_QUERY_STATE|TIMER_MODIFY_STATE)
/*
 * To prevent gcc compiler warnings, bracket these defines when initialising
 * a  SID_IDENTIFIER_AUTHORITY, eg.
 * SID_IDENTIFIER_AUTHORITY aNullSidAuthority = {SECURITY_NULL_SID_AUTHORITY};
 */
#define SECURITY_NULL_SID_AUTHORITY		{0,0,0,0,0,0}
#define SECURITY_WORLD_SID_AUTHORITY		{0,0,0,0,0,1}
#define SECURITY_LOCAL_SID_AUTHORITY		{0,0,0,0,0,2}
#define SECURITY_CREATOR_SID_AUTHORITY		{0,0,0,0,0,3}
#define SECURITY_NON_UNIQUE_AUTHORITY		{0,0,0,0,0,4}
#define SECURITY_NT_AUTHORITY			{0,0,0,0,0,5}
#define SECURITY_MANDATORY_LABEL_AUTHORITY	{0,0,0,0,0,16}
#define SECURITY_NULL_RID				 0
#define SECURITY_WORLD_RID			(0x00000000L)
#define SECURITY_LOCAL_RID				 0
#define SECURITY_CREATOR_OWNER_RID			 0
#define SECURITY_CREATOR_GROUP_RID			 1
#define SECURITY_DIALUP_RID				 1
#define SECURITY_NETWORK_RID				 2
#define SECURITY_BATCH_RID				 3
#define SECURITY_INTERACTIVE_RID			 4
#define SECURITY_LOGON_IDS_RID				 5
#define SECURITY_SERVICE_RID				 6
#define SECURITY_LOCAL_SYSTEM_RID			18
#define SECURITY_BUILTIN_DOMAIN_RID			32
#define SECURITY_PRINCIPAL_SELF_RID			10
#define SECURITY_CREATOR_OWNER_SERVER_RID	       0x2
#define SECURITY_CREATOR_GROUP_SERVER_RID	       0x3
#define SECURITY_LOGON_IDS_RID_COUNT		       0x3
#define SECURITY_ANONYMOUS_LOGON_RID		       0x7
#define SECURITY_PROXY_RID			       0x8
#define SECURITY_ENTERPRISE_CONTROLLERS_RID	       0x9
#define SECURITY_SERVER_LOGON_RID		SECURITY_ENTERPRISE_CONTROLLERS_RID
#define SECURITY_AUTHENTICATED_USER_RID 	       0xB
#define SECURITY_RESTRICTED_CODE_RID		       0xC
#define SECURITY_NT_NON_UNIQUE_RID		      0x15
#define SID_REVISION					 1
#if (_WIN32_WINNT >= 0x0600)
#define SID_HASH_SIZE					32
#endif
#define DOMAIN_USER_RID_ADMIN			     0x1F4L
#define DOMAIN_USER_RID_GUEST			     0x1F5L
#define DOMAIN_GROUP_RID_ADMINS 		     0x200L
#define DOMAIN_GROUP_RID_USERS			     0x201L
#define DOMAIN_ALIAS_RID_ADMINS 		     0x220L
#define DOMAIN_ALIAS_RID_USERS			     0x221L
#define DOMAIN_ALIAS_RID_GUESTS 		     0x222L
#define DOMAIN_ALIAS_RID_POWER_USERS		     0x223L
#define DOMAIN_ALIAS_RID_ACCOUNT_OPS		     0x224L
#define DOMAIN_ALIAS_RID_SYSTEM_OPS		     0x225L
#define DOMAIN_ALIAS_RID_PRINT_OPS		     0x226L
#define DOMAIN_ALIAS_RID_BACKUP_OPS		     0x227L
#define DOMAIN_ALIAS_RID_REPLICATOR		     0x228L
#define DOMAIN_ALIAS_RID_REMOTE_DESKTOP_USERS	     0x22BL
#define SECURITY_MANDATORY_UNTRUSTED_RID		 0
#define SECURITY_MANDATORY_LOW_RID		    0x1000
#define SECURITY_MANDATORY_MEDIUM_RID		    0x2000
#define SECURITY_MANDATORY_HIGH_RID		    0x3000
#define SECURITY_MANDATORY_SYSTEM_RID		    0x4000
#define SECURITY_MANDATORY_PROTECTED_PROCESS_RID    0x5000

#define SECURITY_MANDATORY_MAXIMUM_USER_RID	SECURITY_MANDATORY_SYSTEM_RID
#define SE_CREATE_TOKEN_NAME			TEXT("SeCreateTokenPrivilege")
#define SE_ASSIGNPRIMARYTOKEN_NAME		TEXT("SeAssignPrimaryTokenPrivilege")
#define SE_LOCK_MEMORY_NAME			TEXT("SeLockMemoryPrivilege")
#define SE_INCREASE_QUOTA_NAME			TEXT("SeIncreaseQuotaPrivilege")
#define SE_UNSOLICITED_INPUT_NAME		TEXT("SeUnsolicitedInputPrivilege")
#define SE_MACHINE_ACCOUNT_NAME 		TEXT("SeMachineAccountPrivilege")
#define SE_TCB_NAME				TEXT("SeTcbPrivilege")
#define SE_SECURITY_NAME			TEXT("SeSecurityPrivilege")
#define SE_TAKE_OWNERSHIP_NAME			TEXT("SeTakeOwnershipPrivilege")
#define SE_LOAD_DRIVER_NAME			TEXT("SeLoadDriverPrivilege")
#define SE_SYSTEM_PROFILE_NAME			TEXT("SeSystemProfilePrivilege")
#define SE_SYSTEMTIME_NAME			TEXT("SeSystemtimePrivilege")
#define SE_PROF_SINGLE_PROCESS_NAME		TEXT("SeProfileSingleProcessPrivilege")
#define SE_INC_BASE_PRIORITY_NAME		TEXT("SeIncreaseBasePriorityPrivilege")
#define SE_CREATE_PAGEFILE_NAME 		TEXT("SeCreatePagefilePrivilege")
#define SE_CREATE_PERMANENT_NAME		TEXT("SeCreatePermanentPrivilege")
#define SE_BACKUP_NAME				TEXT("SeBackupPrivilege")
#define SE_RESTORE_NAME 			TEXT("SeRestorePrivilege")
#define SE_SHUTDOWN_NAME			TEXT("SeShutdownPrivilege")
#define SE_DEBUG_NAME				TEXT("SeDebugPrivilege")
#define SE_AUDIT_NAME				TEXT("SeAuditPrivilege")
#define SE_SYSTEM_ENVIRONMENT_NAME		TEXT("SeSystemEnvironmentPrivilege")
#define SE_CHANGE_NOTIFY_NAME			TEXT("SeChangeNotifyPrivilege")
#define SE_REMOTE_SHUTDOWN_NAME 		TEXT("SeRemoteShutdownPrivilege")
#define SE_CREATE_GLOBAL_NAME			TEXT("SeCreateGlobalPrivilege")
#define SE_UNDOCK_NAME				TEXT("SeUndockPrivilege")
#define SE_MANAGE_VOLUME_NAME			TEXT("SeManageVolumePrivilege")
#define SE_IMPERSONATE_NAME			TEXT("SeImpersonatePrivilege")
#define SE_ENABLE_DELEGATION_NAME		TEXT("SeEnableDelegationPrivilege")
#define SE_SYNC_AGENT_NAME			TEXT("SeSyncAgentPrivilege")
#define SE_TRUSTED_CREDMAN_ACCESS_NAME		TEXT("SeTrustedCredManAccessPrivilege")
#define SE_RELABEL_NAME 			TEXT("SeRelabelPrivilege")
#define SE_INCREASE_WORKING_SET_NAME		TEXT("SeIncreaseWorkingSetPrivilege")
#define SE_TIME_ZONE_NAME			TEXT("SeTimeZonePrivilege")
#define SE_CREATE_SYMBOLIC_LINK_NAME		TEXT("SeCreateSymbolicLinkPrivilege")
#define SE_GROUP_MANDATORY				 1
#define SE_GROUP_ENABLED_BY_DEFAULT			 2
#define SE_GROUP_ENABLED				 4
#define SE_GROUP_OWNER					 8
#define SE_GROUP_USE_FOR_DENY_ONLY			16
#define SE_GROUP_INTEGRITY				32
#define SE_GROUP_INTEGRITY_ENABLED			64
#define SE_GROUP_LOGON_ID			3221225472U
#define SE_GROUP_RESOURCE			 536870912
#define LANG_NEUTRAL				      0x00
#define LANG_ARABIC				      0x01
#define LANG_BULGARIAN				      0x02
#define LANG_CATALAN				      0x03
#define LANG_CHINESE				      0x04
#define LANG_CZECH				      0x05
#define LANG_DANISH				      0x06
#define LANG_GERMAN				      0x07
#define LANG_GREEK				      0x08
#define LANG_ENGLISH				      0x09
#define LANG_SPANISH				      0x0a
#define LANG_FINNISH				      0x0b
#define LANG_FRENCH				      0x0c
#define LANG_HEBREW				      0x0d
#define LANG_HUNGARIAN				      0x0e
#define LANG_ICELANDIC				      0x0f
#define LANG_ITALIAN				      0x10
#define LANG_JAPANESE				      0x11
#define LANG_KOREAN				      0x12
#define LANG_DUTCH				      0x13
#define LANG_NORWEGIAN				      0x14
#define LANG_POLISH				      0x15
#define LANG_PORTUGUESE 			      0x16
#define LANG_ROMANIAN				      0x18
#define LANG_RUSSIAN				      0x19
#define LANG_CROATIAN				      0x1a
#define LANG_SERBIAN				      0x1a
#define LANG_SERBIAN_NEUTRAL			    0x7c1a
#define LANG_BOSNIAN				      0x1a
#define LANG_BOSNIAN_NEUTRAL			    0x781a
#define LANG_SLOVAK				      0x1b
#define LANG_ALBANIAN				      0x1c
#define LANG_SWEDISH				      0x1d
#define LANG_THAI				      0x1e
#define LANG_TURKISH				      0x1f
#define LANG_URDU				      0x20
#define LANG_INDONESIAN 			      0x21
#define LANG_UKRAINIAN				      0x22
#define LANG_BELARUSIAN 			      0x23
#define LANG_SLOVENIAN				      0x24
#define LANG_ESTONIAN				      0x25
#define LANG_LATVIAN				      0x26
#define LANG_LITHUANIAN 			      0x27
#define LANG_PERSIAN				      0x29
#define LANG_FARSI				  LANG_PERSIAN
#define LANG_VIETNAMESE 			      0x2a
#define LANG_ARMENIAN				      0x2b
#define LANG_AZERI				      0x2c
#define LANG_BASQUE				      0x2d
#define LANG_LOWER_SORBIAN			      0x2e
#define LANG_UPPER_SORBIAN			      0x2e
#define LANG_MACEDONIAN 			      0x2f
#define LANG_TSWANA				      0x32
#define LANG_XHOSA				      0x34
#define LANG_ZULU				      0x35
#define LANG_AFRIKAANS				      0x36
#define LANG_GEORGIAN				      0x37
#define LANG_FAEROESE				      0x38
#define LANG_HINDI				      0x39
#define LANG_MALTESE				      0x3a
#define LANG_SAMI				      0x3b
#define LANG_IRISH				      0x3c
#define LANG_MALAY				      0x3e
#define LANG_KAZAK				      0x3f
#define LANG_KYRGYZ				      0x40
#define LANG_SWAHILI				      0x41
#define LANG_UZBEK				      0x43
#define LANG_TATAR				      0x44
#define LANG_BENGALI				      0x45
#define LANG_PUNJABI				      0x46
#define LANG_GUJARATI				      0x47
#define LANG_ORIYA				      0x48
#define LANG_TAMIL				      0x49
#define LANG_TELUGU				      0x4a
#define LANG_KANNADA				      0x4b
#define LANG_MALAYALAM				      0x4c
#define LANG_ASSAMESE				      0x4d
#define LANG_MARATHI				      0x4e
#define LANG_SANSKRIT				      0x4f
#define LANG_MONGOLIAN				      0x50
#define LANG_TIBETAN				      0x51
#define LANG_WELSH				      0x52
#define LANG_KHMER				      0x53
#define LANG_LAO				      0x54
#define LANG_GALICIAN				      0x56
#define LANG_KONKANI				      0x57
#define LANG_MANIPURI				      0x58
#define LANG_SINDHI				      0x59
#define LANG_SYRIAC				      0x5a
#define LANG_SINHALESE				      0x5b
#define LANG_INUKTITUT				      0x5d
#define LANG_AMHARIC				      0x5e
#define LANG_TAMAZIGHT				      0x5f
#define LANG_KASHMIRI				      0x60
#define LANG_NEPALI				      0x61
#define LANG_FRISIAN				      0x62
#define LANG_PASHTO				      0x63
#define LANG_FILIPINO				      0x64
#define LANG_DIVEHI				      0x65
#define LANG_HAUSA				      0x68
#define LANG_YORUBA				      0x6a
#define LANG_QUECHUA				      0x6b
#define LANG_SOTHO				      0x6c
#define LANG_BASHKIR				      0x6d
#define LANG_LUXEMBOURGISH			      0x6e
#define LANG_GREENLANDIC			      0x6f
#define LANG_IGBO				      0x70
#define LANG_TIGRIGNA				      0x73
#define LANG_YI 				      0x78
#define LANG_MAPUDUNGUN 			      0x7a
#define LANG_MOHAWK				      0x7c
#define LANG_BRETON				      0x7e
#define LANG_INVARIANT				      0x7f
#define LANG_UIGHUR				      0x80
#define LANG_MAORI				      0x81
#define LANG_OCCITAN				      0x82
#define LANG_CORSICAN				      0x83
#define LANG_ALSATIAN				      0x84
#define LANG_YAKUT				      0x85
#define LANG_KICHE				      0x86
#define LANG_KINYARWANDA			      0x87
#define LANG_WOLOF				      0x88
#define LANG_DARI				      0x8c
#define LANG_MALAGASY				      0x8d
#define SUBLANG_NEUTRAL 			      0x00
#define SUBLANG_DEFAULT 			      0x01
#define SUBLANG_SYS_DEFAULT			      0x02
#define SUBLANG_CUSTOM_DEFAULT			      0x03
#define SUBLANG_CUSTOM_UNSPECIFIED		      0x04
#define SUBLANG_UI_CUSTOM_DEFAULT		      0x05
#define SUBLANG_AFRIKAANS_SOUTH_AFRICA		      0x01
#define SUBLANG_ALBANIAN_ALBANIA		      0x01
#define SUBLANG_ALSATIAN_FRANCE 		      0x01
#define SUBLANG_AMHARIC_ETHIOPIA		      0x01
#define SUBLANG_ARABIC_SAUDI_ARABIA		      0x01
#define SUBLANG_ARABIC_IRAQ			      0x02
#define SUBLANG_ARABIC_EGYPT			      0x03
#define SUBLANG_ARABIC_LIBYA			      0x04
#define SUBLANG_ARABIC_ALGERIA			      0x05
#define SUBLANG_ARABIC_MOROCCO			      0x06
#define SUBLANG_ARABIC_TUNISIA			      0x07
#define SUBLANG_ARABIC_OMAN			      0x08
#define SUBLANG_ARABIC_YEMEN			      0x09
#define SUBLANG_ARABIC_SYRIA			      0x0a
#define SUBLANG_ARABIC_JORDAN			      0x0b
#define SUBLANG_ARABIC_LEBANON			      0x0c
#define SUBLANG_ARABIC_KUWAIT			      0x0d
#define SUBLANG_ARABIC_UAE			      0x0e
#define SUBLANG_ARABIC_BAHRAIN			      0x0f
#define SUBLANG_ARABIC_QATAR			      0x10
#define SUBLANG_ARMENIAN_ARMENIA		      0x01
#define SUBLANG_ASSAMESE_INDIA			      0x01
#define SUBLANG_AZERI_LATIN			      0x01
#define SUBLANG_AZERI_CYRILLIC			      0x02
#define SUBLANG_BASHKIR_RUSSIA			      0x01
#define SUBLANG_BASQUE_BASQUE			      0x01
#define SUBLANG_BELARUSIAN_BELARUS		      0x01
#define SUBLANG_BENGALI_INDIA			      0x01
#define SUBLANG_BENGALI_BANGLADESH		      0x02
#define SUBLANG_BOSNIAN_BOSNIA_HERZEGOVINA_LATIN      0x05
#define SUBLANG_BOSNIAN_BOSNIA_HERZEGOVINA_CYRILLIC   0x08
#define SUBLANG_BRETON_FRANCE			      0x01
#define SUBLANG_BULGARIAN_BULGARIA		      0x01
#define SUBLANG_CATALAN_CATALAN 		      0x01
#define SUBLANG_CHINESE_TRADITIONAL		      0x01
#define SUBLANG_CHINESE_SIMPLIFIED		      0x02
#define SUBLANG_CHINESE_HONGKONG		      0x03
#define SUBLANG_CHINESE_SINGAPORE		      0x04
#define SUBLANG_CHINESE_MACAU			      0x05
#define SUBLANG_CORSICAN_FRANCE 		      0x01
#define SUBLANG_CZECH_CZECH_REPUBLIC		      0x01
#define SUBLANG_CROATIAN_CROATIA		      0x01
#define SUBLANG_CROATIAN_BOSNIA_HERZEGOVINA_LATIN     0x04
#define SUBLANG_DANISH_DENMARK			      0x01
#define SUBLANG_DIVEHI_MALDIVES 		      0x01
#define SUBLANG_DUTCH				      0x01
#define SUBLANG_DUTCH_BELGIAN			      0x02
#define SUBLANG_ENGLISH_US			      0x01
#define SUBLANG_ENGLISH_UK			      0x02
#define SUBLANG_ENGLISH_AUS			      0x03
#define SUBLANG_ENGLISH_CAN			      0x04
#define SUBLANG_ENGLISH_NZ			      0x05
#if (WINVER >= 0x0600)
#define SUBLANG_ENGLISH_IRELAND 		      0x06
#endif
#define SUBLANG_ENGLISH_EIRE			      0x06
#define SUBLANG_ENGLISH_SOUTH_AFRICA		      0x07
#define SUBLANG_ENGLISH_JAMAICA 		      0x08
#define SUBLANG_ENGLISH_CARIBBEAN		      0x09
#define SUBLANG_ENGLISH_BELIZE			      0x0a
#define SUBLANG_ENGLISH_TRINIDAD		      0x0b
#define SUBLANG_ENGLISH_ZIMBABWE		      0x0c
#define SUBLANG_ENGLISH_PHILIPPINES		      0x0d
#define SUBLANG_ENGLISH_INDIA			      0x10
#define SUBLANG_ENGLISH_MALAYSIA		      0x11
#define SUBLANG_ENGLISH_SINGAPORE		      0x12
#define SUBLANG_ESTONIAN_ESTONIA		      0x01
#define SUBLANG_FAEROESE_FAROE_ISLANDS		      0x01
#define SUBLANG_FILIPINO_PHILIPPINES		      0x01
#define SUBLANG_FINNISH_FINLAND 		      0x01
#define SUBLANG_FRENCH				      0x01
#define SUBLANG_FRENCH_BELGIAN			      0x02
#define SUBLANG_FRENCH_CANADIAN 		      0x03
#define SUBLANG_FRENCH_SWISS			      0x04
#define SUBLANG_FRENCH_LUXEMBOURG		      0x05
#define SUBLANG_FRENCH_MONACO			      0x06
#define SUBLANG_FRISIAN_NETHERLANDS		      0x01
#define SUBLANG_GALICIAN_GALICIAN		      0x01
#define SUBLANG_GEORGIAN_GEORGIA		      0x01
#define SUBLANG_GERMAN				      0x01
#define SUBLANG_GERMAN_SWISS			      0x02
#define SUBLANG_GERMAN_AUSTRIAN 		      0x03
#define SUBLANG_GERMAN_LUXEMBOURG		      0x04
#define SUBLANG_GERMAN_LIECHTENSTEIN		      0x05
#define SUBLANG_GREEK_GREECE			      0x01
#define SUBLANG_GREENLANDIC_GREENLAND		      0x01
#define SUBLANG_GUJARATI_INDIA			      0x01
#define SUBLANG_HAUSA_NIGERIA_LATIN		      0x01
#define SUBLANG_HAUSA_NIGERIA			SUBLANG_HAUSA_NIGERIA_LATIN /* SUBLANG_HAUSA_NIGERIA_LATIN is what MS defines */
#define SUBLANG_HEBREW_ISRAEL			      0x01
#define SUBLANG_HINDI_INDIA			      0x01
#define SUBLANG_HUNGARIAN_HUNGARY		      0x01
#define SUBLANG_ICELANDIC_ICELAND		      0x01
#define SUBLANG_IGBO_NIGERIA			      0x01
#define SUBLANG_INDONESIAN_INDONESIA		      0x01
#define SUBLANG_INUKTITUT_CANADA		      0x01
#define SUBLANG_INUKTITUT_CANADA_LATIN		      0x02
#define SUBLANG_IRISH_IRELAND			      0x02
#define SUBLANG_ITALIAN 			      0x01
#define SUBLANG_ITALIAN_SWISS			      0x02
#define SUBLANG_JAPANESE_JAPAN			      0x01
#define SUBLANG_KASHMIRI_INDIA			      0x02
#define SUBLANG_KASHMIRI_SASIA			      0x02
#define SUBLANG_KAZAK_KAZAKHSTAN		      0x01
#define SUBLANG_KHMER_CAMBODIA			      0x01
#define SUBLANG_KICHE_GUATEMALA 		      0x01
#define SUBLANG_KINYARWANDA_RWANDA		      0x01
#define SUBLANG_KONKANI_INDIA			      0x01
#define SUBLANG_KOREAN				      0x01
#define SUBLANG_KYRGYZ_KYRGYZSTAN		      0x01
#define SUBLANG_LAO_LAO 			      0x01
#define SUBLANG_LAO_LAO_PDR			SUBLANG_LAO_LAO /* SUBLANG_LAO_LAO is what MS defines */
#define SUBLANG_LATVIAN_LATVIA			      0x01
#if (WINVER >= 0x0600)
#define SUBLANG_LITHUANIAN_LITHUANIA		      0x01
#endif
#define SUBLANG_LITHUANIAN			      0x01
#define SUBLANG_LOWER_SORBIAN_GERMANY		      0x02
#define SUBLANG_LUXEMBOURGISH_LUXEMBOURG	      0x01
#define SUBLANG_MACEDONIAN_MACEDONIA		      0x01
#define SUBLANG_MALAY_MALAYSIA			      0x01
#define SUBLANG_MALAY_BRUNEI_DARUSSALAM 	      0x02
#define SUBLANG_MALAYALAM_INDIA 		      0x01
#define SUBLANG_MALTESE_MALTA			      0x01
#define SUBLANG_MAORI_NEW_ZEALAND		      0x01
#define SUBLANG_MAPUDUNGUN_CHILE		      0x01
#define SUBLANG_MARATHI_INDIA			      0x01
#define SUBLANG_MOHAWK_MOHAWK			      0x01
#define SUBLANG_MONGOLIAN_CYRILLIC_MONGOLIA	      0x01
#define SUBLANG_MONGOLIAN_PRC			      0x02
#define SUBLANG_NEPALI_NEPAL			      0x01
#define SUBLANG_NEPALI_INDIA			      0x02
#define SUBLANG_NORWEGIAN_BOKMAL		      0x01
#define SUBLANG_NORWEGIAN_NYNORSK		      0x02
#define SUBLANG_OCCITAN_FRANCE			      0x01
#define SUBLANG_ORIYA_INDIA			      0x01
#define SUBLANG_PASHTO_AFGHANISTAN		      0x01
#define SUBLANG_PERSIAN_IRAN			      0x01
#define SUBLANG_POLISH_POLAND			      0x01
#define SUBLANG_PORTUGUESE_BRAZILIAN		      0x01
#if (WINVER >= 0x0600)
#define SUBLANG_PORTUGUESE_PORTUGAL		      0x02
#endif
#define SUBLANG_PORTUGUESE			      0x02
#define SUBLANG_PUNJABI_INDIA			      0x01
/* ??? #define SUBLANG_PUNJABI_PAKISTAN		      0x01 ??? */
#define SUBLANG_QUECHUA_BOLIVIA 		      0x01
#define SUBLANG_QUECHUA_ECUADOR 		      0x02
#define SUBLANG_QUECHUA_PERU			      0x03
#define SUBLANG_ROMANIAN_ROMANIA		      0x01
/* ??? #define SUBLANG_ROMANIAN_MOLDOVA		      0x01 ??? */
#define SUBLANG_ROMANSH_SWITZERLAND		      0x01
#define SUBLANG_RUSSIAN_RUSSIA			      0x01
#define SUBLANG_SAMI_NORTHERN_NORWAY		      0x01
#define SUBLANG_SAMI_NORTHERN_SWEDEN		      0x02
#define SUBLANG_SAMI_NORTHERN_FINLAND		      0x03
#define SUBLANG_SAMI_LULE_NORWAY		      0x04
#define SUBLANG_SAMI_LULE_SWEDEN		      0x05
#define SUBLANG_SAMI_SOUTHERN_NORWAY		      0x06
#define SUBLANG_SAMI_SOUTHERN_SWEDEN		      0x07
#define SUBLANG_SAMI_SKOLT_FINLAND		      0x08
#define SUBLANG_SAMI_INARI_FINLAND		      0x09
#define SUBLANG_SANSKRIT_INDIA			      0x01
#define SUBLANG_SERBIAN_LATIN			      0x02
#define SUBLANG_SERBIAN_CYRILLIC		      0x03
#define SUBLANG_SERBIAN_BOSNIA_HERZEGOVINA_LATIN      0x06
#define SUBLANG_SERBIAN_BOSNIA_HERZEGOVINA_CYRILLIC   0x07
#define SUBLANG_SINDHI_AFGHANISTAN		      0x02
#define SUBLANG_SINHALESE_SRI_LANKA		      0x01
#define SUBLANG_SOTHO_NORTHERN_SOUTH_AFRICA	      0x01
#define SUBLANG_SLOVAK_SLOVAKIA 		      0x01
#define SUBLANG_SLOVENIAN_SLOVENIA		      0x01
#define SUBLANG_SPANISH 			      0x01
#define SUBLANG_SPANISH_MEXICAN 		      0x02
#define SUBLANG_SPANISH_MODERN			      0x03
#define SUBLANG_SPANISH_GUATEMALA		      0x04
#define SUBLANG_SPANISH_COSTA_RICA		      0x05
#define SUBLANG_SPANISH_PANAMA			      0x06
#define SUBLANG_SPANISH_DOMINICAN_REPUBLIC	      0x07
#define SUBLANG_SPANISH_VENEZUELA		      0x08
#define SUBLANG_SPANISH_COLOMBIA		      0x09
#define SUBLANG_SPANISH_PERU			      0x0a
#define SUBLANG_SPANISH_ARGENTINA		      0x0b
#define SUBLANG_SPANISH_ECUADOR 		      0x0c
#define SUBLANG_SPANISH_CHILE			      0x0d
#define SUBLANG_SPANISH_URUGUAY 		      0x0e
#define SUBLANG_SPANISH_PARAGUAY		      0x0f
#define SUBLANG_SPANISH_BOLIVIA 		      0x10
#define SUBLANG_SPANISH_EL_SALVADOR		      0x11
#define SUBLANG_SPANISH_HONDURAS		      0x12
#define SUBLANG_SPANISH_NICARAGUA		      0x13
#define SUBLANG_SPANISH_PUERTO_RICO		      0x14
#define SUBLANG_SPANISH_US			      0x15
#if (WINVER >= 0x0600)
#define SUBLANG_SWEDISH_SWEDEN			      0x01
#endif
#define SUBLANG_SWEDISH 			      0x01
#define SUBLANG_SWEDISH_FINLAND 		      0x02
#define SUBLANG_SYRIAC				      0x01
#define SUBLANG_SYRIAC_SYRIA			SUBLANG_SYRIAC /* SUBLANG_SYRIAC_SYRIA is what MSDN mentions */
#define SUBLANG_TAJIK_TAJIKISTAN		      0x01
#define SUBLANG_TAMAZIGHT_ALGERIA_LATIN 	      0x02
#define SUBLANG_TAMIL_INDIA			      0x01
#define SUBLANG_TATAR_RUSSIA			      0x01
#define SUBLANG_TELUGU_INDIA			      0x01
#define SUBLANG_THAI_THAILAND			      0x01
#define SUBLANG_TIBETAN_PRC			      0x01
#define SUBLANG_TIBETAN_BHUTAN			      0x02
#define SUBLANG_TIGRIGNA_ERITREA		      0x02
#define SUBLANG_TSWANA_SOUTH_AFRICA		      0x01
#define SUBLANG_TURKISH_TURKEY			      0x01
#define SUBLANG_TURKMEN_TURKMENISTAN		      0x01
#define SUBLANG_UIGHUR_PRC			      0x01
#define SUBLANG_UKRAINIAN_UKRAINE		      0x01
#define SUBLANG_UPPER_SORBIAN_GERMANY		      0x01
#define SUBLANG_URDU_PAKISTAN			      0x01
#define SUBLANG_URDU_INDIA			      0x02
#define SUBLANG_UZBEK_LATIN			      0x01
#define SUBLANG_UZBEK_CYRILLIC			      0x02
#define SUBLANG_VIETNAMESE_VIETNAM		      0x01
#define SUBLANG_WELSH_UNITED_KINGDOM		      0x01
#define SUBLANG_WOLOF_SENEGAL			      0x01
#define SUBLANG_YORUBA_NIGERIA			      0x01
#define SUBLANG_XHOSA_SOUTH_AFRICA		      0x01
#define SUBLANG_YAKUT_RUSSIA			      0x01
#define SUBLANG_YI_PRC				      0x01
#define SUBLANG_ZULU_SOUTH_AFRICA		      0x01
#define NLS_VALID_LOCALE_MASK			   1048575
#define SORT_DEFAULT					 0
#define SORT_JAPANESE_XJIS				 0
#define SORT_JAPANESE_UNICODE				 1
#define SORT_CHINESE_BIG5				 0
#define SORT_CHINESE_PRCP				 0
#define SORT_CHINESE_UNICODE				 1
#define SORT_CHINESE_PRC				 2
#define SORT_CHINESE_BOPOMOFO				 3
#define SORT_KOREAN_KSC 				 0
#define SORT_KOREAN_UNICODE				 1
#define SORT_GERMAN_PHONE_BOOK				 1
#define SORT_HUNGARIAN_DEFAULT				 0
#define SORT_HUNGARIAN_TECHNICAL			 1
#define SORT_GEORGIAN_TRADITIONAL			 0
#define SORT_GEORGIAN_MODERN				 1
#define MAKELANGID(p,s) 		((((WORD)(s))<<10)|(WORD)(p))
#define MAKELCID(l,s)			((DWORD)((((DWORD)((WORD)(s)))<<16)|((DWORD)((WORD)(l)))))
#define PRIMARYLANGID(l)		((WORD)(l)&0x3ff)
#define SORTIDFROMLCID(l)		((WORD)((((DWORD)(l))&NLS_VALID_LOCALE_MASK)>>16))
#define SORTVERSIONFROMLCID(l)		((WORD)((((DWORD)(l))>>20)&0xf))
#define SUBLANGID(l)			((WORD)(l)>>10)
#define LANGIDFROMLCID(l)		((WORD)(l))
#define LANG_SYSTEM_DEFAULT		MAKELANGID(LANG_NEUTRAL,SUBLANG_SYS_DEFAULT)
#define LANG_USER_DEFAULT		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT)
#define LOCALE_NEUTRAL			MAKELCID(MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL),SORT_DEFAULT)
#define ACL_REVISION					 2
#define ACL_REVISION_DS 				 4
#define ACL_REVISION1					 1
#define ACL_REVISION2					 2
#define ACL_REVISION3					 3
#define ACL_REVISION4					 4
#define MIN_ACL_REVISION				 2
#define MAX_ACL_REVISION				 4
#define MINCHAR 				      0x80
#define MAXCHAR 				      0x7f
#define MINSHORT				    0x8000
#define MAXSHORT				    0x7fff
#define MINLONG 				0x80000000
#define MAXLONG  				0x7fffffff
#define MAXBYTE 				      0xff
#define MAXWORD 				    0xffff
#define MAXDWORD				0xffffffff
#define PROCESSOR_INTEL_386			       386
#define PROCESSOR_INTEL_486			       486
#define PROCESSOR_INTEL_PENTIUM 		       586
#define PROCESSOR_MIPS_R4000			      4000
#define PROCESSOR_ALPHA_21064			     21064
#define PROCESSOR_INTEL_IA64			      2200
#define PROCESSOR_ARCHITECTURE_INTEL			 0
#define PROCESSOR_ARCHITECTURE_MIPS			 1
#define PROCESSOR_ARCHITECTURE_ALPHA			 2
#define PROCESSOR_ARCHITECTURE_PPC			 3
#define PROCESSOR_ARCHITECTURE_SHX			 4
#define PROCESSOR_ARCHITECTURE_ARM			 5
#define PROCESSOR_ARCHITECTURE_IA64			 6
#define PROCESSOR_ARCHITECTURE_ALPHA64			 7
#define PROCESSOR_ARCHITECTURE_MSIL			 8
#define PROCESSOR_ARCHITECTURE_AMD64			 9
#define PROCESSOR_ARCHITECTURE_IA32_ON_WIN64		10
#define PROCESSOR_ARCHITECTURE_UNKNOWN		    0xFFFF
#define PF_FLOATING_POINT_PRECISION_ERRATA		 0
#define PF_FLOATING_POINT_EMULATED			 1
#define PF_COMPARE_EXCHANGE_DOUBLE			 2
#define PF_MMX_INSTRUCTIONS_AVAILABLE			 3
#define PF_PPC_MOVEMEM_64BIT_OK 			 4
#define PF_ALPHA_BYTE_INSTRUCTIONS			 5
#define PF_XMMI_INSTRUCTIONS_AVAILABLE			 6
#define PF_3DNOW_INSTRUCTIONS_AVAILABLE 		 7
#define PF_RDTSC_INSTRUCTION_AVAILABLE			 8
#define PF_PAE_ENABLED					 9
#define PF_XMMI64_INSTRUCTIONS_AVAILABLE		10
/* also in ddk/ntifs.h */
#define FILE_ACTION_ADDED			0x00000001
#define FILE_ACTION_REMOVED			0x00000002
#define FILE_ACTION_MODIFIED			0x00000003
#define FILE_ACTION_RENAMED_OLD_NAME		0x00000004
#define FILE_ACTION_RENAMED_NEW_NAME		0x00000005
#define FILE_ACTION_ADDED_STREAM		0x00000006
#define FILE_ACTION_REMOVED_STREAM		0x00000007
#define FILE_ACTION_MODIFIED_STREAM		0x00000008
#define FILE_ACTION_REMOVED_BY_DELETE		0x00000009
#define FILE_ACTION_ID_NOT_TUNNELLED		0x0000000A
#define FILE_ACTION_TUNNELLED_ID_COLLISION	0x0000000B
/* end ntifs.h */
#define HEAP_NO_SERIALIZE				 1
#define HEAP_GROWABLE					 2
#define HEAP_GENERATE_EXCEPTIONS			 4
#define HEAP_ZERO_MEMORY				 8
#define HEAP_REALLOC_IN_PLACE_ONLY			16
#define HEAP_TAIL_CHECKING_ENABLED			32
#define HEAP_FREE_CHECKING_ENABLED			64
#define HEAP_DISABLE_COALESCE_ON_FREE		       128
#define HEAP_CREATE_ALIGN_16			    0x0000
#define HEAP_CREATE_ENABLE_TRACING		   0x20000
#define HEAP_MAXIMUM_TAG			     0xFFF
#define HEAP_PSEUDO_TAG_FLAG			    0x8000
#define HEAP_TAG_SHIFT					16
#define HEAP_MAKE_TAG_FLAGS(b,o)	     ((DWORD)((b)+(o)<<16))
#define KEY_QUERY_VALUE 				 1
#define KEY_SET_VALUE					 2
#define KEY_CREATE_SUB_KEY				 4
#define KEY_ENUMERATE_SUB_KEYS				 8
#define KEY_NOTIFY					16
#define KEY_CREATE_LINK 				32
#define KEY_WRITE				   0x20006
#define KEY_EXECUTE				   0x20019
#define KEY_READ				   0x20019
#if (_WIN32_WINNT >= 0x0502)
#define KEY_WOW64_64KEY 			    0x0100
#define KEY_WOW64_32KEY 			    0x0200
#endif
#define KEY_ALL_ACCESS				   0xf003f
#define REG_WHOLE_HIVE_VOLATILE 			 1
#define REG_REFRESH_HIVE				 2
#define REG_NO_LAZY_FLUSH				 4
#define REG_OPTION_RESERVED				 0
#define REG_OPTION_NON_VOLATILE 			 0
#define REG_OPTION_VOLATILE				 1
#define REG_OPTION_CREATE_LINK				 2
#define REG_OPTION_BACKUP_RESTORE			 4
#define REG_OPTION_OPEN_LINK				 8
#define REG_LEGAL_OPTION				15
#define OWNER_SECURITY_INFORMATION		0x00000001
#define GROUP_SECURITY_INFORMATION		0x00000002
#define DACL_SECURITY_INFORMATION		0x00000004
#define SACL_SECURITY_INFORMATION		0x00000008
#define LABEL_SECURITY_INFORMATION		0x00000010
#define UNPROTECTED_SACL_SECURITY_INFORMATION	0x10000000
#define UNPROTECTED_DACL_SECURITY_INFORMATION	0x20000000
#define PROTECTED_SACL_SECURITY_INFORMATION	0x40000000
#define PROTECTED_DACL_SECURITY_INFORMATION	0x80000000
#define MAXIMUM_PROCESSORS				32
#define PAGE_NOACCESS				    0x0001
#define PAGE_READONLY				    0x0002
#define PAGE_READWRITE				    0x0004
#define PAGE_WRITECOPY				    0x0008
#define PAGE_EXECUTE				    0x0010
#define PAGE_EXECUTE_READ			    0x0020
#define PAGE_EXECUTE_READWRITE			    0x0040
#define PAGE_EXECUTE_WRITECOPY			    0x0080
#define PAGE_TARGETS_INVALID			0x40000000
#define PAGE_TARGETS_NO_UPDATE			0x40000000
#define PAGE_GUARD				    0x0100
#define PAGE_NOCACHE				    0x0200
#define PAGE_WRITECOMBINE			    0x0400
#define MEM_COMMIT				    0x1000
#define MEM_RESERVE				    0x2000
#define MEM_DECOMMIT				    0x4000
#define MEM_RELEASE				    0x8000
#define MEM_FREE				   0x10000
#define MEM_PRIVATE				   0x20000
#define MEM_MAPPED				   0x40000
#define MEM_RESET				   0x80000
#define MEM_TOP_DOWN				  0x100000
#define MEM_WRITE_WATCH 			  0x200000 /* 98/Me */
#define MEM_PHYSICAL				  0x400000
#define MEM_4MB_PAGES				0x80000000
/* also in ddk/ntifs.h */
#define MEM_IMAGE				SEC_IMAGE
#define SEC_BASED				0x00200000
#define SEC_NO_CHANGE				0x00400000
#define SEC_FILE				0x00800000
#define SEC_IMAGE				0x01000000
#define SEC_VLM 				0x02000000
#define SEC_RESERVE				0x04000000
#define SEC_COMMIT				0x08000000
#define SEC_NOCACHE				0x10000000
/* end ntifs.h */
#define SECTION_EXTEND_SIZE				16
#define SECTION_MAP_READ				 4
#define SECTION_MAP_WRITE				 2
#define SECTION_QUERY					 1
#define SECTION_MAP_EXECUTE				 8
#define SECTION_ALL_ACCESS			   0xf001f
#define MESSAGE_RESOURCE_UNICODE			 1
#define RTL_CRITSECT_TYPE				 0
#define RTL_RESOURCE_TYPE				 1
/* Also in winddk.h */
#define FIELD_OFFSET(t,f)		((LONG)&(((t*)0)->f))
#ifndef CONTAINING_RECORD
#define CONTAINING_RECORD(address, type, field) \
  ((type*)((PCHAR)(address) - (PCHAR)(&((type *)0)->field)))
#endif
/* end winddk.h */
#define IMAGE_SIZEOF_FILE_HEADER			20
#define IMAGE_FILE_RELOCS_STRIPPED			 1
#define IMAGE_FILE_EXECUTABLE_IMAGE			 2
#define IMAGE_FILE_LINE_NUMS_STRIPPED			 4
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED			 8
#define IMAGE_FILE_AGGRESIVE_WS_TRIM			16
#define IMAGE_FILE_LARGE_ADDRESS_AWARE			32
#define IMAGE_FILE_BYTES_REVERSED_LO		       128
#define IMAGE_FILE_32BIT_MACHINE		       256
#define IMAGE_FILE_DEBUG_STRIPPED		       512
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP	      1024
#define IMAGE_FILE_NET_RUN_FROM_SWAP		      2048
#define IMAGE_FILE_SYSTEM			      4096
#define IMAGE_FILE_DLL				      8192
#define IMAGE_FILE_UP_SYSTEM_ONLY		     16384
#define IMAGE_FILE_BYTES_REVERSED_HI		     32768

#define IMAGE_FILE_MACHINE_UNKNOWN		    0x0000
#define IMAGE_FILE_MACHINE_AM33 		    0x01d3 /* Matsushita AM33 */
#define IMAGE_FILE_MACHINE_AMD64		    0x8664 /* x64 */
#define IMAGE_FILE_MACHINE_ARM			    0x01c0 /* ARM little endian */
#define IMAGE_FILE_MACHINE_EBC			    0x0ebc /* EFI byte code */
#define IMAGE_FILE_MACHINE_I386 		    0x014c /* Intel 386 or later processors and compatible processors */
#define IMAGE_FILE_MACHINE_IA64 		    0x0200 /* Intel Itanium processor family */
#define IMAGE_FILE_MACHINE_M32R 		    0x9041 /* Mitsubishi M32R little endian */
#define IMAGE_FILE_MACHINE_MIPS16		    0x0266 /* MIPS16 */
#define IMAGE_FILE_MACHINE_MIPSFPU		    0x0366 /* MIPS with FPU */
#define IMAGE_FILE_MACHINE_MIPSFPU16		    0x0466 /* MIPS16 with FPU */
#define IMAGE_FILE_MACHINE_POWERPC		    0x01f0 /* Power PC little endian */
#define IMAGE_FILE_MACHINE_POWERPCFP		    0x01f1 /* Power PC with floating point support */
#define IMAGE_FILE_MACHINE_R4000		    0x0166 /* MIPS little endian */
#define IMAGE_FILE_MACHINE_SH3			    0x01a2 /* Hitachi SH3 */
#define IMAGE_FILE_MACHINE_SH3DSP		    0x01a3 /* Hitachi SH3 DSP */
#define IMAGE_FILE_MACHINE_SH4			    0x01a6 /* Hitachi SH4 */
#define IMAGE_FILE_MACHINE_SH5			    0x01a8 /* Hitachi SH5 */
#define IMAGE_FILE_MACHINE_THUMB		    0x01c2 /* Thumb */
#define IMAGE_FILE_MACHINE_WCEMIPSV2		    0x0169 /* MIPS little-endian WCE v2 */

#define IMAGE_DOS_SIGNATURE			    0x5A4D
#define IMAGE_OS2_SIGNATURE			    0x454E
#define IMAGE_OS2_SIGNATURE_LE			    0x454C
#define IMAGE_VXD_SIGNATURE			    0x454C
#define IMAGE_NT_SIGNATURE			0x00004550
#define IMAGE_NT_OPTIONAL_HDR32_MAGIC		     0x10b
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC		     0x20b
#ifdef _WIN64
#define IMAGE_NT_OPTIONAL_HDR_MAGIC	IMAGE_NT_OPTIONAL_HDR64_MAGIC
#else
#define IMAGE_NT_OPTIONAL_HDR_MAGIC	IMAGE_NT_OPTIONAL_HDR32_MAGIC
#endif
#define IMAGE_ROM_OPTIONAL_HDR_MAGIC		     0x107
#define IMAGE_SEPARATE_DEBUG_SIGNATURE		    0x4944
#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES		16
#define IMAGE_SIZEOF_ROM_OPTIONAL_HEADER		56
#define IMAGE_SIZEOF_STD_OPTIONAL_HEADER		28
#define IMAGE_SIZEOF_NT_OPTIONAL_HEADER 	       224
#define IMAGE_SIZEOF_SHORT_NAME 			 8
#define IMAGE_SIZEOF_SECTION_HEADER			40
#define IMAGE_SIZEOF_SYMBOL				18
#define IMAGE_SIZEOF_AUX_SYMBOL 			18
#define IMAGE_SIZEOF_RELOCATION 			10
#define IMAGE_SIZEOF_BASE_RELOCATION			 8
#define IMAGE_SIZEOF_LINENUMBER 			 6
#define IMAGE_SIZEOF_ARCHIVE_MEMBER_HDR 		60
#define SIZEOF_RFPO_DATA				16

#define IMAGE_SUBSYSTEM_UNKNOWN 			 0
#define IMAGE_SUBSYSTEM_NATIVE				 1
#define IMAGE_SUBSYSTEM_WINDOWS_GUI			 2
#define IMAGE_SUBSYSTEM_WINDOWS_CUI			 3
#define IMAGE_SUBSYSTEM_OS2_CUI 			 5  /* Not in PECOFF v8 spec */
#define IMAGE_SUBSYSTEM_POSIX_CUI			 7
#define IMAGE_SUBSYSTEM_NATIVE_WINDOWS			 8  /* Not in PECOFF v8 spec */
#define IMAGE_SUBSYSTEM_WINDOWS_CE_GUI			 9
#define IMAGE_SUBSYSTEM_EFI_APPLICATION 		10
#define IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER 	11
#define IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER		12
#define IMAGE_SUBSYSTEM_EFI_ROM 			13
#define IMAGE_SUBSYSTEM_XBOX				14

#define IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE		0x0040
#define IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY	0x0080
#define IMAGE_DLLCHARACTERISTICS_NX_COMPAT		0x0100
#define IMAGE_DLLCHARACTERISTICS_NO_ISOLATION		0x0200
#define IMAGE_DLLCHARACTERISTICS_NO_SEH 		0x0400
#define IMAGE_DLLCHARACTERISTICS_NO_BIND		0x0800
#define IMAGE_DLLCHARACTERISTICS_WDM_DRIVER		0x2000
#define IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE	0x8000

/* Keep broken definitions for backward compatibility. */
#define IMAGE_DLL_CHARACTERISTICS_DYNAMIC_BASE		IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE
#define IMAGE_DLL_CHARACTERISTICS_FORCE_INTEGRITY	IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY
#define IMAGE_DLL_CHARACTERISTICS_NX_COMPAT		IMAGE_DLLCHARACTERISTICS_NX_COMPAT

#define IMAGE_FIRST_SECTION(h)				((PIMAGE_SECTION_HEADER) ((DWORD)h+FIELD_OFFSET(IMAGE_NT_HEADERS,OptionalHeader)+((PIMAGE_NT_HEADERS)(h))->FileHeader.SizeOfOptionalHeader))
#define IMAGE_DIRECTORY_ENTRY_EXPORT			 0
#define IMAGE_DIRECTORY_ENTRY_IMPORT			 1
#define IMAGE_DIRECTORY_ENTRY_RESOURCE			 2
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION 		 3
#define IMAGE_DIRECTORY_ENTRY_SECURITY			 4
#define IMAGE_DIRECTORY_ENTRY_BASERELOC 		 5
#define IMAGE_DIRECTORY_ENTRY_DEBUG			 6
#define IMAGE_DIRECTORY_ENTRY_COPYRIGHT 		 7
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE		 7
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR 		 8
#define IMAGE_DIRECTORY_ENTRY_TLS			 9
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG		10
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT		11
#define IMAGE_DIRECTORY_ENTRY_IAT			12
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT		13
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR		14
#define IMAGE_SCN_TYPE_REG				 0
#define IMAGE_SCN_TYPE_DSECT				 1
#define IMAGE_SCN_TYPE_NOLOAD				 2
#define IMAGE_SCN_TYPE_GROUP				 4
#define IMAGE_SCN_TYPE_NO_PAD				 8
#define IMAGE_SCN_TYPE_COPY				16
#define IMAGE_SCN_CNT_CODE				32
#define IMAGE_SCN_CNT_INITIALIZED_DATA			64
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA	       128
#define IMAGE_SCN_LNK_OTHER			       256
#define IMAGE_SCN_LNK_INFO			       512
#define IMAGE_SCN_TYPE_OVER			      1024
#define IMAGE_SCN_LNK_REMOVE			      2048
#define IMAGE_SCN_LNK_COMDAT			      4096
#define IMAGE_SCN_GPREL 			    0x8000 /*  Valid only for IA64 */
#define IMAGE_SCN_MEM_FARDATA			    0x8000 /* Not in PECOFF v8 spec */
#define IMAGE_SCN_MEM_PURGEABLE 		   0x20000
#define IMAGE_SCN_MEM_16BIT			   0x20000
#define IMAGE_SCN_MEM_LOCKED			   0x40000
#define IMAGE_SCN_MEM_PRELOAD			   0x80000
#define IMAGE_SCN_ALIGN_1BYTES			  0x100000
#define IMAGE_SCN_ALIGN_2BYTES			  0x200000
#define IMAGE_SCN_ALIGN_4BYTES			  0x300000
#define IMAGE_SCN_ALIGN_8BYTES			  0x400000
#define IMAGE_SCN_ALIGN_16BYTES 		  0x500000
#define IMAGE_SCN_ALIGN_32BYTES 		  0x600000
#define IMAGE_SCN_ALIGN_64BYTES 		  0x700000
#define IMAGE_SCN_ALIGN_128BYTES		  0x800000
#define IMAGE_SCN_ALIGN_256BYTES		  0x900000
#define IMAGE_SCN_ALIGN_512BYTES		  0xa00000
#define IMAGE_SCN_ALIGN_1024BYTES		  0xb00000
#define IMAGE_SCN_ALIGN_2048BYTES		  0xc00000
#define IMAGE_SCN_ALIGN_4096BYTES		  0xd00000
#define IMAGE_SCN_ALIGN_8192BYTES		  0xe00000
#define IMAGE_SCN_LNK_NRELOC_OVFL		 0x1000000
#define IMAGE_SCN_MEM_DISCARDABLE		 0x2000000
#define IMAGE_SCN_MEM_NOT_CACHED		 0x4000000
#define IMAGE_SCN_MEM_NOT_PAGED 		 0x8000000
#define IMAGE_SCN_MEM_SHARED			0x10000000
#define IMAGE_SCN_MEM_EXECUTE			0x20000000
#define IMAGE_SCN_MEM_READ			0x40000000
#define IMAGE_SCN_MEM_WRITE			0x80000000
#define IMAGE_SYM_UNDEFINED				 0
#define IMAGE_SYM_ABSOLUTE			       (-1)
#define IMAGE_SYM_DEBUG 			       (-2)
#define IMAGE_SYM_TYPE_NULL				 0
#define IMAGE_SYM_TYPE_VOID				 1
#define IMAGE_SYM_TYPE_CHAR				 2
#define IMAGE_SYM_TYPE_SHORT				 3
#define IMAGE_SYM_TYPE_INT				 4
#define IMAGE_SYM_TYPE_LONG				 5
#define IMAGE_SYM_TYPE_FLOAT				 6
#define IMAGE_SYM_TYPE_DOUBLE				 7
#define IMAGE_SYM_TYPE_STRUCT				 8
#define IMAGE_SYM_TYPE_UNION				 9
#define IMAGE_SYM_TYPE_ENUM				10
#define IMAGE_SYM_TYPE_MOE				11
#define IMAGE_SYM_TYPE_BYTE				12
#define IMAGE_SYM_TYPE_WORD				13
#define IMAGE_SYM_TYPE_UINT				14
#define IMAGE_SYM_TYPE_DWORD				15
#define IMAGE_SYM_TYPE_PCODE			     32768
#define IMAGE_SYM_DTYPE_NULL				 0
#define IMAGE_SYM_DTYPE_POINTER 			 1
#define IMAGE_SYM_DTYPE_FUNCTION			 2
#define IMAGE_SYM_DTYPE_ARRAY				 3
#define IMAGE_SYM_CLASS_END_OF_FUNCTION 	       (-1)
#define IMAGE_SYM_CLASS_NULL				 0
#define IMAGE_SYM_CLASS_AUTOMATIC			 1
#define IMAGE_SYM_CLASS_EXTERNAL			 2
#define IMAGE_SYM_CLASS_STATIC				 3
#define IMAGE_SYM_CLASS_REGISTER			 4
#define IMAGE_SYM_CLASS_EXTERNAL_DEF			 5
#define IMAGE_SYM_CLASS_LABEL				 6
#define IMAGE_SYM_CLASS_UNDEFINED_LABEL 		 7
#define IMAGE_SYM_CLASS_MEMBER_OF_STRUCT		 8
#define IMAGE_SYM_CLASS_ARGUMENT			 9
#define IMAGE_SYM_CLASS_STRUCT_TAG			10
#define IMAGE_SYM_CLASS_MEMBER_OF_UNION 		11
#define IMAGE_SYM_CLASS_UNION_TAG			12
#define IMAGE_SYM_CLASS_TYPE_DEFINITION 		13
#define IMAGE_SYM_CLASS_UNDEFINED_STATIC		14
#define IMAGE_SYM_CLASS_ENUM_TAG			15
#define IMAGE_SYM_CLASS_MEMBER_OF_ENUM			16
#define IMAGE_SYM_CLASS_REGISTER_PARAM			17
#define IMAGE_SYM_CLASS_BIT_FIELD			18
#define IMAGE_SYM_CLASS_FAR_EXTERNAL			68 /* Not in PECOFF v8 spec */
#define IMAGE_SYM_CLASS_BLOCK			       100
#define IMAGE_SYM_CLASS_FUNCTION		       101
#define IMAGE_SYM_CLASS_END_OF_STRUCT		       102
#define IMAGE_SYM_CLASS_FILE			       103
#define IMAGE_SYM_CLASS_SECTION 		       104
#define IMAGE_SYM_CLASS_WEAK_EXTERNAL		       105
#define IMAGE_SYM_CLASS_CLR_TOKEN		       107

#define IMAGE_COMDAT_SELECT_NODUPLICATES		 1
#define IMAGE_COMDAT_SELECT_ANY 			 2
#define IMAGE_COMDAT_SELECT_SAME_SIZE			 3
#define IMAGE_COMDAT_SELECT_EXACT_MATCH 		 4
#define IMAGE_COMDAT_SELECT_ASSOCIATIVE 		 5
#define IMAGE_COMDAT_SELECT_LARGEST			 6
#define IMAGE_COMDAT_SELECT_NEWEST			 7 /* Not in PECOFF v8 spec */

#define IMAGE_WEAK_EXTERN_SEARCH_NOLIBRARY		 1
#define IMAGE_WEAK_EXTERN_SEARCH_LIBRARY		 2
#define IMAGE_WEAK_EXTERN_SEARCH_ALIAS			 3

#define IMAGE_REL_I386_ABSOLUTE 		    0x0000
#define IMAGE_REL_I386_DIR16			    0x0001
#define IMAGE_REL_I386_REL16			    0x0002
#define IMAGE_REL_I386_DIR32			    0x0006
#define IMAGE_REL_I386_DIR32NB			    0x0007
#define IMAGE_REL_I386_SEG12			    0x0009
#define IMAGE_REL_I386_SECTION			    0x000A
#define IMAGE_REL_I386_SECREL			    0x000B
#define IMAGE_REL_I386_TOKEN			    0x000C
#define IMAGE_REL_I386_SECREL7			    0x000D
#define IMAGE_REL_I386_REL32			    0x0014

#define IMAGE_REL_AMD64_ABSOLUTE		    0x0000
#define IMAGE_REL_AMD64_ADDR64			    0x0001
#define IMAGE_REL_AMD64_ADDR32			    0x0002
#define IMAGE_REL_AMD64_ADDR32NB		    0x0003
#define IMAGE_REL_AMD64_REL32			    0x0004
#define IMAGE_REL_AMD64_REL32_1 		    0x0005
#define IMAGE_REL_AMD64_REL32_2 		    0x0006
#define IMAGE_REL_AMD64_REL32_3 		    0x0007
#define IMAGE_REL_AMD64_REL32_4 		    0x0008
#define IMAGE_REL_AMD64_REL32_5 		    0x0009
#define IMAGE_REL_AMD64_SECTION 		    0x000A
#define IMAGE_REL_AMD64_SECREL			    0x000B
#define IMAGE_REL_AMD64_SECREL7 		    0x000C
#define IMAGE_REL_AMD64_TOKEN			    0x000D
#define IMAGE_REL_AMD64_SREL32			    0x000E
#define IMAGE_REL_AMD64_PAIR			    0x000F
#define IMAGE_REL_AMD64_SSPAN32 		    0x0010

#define IMAGE_REL_IA64_ABSOLUTE 		    0x0000
#define IMAGE_REL_IA64_IMM14			    0x0001
#define IMAGE_REL_IA64_IMM22			    0x0002
#define IMAGE_REL_IA64_IMM64			    0x0003
#define IMAGE_REL_IA64_DIR32			    0x0004
#define IMAGE_REL_IA64_DIR64			    0x0005
#define IMAGE_REL_IA64_PCREL21B 		    0x0006
#define IMAGE_REL_IA64_PCREL21M 		    0x0007
#define IMAGE_REL_IA64_PCREL21F 		    0x0008
#define IMAGE_REL_IA64_GPREL22			    0x0009
#define IMAGE_REL_IA64_LTOFF22			    0x000A
#define IMAGE_REL_IA64_SECTION			    0x000B
#define IMAGE_REL_IA64_SECREL22 		    0x000C
#define IMAGE_REL_IA64_SECREL64I		    0x000D
#define IMAGE_REL_IA64_SECREL32 		    0x000E
#define IMAGE_REL_IA64_DIR32NB			    0x0010
#define IMAGE_REL_IA64_SREL14			    0x0011
#define IMAGE_REL_IA64_SREL22			    0x0012
#define IMAGE_REL_IA64_SREL32			    0x0013
#define IMAGE_REL_IA64_UREL32			    0x0014
#define IMAGE_REL_IA64_PCREL60X 		    0x0015
#define IMAGE_REL_IA64_PCREL60B 		    0x0016
#define IMAGE_REL_IA64_PCREL60F 		    0x0017
#define IMAGE_REL_IA64_PCREL60I 		    0x0018
#define IMAGE_REL_IA64_PCREL60M 		    0x0019
#define IMAGE_REL_IA64_IMMGPREL64		    0x001a
#define IMAGE_REL_IA64_TOKEN			    0x001b
#define IMAGE_REL_IA64_GPREL32			    0x001c
#define IMAGE_REL_IA64_ADDEND			    0x001F

#define IMAGE_REL_SH3_ABSOLUTE			    0x0000
#define IMAGE_REL_SH3_DIRECT16			    0x0001
#define IMAGE_REL_SH3_DIRECT32			    0x0002
#define IMAGE_REL_SH3_DIRECT8			    0x0003
#define IMAGE_REL_SH3_DIRECT8_WORD		    0x0004
#define IMAGE_REL_SH3_DIRECT8_LONG		    0x0005
#define IMAGE_REL_SH3_DIRECT4			    0x0006
#define IMAGE_REL_SH3_DIRECT4_WORD		    0x0007
#define IMAGE_REL_SH3_DIRECT4_LONG		    0x0008
#define IMAGE_REL_SH3_PCREL8_WORD		    0x0009
#define IMAGE_REL_SH3_PCREL8_LONG		    0x000A
#define IMAGE_REL_SH3_PCREL12_WORD		    0x000B
#define IMAGE_REL_SH3_STARTOF_SECTION		    0x000C
#define IMAGE_REL_SH3_SIZEOF_SECTION		    0x000D
#define IMAGE_REL_SH3_SECTION			    0x000E
#define IMAGE_REL_SH3_SECREL			    0x000F
#define IMAGE_REL_SH3_DIRECT32_NB		    0x0010
#define IMAGE_REL_SH3_GPREL4_LONG		    0x0011
#define IMAGE_REL_SH3_TOKEN			    0x0012
#define IMAGE_REL_SHM_PCRELPT			    0x0013
#define IMAGE_REL_SHM_REFLO			    0x0014
#define IMAGE_REL_SHM_REFHALF			    0x0015
#define IMAGE_REL_SHM_RELLO			    0x0016
#define IMAGE_REL_SHM_RELHALF			    0x0017
#define IMAGE_REL_SHM_PAIR			    0x0018
#define IMAGE_REL_SHM_NOMODE			    0x8000

#define IMAGE_REL_M32R_ABSOLUTE 		    0x0000
#define IMAGE_REL_M32R_ADDR32			    0x0001
#define IMAGE_REL_M32R_ADDR32NB 		    0x0002
#define IMAGE_REL_M32R_ADDR24			    0x0003
#define IMAGE_REL_M32R_GPREL16			    0x0004
#define IMAGE_REL_M32R_PCREL24			    0x0005
#define IMAGE_REL_M32R_PCREL16			    0x0006
#define IMAGE_REL_M32R_PCREL8			    0x0007
#define IMAGE_REL_M32R_REFHALF			    0x0008
#define IMAGE_REL_M32R_REFHI			    0x0009
#define IMAGE_REL_M32R_REFLO			    0x000A
#define IMAGE_REL_M32R_PAIR			    0x000B
#define IMAGE_REL_M32R_SECTION			    0x000C
#define IMAGE_REL_M32R_SECREL			    0x000D
#define IMAGE_REL_M32R_TOKEN			    0x000E

#define IMAGE_REL_MIPS_ABSOLUTE 		    0x0000
#define IMAGE_REL_MIPS_REFHALF			    0x0001
#define IMAGE_REL_MIPS_REFWORD			    0x0002
#define IMAGE_REL_MIPS_JMPADDR			    0x0003
#define IMAGE_REL_MIPS_REFHI			    0x0004
#define IMAGE_REL_MIPS_REFLO			    0x0005
#define IMAGE_REL_MIPS_GPREL			    0x0006
#define IMAGE_REL_MIPS_LITERAL			    0x0007
#define IMAGE_REL_MIPS_SECTION			    0x000A
#define IMAGE_REL_MIPS_SECREL			    0x000B
#define IMAGE_REL_MIPS_SECRELLO 		    0x000C
#define IMAGE_REL_MIPS_SECRELHI 		    0x000D
#define IMAGE_REL_MIPS_JMPADDR16		    0x0010
#define IMAGE_REL_MIPS_REFWORDNB		    0x0022
#define IMAGE_REL_MIPS_PAIR			    0x0025

#define IMAGE_REL_ALPHA_ABSOLUTE			 0
#define IMAGE_REL_ALPHA_REFLONG 			 1
#define IMAGE_REL_ALPHA_REFQUAD 			 2
#define IMAGE_REL_ALPHA_GPREL32 			 3
#define IMAGE_REL_ALPHA_LITERAL 			 4
#define IMAGE_REL_ALPHA_LITUSE				 5
#define IMAGE_REL_ALPHA_GPDISP				 6
#define IMAGE_REL_ALPHA_BRADDR				 7
#define IMAGE_REL_ALPHA_HINT				 8
#define IMAGE_REL_ALPHA_INLINE_REFLONG			 9
#define IMAGE_REL_ALPHA_REFHI				10
#define IMAGE_REL_ALPHA_REFLO				11
#define IMAGE_REL_ALPHA_PAIR				12
#define IMAGE_REL_ALPHA_MATCH				13
#define IMAGE_REL_ALPHA_SECTION 			14
#define IMAGE_REL_ALPHA_SECREL				15
#define IMAGE_REL_ALPHA_REFLONGNB			16
#define IMAGE_REL_ALPHA_SECRELLO			17
#define IMAGE_REL_ALPHA_SECRELHI			18
#define IMAGE_REL_PPC_ABSOLUTE				 0
#define IMAGE_REL_PPC_ADDR64				 1
#define IMAGE_REL_PPC_ADDR32				 2
#define IMAGE_REL_PPC_ADDR24				 3
#define IMAGE_REL_PPC_ADDR16				 4
#define IMAGE_REL_PPC_ADDR14				 5
#define IMAGE_REL_PPC_REL24				 6
#define IMAGE_REL_PPC_REL14				 7
#define IMAGE_REL_PPC_TOCREL16				 8
#define IMAGE_REL_PPC_TOCREL14				 9
#define IMAGE_REL_PPC_ADDR32NB				10
#define IMAGE_REL_PPC_SECREL				11
#define IMAGE_REL_PPC_SECTION				12
#define IMAGE_REL_PPC_IFGLUE				13
#define IMAGE_REL_PPC_IMGLUE				14
#define IMAGE_REL_PPC_SECREL16				15
#define IMAGE_REL_PPC_REFHI				16
#define IMAGE_REL_PPC_REFLO				17
#define IMAGE_REL_PPC_PAIR				18
#define IMAGE_REL_PPC_TYPEMASK			       255
#define IMAGE_REL_PPC_NEG			       256
#define IMAGE_REL_PPC_BRTAKEN			       512
#define IMAGE_REL_PPC_BRNTAKEN			      1024
#define IMAGE_REL_PPC_TOCDEFN			      2048
#define IMAGE_REL_BASED_ABSOLUTE			 0
#define IMAGE_REL_BASED_HIGH				 1
#define IMAGE_REL_BASED_LOW				 2
#define IMAGE_REL_BASED_HIGHLOW 			 3
#define IMAGE_REL_BASED_HIGHADJ 			 4
#define IMAGE_REL_BASED_MIPS_JMPADDR			 5
#define IMAGE_ARCHIVE_START_SIZE			 8
#define IMAGE_ARCHIVE_START			"!<arch>\n"
#define IMAGE_ARCHIVE_END			"`\n"
#define IMAGE_ARCHIVE_PAD			"\n"
#define IMAGE_ARCHIVE_LINKER_MEMBER		"/               "
#define IMAGE_ARCHIVE_LONGNAMES_MEMBER		"//              "
#define IMAGE_ORDINAL_FLAG32			0x80000000
#define IMAGE_SNAP_BY_ORDINAL32(o)		((o&IMAGE_ORDINAL_FLAG32)!=0)
#define IMAGE_ORDINAL32(o)			(o&0xffff)
#define IMAGE_ORDINAL_FLAG64			0x8000000000000000ULL
#define IMAGE_SNAP_BY_ORDINAL64(o)		((o&IMAGE_ORDINAL_FLAG64)!=0)
#define IMAGE_ORDINAL64(o)			(o&0xffff)
#ifdef _WIN64
#define IMAGE_ORDINAL_FLAG			IMAGE_ORDINAL_FLAG64
#define IMAGE_SNAP_BY_ORDINAL(o)		IMAGE_SNAP_BY_ORDINAL64(o)
#define IMAGE_ORDINAL(o)			IMAGE_ORDINAL64(o)
#else
#define IMAGE_ORDINAL_FLAG			IMAGE_ORDINAL_FLAG32
#define IMAGE_SNAP_BY_ORDINAL(o)		IMAGE_SNAP_BY_ORDINAL32(o)
#define IMAGE_ORDINAL(o)			IMAGE_ORDINAL32(o)
#endif
#define IMAGE_RESOURCE_NAME_IS_STRING		0x80000000
#define IMAGE_RESOURCE_DATA_IS_DIRECTORY	0x80000000
#define IMAGE_DEBUG_TYPE_UNKNOWN			 0
#define IMAGE_DEBUG_TYPE_COFF				 1
#define IMAGE_DEBUG_TYPE_CODEVIEW			 2
#define IMAGE_DEBUG_TYPE_FPO				 3
#define IMAGE_DEBUG_TYPE_MISC				 4
#define IMAGE_DEBUG_TYPE_EXCEPTION			 5
#define IMAGE_DEBUG_TYPE_FIXUP				 6
#define IMAGE_DEBUG_TYPE_OMAP_TO_SRC			 7
#define IMAGE_DEBUG_TYPE_OMAP_FROM_SRC			 8
#define IMAGE_DEBUG_TYPE_BORLAND			 9
#define FRAME_FPO					 0
#define FRAME_TRAP					 1
#define FRAME_TSS					 2
#define FRAME_NONFPO					 3
#define IMAGE_DEBUG_MISC_EXENAME			 1
#define N_BTMASK				    0x000F
#define N_TMASK 				    0x0030
#define N_TMASK1				    0x00C0
#define N_TMASK2				    0x00F0
#define N_BTSHFT					 4
#define N_TSHIFT					 2
#define IS_TEXT_UNICODE_ASCII16 			 1
#define IS_TEXT_UNICODE_REVERSE_ASCII16 		16
#define IS_TEXT_UNICODE_STATISTICS			 2
#define IS_TEXT_UNICODE_REVERSE_STATISTICS		32
#define IS_TEXT_UNICODE_CONTROLS			 4
#define IS_TEXT_UNICODE_REVERSE_CONTROLS		64
#define IS_TEXT_UNICODE_SIGNATURE			 8
#define IS_TEXT_UNICODE_REVERSE_SIGNATURE	       128
#define IS_TEXT_UNICODE_ILLEGAL_CHARS		       256
#define IS_TEXT_UNICODE_ODD_LENGTH		       512
#define IS_TEXT_UNICODE_NULL_BYTES		      4096
#define IS_TEXT_UNICODE_UNICODE_MASK			15
#define IS_TEXT_UNICODE_REVERSE_MASK		       240
#define IS_TEXT_UNICODE_NOT_UNICODE_MASK	      3840
#define IS_TEXT_UNICODE_NOT_ASCII_MASK		     61440
#define SERVICE_KERNEL_DRIVER				 1
#define SERVICE_FILE_SYSTEM_DRIVER			 2
#define SERVICE_ADAPTER 				 4
#define SERVICE_RECOGNIZER_DRIVER			 8
#define SERVICE_DRIVER  (SERVICE_KERNEL_DRIVER|SERVICE_FILE_SYSTEM_DRIVER|SERVICE_RECOGNIZER_DRIVER)
#define SERVICE_WIN32_OWN_PROCESS			16
#define SERVICE_WIN32_SHARE_PROCESS			32
#define SERVICE_WIN32  (SERVICE_WIN32_OWN_PROCESS|SERVICE_WIN32_SHARE_PROCESS)
#define SERVICE_INTERACTIVE_PROCESS		       256
#define SERVICE_TYPE_ALL  (SERVICE_WIN32|SERVICE_ADAPTER|SERVICE_DRIVER|SERVICE_INTERACTIVE_PROCESS)
#define SERVICE_BOOT_START				 0
#define SERVICE_SYSTEM_START				 1
#define SERVICE_AUTO_START				 2
#define SERVICE_DEMAND_START				 3
#define SERVICE_DISABLED				 4
#define SERVICE_ERROR_IGNORE				 0
#define SERVICE_ERROR_NORMAL				 1
#define SERVICE_ERROR_SEVERE				 2
#define SERVICE_ERROR_CRITICAL				 3
#define SE_OWNER_DEFAULTED				 1
#define SE_GROUP_DEFAULTED				 2
#define SE_DACL_PRESENT 				 4
#define SE_DACL_DEFAULTED				 8
#define SE_SACL_PRESENT 				16
#define SE_SACL_DEFAULTED				32
#define SE_DACL_AUTO_INHERIT_REQ		       256
#define SE_SACL_AUTO_INHERIT_REQ		       512
#define SE_DACL_AUTO_INHERITED			      1024
#define SE_SACL_AUTO_INHERITED			      2048
#define SE_DACL_PROTECTED			      4096
#define SE_SACL_PROTECTED			      8192
#define SE_SELF_RELATIVE			    0x8000
#define SECURITY_DESCRIPTOR_MIN_LENGTH			20
#define SECURITY_DESCRIPTOR_REVISION			 1
#define SECURITY_DESCRIPTOR_REVISION1			 1
#define SE_PRIVILEGE_ENABLED_BY_DEFAULT 		 1
#define SE_PRIVILEGE_ENABLED				 2
#define SE_PRIVILEGE_USED_FOR_ACCESS		0x80000000
#define PRIVILEGE_SET_ALL_NECESSARY			 1
#define SECURITY_MAX_IMPERSONATION_LEVEL	SecurityDelegation
#define DEFAULT_IMPERSONATION_LEVEL		SecurityImpersonation
#define SECURITY_DYNAMIC_TRACKING		      TRUE
#define SECURITY_STATIC_TRACKING		     FALSE
/* also in ddk/ntifs.h */
#define TOKEN_ASSIGN_PRIMARY			   (0x0001)
#define TOKEN_DUPLICATE 			   (0x0002)
#define TOKEN_IMPERSONATE			   (0x0004)
#define TOKEN_QUERY				   (0x0008)
#define TOKEN_QUERY_SOURCE			   (0x0010)
#define TOKEN_ADJUST_PRIVILEGES 		   (0x0020)
#define TOKEN_ADJUST_GROUPS			   (0x0040)
#define TOKEN_ADJUST_DEFAULT			   (0x0080)

#define TOKEN_ALL_ACCESS     \
  ( STANDARD_RIGHTS_REQUIRED | TOKEN_ASSIGN_PRIMARY | TOKEN_DUPLICATE      | \
    TOKEN_IMPERSONATE        | TOKEN_QUERY          | TOKEN_QUERY_SOURCE   | \
    TOKEN_ADJUST_PRIVILEGES  | TOKEN_ADJUST_GROUPS  | TOKEN_ADJUST_DEFAULT \
  )
#define TOKEN_READ	  ( STANDARD_RIGHTS_READ | TOKEN_QUERY )
#define TOKEN_WRITE	  \
  ( STANDARD_RIGHTS_WRITE | TOKEN_ADJUST_PRIVILEGES | \
    TOKEN_ADJUST_GROUPS   | TOKEN_ADJUST_DEFAULT    \
  )
#define TOKEN_EXECUTE	  ( STANDARD_RIGHTS_EXECUTE )

#define TOKEN_SOURCE_LENGTH				 8
#if (_WIN32_WINNT >= 0x0600)
#define TOKEN_MANDATORY_POLICY_OFF			 0
#define TOKEN_MANDATORY_POLICY_NO_WRITE_UP		 1
#define TOKEN_MANDATORY_POLICY_NEW_PROCESS_MIN		 2
#define TOKEN_MANDATORY_POLICY_VALID_MASK		 3
#endif
/* end ddk/ntifs.h */
#define DLL_PROCESS_DETACH				 0
#define DLL_PROCESS_ATTACH				 1
#define DLL_THREAD_ATTACH				 2
#define DLL_THREAD_DETACH				 3
#define DBG_CONTINUE				   0x10002
#define DBG_TERMINATE_THREAD			0x40010003
#define DBG_TERMINATE_PROCESS			0x40010004
#define DBG_CONTROL_C				0x40010005
#define DBG_CONTROL_BREAK			0x40010008
#define DBG_EXCEPTION_NOT_HANDLED		0x80010001
#define TAPE_ABSOLUTE_POSITION				 0
#define TAPE_LOGICAL_POSITION				 1
#define TAPE_PSEUDO_LOGICAL_POSITION			 2
#define TAPE_REWIND					 0
#define TAPE_ABSOLUTE_BLOCK				 1
#define TAPE_LOGICAL_BLOCK				 2
#define TAPE_PSEUDO_LOGICAL_BLOCK			 3
#define TAPE_SPACE_END_OF_DATA				 4
#define TAPE_SPACE_RELATIVE_BLOCKS			 5
#define TAPE_SPACE_FILEMARKS				 6
#define TAPE_SPACE_SEQUENTIAL_FMKS			 7
#define TAPE_SPACE_SETMARKS				 8
#define TAPE_SPACE_SEQUENTIAL_SMKS			 9
#define TAPE_DRIVE_FIXED				 1
#define TAPE_DRIVE_SELECT				 2
#define TAPE_DRIVE_INITIATOR				 4
#define TAPE_DRIVE_ERASE_SHORT				16
#define TAPE_DRIVE_ERASE_LONG				32
#define TAPE_DRIVE_ERASE_BOP_ONLY			64
#define TAPE_DRIVE_ERASE_IMMEDIATE		       128
#define TAPE_DRIVE_TAPE_CAPACITY		       256
#define TAPE_DRIVE_TAPE_REMAINING		       512
#define TAPE_DRIVE_FIXED_BLOCK			      1024
#define TAPE_DRIVE_VARIABLE_BLOCK		      2048
#define TAPE_DRIVE_WRITE_PROTECT		      4096
#define TAPE_DRIVE_EOT_WZ_SIZE			      8192
#define TAPE_DRIVE_ECC				   0x10000
#define TAPE_DRIVE_COMPRESSION			   0x20000
#define TAPE_DRIVE_PADDING			   0x40000
#define TAPE_DRIVE_REPORT_SMKS			   0x80000
#define TAPE_DRIVE_GET_ABSOLUTE_BLK		  0x100000
#define TAPE_DRIVE_GET_LOGICAL_BLK		  0x200000
#define TAPE_DRIVE_SET_EOT_WZ_SIZE		  0x400000
#define TAPE_DRIVE_EJECT_MEDIA			 0x1000000
#define TAPE_DRIVE_CLEAN_REQUESTS		 0x2000000
#define TAPE_DRIVE_SET_CMP_BOP_ONLY		 0x4000000
#define TAPE_DRIVE_RESERVED_BIT 		0x80000000
#define TAPE_DRIVE_LOAD_UNLOAD			0x80000001
#define TAPE_DRIVE_TENSION			0x80000002
#define TAPE_DRIVE_LOCK_UNLOCK			0x80000004
#define TAPE_DRIVE_REWIND_IMMEDIATE		0x80000008
#define TAPE_DRIVE_SET_BLOCK_SIZE		0x80000010
#define TAPE_DRIVE_LOAD_UNLD_IMMED		0x80000020
#define TAPE_DRIVE_TENSION_IMMED		0x80000040
#define TAPE_DRIVE_LOCK_UNLK_IMMED		0x80000080
#define TAPE_DRIVE_SET_ECC			0x80000100
#define TAPE_DRIVE_SET_COMPRESSION		0x80000200
#define TAPE_DRIVE_SET_PADDING			0x80000400
#define TAPE_DRIVE_SET_REPORT_SMKS		0x80000800
#define TAPE_DRIVE_ABSOLUTE_BLK 		0x80001000
#define TAPE_DRIVE_ABS_BLK_IMMED		0x80002000
#define TAPE_DRIVE_LOGICAL_BLK			0x80004000
#define TAPE_DRIVE_LOG_BLK_IMMED		0x80008000
#define TAPE_DRIVE_END_OF_DATA			0x80010000
#define TAPE_DRIVE_RELATIVE_BLKS		0x80020000
#define TAPE_DRIVE_FILEMARKS			0x80040000
#define TAPE_DRIVE_SEQUENTIAL_FMKS		0x80080000
#define TAPE_DRIVE_SETMARKS			0x80100000
#define TAPE_DRIVE_SEQUENTIAL_SMKS		0x80200000
#define TAPE_DRIVE_REVERSE_POSITION		0x80400000
#define TAPE_DRIVE_SPACE_IMMEDIATE		0x80800000
#define TAPE_DRIVE_WRITE_SETMARKS		0x81000000
#define TAPE_DRIVE_WRITE_FILEMARKS		0x82000000
#define TAPE_DRIVE_WRITE_SHORT_FMKS		0x84000000
#define TAPE_DRIVE_WRITE_LONG_FMKS		0x88000000
#define TAPE_DRIVE_WRITE_MARK_IMMED		0x90000000
#define TAPE_DRIVE_FORMAT			0xA0000000
#define TAPE_DRIVE_FORMAT_IMMEDIATE		0xC0000000
#define TAPE_DRIVE_HIGH_FEATURES		0x80000000
#define TAPE_FIXED_PARTITIONS				 0
#define TAPE_INITIATOR_PARTITIONS			 2
#define TAPE_SELECT_PARTITIONS				 1
#define TAPE_FILEMARKS					 1
#define TAPE_LONG_FILEMARKS				 3
#define TAPE_SETMARKS					 0
#define TAPE_SHORT_FILEMARKS				 2
#define TAPE_ERASE_LONG 				 1
#define TAPE_ERASE_SHORT				 0
#define TAPE_LOAD					 0
#define TAPE_UNLOAD					 1
#define TAPE_TENSION					 2
#define TAPE_LOCK					 3
#define TAPE_UNLOCK					 4
#define TAPE_FORMAT					 5
#if (_WIN32_WINNT >= 0x0500)
#define VER_MINORVERSION			 0x0000001
#define VER_MAJORVERSION			 0x0000002
#define VER_BUILDNUMBER 			 0x0000004
#define VER_PLATFORMID				 0x0000008
#define VER_SERVICEPACKMINOR			 0x0000010
#define VER_SERVICEPACKMAJOR			 0x0000020
#define VER_SUITENAME				 0x0000040
#define VER_PRODUCT_TYPE			 0x0000080
#define VER_EQUAL					 1
#define VER_GREATER					 2
#define VER_GREATER_EQUAL				 3
#define VER_LESS					 4
#define VER_LESS_EQUAL					 5
#define VER_AND 					 6
#define VER_OR						 7
#endif
#define VER_PLATFORM_WIN32s				 0
#define VER_PLATFORM_WIN32_WINDOWS			 1
#define VER_PLATFORM_WIN32_NT				 2
#define VER_NT_WORKSTATION				 1
#define VER_NT_DOMAIN_CONTROLLER			 2
#define VER_NT_SERVER					 3
#define VER_SUITE_SMALLBUSINESS 			 1
#define VER_SUITE_ENTERPRISE				 2
#define VER_SUITE_BACKOFFICE				 4
#define VER_SUITE_TERMINAL				16
#define VER_SUITE_SMALLBUSINESS_RESTRICTED		32
#define VER_SUITE_EMBEDDEDNT				64
#define VER_SUITE_DATACENTER			       128
#define VER_SUITE_SINGLEUSERTS			       256
#define VER_SUITE_PERSONAL			       512
#define VER_SUITE_BLADE 			      1024
#define VER_SUITE_STORAGE_SERVER		      8192
#define VER_SUITE_COMPUTE_SERVER		     16384
#define WT_EXECUTEDEFAULT			0x00000000
#define WT_EXECUTEINIOTHREAD			0x00000001
#define WT_EXECUTEINWAITTHREAD			0x00000004
#define WT_EXECUTEONLYONCE			0x00000008
#define WT_EXECUTELONGFUNCTION			0x00000010
#define WT_EXECUTEINTIMERTHREAD 		0x00000020
#define WT_EXECUTEINPERSISTENTTHREAD		0x00000080
#define WT_TRANSFER_IMPERSONATION		0x00000100
#if (_WIN32_WINNT >= 0x0501)
#define ACTIVATION_CONTEXT_SECTION_ASSEMBLY_INFORMATION 	 1
#define ACTIVATION_CONTEXT_SECTION_DLL_REDIRECTION		 2
#define ACTIVATION_CONTEXT_SECTION_WINDOW_CLASS_REDIRECTION	 3
#define ACTIVATION_CONTEXT_SECTION_COM_SERVER_REDIRECTION	 4
#define ACTIVATION_CONTEXT_SECTION_COM_INTERFACE_REDIRECTION	 5
#define ACTIVATION_CONTEXT_SECTION_COM_TYPE_LIBRARY_REDIRECTION  6
#define ACTIVATION_CONTEXT_SECTION_COM_PROGID_REDIRECTION	 7
#define ACTIVATION_CONTEXT_SECTION_CLR_SURROGATES		 9
#endif /* (_WIN32_WINNT >= 0x0501) */

#define BTYPE(x)	((x)&N_BTMASK)
#define ISPTR(x)	(((x)&N_TMASK)==(IMAGE_SYM_DTYPE_POINTER<<N_BTSHFT))
#define ISFCN(x)	(((x)&N_TMASK)==(IMAGE_SYM_DTYPE_FUNCTION<<N_BTSHFT))
#define ISARY(x)	(((x)&N_TMASK)==(IMAGE_SYM_DTYPE_ARRAY<<N_BTSHFT))
#define ISTAG(x)	((x)==IMAGE_SYM_CLASS_STRUCT_TAG||(x)==IMAGE_SYM_CLASS_UNION_TAG||(x)==IMAGE_SYM_CLASS_ENUM_TAG)
#define INCREF(x)	((((x)&~N_BTMASK)<<N_TSHIFT)|(IMAGE_SYM_DTYPE_POINTER<<N_BTSHFT)|((x)&N_BTMASK))
#define DECREF(x)	((((x)>>N_TSHIFT)&~N_BTMASK)|((x)&N_BTMASK))

#define TLS_MINIMUM_AVAILABLE				64
#define REPARSE_DATA_BUFFER_HEADER_SIZE  FIELD_OFFSET(REPARSE_DATA_BUFFER, GenericReparseBuffer)
#define REPARSE_GUID_DATA_BUFFER_HEADER_SIZE  FIELD_OFFSET(REPARSE_GUID_DATA_BUFFER, GenericReparseBuffer)
#define MAXIMUM_REPARSE_DATA_BUFFER_SIZE	     16384
#define IO_REPARSE_TAG_RESERVED_ZERO			 0
#define IO_REPARSE_TAG_RESERVED_ONE			 1
#define IO_REPARSE_TAG_RESERVED_RANGE	IO_REPARSE_TAG_RESERVED_ONE
#define IsReparseTagMicrosoft(x)	   ((x)&0x80000000)
#define IsReparseTagHighLatency(x)	   ((x)&0x40000000)
#define IsReparseTagNameSurrogate(x)	   ((x)&0x20000000)
#define IO_REPARSE_TAG_VALID_VALUES		0xE000FFFF
#define IsReparseTagValid(x)  (!((x)&~IO_REPARSE_TAG_VALID_VALUES)&&((x)>IO_REPARSE_TAG_RESERVED_RANGE))
#define IO_REPARSE_TAG_SYMBOLIC_LINK  IO_REPARSE_TAG_RESERVED_ZERO
#define IO_REPARSE_TAG_MOUNT_POINT		0xA0000003
#define IO_REPARSE_TAG_SYMLINK			0xA000000C

#define WT_SET_MAX_THREADPOOL_THREADS(Flags,Limit)  ((Flags)|=(Limit)<<16)
#ifndef RC_INVOKED

_BEGIN_C_DECLS

typedef DWORD ACCESS_MASK, *PACCESS_MASK;

#ifdef _GUID_DEFINED
# warning _GUID_DEFINED is deprecated, use GUID_DEFINED instead
#endif

#if ! (defined _GUID_DEFINED || defined GUID_DEFINED) /* also defined in basetyps.h */
#define GUID_DEFINED
typedef struct _GUID
{ unsigned long 	Data1;
  unsigned short	Data2;
  unsigned short	Data3;
  unsigned char 	Data4[8];
} GUID, *REFGUID, *LPGUID;
#define SYSTEM_LUID			      { 0x3e7, 0x0 }
#endif /* GUID_DEFINED */

typedef struct _GENERIC_MAPPING
{ ACCESS_MASK		GenericRead;
  ACCESS_MASK		GenericWrite;
  ACCESS_MASK		GenericExecute;
  ACCESS_MASK		GenericAll;
} GENERIC_MAPPING, *PGENERIC_MAPPING;

typedef struct _ACE_HEADER
{ BYTE			AceType;
  BYTE			AceFlags;
  WORD			AceSize;
} ACE_HEADER, *PACE_HEADER;

typedef struct _ACCESS_ALLOWED_ACE
{ ACE_HEADER		Header;
  ACCESS_MASK		Mask;
  DWORD 		SidStart;
} ACCESS_ALLOWED_ACE, *PACCESS_ALLOWED_ACE;

typedef struct _ACCESS_DENIED_ACE
{ ACE_HEADER		Header;
  ACCESS_MASK		Mask;
  DWORD 		SidStart;
} ACCESS_DENIED_ACE, *PACCESS_DENIED_ACE;

typedef struct _SYSTEM_AUDIT_ACE
{ ACE_HEADER		Header;
  ACCESS_MASK		Mask;
  DWORD 		SidStart;
} SYSTEM_AUDIT_ACE;
typedef SYSTEM_AUDIT_ACE *PSYSTEM_AUDIT_ACE;

typedef struct _SYSTEM_ALARM_ACE
{ ACE_HEADER		Header;
  ACCESS_MASK		Mask;
  DWORD 		SidStart;
} SYSTEM_ALARM_ACE, *PSYSTEM_ALARM_ACE;

typedef struct _ACCESS_ALLOWED_OBJECT_ACE
{ ACE_HEADER		Header;
  ACCESS_MASK		Mask;
  DWORD 		Flags;
  GUID			ObjectType;
  GUID			InheritedObjectType;
  DWORD 		SidStart;
} ACCESS_ALLOWED_OBJECT_ACE, *PACCESS_ALLOWED_OBJECT_ACE;

typedef struct _ACCESS_DENIED_OBJECT_ACE
{ ACE_HEADER		Header;
  ACCESS_MASK		Mask;
  DWORD 		Flags;
  GUID			ObjectType;
  GUID			InheritedObjectType;
  DWORD 		SidStart;
} ACCESS_DENIED_OBJECT_ACE, *PACCESS_DENIED_OBJECT_ACE;

typedef struct _SYSTEM_AUDIT_OBJECT_ACE
{ ACE_HEADER		Header;
  ACCESS_MASK		Mask;
  DWORD 		Flags;
  GUID			ObjectType;
  GUID			InheritedObjectType;
  DWORD 		SidStart;
} SYSTEM_AUDIT_OBJECT_ACE, *PSYSTEM_AUDIT_OBJECT_ACE;

typedef struct _SYSTEM_ALARM_OBJECT_ACE
{ ACE_HEADER		Header;
  ACCESS_MASK		Mask;
  DWORD 		Flags;
  GUID			ObjectType;
  GUID			InheritedObjectType;
  DWORD 		SidStart;
} SYSTEM_ALARM_OBJECT_ACE, *PSYSTEM_ALARM_OBJECT_ACE;

typedef struct _ACL
{ BYTE			AclRevision;
  BYTE			Sbz1;
  WORD			AclSize;
  WORD			AceCount;
  WORD			Sbz2;
} ACL, *PACL;

typedef struct _ACL_REVISION_INFORMATION
{ DWORD 		AclRevision;
} ACL_REVISION_INFORMATION;

typedef struct _ACL_SIZE_INFORMATION
{ DWORD 		AceCount;
  DWORD 		AclBytesInUse;
  DWORD 		AclBytesFree;
} ACL_SIZE_INFORMATION;

/* FIXME: add more machines */
#ifdef _X86_
#define SIZE_OF_80387_REGISTERS 			80
#define CONTEXT_i386				   0x10000
#define CONTEXT_i486				   0x10000
#define CONTEXT_CONTROL 		  (CONTEXT_i386|0x00000001L)
#define CONTEXT_INTEGER 		  (CONTEXT_i386|0x00000002L)
#define CONTEXT_SEGMENTS		  (CONTEXT_i386|0x00000004L)
#define CONTEXT_FLOATING_POINT		  (CONTEXT_i386|0x00000008L)
#define CONTEXT_DEBUG_REGISTERS 	  (CONTEXT_i386|0x00000010L)
#define CONTEXT_EXTENDED_REGISTERS	  (CONTEXT_i386|0x00000020L)

#define CONTEXT_FULL		(CONTEXT_CONTROL|CONTEXT_INTEGER|CONTEXT_SEGMENTS)

#define MAXIMUM_SUPPORTED_EXTENSION		       512

typedef struct _FLOATING_SAVE_AREA
{ DWORD 		ControlWord;
  DWORD 		StatusWord;
  DWORD 		TagWord;
  DWORD 		ErrorOffset;
  DWORD 		ErrorSelector;
  DWORD 		DataOffset;
  DWORD 		DataSelector;
  BYTE			RegisterArea[80];
  DWORD 		Cr0NpxState;
} FLOATING_SAVE_AREA;

typedef struct _CONTEXT
{ DWORD 		ContextFlags;
  DWORD 		Dr0;
  DWORD 		Dr1;
  DWORD 		Dr2;
  DWORD 		Dr3;
  DWORD 		Dr6;
  DWORD 		Dr7;
  FLOATING_SAVE_AREA	FloatSave;
  DWORD 		SegGs;
  DWORD 		SegFs;
  DWORD 		SegEs;
  DWORD 		SegDs;
  DWORD 		Edi;
  DWORD 		Esi;
  DWORD 		Ebx;
  DWORD 		Edx;
  DWORD 		Ecx;
  DWORD 		Eax;
  DWORD 		Ebp;
  DWORD 		Eip;
  DWORD 		SegCs;
  DWORD 		EFlags;
  DWORD 		Esp;
  DWORD 		SegSs;
  BYTE			ExtendedRegisters[MAXIMUM_SUPPORTED_EXTENSION];
} CONTEXT;

#elif defined(_M_PPC)
#define CONTEXT_CONTROL 				 1L
#define CONTEXT_FLOATING_POINT				 2L
#define CONTEXT_INTEGER 				 4L
#define CONTEXT_DEBUG_REGISTERS 			 8L
#define CONTEXT_FULL		(CONTEXT_CONTROL|CONTEXT_FLOATING_POINT|CONTEXT_INTEGER)

typedef struct
{ double		Fpr0;
  double		Fpr1;
  double		Fpr2;
  double		Fpr3;
  double		Fpr4;
  double		Fpr5;
  double		Fpr6;
  double		Fpr7;
  double		Fpr8;
  double		Fpr9;
  double		Fpr10;
  double		Fpr11;
  double		Fpr12;
  double		Fpr13;
  double		Fpr14;
  double		Fpr15;
  double		Fpr16;
  double		Fpr17;
  double		Fpr18;
  double		Fpr19;
  double		Fpr20;
  double		Fpr21;
  double		Fpr22;
  double		Fpr23;
  double		Fpr24;
  double		Fpr25;
  double		Fpr26;
  double		Fpr27;
  double		Fpr28;
  double		Fpr29;
  double		Fpr30;
  double		Fpr31;
  double		Fpscr;
  DWORD 		Gpr0;
  DWORD 		Gpr1;
  DWORD 		Gpr2;
  DWORD 		Gpr3;
  DWORD 		Gpr4;
  DWORD 		Gpr5;
  DWORD 		Gpr6;
  DWORD 		Gpr7;
  DWORD 		Gpr8;
  DWORD 		Gpr9;
  DWORD 		Gpr10;
  DWORD 		Gpr11;
  DWORD 		Gpr12;
  DWORD 		Gpr13;
  DWORD 		Gpr14;
  DWORD 		Gpr15;
  DWORD 		Gpr16;
  DWORD 		Gpr17;
  DWORD 		Gpr18;
  DWORD 		Gpr19;
  DWORD 		Gpr20;
  DWORD 		Gpr21;
  DWORD 		Gpr22;
  DWORD 		Gpr23;
  DWORD 		Gpr24;
  DWORD 		Gpr25;
  DWORD 		Gpr26;
  DWORD 		Gpr27;
  DWORD 		Gpr28;
  DWORD 		Gpr29;
  DWORD 		Gpr30;
  DWORD 		Gpr31;
  DWORD 		Cr;
  DWORD 		Xer;
  DWORD 		Msr;
  DWORD 		Iar;
  DWORD 		Lr;
  DWORD 		Ctr;
  DWORD 		ContextFlags;
  DWORD 		Fill[3];
  DWORD 		Dr0;
  DWORD 		Dr1;
  DWORD 		Dr2;
  DWORD 		Dr3;
  DWORD 		Dr4;
  DWORD 		Dr5;
  DWORD 		Dr6;
  DWORD 		Dr7;
} CONTEXT;

#elif defined(_M_ALPHA)
#define CONTEXT_ALPHA				   0x20000
#define CONTEXT_CONTROL 		      (CONTEXT_ALPHA|1L)
#define CONTEXT_FLOATING_POINT		      (CONTEXT_ALPHA|2L)
#define CONTEXT_INTEGER 		      (CONTEXT_ALPHA|4L)
#define CONTEXT_FULL		(CONTEXT_CONTROL|CONTEXT_FLOATING_POINT|CONTEXT_INTEGER)

typedef struct _CONTEXT
{ ULONGLONG		FltF0;
  ULONGLONG		FltF1;
  ULONGLONG		FltF2;
  ULONGLONG		FltF3;
  ULONGLONG		FltF4;
  ULONGLONG		FltF5;
  ULONGLONG		FltF6;
  ULONGLONG		FltF7;
  ULONGLONG		FltF8;
  ULONGLONG		FltF9;
  ULONGLONG		FltF10;
  ULONGLONG		FltF11;
  ULONGLONG		FltF12;
  ULONGLONG		FltF13;
  ULONGLONG		FltF14;
  ULONGLONG		FltF15;
  ULONGLONG		FltF16;
  ULONGLONG		FltF17;
  ULONGLONG		FltF18;
  ULONGLONG		FltF19;
  ULONGLONG		FltF20;
  ULONGLONG		FltF21;
  ULONGLONG		FltF22;
  ULONGLONG		FltF23;
  ULONGLONG		FltF24;
  ULONGLONG		FltF25;
  ULONGLONG		FltF26;
  ULONGLONG		FltF27;
  ULONGLONG		FltF28;
  ULONGLONG		FltF29;
  ULONGLONG		FltF30;
  ULONGLONG		FltF31;
  ULONGLONG		IntV0;
  ULONGLONG		IntT0;
  ULONGLONG		IntT1;
  ULONGLONG		IntT2;
  ULONGLONG		IntT3;
  ULONGLONG		IntT4;
  ULONGLONG		IntT5;
  ULONGLONG		IntT6;
  ULONGLONG		IntT7;
  ULONGLONG		IntS0;
  ULONGLONG		IntS1;
  ULONGLONG		IntS2;
  ULONGLONG		IntS3;
  ULONGLONG		IntS4;
  ULONGLONG		IntS5;
  ULONGLONG		IntFp;
  ULONGLONG		IntA0;
  ULONGLONG		IntA1;
  ULONGLONG		IntA2;
  ULONGLONG		IntA3;
  ULONGLONG		IntA4;
  ULONGLONG		IntA5;
  ULONGLONG		IntT8;
  ULONGLONG		IntT9;
  ULONGLONG		IntT10;
  ULONGLONG		IntT11;
  ULONGLONG		IntRa;
  ULONGLONG		IntT12;
  ULONGLONG		IntAt;
  ULONGLONG		IntGp;
  ULONGLONG		IntSp;
  ULONGLONG		IntZero;
  ULONGLONG		Fpcr;
  ULONGLONG		SoftFpcr;
  ULONGLONG		Fir;
  DWORD 		Psr;
  DWORD 		ContextFlags;
  DWORD 		Fill[4];
} CONTEXT;

#elif defined(SHx)
/* These are the debug or break registers on the SH3 */
typedef struct _DEBUG_REGISTERS
{ ULONG 		BarA;
  UCHAR 		BasrA;
  UCHAR 		BamrA;
  USHORT		BbrA;
  ULONG 		BarB;
  UCHAR 		BasrB;
  UCHAR 		BamrB;
  USHORT		BbrB;
  ULONG 		BdrB;
  ULONG 		BdmrB;
  USHORT		Brcr;
  USHORT		Align;
} DEBUG_REGISTERS, *PDEBUG_REGISTERS;

/* The following flags control the contents of the CONTEXT structure. */

#define CONTEXT_SH3				0x00000040
#define CONTEXT_SH4				0x000000c0	/* CONTEXT_SH3 | 0x80 - must contain the SH3 bits */

#ifdef SH3
#define CONTEXT_CONTROL 		(CONTEXT_SH3 | 0x00000001L)
#define CONTEXT_INTEGER 		(CONTEXT_SH3 | 0x00000002L)
#define CONTEXT_DEBUG_REGISTERS 	(CONTEXT_SH3 | 0x00000008L)
#define CONTEXT_FULL			(CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_DEBUG_REGISTERS)
#else	/* SH4 */
#define CONTEXT_CONTROL 		(CONTEXT_SH4 | 0x00000001L)
#define CONTEXT_INTEGER 		(CONTEXT_SH4 | 0x00000002L)
#define CONTEXT_DEBUG_REGISTERS 	(CONTEXT_SH4 | 0x00000008L)
#define CONTEXT_FLOATING_POINT		(CONTEXT_SH4 | 0x00000004L)
#define CONTEXT_FULL			(CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_DEBUG_REGISTERS | CONTEXT_FLOATING_POINT)
#endif

/* Context Frame
 * -------------
 *
 * This frame is used to store a limited processor context into the
 * thread structure for CPUs which have no floating point support.
 */
typedef struct _CONTEXT
{ /* The flags values within this structure control the contents
   * of a CONTEXT record.
   *
   * If the context record is used as an input parameter, then
   * for each portion of the context record controlled by a flag
   * whose value is set, it is assumed that that portion of the
   * context record contains valid context. If the context record
   * is being used to modify a thread's context, then only that
   * portion of the threads context will be modified.
   *
   * If the context record is used as an IN OUT parameter to
   * capture the context of a thread, then only those portions
   * of the thread's context corresponding to set flags will be
   * returned.
   *
   * The context record is never used as an OUT only parameter.
   */
  ULONG 		ContextFlags;
  /*
   * This section is specified/returned if the ContextFlags word
   * contains the flag CONTEXT_INTEGER.
   *
   * N.B. The registers RA and R15 are defined in this section,
   * but are considered part of the control context rather than
   * part of the integer context.
   */
  ULONG 		PR;
  ULONG 		MACH;
  ULONG 		MACL;
  ULONG 		GBR;
  ULONG 		R0;
  ULONG 		R1;
  ULONG 		R2;
  ULONG 		R3;
  ULONG 		R4;
  ULONG 		R5;
  ULONG 		R6;
  ULONG 		R7;
  ULONG 		R8;
  ULONG 		R9;
  ULONG 		R10;
  ULONG 		R11;
  ULONG 		R12;
  ULONG 		R13;
  ULONG 		R14;
  ULONG 		R15;
  /*
   * This section is specified/returned if the ContextFlags
   * word contains the flag CONTEXT_CONTROL.
   *
   * N.B. The registers r15 and ra are defined in the integer
   * section, but are considered part of the control context
   * rather than part of the integer context.
   */
  ULONG 		Fir;
  ULONG 		Psr;
#if !defined(SH3e) && !defined(SH4)
  ULONG 		OldStuff[2];
  DEBUG_REGISTERS	DebugRegisters;
#else
  ULONG 		Fpscr;
  ULONG 		Fpul;
  ULONG 		FRegs[16];
#if defined(SH4)
  ULONG 		xFRegs[16];
#endif
#endif
} CONTEXT;

#elif defined(_M_MRX000)	/* MIPS */

/* The following flags control the contents of the CONTEXT structure.
 */
#define CONTEXT_R4000				0x00010000    /* r4000 context */

#define CONTEXT_CONTROL 		(CONTEXT_R4000 | 0x00000001L)
#define CONTEXT_FLOATING_POINT		(CONTEXT_R4000 | 0x00000002L)
#define CONTEXT_INTEGER 		(CONTEXT_R4000 | 0x00000004L)

#define CONTEXT_FULL		(CONTEXT_CONTROL | CONTEXT_FLOATING_POINT | CONTEXT_INTEGER)

/* Context Frame
 * -------------
 *
 * N.B. This frame must be exactly a multiple of 16 bytes in
 * length; it has a several purposes:--
 *
 *   1) it is used as an argument to NtContinue;
 *   2) it is used to constuct a call frame for APC delivery;
 *   3) it is used to construct a call frame for exception
 *      dispatching in user mode;
 *   4) it is used in user level thread creation routines.
 *
 * The layout of the record conforms to a standard call frame.
 */
typedef struct _CONTEXT
{ /* This section is always present and is used as an argument
   * build area.
   */
  DWORD 		Argument[4];
  /*
   * This section is specified/returned if the ContextFlags word
   * contains the flag CONTEXT_FLOATING_POINT.
   */
  DWORD 		FltF0;
  DWORD 		FltF1;
  DWORD 		FltF2;
  DWORD 		FltF3;
  DWORD 		FltF4;
  DWORD 		FltF5;
  DWORD 		FltF6;
  DWORD 		FltF7;
  DWORD 		FltF8;
  DWORD 		FltF9;
  DWORD 		FltF10;
  DWORD 		FltF11;
  DWORD 		FltF12;
  DWORD 		FltF13;
  DWORD 		FltF14;
  DWORD 		FltF15;
  DWORD 		FltF16;
  DWORD 		FltF17;
  DWORD 		FltF18;
  DWORD 		FltF19;
  DWORD 		FltF20;
  DWORD 		FltF21;
  DWORD 		FltF22;
  DWORD 		FltF23;
  DWORD 		FltF24;
  DWORD 		FltF25;
  DWORD 		FltF26;
  DWORD 		FltF27;
  DWORD 		FltF28;
  DWORD 		FltF29;
  DWORD 		FltF30;
  DWORD 		FltF31;
  /*
   * This section is specified/returned if the ContextFlags word
   * contains the flag CONTEXT_INTEGER.
   *
   * N.B. The registers gp, sp, and ra are defined in this section,
   * but are considered part of the control context rather than part
   * of the integer context.
   *
   * N.B. Register zero is not stored in the frame.
   */
  DWORD 		IntZero;
  DWORD 		IntAt;
  DWORD 		IntV0;
  DWORD 		IntV1;
  DWORD 		IntA0;
  DWORD 		IntA1;
  DWORD 		IntA2;
  DWORD 		IntA3;
  DWORD 		IntT0;
  DWORD 		IntT1;
  DWORD 		IntT2;
  DWORD 		IntT3;
  DWORD 		IntT4;
  DWORD 		IntT5;
  DWORD 		IntT6;
  DWORD 		IntT7;
  DWORD 		IntS0;
  DWORD 		IntS1;
  DWORD 		IntS2;
  DWORD 		IntS3;
  DWORD 		IntS4;
  DWORD 		IntS5;
  DWORD 		IntS6;
  DWORD 		IntS7;
  DWORD 		IntT8;
  DWORD 		IntT9;
  DWORD 		IntK0;
  DWORD 		IntK1;
  DWORD 		IntGp;
  DWORD 		IntSp;
  DWORD 		IntS8;
  DWORD 		IntRa;
  DWORD 		IntLo;
  DWORD 		IntHi;
  /*
   * This section is specified/returned if the ContextFlags word
   * contains the flag CONTEXT_FLOATING_POINT.
   */
  DWORD 		Fsr;
  /*
   * This section is specified/returned if the ContextFlags word
   * contains the flag CONTEXT_CONTROL.
   *
   * N.B. The registers gp, sp, and ra are defined in the integer
   * section, but are considered part of the control context rather
   * than part of the integer context.
   */
  DWORD 		Fir;
  DWORD 		Psr;
  /*
   * The bits within the following flag control the contents of
   * a CONTEXT record.  If the context record is used as an input
   * parameter, then for each portion of the context record which is
   * controlled by a flag bit whose value is set, it is assumed that
   * the associated portion of the context record specifies valid
   * context.  If the context record is being used to modify a
   * thread's context, then only that portion of the thread's
   * context will be modified.
   *
   * If the context record is used as an IN OUT parameter, to
   * capture the context of a thread, then only those portions of
   * the thread's context corresponding to flag bits which have
   * been set will be returned.
   *
   * The context record is never used as an OUT only parameter.
   */
  DWORD 		ContextFlags;
  DWORD 		Fill[2];
} CONTEXT;

#elif defined(_M_ARM)
/* The following flags control the contents of the CONTEXT structure.
 */
#define CONTEXT_ARM				0x0000040
#define CONTEXT_CONTROL 		(CONTEXT_ARM | 0x00000001L)
#define CONTEXT_INTEGER 		(CONTEXT_ARM | 0x00000002L)

#define CONTEXT_FULL		    (CONTEXT_CONTROL | CONTEXT_INTEGER)

typedef struct _CONTEXT
{ /* The bit values within this flag control the contents of
   * a CONTEXT record.
   *
   * If the context record is used as an input parameter, then
   * for each portion of the context record controlled by a bit
   * flag which is set, it is assumed that the associated portion
   * of the context record contains valid context.  If the context
   * record is being used to modify a thread's context, then only
   * that portion of the thread's context will be modified.
   *
   * If the context record is used as an IN OUT parameter to,
   * capture the context of a thread, then only those portions
   * of the thread's context corresponding to flag bits which
   * have been set will be returned.
   *
   * The context record is never used as an OUT only parameter.
   */
  ULONG 		ContextFlags;
  /*
   * This section is specified/returned if the ContextFlags word
   * contains the flag CONTEXT_INTEGER.
   */
  ULONG 		R0;
  ULONG 		R1;
  ULONG 		R2;
  ULONG 		R3;
  ULONG 		R4;
  ULONG 		R5;
  ULONG 		R6;
  ULONG 		R7;
  ULONG 		R8;
  ULONG 		R9;
  ULONG 		R10;
  ULONG 		R11;
  ULONG 		R12;
  ULONG 		Sp;
  ULONG 		Lr;
  ULONG 		Pc;
  ULONG 		Psr;
} CONTEXT;

#else
#error "undefined processor type"
#endif
typedef CONTEXT *PCONTEXT, *LPCONTEXT;

typedef struct _EXCEPTION_RECORD
{ DWORD 			ExceptionCode;
  DWORD 			ExceptionFlags;
  struct _EXCEPTION_RECORD     *ExceptionRecord;
  PVOID 			ExceptionAddress;
  DWORD 			NumberParameters;
  DWORD 			ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
} EXCEPTION_RECORD, *PEXCEPTION_RECORD, *LPEXCEPTION_RECORD;

typedef struct _EXCEPTION_POINTERS
{ PEXCEPTION_RECORD		ExceptionRecord;
  PCONTEXT			ContextRecord;
} EXCEPTION_POINTERS, *PEXCEPTION_POINTERS, *LPEXCEPTION_POINTERS;

typedef union _LARGE_INTEGER
{ struct
  { DWORD			  LowPart;
    LONG			  HighPart;
  }				u;
#if ! defined(NONAMELESSUNION) || defined(__cplusplus)
  _ANONYMOUS_STRUCT struct
  { DWORD			LowPart;
    LONG			HighPart;
  };
#endif /* NONAMELESSUNION */
  LONGLONG			QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef union _ULARGE_INTEGER
{ struct
  { DWORD			  LowPart;
    DWORD			  HighPart;
  }				u;
#if ! defined(NONAMELESSUNION) || defined(__cplusplus)
  _ANONYMOUS_STRUCT struct
  { DWORD			LowPart;
    DWORD			HighPart;
  };
#endif /* NONAMELESSUNION */
  ULONGLONG			QuadPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;

typedef struct _LUID
{ DWORD 		LowPart;
  LONG			HighPart;
} LUID, *PLUID;
#pragma pack(push,4)

typedef struct _LUID_AND_ATTRIBUTES
{ LUID			Luid;
  DWORD 		Attributes;
} LUID_AND_ATTRIBUTES, *PLUID_AND_ATTRIBUTES;
#pragma pack(pop)
typedef LUID_AND_ATTRIBUTES LUID_AND_ATTRIBUTES_ARRAY[ANYSIZE_ARRAY];
typedef LUID_AND_ATTRIBUTES_ARRAY *PLUID_AND_ATTRIBUTES_ARRAY;

typedef struct _PRIVILEGE_SET
{ DWORD 		PrivilegeCount;
  DWORD 		Control;
  LUID_AND_ATTRIBUTES	Privilege[ANYSIZE_ARRAY];
} PRIVILEGE_SET, *PPRIVILEGE_SET;

typedef struct _SECURITY_ATTRIBUTES
{ DWORD 		nLength;
  LPVOID		lpSecurityDescriptor;
  BOOL			bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef enum _SECURITY_IMPERSONATION_LEVEL
{ SecurityAnonymous,
  SecurityIdentification,
  SecurityImpersonation,
  SecurityDelegation
} SECURITY_IMPERSONATION_LEVEL, *PSECURITY_IMPERSONATION_LEVEL;
typedef BOOLEAN SECURITY_CONTEXT_TRACKING_MODE, *PSECURITY_CONTEXT_TRACKING_MODE;

typedef struct _SECURITY_QUALITY_OF_SERVICE
{ DWORD 				Length;
  SECURITY_IMPERSONATION_LEVEL		ImpersonationLevel;
  SECURITY_CONTEXT_TRACKING_MODE	ContextTrackingMode;
  BOOLEAN				EffectiveOnly;
} SECURITY_QUALITY_OF_SERVICE, *PSECURITY_QUALITY_OF_SERVICE;
typedef PVOID PACCESS_TOKEN;

typedef struct _SE_IMPERSONATION_STATE
{ PACCESS_TOKEN 		Token;
  BOOLEAN			CopyOnOpen;
  BOOLEAN			EffectiveOnly;
  SECURITY_IMPERSONATION_LEVEL	Level;
} SE_IMPERSONATION_STATE, *PSE_IMPERSONATION_STATE;

typedef struct _SID_IDENTIFIER_AUTHORITY
{ BYTE				Value[6];
} SID_IDENTIFIER_AUTHORITY, *PSID_IDENTIFIER_AUTHORITY, *LPSID_IDENTIFIER_AUTHORITY;
typedef PVOID PSID;

typedef struct _SID
{ BYTE				Revision;
  BYTE				SubAuthorityCount;
  SID_IDENTIFIER_AUTHORITY	IdentifierAuthority;
  DWORD 			SubAuthority[ANYSIZE_ARRAY];
} SID, *PISID;

typedef struct _SID_AND_ATTRIBUTES
{ PSID			Sid;
  DWORD 		Attributes;
} SID_AND_ATTRIBUTES, *PSID_AND_ATTRIBUTES;
typedef SID_AND_ATTRIBUTES SID_AND_ATTRIBUTES_ARRAY[ANYSIZE_ARRAY];
typedef SID_AND_ATTRIBUTES_ARRAY *PSID_AND_ATTRIBUTES_ARRAY;

#if (_WIN32_WINNT >= 0x0600)
typedef ULONG_PTR SID_HASH_ENTRY, *PSID_HASH_ENTRY;

typedef struct _SID_AND_ATTRIBUTES_HASH
{ DWORD 		SidCount;
  PSID_AND_ATTRIBUTES	SidAttr;
  SID_HASH_ENTRY	Hash[SID_HASH_SIZE];
} SID_AND_ATTRIBUTES_HASH, *PSID_AND_ATTRIBUTES_HASH;
#endif

typedef struct _TOKEN_SOURCE
{ CHAR			SourceName[TOKEN_SOURCE_LENGTH];
  LUID			SourceIdentifier;
} TOKEN_SOURCE, *PTOKEN_SOURCE;

typedef struct _TOKEN_CONTROL
{ LUID			TokenId;
  LUID			AuthenticationId;
  LUID			ModifiedId;
  TOKEN_SOURCE		TokenSource;
} TOKEN_CONTROL, *PTOKEN_CONTROL;

typedef struct _TOKEN_DEFAULT_DACL
{ PACL			DefaultDacl;
} TOKEN_DEFAULT_DACL, *PTOKEN_DEFAULT_DACL;

typedef struct _TOKEN_GROUPS
{ DWORD 		GroupCount;
  SID_AND_ATTRIBUTES	Groups[ANYSIZE_ARRAY];
} TOKEN_GROUPS, *PTOKEN_GROUPS, *LPTOKEN_GROUPS;

typedef struct _TOKEN_OWNER
{ PSID			Owner;
} TOKEN_OWNER, *PTOKEN_OWNER;

typedef struct _TOKEN_PRIMARY_GROUP
{ PSID			PrimaryGroup;
} TOKEN_PRIMARY_GROUP, *PTOKEN_PRIMARY_GROUP;

typedef struct _TOKEN_PRIVILEGES
{ DWORD 		PrivilegeCount;
  LUID_AND_ATTRIBUTES	Privileges[ANYSIZE_ARRAY];
} TOKEN_PRIVILEGES, *PTOKEN_PRIVILEGES, *LPTOKEN_PRIVILEGES;

typedef enum tagTOKEN_TYPE
{ TokenPrimary		=	1,
  TokenImpersonation
} TOKEN_TYPE, *PTOKEN_TYPE;

typedef struct _TOKEN_STATISTICS
{ LUID				TokenId;
  LUID				AuthenticationId;
  LARGE_INTEGER 		ExpirationTime;
  TOKEN_TYPE			TokenType;
  SECURITY_IMPERSONATION_LEVEL	ImpersonationLevel;
  DWORD 			DynamicCharged;
  DWORD 			DynamicAvailable;
  DWORD 			GroupCount;
  DWORD 			PrivilegeCount;
  LUID ModifiedId;
} TOKEN_STATISTICS, *PTOKEN_STATISTICS;

typedef struct _TOKEN_USER
{ SID_AND_ATTRIBUTES		User;
} TOKEN_USER, *PTOKEN_USER;
#if (_WIN32_WINNT >= 0x0600)

typedef struct _TOKEN_LINKED_TOKEN
{ HANDLE			LinkedToken;
} TOKEN_LINKED_TOKEN, *PTOKEN_LINKED_TOKEN;

typedef struct _TOKEN_MANDATORY_LABEL
{ SID_AND_ATTRIBUTES		Label;
} TOKEN_MANDATORY_LABEL, *PTOKEN_MANDATORY_LABEL;

typedef struct _TOKEN_MANDATORY_POLICY
{ DWORD 			Policy;
} TOKEN_MANDATORY_POLICY, *PTOKEN_MANDATORY_POLICY;

typedef struct _TOKEN_ELEVATION
{ DWORD 			TokenIsElevated;
} TOKEN_ELEVATION, *PTOKEN_ELEVATION;

typedef struct _TOKEN_ACCESS_INFORMATION
{ PSID_AND_ATTRIBUTES_HASH	SidHash;
  PSID_AND_ATTRIBUTES_HASH	RestrictedSidHash;
  PTOKEN_PRIVILEGES		Privileges;
  LUID				AuthenticationId;
  TOKEN_TYPE			TokenType;
  SECURITY_IMPERSONATION_LEVEL	ImpersonationLevel;
  TOKEN_MANDATORY_POLICY	MandatoryPolicy;
  DWORD 			Flags;
} TOKEN_ACCESS_INFORMATION, *PTOKEN_ACCESS_INFORMATION;
#endif
typedef DWORD SECURITY_INFORMATION, *PSECURITY_INFORMATION;
typedef WORD SECURITY_DESCRIPTOR_CONTROL, *PSECURITY_DESCRIPTOR_CONTROL;

typedef struct _SECURITY_DESCRIPTOR
{ BYTE				Revision;
  BYTE				Sbz1;
  SECURITY_DESCRIPTOR_CONTROL	Control;
  PSID				Owner;
  PSID				Group;
  PACL				Sacl;
  PACL				Dacl;
} SECURITY_DESCRIPTOR, *PSECURITY_DESCRIPTOR, *PISECURITY_DESCRIPTOR;

typedef enum _TOKEN_INFORMATION_CLASS
{ TokenUser		=	1,
  TokenGroups,
  TokenPrivileges,
  TokenOwner,
  TokenPrimaryGroup,
  TokenDefaultDacl,
  TokenSource,
  TokenType,
  TokenImpersonationLevel,
  TokenStatistics,
  TokenRestrictedSids,
  TokenSessionId,
  TokenGroupsAndPrivileges,
  TokenSessionReference,
  TokenSandBoxInert,
  TokenAuditPolicy,
  TokenOrigin,
#if (_WIN32_WINNT >= 0x0600)
  TokenElevationType,
  TokenLinkedToken,
  TokenElevation,
  TokenHasRestrictions,
  TokenAccessInformation,
  TokenVirtualizationAllowed,
  TokenVirtualizationEnabled,
  TokenIntegrityLevel,
  TokenUIAccess,
  TokenMandatoryPolicy,
  TokenLogonSid,
#endif
  MaxTokenInfoClass
} TOKEN_INFORMATION_CLASS;

typedef enum _SID_NAME_USE
{ SidTypeUser		=	1,
  SidTypeGroup,
  SidTypeDomain,
  SidTypeAlias,
  SidTypeWellKnownGroup,
  SidTypeDeletedAccount,
  SidTypeInvalid,
  SidTypeUnknown,
  SidTypeComputer
} SID_NAME_USE, *PSID_NAME_USE;

typedef struct _QUOTA_LIMITS
{ SIZE_T		PagedPoolLimit;
  SIZE_T		NonPagedPoolLimit;
  SIZE_T		MinimumWorkingSetSize;
  SIZE_T		MaximumWorkingSetSize;
  SIZE_T		PagefileLimit;
  LARGE_INTEGER 	TimeLimit;
} QUOTA_LIMITS, *PQUOTA_LIMITS;

typedef struct _IO_COUNTERS
{ ULONGLONG		ReadOperationCount;
  ULONGLONG		WriteOperationCount;
  ULONGLONG		OtherOperationCount;
  ULONGLONG		ReadTransferCount;
  ULONGLONG		WriteTransferCount;
  ULONGLONG		OtherTransferCount;
} IO_COUNTERS, *PIO_COUNTERS;

typedef struct _FILE_NOTIFY_INFORMATION
{ DWORD 		NextEntryOffset;
  DWORD 		Action;
  DWORD 		FileNameLength;
  WCHAR 		FileName[1];
} FILE_NOTIFY_INFORMATION, *PFILE_NOTIFY_INFORMATION;

typedef struct _TAPE_ERASE
{ DWORD 		Type;
  BOOLEAN		Immediate;
} TAPE_ERASE, *PTAPE_ERASE;

typedef struct _TAPE_GET_DRIVE_PARAMETERS
{ BOOLEAN		ECC;
  BOOLEAN		Compression;
  BOOLEAN		DataPadding;
  BOOLEAN		ReportSetmarks;
  DWORD 		DefaultBlockSize;
  DWORD 		MaximumBlockSize;
  DWORD 		MinimumBlockSize;
  DWORD 		MaximumPartitionCount;
  DWORD 		FeaturesLow;
  DWORD 		FeaturesHigh;
  DWORD 		EOTWarningZoneSize;
} TAPE_GET_DRIVE_PARAMETERS, *PTAPE_GET_DRIVE_PARAMETERS;

typedef struct _TAPE_GET_MEDIA_PARAMETERS
{ LARGE_INTEGER 	Capacity;
  LARGE_INTEGER 	Remaining;
  DWORD 		BlockSize;
  DWORD 		PartitionCount;
  BOOLEAN		WriteProtected;
} TAPE_GET_MEDIA_PARAMETERS, *PTAPE_GET_MEDIA_PARAMETERS;

typedef struct _TAPE_GET_POSITION
{ ULONG 		Type;
  ULONG 		Partition;
  ULONG 		OffsetLow;
  ULONG 		OffsetHigh;
} TAPE_GET_POSITION, *PTAPE_GET_POSITION;

typedef struct _TAPE_PREPARE
{ DWORD 		Operation;
  BOOLEAN		Immediate;
} TAPE_PREPARE, *PTAPE_PREPARE;

typedef struct _TAPE_SET_DRIVE_PARAMETERS
{ BOOLEAN		ECC;
  BOOLEAN		Compression;
  BOOLEAN		DataPadding;
  BOOLEAN		ReportSetmarks;
  ULONG 		EOTWarningZoneSize;
} TAPE_SET_DRIVE_PARAMETERS, *PTAPE_SET_DRIVE_PARAMETERS;

typedef struct _TAPE_SET_MEDIA_PARAMETERS
{ ULONG 		BlockSize;
} TAPE_SET_MEDIA_PARAMETERS, *PTAPE_SET_MEDIA_PARAMETERS;

typedef struct _TAPE_SET_POSITION
{ DWORD 		Method;
  DWORD 		Partition;
  LARGE_INTEGER 	Offset;
  BOOLEAN		Immediate;
} TAPE_SET_POSITION, *PTAPE_SET_POSITION;

typedef struct _TAPE_WRITE_MARKS
{ DWORD 		Type;
  DWORD 		Count;
  BOOLEAN		Immediate;
} TAPE_WRITE_MARKS, *PTAPE_WRITE_MARKS;

typedef struct _TAPE_CREATE_PARTITION
{ DWORD 		Method;
  DWORD 		Count;
  DWORD 		Size;
} TAPE_CREATE_PARTITION, *PTAPE_CREATE_PARTITION;

typedef struct _MEMORY_BASIC_INFORMATION
{ PVOID 		BaseAddress;
  PVOID 		AllocationBase;
  DWORD 		AllocationProtect;
  DWORD 		RegionSize;
  DWORD 		State;
  DWORD 		Protect;
  DWORD 		Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

typedef struct _MESSAGE_RESOURCE_ENTRY
{ WORD			Length;
  WORD			Flags;
  BYTE			Text[1];
} MESSAGE_RESOURCE_ENTRY, *PMESSAGE_RESOURCE_ENTRY;

typedef struct _MESSAGE_RESOURCE_BLOCK
{ DWORD 		LowId;
  DWORD 		HighId;
  DWORD 		OffsetToEntries;
} MESSAGE_RESOURCE_BLOCK, *PMESSAGE_RESOURCE_BLOCK;

typedef struct _MESSAGE_RESOURCE_DATA
{ DWORD 			NumberOfBlocks;
  MESSAGE_RESOURCE_BLOCK	Blocks[1];
} MESSAGE_RESOURCE_DATA, *PMESSAGE_RESOURCE_DATA;

typedef struct _LIST_ENTRY
{ struct _LIST_ENTRY		*Flink;
  struct _LIST_ENTRY		*Blink;
} LIST_ENTRY, *PLIST_ENTRY;

typedef struct _SINGLE_LIST_ENTRY
{ struct _SINGLE_LIST_ENTRY	*Next;
} SINGLE_LIST_ENTRY, *PSINGLE_LIST_ENTRY;

#ifndef _SLIST_HEADER_
#define _SLIST_HEADER_
#define SLIST_ENTRY		SINGLE_LIST_ENTRY
#define _SLIST_ENTRY	       _SINGLE_LIST_ENTRY
#define PSLIST_ENTRY		PSINGLE_LIST_ENTRY

typedef union _SLIST_HEADER
{ ULONGLONG			Alignment;
  _ANONYMOUS_STRUCT struct
  { SLIST_ENTRY 		  Next;
    WORD			  Depth;
    WORD			  Sequence;
  }				DUMMYSTRUCTNAME;
} SLIST_HEADER, *PSLIST_HEADER;
#endif /* !_SLIST_HEADER_ */

typedef struct _RTL_CRITICAL_SECTION_DEBUG
{ WORD				Type;
  WORD				CreatorBackTraceIndex;
  struct _RTL_CRITICAL_SECTION *CriticalSection;
  LIST_ENTRY			ProcessLocksList;
  DWORD 			EntryCount;
  DWORD 			ContentionCount;
  DWORD 			Spare[2];
} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG;

typedef struct _RTL_CRITICAL_SECTION
{ PRTL_CRITICAL_SECTION_DEBUG	DebugInfo;
  LONG				LockCount;
  LONG				RecursionCount;
  HANDLE			OwningThread;
  HANDLE			LockSemaphore;
  DWORD 			Reserved;
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

typedef struct _EVENTLOGRECORD
{ DWORD 		Length;
  DWORD 		Reserved;
  DWORD 		RecordNumber;
  DWORD 		TimeGenerated;
  DWORD 		TimeWritten;
  DWORD 		EventID;
  WORD			EventType;
  WORD			NumStrings;
  WORD			EventCategory;
  WORD			ReservedFlags;
  DWORD 		ClosingRecordNumber;
  DWORD 		StringOffset;
  DWORD 		UserSidLength;
  DWORD 		UserSidOffset;
  DWORD 		DataLength;
  DWORD 		DataOffset;
} EVENTLOGRECORD, *PEVENTLOGRECORD;

typedef struct _OSVERSIONINFOA
{ DWORD 		dwOSVersionInfoSize;
  DWORD 		dwMajorVersion;
  DWORD 		dwMinorVersion;
  DWORD 		dwBuildNumber;
  DWORD 		dwPlatformId;
  CHAR			szCSDVersion[128];
} OSVERSIONINFOA, *POSVERSIONINFOA, *LPOSVERSIONINFOA;

typedef struct _OSVERSIONINFOW
{ DWORD 		dwOSVersionInfoSize;
  DWORD 		dwMajorVersion;
  DWORD 		dwMinorVersion;
  DWORD 		dwBuildNumber;
  DWORD 		dwPlatformId;
  WCHAR 		szCSDVersion[128];
} OSVERSIONINFOW, *POSVERSIONINFOW, *LPOSVERSIONINFOW;

typedef struct _OSVERSIONINFOEXA
{ DWORD 		dwOSVersionInfoSize;
  DWORD 		dwMajorVersion;
  DWORD 		dwMinorVersion;
  DWORD 		dwBuildNumber;
  DWORD 		dwPlatformId;
  CHAR			szCSDVersion[128];
  WORD			wServicePackMajor;
  WORD			wServicePackMinor;
  WORD			wSuiteMask;
  BYTE			wProductType;
  BYTE			wReserved;
} OSVERSIONINFOEXA, *POSVERSIONINFOEXA, *LPOSVERSIONINFOEXA;

typedef struct _OSVERSIONINFOEXW
{ DWORD 		dwOSVersionInfoSize;
  DWORD 		dwMajorVersion;
  DWORD 		dwMinorVersion;
  DWORD 		dwBuildNumber;
  DWORD 		dwPlatformId;
  WCHAR szCSDVersion[128];
  WORD			wServicePackMajor;
  WORD			wServicePackMinor;
  WORD			wSuiteMask;
  BYTE			wProductType;
  BYTE			wReserved;
} OSVERSIONINFOEXW, *POSVERSIONINFOEXW, *LPOSVERSIONINFOEXW;

#pragma pack(push,2)
typedef struct _IMAGE_VXD_HEADER
{ WORD			e32_magic;
  BYTE			e32_border;
  BYTE			e32_worder;
  DWORD 		e32_level;
  WORD			e32_cpu;
  WORD			e32_os;
  DWORD 		e32_ver;
  DWORD 		e32_mflags;
  DWORD 		e32_mpages;
  DWORD 		e32_startobj;
  DWORD 		e32_eip;
  DWORD 		e32_stackobj;
  DWORD 		e32_esp;
  DWORD 		e32_pagesize;
  DWORD 		e32_lastpagesize;
  DWORD 		e32_fixupsize;
  DWORD 		e32_fixupsum;
  DWORD 		e32_ldrsize;
  DWORD 		e32_ldrsum;
  DWORD 		e32_objtab;
  DWORD 		e32_objcnt;
  DWORD 		e32_objmap;
  DWORD 		e32_itermap;
  DWORD 		e32_rsrctab;
  DWORD 		e32_rsrccnt;
  DWORD 		e32_restab;
  DWORD 		e32_enttab;
  DWORD 		e32_dirtab;
  DWORD 		e32_dircnt;
  DWORD 		e32_fpagetab;
  DWORD 		e32_frectab;
  DWORD 		e32_impmod;
  DWORD 		e32_impmodcnt;
  DWORD 		e32_impproc;
  DWORD 		e32_pagesum;
  DWORD 		e32_datapage;
  DWORD 		e32_preload;
  DWORD 		e32_nrestab;
  DWORD 		e32_cbnrestab;
  DWORD 		e32_nressum;
  DWORD 		e32_autodata;
  DWORD 		e32_debuginfo;
  DWORD 		e32_debuglen;
  DWORD 		e32_instpreload;
  DWORD 		e32_instdemand;
  DWORD 		e32_heapsize;
  BYTE			e32_res3[12];
  DWORD 		e32_winresoff;
  DWORD 		e32_winreslen;
  WORD			e32_devid;
  WORD			e32_ddkver;
} IMAGE_VXD_HEADER, *PIMAGE_VXD_HEADER;
#pragma pack(pop)

#pragma pack(push,4)
typedef struct _IMAGE_FILE_HEADER
{ WORD			Machine;
  WORD			NumberOfSections;
  DWORD 		TimeDateStamp;
  DWORD 		PointerToSymbolTable;
  DWORD 		NumberOfSymbols;
  WORD			SizeOfOptionalHeader;
  WORD			Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY
{ DWORD 		VirtualAddress;
  DWORD 		Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER
{ WORD			Magic;
  BYTE			MajorLinkerVersion;
  BYTE			MinorLinkerVersion;
  DWORD 		SizeOfCode;
  DWORD 		SizeOfInitializedData;
  DWORD 		SizeOfUninitializedData;
  DWORD 		AddressOfEntryPoint;
  DWORD 		BaseOfCode;
  DWORD 		BaseOfData;
  DWORD 		ImageBase;
  DWORD 		SectionAlignment;
  DWORD 		FileAlignment;
  WORD			MajorOperatingSystemVersion;
  WORD			MinorOperatingSystemVersion;
  WORD			MajorImageVersion;
  WORD			MinorImageVersion;
  WORD			MajorSubsystemVersion;
  WORD			MinorSubsystemVersion;
  DWORD 		Win32VersionValue;
  DWORD 		SizeOfImage;
  DWORD 		SizeOfHeaders;
  DWORD 		CheckSum;
  WORD			Subsystem;
  WORD			DllCharacteristics;
  DWORD 		SizeOfStackReserve;
  DWORD 		SizeOfStackCommit;
  DWORD 		SizeOfHeapReserve;
  DWORD 		SizeOfHeapCommit;
  DWORD 		LoaderFlags;
  DWORD 		NumberOfRvaAndSizes;
  IMAGE_DATA_DIRECTORY	DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_OPTIONAL_HEADER64
{ WORD			Magic;
  BYTE			MajorLinkerVersion;
  BYTE			MinorLinkerVersion;
  DWORD 		SizeOfCode;
  DWORD 		SizeOfInitializedData;
  DWORD 		SizeOfUninitializedData;
  DWORD 		AddressOfEntryPoint;
  DWORD 		BaseOfCode;
  ULONGLONG		ImageBase;
  DWORD 		SectionAlignment;
  DWORD 		FileAlignment;
  WORD			MajorOperatingSystemVersion;
  WORD			MinorOperatingSystemVersion;
  WORD			MajorImageVersion;
  WORD			MinorImageVersion;
  WORD			MajorSubsystemVersion;
  WORD			MinorSubsystemVersion;
  DWORD 		Win32VersionValue;
  DWORD 		SizeOfImage;
  DWORD 		SizeOfHeaders;
  DWORD 		CheckSum;
  WORD			Subsystem;
  WORD			DllCharacteristics;
  ULONGLONG		SizeOfStackReserve;
  ULONGLONG		SizeOfStackCommit;
  ULONGLONG		SizeOfHeapReserve;
  ULONGLONG		SizeOfHeapCommit;
  DWORD 		LoaderFlags;
  DWORD 		NumberOfRvaAndSizes;
  IMAGE_DATA_DIRECTORY	DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;
#ifdef _WIN64
typedef IMAGE_OPTIONAL_HEADER64		IMAGE_OPTIONAL_HEADER;
typedef PIMAGE_OPTIONAL_HEADER64	PIMAGE_OPTIONAL_HEADER;
#else
typedef IMAGE_OPTIONAL_HEADER32		IMAGE_OPTIONAL_HEADER;
typedef PIMAGE_OPTIONAL_HEADER32	PIMAGE_OPTIONAL_HEADER;
#endif

typedef struct _IMAGE_ROM_OPTIONAL_HEADER
{ WORD			Magic;
  BYTE			MajorLinkerVersion;
  BYTE			MinorLinkerVersion;
  DWORD 		SizeOfCode;
  DWORD 		SizeOfInitializedData;
  DWORD 		SizeOfUninitializedData;
  DWORD 		AddressOfEntryPoint;
  DWORD 		BaseOfCode;
  DWORD 		BaseOfData;
  DWORD 		BaseOfBss;
  DWORD 		GprMask;
  DWORD 		CprMask[4];
  DWORD 		GpValue;
} IMAGE_ROM_OPTIONAL_HEADER, *PIMAGE_ROM_OPTIONAL_HEADER;
#pragma pack(pop)

#pragma pack(push,2)
typedef struct _IMAGE_DOS_HEADER
{ WORD			e_magic;
  WORD			e_cblp;
  WORD			e_cp;
  WORD			e_crlc;
  WORD			e_cparhdr;
  WORD			e_minalloc;
  WORD			e_maxalloc;
  WORD			e_ss;
  WORD			e_sp;
  WORD			e_csum;
  WORD			e_ip;
  WORD			e_cs;
  WORD			e_lfarlc;
  WORD			e_ovno;
  WORD			e_res[4];
  WORD			e_oemid;
  WORD			e_oeminfo;
  WORD			e_res2[10];
  LONG			e_lfanew;
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

typedef struct _IMAGE_OS2_HEADER
{ WORD			ne_magic;
  CHAR			ne_ver;
  CHAR			ne_rev;
  WORD			ne_enttab;
  WORD			ne_cbenttab;
  LONG			ne_crc;
  WORD			ne_flags;
  WORD			ne_autodata;
  WORD			ne_heap;
  WORD			ne_stack;
  LONG			ne_csip;
  LONG			ne_sssp;
  WORD			ne_cseg;
  WORD			ne_cmod;
  WORD			ne_cbnrestab;
  WORD			ne_segtab;
  WORD			ne_rsrctab;
  WORD			ne_restab;
  WORD			ne_modtab;
  WORD			ne_imptab;
  LONG			ne_nrestab;
  WORD			ne_cmovent;
  WORD			ne_align;
  WORD			ne_cres;
  BYTE			ne_exetyp;
  BYTE			ne_flagsothers;
  WORD			ne_pretthunks;
  WORD			ne_psegrefbytes;
  WORD			ne_swaparea;
  WORD			ne_expver;
} IMAGE_OS2_HEADER, *PIMAGE_OS2_HEADER;
#pragma pack(pop)

#pragma pack(push,4)
typedef struct _IMAGE_NT_HEADERS
{ DWORD 			Signature;
  IMAGE_FILE_HEADER		FileHeader;
  IMAGE_OPTIONAL_HEADER32	OptionalHeader;
} IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;

typedef struct _IMAGE_NT_HEADERS64
{ DWORD 			Signature;
  IMAGE_FILE_HEADER		FileHeader;
  IMAGE_OPTIONAL_HEADER64	OptionalHeader;
} IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;
#ifdef _WIN64
typedef IMAGE_NT_HEADERS64	IMAGE_NT_HEADERS;
typedef PIMAGE_NT_HEADERS64	PIMAGE_NT_HEADERS;
#else
typedef IMAGE_NT_HEADERS32	IMAGE_NT_HEADERS;
typedef PIMAGE_NT_HEADERS32	PIMAGE_NT_HEADERS;
#endif

typedef struct _IMAGE_ROM_HEADERS
{ IMAGE_FILE_HEADER		FileHeader;
  IMAGE_ROM_OPTIONAL_HEADER	OptionalHeader;
} IMAGE_ROM_HEADERS, *PIMAGE_ROM_HEADERS;

typedef struct _IMAGE_SECTION_HEADER
{ BYTE		Name[IMAGE_SIZEOF_SHORT_NAME];
  union
  { DWORD	  PhysicalAddress;
    DWORD	  VirtualSize;
  }		Misc;
  DWORD 	VirtualAddress;
  DWORD 	SizeOfRawData;
  DWORD 	PointerToRawData;
  DWORD 	PointerToRelocations;
  DWORD 	PointerToLinenumbers;
  WORD		NumberOfRelocations;
  WORD		NumberOfLinenumbers;
  DWORD 	Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;
#pragma pack(pop)

#pragma pack(push,2)
typedef struct _IMAGE_SYMBOL
{ union
  { BYTE	  ShortName[8];
    struct
    { DWORD	    Short;
      DWORD	    Long;
    }		  Name;
    PBYTE	  LongName[2];
  }		N;
  DWORD 			Value;
  SHORT 	SectionNumber;
  WORD		Type;
  BYTE		StorageClass;
  BYTE		NumberOfAuxSymbols;
} IMAGE_SYMBOL, *PIMAGE_SYMBOL;

typedef union _IMAGE_AUX_SYMBOL
{ struct
  { DWORD	  TagIndex;
    union
    { struct
      { WORD	      Linenumber;
	WORD	      Size;
      } 	    LnSz;
      DWORD	    TotalSize;
    }		  Misc;
    union
    { struct
      { DWORD	      PointerToLinenumber;
	DWORD	      PointerToNextFunction;
      } 	    Function;
      struct
      { WORD	      Dimension[4];
      } 	    Array;
    }		  FcnAry;
    WORD	  TvIndex;
  }		Sym;
  struct
  { BYTE	  Name[IMAGE_SIZEOF_SYMBOL];
  }		File;
  struct
  { DWORD	  Length;
    WORD	  NumberOfRelocations;
    WORD	  NumberOfLinenumbers;
    DWORD	  CheckSum;
    SHORT	  Number;
    BYTE	  Selection;
  }		Section;
} IMAGE_AUX_SYMBOL, *PIMAGE_AUX_SYMBOL;

typedef struct _IMAGE_COFF_SYMBOLS_HEADER
{ DWORD 	NumberOfSymbols;
  DWORD 	LvaToFirstSymbol;
  DWORD 	NumberOfLinenumbers;
  DWORD 	LvaToFirstLinenumber;
  DWORD 	RvaToFirstByteOfCode;
  DWORD 	RvaToLastByteOfCode;
  DWORD 	RvaToFirstByteOfData;
  DWORD 	RvaToLastByteOfData;
} IMAGE_COFF_SYMBOLS_HEADER, *PIMAGE_COFF_SYMBOLS_HEADER;

typedef struct _IMAGE_RELOCATION
{ _ANONYMOUS_UNION union
  { DWORD		  VirtualAddress;
    DWORD		  RelocCount;
  }			DUMMYUNIONNAME;
  DWORD 		SymbolTableIndex;
  WORD			Type;
} IMAGE_RELOCATION, *PIMAGE_RELOCATION;
#pragma pack(pop)

#pragma pack(push,4)
typedef struct _IMAGE_BASE_RELOCATION
{ DWORD 	VirtualAddress;
  DWORD 	SizeOfBlock;
} IMAGE_BASE_RELOCATION, *PIMAGE_BASE_RELOCATION;
#pragma pack(pop)

#pragma pack(push,2)
typedef struct _IMAGE_LINENUMBER
{ union
  { DWORD	  SymbolTableIndex;
    DWORD	  VirtualAddress;
  }		Type;
  WORD		Linenumber;
} IMAGE_LINENUMBER, *PIMAGE_LINENUMBER;
#pragma pack(pop)

#pragma pack(push,4)
typedef struct _IMAGE_ARCHIVE_MEMBER_HEADER
{ BYTE		Name[16];
  BYTE		Date[12];
  BYTE		UserID[6];
  BYTE		GroupID[6];
  BYTE		Mode[8];
  BYTE		Size[10];
  BYTE		EndHeader[2];
} IMAGE_ARCHIVE_MEMBER_HEADER, *PIMAGE_ARCHIVE_MEMBER_HEADER;

typedef struct _IMAGE_EXPORT_DIRECTORY
{ DWORD 	Characteristics;
  DWORD 	TimeDateStamp;
  WORD		MajorVersion;
  WORD		MinorVersion;
  DWORD 	Name;
  DWORD 	Base;
  DWORD 	NumberOfFunctions;
  DWORD 	NumberOfNames;
  DWORD 	AddressOfFunctions;
  DWORD 	AddressOfNames;
  DWORD 	AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

typedef struct _IMAGE_IMPORT_BY_NAME
{ WORD		Hint;
  BYTE		Name[1];
} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;

typedef struct _IMAGE_THUNK_DATA32
{ union
  { DWORD	  ForwarderString;
    DWORD	  Function;
    DWORD	  Ordinal;
    DWORD	  AddressOfData;
  }		u1;
} IMAGE_THUNK_DATA32, *PIMAGE_THUNK_DATA32;

typedef struct _IMAGE_THUNK_DATA64
{ union
  { ULONGLONG	  ForwarderString;
    ULONGLONG	  Function;
    ULONGLONG	  Ordinal;
    ULONGLONG	  AddressOfData;
  }		u1;
} IMAGE_THUNK_DATA64, *PIMAGE_THUNK_DATA64;
#ifdef __WIN64
typedef IMAGE_THUNK_DATA64	IMAGE_THUNK_DATA;
typedef PIMAGE_THUNK_DATA64	PIMAGE_THUNK_DATA;
#else
typedef IMAGE_THUNK_DATA32	IMAGE_THUNK_DATA;
typedef PIMAGE_THUNK_DATA32	PIMAGE_THUNK_DATA;
#endif

typedef struct _IMAGE_IMPORT_DESCRIPTOR
{ _ANONYMOUS_UNION union
  { DWORD		  Characteristics;
    DWORD		  OriginalFirstThunk;
  }			DUMMYUNIONNAME;
  DWORD 		TimeDateStamp;
  DWORD 		ForwarderChain;
  DWORD 		Name;
  DWORD 		FirstThunk;
} IMAGE_IMPORT_DESCRIPTOR, *PIMAGE_IMPORT_DESCRIPTOR;

typedef struct _IMAGE_BOUND_IMPORT_DESCRIPTOR
{ DWORD 	TimeDateStamp;
  WORD		OffsetModuleName;
  WORD		NumberOfModuleForwarderRefs;
} IMAGE_BOUND_IMPORT_DESCRIPTOR, *PIMAGE_BOUND_IMPORT_DESCRIPTOR;

typedef struct _IMAGE_BOUND_FORWARDER_REF
{ DWORD 	TimeDateStamp;
  WORD		OffsetModuleName;
  WORD		Reserved;
} IMAGE_BOUND_FORWARDER_REF, *PIMAGE_BOUND_FORWARDER_REF;
typedef void(NTAPI *PIMAGE_TLS_CALLBACK)(PVOID,DWORD,PVOID);

typedef struct _IMAGE_TLS_DIRECTORY32
{ DWORD 	StartAddressOfRawData;
  DWORD 	EndAddressOfRawData;
  DWORD 	AddressOfIndex;
  DWORD 	AddressOfCallBacks;
  DWORD 	SizeOfZeroFill;
  DWORD 	Characteristics;
} IMAGE_TLS_DIRECTORY32, *PIMAGE_TLS_DIRECTORY32;

typedef struct _IMAGE_TLS_DIRECTORY64
{ ULONGLONG	StartAddressOfRawData;
  ULONGLONG	EndAddressOfRawData;
  ULONGLONG	AddressOfIndex;
  ULONGLONG	AddressOfCallBacks;
  DWORD 	SizeOfZeroFill;
  DWORD 	Characteristics;
} IMAGE_TLS_DIRECTORY64, *PIMAGE_TLS_DIRECTORY64;
#ifdef __WIN64
typedef IMAGE_TLS_DIRECTORY64	IMAGE_TLS_DIRECTORY;
typedef PIMAGE_TLS_DIRECTORY64	PIMAGE_TLS_DIRECTORY;
#else
typedef IMAGE_TLS_DIRECTORY32	IMAGE_TLS_DIRECTORY;
typedef PIMAGE_TLS_DIRECTORY32	PIMAGE_TLS_DIRECTORY;
#endif

typedef struct _IMAGE_RESOURCE_DIRECTORY
{ DWORD 	Characteristics;
  DWORD 	TimeDateStamp;
  WORD		MajorVersion;
  WORD		MinorVersion;
  WORD		NumberOfNamedEntries;
  WORD		NumberOfIdEntries;
} IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;

_ANONYMOUS_STRUCT typedef struct _IMAGE_RESOURCE_DIRECTORY_ENTRY
{ _ANONYMOUS_UNION union
  { _ANONYMOUS_STRUCT struct
    { DWORD			    NameOffset	  : 31;
      DWORD			    NameIsString  :  1;
    }				  DUMMYSTRUCTNAME;
    DWORD			  Name;
    WORD			  Id;
  }				DUMMYUNIONNAME;
  _ANONYMOUS_UNION union
  { DWORD			  OffsetToData;
    _ANONYMOUS_STRUCT struct
    { DWORD			    OffsetToDirectory:31;
      DWORD			    DataIsDirectory:1;
    }				  DUMMYSTRUCTNAME2;
  }				DUMMYUNIONNAME2;
} IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;

typedef struct _IMAGE_RESOURCE_DIRECTORY_STRING
{ WORD		Length;
  CHAR		NameString[1];
} IMAGE_RESOURCE_DIRECTORY_STRING, *PIMAGE_RESOURCE_DIRECTORY_STRING;

typedef struct _IMAGE_RESOURCE_DIR_STRING_U
{ WORD		Length;
  WCHAR 	NameString[1];
} IMAGE_RESOURCE_DIR_STRING_U, *PIMAGE_RESOURCE_DIR_STRING_U;

typedef struct _IMAGE_RESOURCE_DATA_ENTRY
{ DWORD 	OffsetToData;
  DWORD 	Size;
  DWORD 	CodePage;
  DWORD 	Reserved;
} IMAGE_RESOURCE_DATA_ENTRY, *PIMAGE_RESOURCE_DATA_ENTRY;

typedef struct _IMAGE_LOAD_CONFIG_DIRECTORY
{ DWORD 	Characteristics;
  DWORD 	TimeDateStamp;
  WORD		MajorVersion;
  WORD		MinorVersion;
  DWORD 	GlobalFlagsClear;
  DWORD 	GlobalFlagsSet;
  DWORD 	CriticalSectionDefaultTimeout;
  DWORD 	DeCommitFreeBlockThreshold;
  DWORD 	DeCommitTotalFreeThreshold;
  PVOID 	LockPrefixTable;
  DWORD 	MaximumAllocationSize;
  DWORD 	VirtualMemoryThreshold;
  DWORD 	ProcessHeapFlags;
  DWORD 	Reserved[4];
} IMAGE_LOAD_CONFIG_DIRECTORY, *PIMAGE_LOAD_CONFIG_DIRECTORY;

typedef struct
{ DWORD 	Characteristics;
  DWORD 	TimeDateStamp;
  WORD		MajorVersion;
  WORD		MinorVersion;
  DWORD 	GlobalFlagsClear;
  DWORD 	GlobalFlagsSet;
  DWORD 	CriticalSectionDefaultTimeout;
  ULONGLONG	DeCommitFreeBlockThreshold;
  ULONGLONG	DeCommitTotalFreeThreshold;
  ULONGLONG	LockPrefixTable;
  ULONGLONG	MaximumAllocationSize;
  ULONGLONG	VirtualMemoryThreshold;
  ULONGLONG	ProcessAffinityMask;
  DWORD 	ProcessHeapFlags;
  WORD		CSDFlags;
  WORD		Reserved1;
  ULONGLONG	EditList;
  DWORD 	Reserved[2];
} IMAGE_LOAD_CONFIG_DIRECTORY64, *PIMAGE_LOAD_CONFIG_DIRECTORY64;

typedef struct _IMAGE_RUNTIME_FUNCTION_ENTRY
{ DWORD 	BeginAddress;
  DWORD 	EndAddress;
  PVOID 	ExceptionHandler;
  PVOID 	HandlerData;
  DWORD 	PrologEndAddress;
} IMAGE_RUNTIME_FUNCTION_ENTRY, *PIMAGE_RUNTIME_FUNCTION_ENTRY;

typedef struct _IMAGE_CE_RUNTIME_FUNCTION_ENTRY
{ unsigned int	FuncStart:32;
  unsigned int	PrologLen:8;
  unsigned int	FuncLen:22;
  unsigned int	ThirtyTwoBit:1;
  unsigned int	ExceptionFlag:1;
} IMAGE_CE_RUNTIME_FUNCTION_ENTRY, *PIMAGE_CE_RUNTIME_FUNCTION_ENTRY;

typedef struct _IMAGE_DEBUG_DIRECTORY
{ DWORD 	Characteristics;
  DWORD 	TimeDateStamp;
  WORD		MajorVersion;
  WORD		MinorVersion;
  DWORD 	Type;
  DWORD 	SizeOfData;
  DWORD 	AddressOfRawData;
  DWORD 	PointerToRawData;
} IMAGE_DEBUG_DIRECTORY, *PIMAGE_DEBUG_DIRECTORY;

typedef struct _FPO_DATA
{ DWORD 	ulOffStart;
  DWORD 	cbProcSize;
  DWORD 	cdwLocals;
  WORD		cdwParams;
  WORD		cbProlog:8;
  WORD		cbRegs:3;
  WORD		fHasSEH:1;
  WORD		fUseBP:1;
  WORD		reserved:1;
  WORD		cbFrame:2;
} FPO_DATA, *PFPO_DATA;

typedef struct _IMAGE_DEBUG_MISC
{ DWORD 	DataType;
  DWORD 	Length;
  BOOLEAN	Unicode;
  BYTE		Reserved[3];
  BYTE		Data[1];
} IMAGE_DEBUG_MISC, *PIMAGE_DEBUG_MISC;

typedef struct _IMAGE_FUNCTION_ENTRY
{ DWORD 	StartingAddress;
  DWORD 	EndingAddress;
  DWORD 	EndOfPrologue;
} IMAGE_FUNCTION_ENTRY, *PIMAGE_FUNCTION_ENTRY;

typedef struct _IMAGE_FUNCTION_ENTRY64
{ ULONGLONG		StartingAddress;
  ULONGLONG		EndingAddress;
  _ANONYMOUS_UNION union
  { ULONGLONG		  EndOfPrologue;
    ULONGLONG		  UnwindInfoAddress;
  }			DUMMYUNIONNAME;
} IMAGE_FUNCTION_ENTRY64, *PIMAGE_FUNCTION_ENTRY64;

typedef struct _IMAGE_SEPARATE_DEBUG_HEADER
{ WORD		Signature;
  WORD		Flags;
  WORD		Machine;
  WORD		Characteristics;
  DWORD 	TimeDateStamp;
  DWORD 	CheckSum;
  DWORD 	ImageBase;
  DWORD 	SizeOfImage;
  DWORD 	NumberOfSections;
  DWORD 	ExportedNamesSize;
  DWORD 	DebugDirectorySize;
  DWORD 	SectionAlignment;
  DWORD 	Reserved[2];
} IMAGE_SEPARATE_DEBUG_HEADER, *PIMAGE_SEPARATE_DEBUG_HEADER;
#pragma pack(pop)

typedef enum _CM_SERVICE_NODE_TYPE
{ DriverType			=	SERVICE_KERNEL_DRIVER,
  FileSystemType		=	SERVICE_FILE_SYSTEM_DRIVER,
  Win32ServiceOwnProcess	=	SERVICE_WIN32_OWN_PROCESS,
  Win32ServiceShareProcess	=	SERVICE_WIN32_SHARE_PROCESS,
  AdapterType			=	SERVICE_ADAPTER,
  RecognizerType		=	SERVICE_RECOGNIZER_DRIVER
} SERVICE_NODE_TYPE;

typedef enum _CM_SERVICE_LOAD_TYPE
{ BootLoad			=	SERVICE_BOOT_START,
  SystemLoad			=	SERVICE_SYSTEM_START,
  AutoLoad			=	SERVICE_AUTO_START,
  DemandLoad			=	SERVICE_DEMAND_START,
  DisableLoad			=	SERVICE_DISABLED
} SERVICE_LOAD_TYPE;

typedef enum _CM_ERROR_CONTROL_TYPE
{ IgnoreError			=	SERVICE_ERROR_IGNORE,
  NormalError			=	SERVICE_ERROR_NORMAL,
  SevereError			=	SERVICE_ERROR_SEVERE,
  CriticalError			=	SERVICE_ERROR_CRITICAL
} SERVICE_ERROR_TYPE;

typedef struct _NT_TIB
{ struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList;
  PVOID 				 StackBase;
  PVOID 				 StackLimit;
  PVOID 				 SubSystemTib;
  _ANONYMOUS_UNION union
  { PVOID		  		   FiberData;
    DWORD		  		   Version;
  }					 DUMMYUNIONNAME;
  PVOID 				 ArbitraryUserPointer;
  struct _NT_TIB			*Self;
} NT_TIB, *PNT_TIB;

typedef struct _REPARSE_DATA_BUFFER
{ DWORD 			ReparseTag;
  WORD				ReparseDataLength;
  WORD				Reserved;
  _ANONYMOUS_UNION union
  { struct
    { WORD			    SubstituteNameOffset;
      WORD			    SubstituteNameLength;
      WORD			    PrintNameOffset;
      WORD			    PrintNameLength;
      ULONG			    Flags;
      WCHAR			    PathBuffer[1];
    }				  SymbolicLinkReparseBuffer;
    struct
    { WORD			    SubstituteNameOffset;
      WORD			    SubstituteNameLength;
      WORD			    PrintNameOffset;
      WORD			    PrintNameLength;
      WCHAR			    PathBuffer[1];
    }				  MountPointReparseBuffer;
    struct
    { BYTE			    DataBuffer[1];
    }				  GenericReparseBuffer;
  }				DUMMYUNIONNAME;
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;

typedef struct _REPARSE_GUID_DATA_BUFFER
{ DWORD 	ReparseTag;
  WORD		ReparseDataLength;
  WORD		Reserved;
  GUID  	ReparseGuid;
  struct
  { BYTE	  DataBuffer[1];
  }		GenericReparseBuffer;
} REPARSE_GUID_DATA_BUFFER, *PREPARSE_GUID_DATA_BUFFER;

typedef struct _REPARSE_POINT_INFORMATION
{ WORD		ReparseDataLength;
  WORD		UnparsedNameLength;
} REPARSE_POINT_INFORMATION, *PREPARSE_POINT_INFORMATION;


typedef union _FILE_SEGMENT_ELEMENT
{ PVOID64	Buffer;
  ULONGLONG	Alignment;
} FILE_SEGMENT_ELEMENT, *PFILE_SEGMENT_ELEMENT;

/* JOBOBJECT_BASIC_LIMIT_INFORMATION.LimitFlags constants:
 * https://msdn.microsoft.com/en-us/library/windows/desktop/ms684147(v=vs.85).aspx
 */
#define JOB_OBJECT_LIMIT_WORKINGSET			0x0001
#define JOB_OBJECT_LIMIT_PROCESS_TIME			0x0002
#define JOB_OBJECT_LIMIT_JOB_TIME			0x0004
#define JOB_OBJECT_LIMIT_ACTIVE_PROCESS 		0x0008
#define JOB_OBJECT_LIMIT_AFFINITY			0x0010
#define JOB_OBJECT_LIMIT_PRIORITY_CLASS 		0x0020
#define JOB_OBJECT_LIMIT_PRESERVE_JOB_TIME		0x0040
#define JOB_OBJECT_LIMIT_SCHEDULING_CLASS		0x0080
#define JOB_OBJECT_LIMIT_PROCESS_MEMORY 		0x0100
#define JOB_OBJECT_LIMIT_JOB_MEMORY			0x0200
#define JOB_OBJECT_LIMIT_DIE_ON_UNHANDLED_EXCEPTION	0x0400
#define JOB_OBJECT_LIMIT_BREAKAWAY_OK			0x0800
#define JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK		0x1000
#define JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE		0x2000

/* Wrong definitions, but keep for backward compatibility. */
#define JOB_OBJECT_BREAKAWAY_OK 	JOB_OBJECT_LIMIT_BREAKAWAY_OK
#define JOB_OBJECT_SILENT_BREAKAWAY	JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK

/* JOBOBJECT_BASIC_UI_RESTRICTIONS.UIRestrictionsClass constants */
#define JOB_OBJECT_UILIMIT_HANDLES			0x0001
#define JOB_OBJECT_UILIMIT_READCLIPBOARD		0x0002
#define JOB_OBJECT_UILIMIT_WRITECLIPBOARD		0x0004
#define JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS		0x0008
#define JOB_OBJECT_UILIMIT_DISPLAYSETTINGS		0x0010
#define JOB_OBJECT_UILIMIT_GLOBALATOMS			0x0020
#define JOB_OBJECT_UILIMIT_DESKTOP			0x0040
#define JOB_OBJECT_UILIMIT_EXITWINDOWS			0x0080

/* JOBOBJECT_SECURITY_LIMIT_INFORMATION.SecurityLimitFlags constants */
#define JOB_OBJECT_SECURITY_NO_ADMIN			0x0001
#define JOB_OBJECT_SECURITY_RESTRICTED_TOKEN		0x0002
#define JOB_OBJECT_SECURITY_ONLY_TOKEN			0x0004
#define JOB_OBJECT_SECURITY_FILTER_TOKENS		0x0008

/* JOBOBJECT_END_OF_JOB_TIME_INFORMATION.EndOfJobTimeAction constants */
#define JOB_OBJECT_TERMINATE_AT_END_OF_JOB		 0
#define JOB_OBJECT_POST_AT_END_OF_JOB			 1

#define JOB_OBJECT_MSG_END_OF_JOB_TIME			 1
#define JOB_OBJECT_MSG_END_OF_PROCESS_TIME		 2
#define JOB_OBJECT_MSG_ACTIVE_PROCESS_LIMIT		 3
#define JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO		 4
#define JOB_OBJECT_MSG_NEW_PROCESS			 6
#define JOB_OBJECT_MSG_EXIT_PROCESS			 7
#define JOB_OBJECT_MSG_ABNORMAL_EXIT_PROCESS		 8
#define JOB_OBJECT_MSG_PROCESS_MEMORY_LIMIT		 9
#define JOB_OBJECT_MSG_JOB_MEMORY_LIMIT 		10


typedef enum _JOBOBJECTINFOCLASS
{ JobObjectBasicAccountingInformation		=	 1,
  JobObjectBasicLimitInformation,
  JobObjectBasicProcessIdList,
  JobObjectBasicUIRestrictions,
  JobObjectSecurityLimitInformation,
  JobObjectEndOfJobTimeInformation,
  JobObjectAssociateCompletionPortInformation,
  JobObjectBasicAndIoAccountingInformation,
  JobObjectExtendedLimitInformation,
  JobObjectJobSetInformation,
  MaxJobObjectInfoClass
} JOBOBJECTINFOCLASS;

typedef struct _JOBOBJECT_BASIC_ACCOUNTING_INFORMATION
{ LARGE_INTEGER 	TotalUserTime;
  LARGE_INTEGER 	TotalKernelTime;
  LARGE_INTEGER 	ThisPeriodTotalUserTime;
  LARGE_INTEGER 	ThisPeriodTotalKernelTime;
  DWORD 		TotalPageFaultCount;
  DWORD 		TotalProcesses;
  DWORD 		ActiveProcesses;
  DWORD 		TotalTerminatedProcesses;
} JOBOBJECT_BASIC_ACCOUNTING_INFORMATION, *PJOBOBJECT_BASIC_ACCOUNTING_INFORMATION;

typedef struct _JOBOBJECT_BASIC_LIMIT_INFORMATION
{ LARGE_INTEGER 	PerProcessUserTimeLimit;
  LARGE_INTEGER 	PerJobUserTimeLimit;
  DWORD 		LimitFlags;
  SIZE_T		MinimumWorkingSetSize;
  SIZE_T		MaximumWorkingSetSize;
  DWORD 		ActiveProcessLimit;
  ULONG_PTR		Affinity;
  DWORD 		PriorityClass;
  DWORD 		SchedulingClass;
} JOBOBJECT_BASIC_LIMIT_INFORMATION, *PJOBOBJECT_BASIC_LIMIT_INFORMATION;

typedef struct _JOBOBJECT_BASIC_PROCESS_ID_LIST
{ DWORD 		NumberOfAssignedProcesses;
  DWORD 		NumberOfProcessIdsInList;
  ULONG_PTR		ProcessIdList[1];
} JOBOBJECT_BASIC_PROCESS_ID_LIST, *PJOBOBJECT_BASIC_PROCESS_ID_LIST;

typedef struct _JOBOBJECT_BASIC_UI_RESTRICTIONS
{ DWORD 		UIRestrictionsClass;
} JOBOBJECT_BASIC_UI_RESTRICTIONS, *PJOBOBJECT_BASIC_UI_RESTRICTIONS;

typedef struct _JOBOBJECT_SECURITY_LIMIT_INFORMATION
{ DWORD 		SecurityLimitFlags;
  HANDLE		JobToken;
  PTOKEN_GROUPS 	SidsToDisable;
  PTOKEN_PRIVILEGES	PrivilegesToDelete;
  PTOKEN_GROUPS 	RestrictedSids;
} JOBOBJECT_SECURITY_LIMIT_INFORMATION, *PJOBOBJECT_SECURITY_LIMIT_INFORMATION;

typedef struct _JOBOBJECT_END_OF_JOB_TIME_INFORMATION
{ DWORD 		EndOfJobTimeAction;
} JOBOBJECT_END_OF_JOB_TIME_INFORMATION, *PJOBOBJECT_END_OF_JOB_TIME_INFORMATION;

typedef struct _JOBOBJECT_ASSOCIATE_COMPLETION_PORT
{ PVOID 		CompletionKey;
  HANDLE		CompletionPort;
} JOBOBJECT_ASSOCIATE_COMPLETION_PORT, *PJOBOBJECT_ASSOCIATE_COMPLETION_PORT;

typedef struct _JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION
{ JOBOBJECT_BASIC_ACCOUNTING_INFORMATION	BasicInfo;
  IO_COUNTERS					IoInfo;
} JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION, *PJOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION;

typedef struct _JOBOBJECT_EXTENDED_LIMIT_INFORMATION
{ JOBOBJECT_BASIC_LIMIT_INFORMATION		BasicLimitInformation;
  IO_COUNTERS					IoInfo;
  SIZE_T					ProcessMemoryLimit;
  SIZE_T					JobMemoryLimit;
  SIZE_T					PeakProcessMemoryUsed;
  SIZE_T					PeakJobMemoryUsed;
} JOBOBJECT_EXTENDED_LIMIT_INFORMATION, *PJOBOBJECT_EXTENDED_LIMIT_INFORMATION;

typedef struct _JOBOBJECT_JOBSET_INFORMATION
{ DWORD 		MemberLevel;
} JOBOBJECT_JOBSET_INFORMATION, *PJOBOBJECT_JOBSET_INFORMATION;

/* Fixme: Making these defines conditional on WINVER will break ddk includes */
#if 1 /* (WINVER >= 0x0500) */
#include <pshpack4.h>

#define ES_SYSTEM_REQUIRED			0x00000001
#define ES_DISPLAY_REQUIRED			0x00000002
#define ES_USER_PRESENT 			0x00000004
#define ES_CONTINUOUS				0x80000000


typedef enum _LATENCY_TIME
{ LT_DONT_CARE,
  LT_LOWEST_LATENCY
} LATENCY_TIME, *PLATENCY_TIME;


typedef enum _SYSTEM_POWER_STATE
{ PowerSystemUnspecified,
  PowerSystemWorking,
  PowerSystemSleeping1,
  PowerSystemSleeping2,
  PowerSystemSleeping3,
  PowerSystemHibernate,
  PowerSystemShutdown,
  PowerSystemMaximum
} SYSTEM_POWER_STATE, *PSYSTEM_POWER_STATE;
#define POWER_SYSTEM_MAXIMUM  PowerSystemMaximum

typedef enum
{ PowerActionNone,
  PowerActionReserved,
  PowerActionSleep,
  PowerActionHibernate,
  PowerActionShutdown,
  PowerActionShutdownReset,
  PowerActionShutdownOff,
  PowerActionWarmEject
} POWER_ACTION, *PPOWER_ACTION;

#if (_WIN32_WINNT >= 0x0600)

typedef enum
{ PoAc			=	0,
  PoDc			=	1,
  PoHot 		=	2,
  PoConditionMaximum	=	3
} SYSTEM_POWER_CONDITION, *PSYSTEM_POWER_CONDITION;
#endif


typedef enum _DEVICE_POWER_STATE
{ PowerDeviceUnspecified,
  PowerDeviceD0,
  PowerDeviceD1,
  PowerDeviceD2,
  PowerDeviceD3,
  PowerDeviceMaximum
} DEVICE_POWER_STATE, *PDEVICE_POWER_STATE;


typedef struct
{ DWORD 		Granularity;
  DWORD 		Capacity;
} BATTERY_REPORTING_SCALE, *PBATTERY_REPORTING_SCALE;


typedef struct _POWER_ACTION_POLICY
{ POWER_ACTION		Action;
  ULONG 		Flags;
  ULONG 		EventCode;
} POWER_ACTION_POLICY, *PPOWER_ACTION_POLICY;

/* POWER_ACTION_POLICY.Flags constants */
#define POWER_ACTION_QUERY_ALLOWED		0x00000001
#define POWER_ACTION_UI_ALLOWED 		0x00000002
#define POWER_ACTION_OVERRIDE_APPS		0x00000004
#define POWER_ACTION_LIGHTEST_FIRST		0x10000000
#define POWER_ACTION_LOCK_CONSOLE		0x20000000
#define POWER_ACTION_DISABLE_WAKES		0x40000000
#define POWER_ACTION_CRITICAL			0x80000000

/* POWER_ACTION_POLICY.EventCode constants */
#define POWER_LEVEL_USER_NOTIFY_TEXT		0x00000001
#define POWER_LEVEL_USER_NOTIFY_SOUND		0x00000002
#define POWER_LEVEL_USER_NOTIFY_EXEC		0x00000004
#define POWER_USER_NOTIFY_BUTTON		0x00000008
#define POWER_USER_NOTIFY_SHUTDOWN		0x00000010
#define POWER_FORCE_TRIGGER_RESET		0x80000000

#define DISCHARGE_POLICY_CRITICAL			 0
#define DISCHARGE_POLICY_LOW				 1
#define NUM_DISCHARGE_POLICIES				 4

#define PO_THROTTLE_NONE				 0
#define PO_THROTTLE_CONSTANT				 1
#define PO_THROTTLE_DEGRADE				 2
#define PO_THROTTLE_ADAPTIVE				 3
#define PO_THROTTLE_MAXIMUM				 4

typedef struct _SYSTEM_POWER_LEVEL
{ BOOLEAN		Enable;
  UCHAR 		Spare[3];
  ULONG 		BatteryLevel;
  POWER_ACTION_POLICY	PowerPolicy;
  SYSTEM_POWER_STATE	MinSystemState;
} SYSTEM_POWER_LEVEL, *PSYSTEM_POWER_LEVEL;

typedef struct _SYSTEM_POWER_POLICY
{ ULONG 		Revision;
  POWER_ACTION_POLICY	PowerButton;
  POWER_ACTION_POLICY	SleepButton;
  POWER_ACTION_POLICY	LidClose;
  SYSTEM_POWER_STATE	LidOpenWake;
  ULONG 		Reserved;
  POWER_ACTION_POLICY	Idle;
  ULONG 		IdleTimeout;
  UCHAR 		IdleSensitivity;
  UCHAR 		DynamicThrottle;
  UCHAR 		Spare2[2];
  SYSTEM_POWER_STATE	MinSleep;
  SYSTEM_POWER_STATE	MaxSleep;
  SYSTEM_POWER_STATE	ReducedLatencySleep;
  ULONG 		WinLogonFlags;
  ULONG 		Spare3;
  ULONG 		DozeS4Timeout;
  ULONG 		BroadcastCapacityResolution;
  SYSTEM_POWER_LEVEL	DischargePolicy[NUM_DISCHARGE_POLICIES];
  ULONG 		VideoTimeout;
  BOOLEAN		VideoDimDisplay;
  ULONG 		VideoReserved[3];
  ULONG 		SpindownTimeout;
  BOOLEAN		OptimizeForPower;
  UCHAR 		FanThrottleTolerance;
  UCHAR 		ForcedThrottle;
  UCHAR 		MinThrottle;
  POWER_ACTION_POLICY	OverThrottled;
} SYSTEM_POWER_POLICY, *PSYSTEM_POWER_POLICY;


typedef struct _SYSTEM_POWER_CAPABILITIES
{ BOOLEAN			PowerButtonPresent;
  BOOLEAN			SleepButtonPresent;
  BOOLEAN			LidPresent;
  BOOLEAN			SystemS1;
  BOOLEAN			SystemS2;
  BOOLEAN			SystemS3;
  BOOLEAN			SystemS4;
  BOOLEAN			SystemS5;
  BOOLEAN			HiberFilePresent;
  BOOLEAN			FullWake;
  BOOLEAN			VideoDimPresent;
  BOOLEAN			ApmPresent;
  BOOLEAN			UpsPresent;
  BOOLEAN			ThermalControl;
  BOOLEAN			ProcessorThrottle;
  UCHAR 			ProcessorMinThrottle;
  UCHAR 			ProcessorMaxThrottle;
  BOOLEAN			FastSystemS4;
  UCHAR 			spare2[3];
  BOOLEAN			DiskSpinDown;
  UCHAR 			spare3[8];
  BOOLEAN			SystemBatteriesPresent;
  BOOLEAN			BatteriesAreShortTerm;
  BATTERY_REPORTING_SCALE	BatteryScale[3];
  SYSTEM_POWER_STATE		AcOnLineWake;
  SYSTEM_POWER_STATE		SoftLidWake;
  SYSTEM_POWER_STATE		RtcWake;
  SYSTEM_POWER_STATE		MinDeviceWakeState;
  SYSTEM_POWER_STATE		DefaultLowLatencyWake;
} SYSTEM_POWER_CAPABILITIES, *PSYSTEM_POWER_CAPABILITIES;


typedef struct _SYSTEM_BATTERY_STATE
{ BOOLEAN			AcOnLine;
  BOOLEAN			BatteryPresent;
  BOOLEAN			Charging;
  BOOLEAN			Discharging;
  BOOLEAN			Spare1[4];
  ULONG 			MaxCapacity;
  ULONG 			RemainingCapacity;
  ULONG 			Rate;
  ULONG 			EstimatedTime;
  ULONG 			DefaultAlert1;
  ULONG 			DefaultAlert2;
} SYSTEM_BATTERY_STATE, *PSYSTEM_BATTERY_STATE;


typedef enum _POWER_INFORMATION_LEVEL
{ SystemPowerPolicyAc,
  SystemPowerPolicyDc,
  VerifySystemPolicyAc,
  VerifySystemPolicyDc,
  SystemPowerCapabilities,
  SystemBatteryState,
  SystemPowerStateHandler,
  ProcessorStateHandler,
  SystemPowerPolicyCurrent,
  AdministratorPowerPolicy,
  SystemReserveHiberFile,
  ProcessorInformation,
  SystemPowerInformation,
  ProcessorStateHandler2,
  LastWakeTime,
  LastSleepTime,
  SystemExecutionState,
  SystemPowerStateNotifyHandler,
  ProcessorPowerPolicyAc,
  ProcessorPowerPolicyDc,
  VerifyProcessorPowerPolicyAc,
  VerifyProcessorPowerPolicyDc,
  ProcessorPowerPolicyCurrent
} POWER_INFORMATION_LEVEL;

#if (_WIN32_WINNT >= 0x0500)
typedef LONG (WINAPI *PVECTORED_EXCEPTION_HANDLER)(PEXCEPTION_POINTERS);
#endif
#if 1 /* (WIN32_WINNT >= 0x0500) */

typedef struct _SYSTEM_POWER_INFORMATION
{ ULONG 	MaxIdlenessAllowed;
  ULONG 	Idleness;
  ULONG 	TimeRemaining;
  UCHAR 	CoolingMode;
} SYSTEM_POWER_INFORMATION, *PSYSTEM_POWER_INFORMATION;
#endif

#if (_WIN32_WINNT >= 0x0501)

typedef enum _HEAP_INFORMATION_CLASS
{ HeapCompatibilityInformation
} HEAP_INFORMATION_CLASS;

typedef enum _ACTIVATION_CONTEXT_INFO_CLASS
{ ActivationContextBasicInformation	=	1,
  ActivationContextDetailedInformation,
  AssemblyDetailedInformationInActivationContext,
  FileInformationInAssemblyOfAssemblyInActivationContext
} ACTIVATION_CONTEXT_INFO_CLASS;

typedef struct _ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION
{ DWORD 		ulFlags;
  DWORD 		ulEncodedAssemblyIdentityLength;
  DWORD 		ulManifestPathType;
  DWORD 		ulManifestPathLength;
  LARGE_INTEGER 	liManifestLastWriteTime;
  DWORD 		ulPolicyPathType;
  DWORD 		ulPolicyPathLength;
  LARGE_INTEGER 	liPolicyLastWriteTime;
  DWORD 		ulMetadataSatelliteRosterIndex;
  DWORD 		ulManifestVersionMajor;
  DWORD 		ulManifestVersionMinor;
  DWORD 		ulPolicyVersionMajor;
  DWORD 		ulPolicyVersionMinor;
  DWORD 		ulAssemblyDirectoryNameLength;
  PCWSTR		lpAssemblyEncodedAssemblyIdentity;
  PCWSTR		lpAssemblyManifestPath;
  PCWSTR		lpAssemblyPolicyPath;
  PCWSTR		lpAssemblyDirectoryName;
} ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION, *PACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION;
typedef const ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION *PCACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION;

typedef struct _ACTIVATION_CONTEXT_DETAILED_INFORMATION
{ DWORD 		dwFlags;
  DWORD 		ulFormatVersion;
  DWORD 		ulAssemblyCount;
  DWORD 		ulRootManifestPathType;
  DWORD 		ulRootManifestPathChars;
  DWORD 		ulRootConfigurationPathType;
  DWORD 		ulRootConfigurationPathChars;
  DWORD 		ulAppDirPathType;
  DWORD 		ulAppDirPathChars;
  PCWSTR 		lpRootManifestPath;
  PCWSTR 		lpRootConfigurationPath;
  PCWSTR 		lpAppDirPath;
} ACTIVATION_CONTEXT_DETAILED_INFORMATION, *PACTIVATION_CONTEXT_DETAILED_INFORMATION;
typedef const ACTIVATION_CONTEXT_DETAILED_INFORMATION *PCACTIVATION_CONTEXT_DETAILED_INFORMATION;

typedef struct _ACTIVATION_CONTEXT_QUERY_INDEX
{ ULONG 		ulAssemblyIndex;
  ULONG 		ulFileIndexInAssembly;
} ACTIVATION_CONTEXT_QUERY_INDEX, *PACTIVATION_CONTEXT_QUERY_INDEX;
typedef const ACTIVATION_CONTEXT_QUERY_INDEX *PCACTIVATION_CONTEXT_QUERY_INDEX;

typedef struct _ASSEMBLY_FILE_DETAILED_INFORMATION
{ DWORD 		ulFlags;
  DWORD 		ulFilenameLength;
  DWORD 		ulPathLength;
  PCWSTR		lpFileName;
  PCWSTR		lpFilePath;
} ASSEMBLY_FILE_DETAILED_INFORMATION, *PASSEMBLY_FILE_DETAILED_INFORMATION;
typedef const ASSEMBLY_FILE_DETAILED_INFORMATION *PCASSEMBLY_FILE_DETAILED_INFORMATION;
#endif /* (WIN32_WINNT >= 0x0501) */

typedef struct _PROCESSOR_POWER_POLICY_INFO
{ ULONG 		TimeCheck;
  ULONG 		DemoteLimit;
  ULONG 		PromoteLimit;
  UCHAR 		DemotePercent;
  UCHAR 		PromotePercent;
  UCHAR 		Spare[2];
  ULONG 		AllowDemotion : 1;
  ULONG 		AllowPromotion : 1;
  ULONG 		Reserved : 30;
} PROCESSOR_POWER_POLICY_INFO, *PPROCESSOR_POWER_POLICY_INFO;

typedef struct _PROCESSOR_POWER_POLICY
{ ULONG 			Revision;
  UCHAR 			DynamicThrottle;
  UCHAR 			Spare[3];
  ULONG 			Reserved;
  ULONG 			PolicyCount;
  PROCESSOR_POWER_POLICY_INFO	Policy[3];
} PROCESSOR_POWER_POLICY, *PPROCESSOR_POWER_POLICY;

typedef struct _ADMINISTRATOR_POWER_POLICY
{ SYSTEM_POWER_STATE	MinSleep;
  SYSTEM_POWER_STATE	MaxSleep;
  ULONG 		MinVideoTimeout;
  ULONG 		MaxVideoTimeout;
  ULONG 		MinSpindownTimeout;
  ULONG 		MaxSpindownTimeout;
} ADMINISTRATOR_POWER_POLICY, *PADMINISTRATOR_POWER_POLICY;
#include <poppack.h>
#endif /* WINVER >= 0x0500 */

#if (_WIN32_WINNT >= 0x0500)
typedef VOID (NTAPI *WAITORTIMERCALLBACKFUNC)(PVOID,BOOLEAN);
#endif

#ifdef UNICODE
typedef OSVERSIONINFOW OSVERSIONINFO, *POSVERSIONINFO, *LPOSVERSIONINFO;
typedef OSVERSIONINFOEXW OSVERSIONINFOEX, *POSVERSIONINFOEX, *LPOSVERSIONINFOEX;
#else
typedef OSVERSIONINFOA OSVERSIONINFO, *POSVERSIONINFO, *LPOSVERSIONINFO;
typedef OSVERSIONINFOEXA OSVERSIONINFOEX, *POSVERSIONINFOEX, *LPOSVERSIONINFOEX;
#endif

#if (_WIN32_WINNT >= 0x0500)
ULONGLONG WINAPI VerSetConditionMask(ULONGLONG,DWORD,BYTE);
#define VER_SET_CONDITION(ConditionMask, TypeBitMask, ComparisonType)  \
	((ConditionMask) = VerSetConditionMask((ConditionMask), \
	(TypeBitMask), (ComparisonType)))
#endif

#if defined(__GNUC__)
#if defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64)
/* On regular _WIN32 and _WIN64 hardware platforms, provide inline
 * assembler pseudo-intrinsics to access the thread information block,
 * and the fiber data referenced therein.
 */
#ifndef _WIN64  /* Not 64-bit, so assume 32-bit */
/* On 32-bit Windows, TIB data is located via the FS segment, with
 * offsets computed in terms of the size of 4-byte pointers; thus the
 * offset of FiberData, preceded by four pointer fields, is 0x10, and
 * Self, preceded by six pointers, is at offset 0x18...
 */
#define __tib_fiber_data__   "fs:0x10"
#define __tib_self_refptr__  "fs:0x18"

#else
/* ...whereas, on 64-bit, the pointer size is doubled to eight bytes,
 * so the offsets double to 0x20 for FiberData, and 0x30 for Self,
 * respectively.
 */
#define __tib_fiber_data__   "gs:0x20"
#define __tib_self_refptr__  "gs:0x30"
#endif

#if (__GNUC__ >= 3)
/* From GCC-3.x onwards, the user may specify -masm=intel, and so
 * generate assembly language intermediate output in Intel/Microsoft
 * syntax; we must specify dual mode inline assembly code to support
 * this, (in addition to the default AT&T syntax).
 */
static __inline__ PVOID GetCurrentFiber (void)
{
  void *ret;
  __asm__ __volatile__ (
      "mov{%z0}\t{%%" __tib_fiber_data__ ", %0|%0," __tib_fiber_data__ "}"
      : "=a" (ret) /* resultant pointer will be returned in eax/rax */
    );
  return ret;
}

static __inline__ PVOID GetFiberData (void)
{
  void *ret;
  __asm__ __volatile__ (
      "mov{%z0}\t{%%" __tib_fiber_data__ ", %0|%0, " __tib_fiber_data__ "}\n\t"
      "mov{%z0}\t{(%0), %0|%0, [%0]}"
      : "=r" (ret) /* likely eax/rax, but must support indexing */
    );
  return ret;
}

static __inline__ struct _TEB *NtCurrentTeb (void)
{
  struct _TEB *ret;
  __asm__ __volatile__ (
      "mov{%z0}\t{%%" __tib_self_refptr__ ", %0|%0, " __tib_self_refptr__ "}"
      : "=a" (ret) /* resultant pointer will be returned in eax/rax */
      : /* no inputs */
    );
  return ret;
}
#else /* __GNUC__ < 3 */
/* Older versions of GCC, predating GCC-3.x, did not support -masm=intel;
 * specify inline assembly code in AT&T syntax only.
 */
static __inline__ PVOID GetCurrentFiber (void)
{
  void *ret;
  __asm__ __volatile__ (
      "mov%z0\t%%" __tib_fiber_data__ ", %0"
      : "=a" (ret) /* resultant pointer will be returned in eax/rax */
    );
  return ret;
}

static __inline__ PVOID GetFiberData (void)
{
  void *ret;
  __asm__ __volatile__ (
      "mov%z0\t%%" __tib_fiber_data__ ", %0\n\t"
      "mov%z0\t(%0), %0"
      : "=r" (ret) /* likely eax/rax, but must support indexing */
    );
  return ret;
}

static __inline__ struct _TEB *NtCurrentTeb (void)
{
  struct _TEB *ret;
  __asm__ __volatile__ (
      "mov%z0\t%%" __tib_self_refptr__ ", %0"
      : "=a" (ret) /* resultant pointer will be returned in eax/rax */
      : /* no inputs */
    );
  return ret;
}
#endif	/* __GNUC__ < 3 */
#endif	/* _M_IX86 || _M_X64 || _M_IA64 */
#else	/* !__GNUC__ */
/* The (unidentified) compiler is not GCC.
 * FIXME: If it isn't GCC, what is it?  Given that what follows is
 * inline assembly code, it surely can't be generically suitable for
 * use with any non-GCC compiler?  Google suggests that this syntax
 * may be applicable for WATCOM compilers.  More to the point: given
 * that our mission is to deliver GCC for Windows, why is this here
 * at all?  Do we REALLY want to carry this maintenance burden?
 */
#if defined(_M_IX86)
PVOID GetCurrentFiber (void);
#pragma aux GetCurrentFiber = \
  "mov	eax, dword ptr fs:0x10" \
  value [eax] \
  modify [eax];

PVOID GetFiberData (void);
#pragma aux GetFiberData = \
  "mov	eax, dword ptr fs:0x10" \
  "mov	eax, [eax]" \
  value [eax] \
  modify [eax];

struct _TEB *NtCurrentTeb (void);
#pragma aux NtCurrentTeb = \
  "mov	eax, dword ptr fs:0x18" \
  value [eax] \
  modify [eax];

#endif	/* _M_IX86 */
#endif	/* !__GNUC__ */

/* MemoryBarrier() Implementation
 * ------------------------------
 * https://msdn.microsoft.com/en-us/library/windows/desktop/ms684208%28v=vs.85%29.aspx
 *
 * MSDN says that this requires one of Vista, or Windows Server-2003,
 * or later; I don't see why that should be, since neither Microsoft's
 * implementation, nor the following, actually exhibits any dependency
 * on any OS feature specific to these, or later versions.
 *
 * For this MinGW.org MemoryBarrier() implementation, we aim to rely,
 * to the maximum practicable extent, on the intrinsic implementations
 * provided by the GCC compiler itself; thus, for those recent versions
 * of GCC, (those which implement the sequentially consistent model for
 * atomic memory access)...
 */
#if defined(__ATOMIC_SEQ_CST)
 /* ...we implement barriers using the __atomic_thread_fence() intrinsic
  * function, in sequentially consistent mode.
  */
# define __mingw_fence_type__	__ATOMIC_SEQ_CST
# define __mingw_thread_fence	__atomic_thread_fence

 /* For earlier compiler versions, which lack __atomic_thread_fence(),
  * we fall back on legacy mechanisms; thus, from GCC-4.4 onward...
  */
#elif ((__GNUC__ << 8) + __GNUC_MINOR__) >= 0x404
 /* ...we fall back to the __sync_synchronize() intrinsic function; (this
  * was first mentioned in the documentation for GCC-4.1, but comments in
  * the GCC source code suggest that it did not achieve a robust state of
  * implementation, until GCC-4.4).
  */
# define __mingw_thread_fence	__sync_synchronize
# define __mingw_fence_type__

#else
 /* Finally, when there is no viable intrinsic implementation, we fall
  * back on inline assembly code.  This is CPU architecture dependent:
  * we choose the instructions, for each architecture, as best we can
  * to match the code emitted by __atomic_thread_fence(), in versions
  * of GCC which support it; thus...
  */
# define __mingw_thread_fence	__asm__ __volatile__
# define __mingw_fence_type__	__mingw_fence_insn__:::"memory"

# if defined(_M_X64) || defined(_M_AMD64) || (_M_IX86_FP >= 2)
  /* ...for X86-64, AMD-64, and 32-bit X86 processors with support for
   * SSE2 instructions, we may issue an "mfence" instruction directly...
   */
#  define __mingw_fence_insn__	"mfence"

# elif defined(_M_IA64)
  /* ...while for IA-64 (Itanium), the equivalent is "mf"...
   */
#  define __mingw_fence_insn__	"mf"

# elif defined(_M_IX86)
  /* ...and, for 32-bit X86 processors without support for the SSE2
   * instruction set, (nominally, our default supported architecture),
   * we perform a bus-locked memory fetch and store, (OR with zero).
   */
#  define __mingw_fence_insn__	"lock or{l}\t{$0, (%%esp)|DWORD PTR [esp], 0}"

  /* FIXME: To support other CPU architectures, any appropriate fall
   * back instructions must be identified here, (with an appropriate
   * "#elif defined(_M_ARCH)" clause for each)...
   */
# else
  /* ...and, as a last resort, we arrange to inject NOTHING into the
   * code stream; (notwthstanding, this may still suffice as a light
   * weight barrier, insofar as it should prevent reordering of code,
   * which might otherwise result from compiler optimizations, about
   * the barrier placement point).
   */
#  define __mingw_fence_insn__	""
# endif
#endif

/* Finally, the preceding implementation choices are encapsulated
 * into the actual MemoryBarrier() implementation; this is provided
 * in the form of a function, with static scope, and defined such
 * that it must always be expanded in-line...
 */
static FORCEINLINE void MemoryBarrier (void)
{ __mingw_thread_fence(__mingw_fence_type__); }

/* ...before purging the implementation choice macros, (which are
 * no longer needed), from the pre-processor namespace.
 */
#undef __mingw_fence_type__
#undef __mingw_thread_fence
#undef __mingw_fence_insn__

_END_C_DECLS

#endif	/* ! RC_INVOKED */
#endif	/* !_WINNT_H: internal recursion break */
#endif	/* !_WINNT_H: $RCSfile: winnt.h,v $: end of file */
