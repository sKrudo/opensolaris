#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

.KEEP_STATE:
.SUFFIXES:

SRCS = dm.c dm_ask.c dm_command.c dm_mem_debug.c dm_msg.c dm_request.c \
	dm_target.c dm_uscsi.c dm_util.c

PROG = mmsdm

OBJS = $(SRCS:%.c=%.o)
lint_SRCS = $(SRCS:%.c=%.ln)

include ../../../Makefile.cmd

ROOTCMDDIR=	$(ROOT)/usr/lib

$(NOT_RELEASE_BUILD)CPPFLAGS += -DDEBUG
CPPFLAGS += -DMMS_OPENSSL
CPPFLAGS += -I. -I../common -I$(SRC)/common/mms/mms
CPPFLAGS += -I$(SRC)/lib/mms/mms/common -I$(SRC)/uts/common/io/mms/dda
CPPFLAGS += -I$(SRC)/uts/common/io/mms/dmd

CFLAGS += $(CTF_FLAGS) $(CC_VERBOSE)

LDLIBS += -lc -lgen -lsecdb
LDLIBS += -L$(SRC)/lib/mms/mms/$(MACH) -lmms

.NO_PARALLEL:
.PARALLEL: $(OBJS) $(LINTFILES)

C99MODE=	$(C99_ENABLE)

all: $(PROG)

$(PROG): $(OBJS)
	$(LINK.c) $(OBJS) -o $@ $(LDLIBS)
	$(CTFMERGE) -L VERSION -o $@ $(OBJS)
	$(POST_PROCESS)

%.o: ../common/%.c
	$(COMPILE.c) $<
	$(CTFCONVERT_O)

clean:
	$(RM) $(OBJS) $(lint_SRCS)

lint: $(lint_SRCS)

%.ln: ../common/%.c
	$(LINT.c) -c $<

install: all $(ROOTCMD)

include ../../../Makefile.targ
