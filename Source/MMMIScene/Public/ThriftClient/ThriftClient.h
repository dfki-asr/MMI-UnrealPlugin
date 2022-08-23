// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Andreas Kaiser, Niclas Delfs, Stephan Adam

/*
	Template class which represents a thrift client
*/

#ifndef _THRIFT_CLIENT_H_
#define _THRIFT_CLIENT_H_

#pragma once

#include <thrift/transport/TTransportUtils.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include "Utils/Logger.h"


using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace std;

namespace MMIStandard {
	template <class T>
	class ThriftClient
	{
	public:

		//	The access of the client
		shared_ptr<T> access;

	private:

		//	The port to connect
		int port;

		// The IP address to connect
		std::string address;

		//	The transport of the client
		shared_ptr<TTransport> transport;

	public:

		//	Basic constructor
		ThriftClient(string const &address, int port, bool autoOpen = true);


		//	Basic destructor closes the connection
		~ThriftClient();



		//	Starts the client and opens the connection
		void Start();

	};

    template <class T>
    ThriftClient<T>::ThriftClient( string const& address, int port, bool autoOpen )
        : address( address ), port( port )
    {
        shared_ptr<TTransport> socket( new TSocket( this->address, this->port ) );
        this->transport = shared_ptr<TTransport>( new TBufferedTransport( socket ) );
        shared_ptr<TProtocol> protocol( new TCompactProtocol{ transport } );
        this->access = shared_ptr<T>( new T{ protocol } );

        if( autoOpen )
            this->Start();
    }

    template <class T>
    ThriftClient<T>::~ThriftClient()
    {
        this->transport->close();
    }

    template <class T>
    void ThriftClient<T>::Start()
    {
        try
        {
            if( !this->transport->isOpen() )
                this->transport->open();
        }
        catch( exception e )
        {
            // Logger::printLog(L_ERROR, boost::current_exception_diagnostic_information());
            Logger::printLog( L_ERROR, e.what() );  // TODO: define error message
        }
    }
}





#endif

