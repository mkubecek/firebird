/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		inet_proto.h
 *	DESCRIPTION:	Prototpe header file for inet.cpp
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

#ifndef REMOTE_INET_PROTO_H
#define REMOTE_INET_PROTO_H

#include "fb_types.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/RefCounted.h"
#include "../common/config/config.h"

namespace Firebird
{
	class ClumpletReader;
}

void			setStopMainThread(FPTR_INT func);

class InetInitializer
{
protected:
	InetInitializer();
};

class InetRemPort : protected InetInitializer, public RemPort
{
public:
	InetRemPort(RemPort* const parent, const USHORT flags);

	virtual bool		accept(const p_cnct* cnct);
	virtual void		disconnect();
	virtual void		force_close();
	virtual RemPort*	receive(PACKET* packet);
	virtual XDR_INT		send(PACKET* packet);
	virtual XDR_INT		send_partial(PACKET* packet);
	virtual RemPort*	aux_connect(PACKET* packet);
	virtual RemPort*	aux_request(PACKET* packet);
	virtual bool		select_multi(UCHAR* buffer, SSHORT bufsize, SSHORT* length, RemPortPtr& port);
	virtual void		abort_aux_connection();

	// public interface
	static InetRemPort*	analyze(ClntAuthBlock* cBlock, const Firebird::PathName& file_name,
								const TEXT* node_name, bool uv_flag, Firebird::ClumpletReader &dpb,
								Firebird::RefPtr<const Config>* config, const Firebird::PathName* ref_db_name,
								Firebird::ICryptKeyCallback* cryptCb, int af = AF_UNSPEC);
	static InetRemPort*	connect(const TEXT* name, PACKET* packet, USHORT flag, Firebird::ClumpletReader* dpb,
								Firebird::RefPtr<const Config>* config, int af = AF_UNSPEC);
	static InetRemPort* reconnect(SOCKET sock);
	static InetRemPort*	server(SOCKET sock);

	// these should be protected eventually
	void				getPeerInfo();
	bool				setNoNagleOption();
	void				error(bool releasePort, const TEXT* function, ISC_STATUS operation, int status);
	void				genError(bool releasePort, const Firebird::Arg::StatusVector& v);

protected:
	static InetRemPort*	try_connect(PACKET* packet, Rdb* rdb, const Firebird::PathName& file_name,
									const TEXT* node_name, Firebird::ClumpletReader& dpb,
									Firebird::RefPtr<const Config>* config, const Firebird::PathName* ref_db_name,
									int af);
};

#endif // REMOTE_INET_PROTO_H
