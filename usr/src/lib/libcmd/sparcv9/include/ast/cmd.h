
/* : : generated by proto : : */
/***********************************************************************
*                                                                      *
*               This software is part of the ast package               *
*          Copyright (c) 1992-2008 AT&T Intellectual Property          *
*                      and is licensed under the                       *
*                  Common Public License, Version 1.0                  *
*                    by AT&T Intellectual Property                     *
*                                                                      *
*                A copy of the License is available at                 *
*            http://www.opensource.org/licenses/cpl1.0.txt             *
*         (with md5 checksum 059e8cd6165cb4c31e351f2b69388fd9)         *
*                                                                      *
*              Information and Software Systems Research               *
*                            AT&T Research                             *
*                           Florham Park NJ                            *
*                                                                      *
*                 Glenn Fowler <gsf@research.att.com>                  *
*                  David Korn <dgk@research.att.com>                   *
*                                                                      *
***********************************************************************/
                  
/*
 * AT&T Research
 *
 * builtin cmd definitions
 */

#ifndef _CMD_H
#if !defined(__PROTO__)
#include <prototyped.h>
#endif
#if !defined(__LINKAGE__)
#define __LINKAGE__		/* 2004-08-11 transition */
#endif

#define _CMD_H

#include <ast.h>
#include <error.h>
#include <stak.h>
#include <shcmd.h>

#define cmdinit			_cmd_init

#if _BLD_cmd && defined(__EXPORT__)
#undef __MANGLE__
#define __MANGLE__ __LINKAGE__		__EXPORT__
#endif

#include <cmdext.h>

#undef __MANGLE__
#define __MANGLE__ __LINKAGE__

#if defined(CMD_BUILTIN) && !defined(CMD_STANDALONE)
#define CMD_STANDALONE	CMD_BUILTIN
#endif

#ifdef CMD_STANDALONE

#define CMD_CONTEXT(c)		((Shbltin_t*)0)

#if CMD_DYNAMIC

#include <dlldefs.h>

typedef int (*Shbltin_f) __PROTO__((int, char**, __V_*));

#else

extern __MANGLE__ int CMD_STANDALONE __PROTO__((int, char**, __V_*));

#endif

#ifndef CMD_BUILTIN

/*
 * command initialization
 */

static int
cmdinit __PARAM__((int argc, register char** argv, __V_* context, const char* catalog, int flags), (argc, argv, context, catalog, flags)) __OTORP__(int argc; register char** argv; __V_* context; const char* catalog; int flags;){
	register char*	cp;
	register char*	pp;

	if (cp = strrchr(argv[0], '/'))
		cp++;
	else
		cp = argv[0];
	if (pp = strrchr(cp, '_'))
		cp = pp + 1;
	error_info.id = cp;
	if (!error_info.catalog)
		error_info.catalog = (char*)catalog;
	opt_info.index = 0;
	if (context)
		error_info.flags |= flags;
	return 0;
}

#endif

int
main __PARAM__((int argc, char** argv), (argc, argv)) __OTORP__(int argc; char** argv;){
#if CMD_DYNAMIC
	register char*	s;
	register char*	t;
	__V_*		dll;
	Shbltin_f	fun;
	char		buf[64];

	if (s = strrchr(argv[0], '/'))
		s++;
	else if (!(s = argv[0]))
		return 127;
	if ((t = strrchr(s, '_')) && *++t)
		s = t;
	buf[0] = '_';
	buf[1] = 'b';
	buf[2] = '_';
	strncpy(buf + 3, s, sizeof(buf) - 4);
	buf[sizeof(buf) - 1] = 0;
	if (t = strchr(buf, '.'))
		*t = 0;
	for (;;)
	{
		if (dll = dlopen(NiL, RTLD_LAZY))
		{
			if (fun = (Shbltin_f)dlsym(dll, buf + 1))
				break;
			if (fun = (Shbltin_f)dlsym(dll, buf))
				break;
		}
		if (dll = dllfind("cmd", NiL, RTLD_LAZY))
		{
			if (fun = (Shbltin_f)dlsym(dll, buf + 1))
				break;
			if (fun = (Shbltin_f)dlsym(dll, buf))
				break;
		}
		return 127;
	}
	return (*fun)(argc, argv, NiL);
#else
	return CMD_STANDALONE(argc, argv, NiL);
#endif
}

#else

#undef	cmdinit
#ifdef _MSC_VER
#define CMD_CONTEXT(p)		((Shbltin_t*)(p))
#define cmdinit(a,b,c,d,e)	do{if(_cmd_init(a,b,c,d,e))return -1;}while(0)
#else
#define CMD_CONTEXT(p)		(((p)&&((Shbltin_t*)(p))->version>=20071012&&((Shbltin_t*)(p))->version<20350101)?((Shbltin_t*)(p)):0)
#define cmdinit(a,b,c,d,e)	do{if((c)&&!CMD_CONTEXT(c))c=0;if(_cmd_init(a,b,c,d,e))return -1;}while(0)
#endif

#if _BLD_cmd && defined(__EXPORT__)
#undef __MANGLE__
#define __MANGLE__ __LINKAGE__ __EXPORT__
#endif

extern __MANGLE__ int	_cmd_init __PROTO__((int, char**, __V_*, const char*, int));

#undef __MANGLE__
#define __MANGLE__ __LINKAGE__

#endif

#endif
