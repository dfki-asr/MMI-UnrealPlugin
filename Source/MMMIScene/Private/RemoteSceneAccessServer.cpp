// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Stephan Adam

// Thrift server that provides the remote access to the Unreal scene. Required by Ajan.
// The methods answerign the client requests are defined in the UnrealSceneAccessServer class,
// which redirects them to the UnrealSceneAccess.

#include "RemoteSceneAccessServer.h"
#include "MMUAccess.h"
#include "UnrealSceneAccessServer.h"
#include "ThriftClient/ThriftClient.h"
#include <iostream>
#include <future>
#include <thread>
#include <chrono>

using namespace MMIStandard;

namespace SceneAccessServer
{
RemoteSceneAccessServer::RemoteSceneAccessServer( string sessionID,
    shared_ptr<UnrealSceneAccessServer> implementation )
    : description( nullptr ), listenAddress( nullptr )
{
    description = new MServiceDescription();
    description->__set_ID( MMUAccess::GetNewGuid() );
    description->__set_Name( "remoteSceneAccessUnreal" );
    description->__set_Language( "C++" );
    //description->__set_Addresses( vector<MIPAddress>{*address} );

    //listenAddress = address;
    handler = implementation;
    isRunning = false;
    _sessionID = sessionID;

}

RemoteSceneAccessServer::~RemoteSceneAccessServer()
{
    if( this->description )
        delete this->description;

    try
    {
        if( this->isRunning )
            this->server->stop();
        delete this->server;
    }
    catch( ... )
    {
    }
}

void RemoteSceneAccessServer::Start()
{
    int counter = 0;
    while( !this->isRunning && counter < 100)
    {

        auto proc = make_shared<MSceneAccessProcessor>( this->handler );
        auto trans_svr = make_shared<TServerSocket>( MMISettings::GetInstance()->MIPRemoteSceneAccessAddress.Port );
        auto trans_fac = make_shared<TBufferedTransportFactory>();
        auto proto_fac = make_shared<TCompactProtocolFactory>();
        // auto proto_fac = make_shared<TBinaryProtocolFactory>();

        // threadmanager for reusing threads
        std::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager();
        threadManager->threadFactory( std::make_shared<ThreadFactory>() );
        threadManager->start();

        // thread pool server
        this->server =
            new TThreadPoolServer( proc, trans_svr, trans_fac, proto_fac, threadManager );
        // simple server -> for debugging
        // this->server = new TSimpleServer(proc, trans_svr, trans_fac, proto_fac);


        std::promise<bool> p;
        auto future = p.get_future();

        auto serv = this->server;
        std::thread th(
            [serv, &p]
            {
                try
                {
                    serv->serve();
                }
                catch( const std::exception& e )
                {
                    // logging required
                    std::cout << "increasing remoce scene access port: " << e.what() << endl;
                }
                p.set_value( true );
            } );
        //std::this_thread::sleep_for(500ms);
      
        auto status = future.wait_for( 200ms );
        if( status == std::future_status::ready )
        {
            th.join();
            MMISettings::GetInstance()->MIPRemoteSceneAccessAddress.Port++;
            counter++;
        }
        else
        {
            this->isRunning = true;

            // register service
            this->listenAddress = new MIPAddress();
            this->listenAddress->Address = ( string ) "127.0.0.1";
            this->listenAddress->Port =
                MMISettings::GetInstance()->MIPRemoteSceneAccessAddress.Port;
            this->description->Addresses.clear();
            this->description->Addresses.push_back( *( this->listenAddress ) );
            auto regClient( new ThriftClient<MMIRegisterServiceClient>(
                MMISettings::GetInstance()->MMIRegisterAddress,
                MMISettings::GetInstance()->MMIRegisterPort, true ) );
            MBoolResponse resp;
            regClient->access->RegisterService( resp, *( this->description ) );

            // block
            th.join();
            return;
        }
    }
    if( counter >= 100 )
    {
        throw std::out_of_range(
            "No suitable Port to open found for the remote scene access server." );
    }
}   

void RemoteSceneAccessServer::Stop()
{
    try
    {
        if( this->server )
        {
            this->server->stop();
            this->isRunning = false;
        }
    }
    catch( ... )
    {
    }
}
}