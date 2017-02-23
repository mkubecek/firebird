/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		wnet_proto.h
 *	DESCRIPTION:	Prototpe header file for wnet.cpp
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
 */

#ifndef REMOTE_WNET_PROTO_H
#define REMOTE_WNET_PROTO_H

#include "../common/classes/fb_string.h"

RemPort*	WNET_analyze(ClntAuthBlock*, const Firebird::PathName&, const TEXT*, bool,
	Firebird::RefPtr<const Config>*, const Firebird::PathName*);
RemPort*	WNET_connect(const TEXT*, struct packet*, USHORT, Firebird::RefPtr<const Config>*);
RemPort*	WNET_reconnect(HANDLE);


class WnetInitializer
{
protected:
	WnetInitializer();
};

class WnetRemPort : protected WnetInitializer, public RemPort
{
public:
	WnetRemPort(RemPort* parent);

	virtual bool		accept(const p_cnct* cnct);
	virtual void		disconnect();
	virtual void		force_close();
	virtual RemPort*	receive(PACKET* packet);
	virtual XDR_INT		send(PACKET* packet);
	virtual XDR_INT		send_partial(PACKET* packet);
};

#endif // REMOTE_WNET_PROTO_H
