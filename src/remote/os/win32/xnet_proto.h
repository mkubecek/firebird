/*
 *	PROGRAM:	JRD Remote Interface/Server
 *      MODULE:         xnet_proto.h
 *      DESCRIPTION:    Prototpe header file for xnet.cpp
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 * 2003.05.01 Victor Seryodkin, Dmitry Yemanov: Completed XNET implementation
 */

#ifndef REMOTE_XNET_PROTO_H
#define REMOTE_XNET_PROTO_H

#include "../common/classes/fb_string.h"

#ifdef NO_PORT
#define RemPort void
#endif

RemPort* XNET_analyze(ClntAuthBlock*, const Firebird::PathName&, bool, Firebird::RefPtr<const Config>*,
	const Firebird::PathName*);
RemPort* XNET_connect(struct packet*, USHORT, Firebird::RefPtr<const Config>*);
RemPort* XNET_reconnect(ULONG);

#ifndef NO_PORT
class XnetRemPort : public RemPort
{
public:
	XnetRemPort(RemPort* parent, UCHAR* send_buffer, ULONG send_length,
				UCHAR* receive_buffer, ULONG receive_length);

	virtual bool		accept(const p_cnct* cnct);
	virtual void		disconnect();
	virtual void		force_close();
	virtual RemPort*	receive(PACKET* packet);
	virtual XDR_INT		send(PACKET* packet);
	virtual XDR_INT		send_partial(PACKET* packet);
};
#endif // NO_PORT

#endif // REMOTE_XNET_PROTO_H
