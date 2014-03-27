#ifndef __WQQ_CRYPTO_H__
#define __WQQ_CRYPTO_H__
/*
 * wqqcrypto.h
 *
 * Copyright (C) 2013 - Wiky L(wiiiky@yeah.net)
 *
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <errno.h>
#include "md5.h"


gchar *wqq_login_encrypt(const gchar * uin, const gchar * plain,
						 const gchar * verify_code);

const gchar *getHash(guint64 uin, const gchar * ptwebqq);

#endif
