/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 */

#include "config.h"
#include <hexchat-plugin.h>

#define MODULE_NAME "otr"

#define PNAME "OTR"
#define PDESC "Off-The-Record Messaging for Hexchat"
#define PVERSION PACKAGE_VERSION
#define OTR_HELP "OTR\n\
    version: Prints version of plugin\n\
    start: Starts an OTR chat (init also works)\n\
    finish [<nick>]: Finish an OTR chat\n\
    trust [<nick>]: Trusts the other user\n\
    auth [<nick>] <password>: Auths a user via password\n\
    authq [<nick>] <question> <answer>: Auths a user via question\n\
    authabort [<nick>]: Aborts auth in progress\n\
    genkey [abort|<accountname>]: Generates a new key\n\
    set [<setting>]: Changes settings, run without args for current values"

#define MAX_FORMAT_PARAMS 10

struct _IRC_CTX
{
	char *nick;
	char *address;
};

typedef struct _IRC_CTX IRC_CTX;

struct _FORMAT_REC
{
	char *tag;
	char *def;
};

typedef struct _FORMAT_REC FORMAT_REC;

enum
{
	MSGLEVEL_CRAP,
	MSGLEVEL_MSGS
} lvls;

extern hexchat_plugin *ph; /* plugin handle */

void printformat (IRC_CTX *ircctx, const char *nick, int lvl, int fnum, ...);

#define otr_noticest(formatnum, ...) \
	printformat (NULL, NULL, MSGLEVEL_MSGS, formatnum, ##__VA_ARGS__)

#define otr_notice(server, nick, formatnum, ...) \
	printformat (server, nick, MSGLEVEL_MSGS, formatnum, ##__VA_ARGS__)

#define otr_infost(formatnum, ...) \
	printformat (NULL, NULL, MSGLEVEL_CRAP, formatnum, ##__VA_ARGS__)

#define otr_info(server, nick, formatnum, ...) \
	printformat (server, nick, MSGLEVEL_CRAP, formatnum, ##__VA_ARGS__)

#define otr_debug(server, nick, formatnum, ...)                                  \
	{                                                                            \
		if (debug)                                                               \
			printformat (server, nick, MSGLEVEL_MSGS, formatnum, ##__VA_ARGS__); \
	}
#define IRCCTX_DUP(ircctx) g_memdup (ircctx, sizeof(IRC_CTX));
#define IRCCTX_ADDR(ircctx) ircctx->address
#define IRCCTX_NICK(ircctx) ircctx->nick
#define IRCCTX_FREE(ircctx) g_free (ircctx)
