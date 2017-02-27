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

class InetInitializer
{
protected:
	InetInitializer();
};

struct addrinfo;
class Select;
class InetRemPort;

typedef Firebird::RefPtr<InetRemPort> InetRemPortPtr;

class InetRemPort : protected InetInitializer, public RemPort
{
protected:
	SOCKET				port_handle;		// handle for INET socket
	SOCKET				port_channel;		// handle for connection (from by OS)
	struct linger		port_linger;		// linger value as defined by SO_LINGER

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
	virtual bool		packet_send(const SCHAR* buffer, SSHORT buffer_length);
	virtual bool		packet_receive(UCHAR* p, SSHORT bufSize, SSHORT* length);

	// public interface
	static InetRemPort*	analyze(ClntAuthBlock* cBlock, const Firebird::PathName& file_name,
								const TEXT* node_name, bool uv_flag, Firebird::ClumpletReader &dpb,
								Firebird::RefPtr<const Config>* config, const Firebird::PathName* ref_db_name,
								Firebird::ICryptKeyCallback* cryptCb, int af = AF_UNSPEC);
	static InetRemPort*	connect(const TEXT* name, PACKET* packet, USHORT flag, Firebird::ClumpletReader* dpb,
								Firebird::RefPtr<const Config>* config, int af = AF_UNSPEC);
	static InetRemPort* reconnect(SOCKET sock);
	static InetRemPort*	server(SOCKET sock);

	static bool_t		putbytes(XDR* xdrs, const SCHAR* buff, u_int count);
	static bool_t		getbytes(XDR* xdrs, SCHAR* buff, u_int count);
	static bool			write(XDR* xdrs);
	static bool			read(XDR* xdrs);

	// these should be protected eventually
	void				error(bool releasePort, const TEXT* function, ISC_STATUS operation, int status);

protected:
	static InetRemPort*	allocPort(RemPort* parent = NULL, USHORT flags = 0);
	static InetRemPort*	try_connect(PACKET* packet, Rdb* rdb, const Firebird::PathName& file_name,
									const TEXT* node_name, Firebird::ClumpletReader& dpb,
									Firebird::RefPtr<const Config>* config, const Firebird::PathName* ref_db_name,
									int af);
	static bool			setFastLoopbackOption(SOCKET s);

	void				getPeerInfo();
	bool				setNoNagleOption();
	void				genError(bool releasePort, const Firebird::Arg::StatusVector& v);
	InetRemPort*		listener_socket(USHORT flag, const addrinfo* pai);
	InetRemPort*		select_accept();
	void				select_port(Select* selct, InetRemPortPtr& port);
	bool				select_wait(Select* selct);

	InetRemPort*		next() { return (InetRemPort*) port_next; };

	friend class InetInitializer;			// needs allocPort()
	friend class Select;					// needs port_handle
};

#endif // REMOTE_INET_PROTO_H
