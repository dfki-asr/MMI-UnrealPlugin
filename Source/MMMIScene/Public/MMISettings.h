// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Stephan Adam

// Storage for the most important IP and Port settings of the Plugin

#pragma once
#include <string>

#include "Windows\AllowWindowsPlatformTypes.h"
#include "gen-cpp/core_types.h"
#include "Windows\HideWindowsPlatformTypes.h"

using namespace std;
using namespace MMIStandard;

class MMISettings
{
public:
    static MMISettings* GetInstance()
    {
        if( _instance == nullptr )
        {
            _instance = new MMISettings();
        }
        return _instance;
    }
    MMISettings( MMISettings const& ) = delete;
    MMISettings& operator=( MMISettings const& ) = delete;


    /// The port for the access
    int MMIRegisterPort;
    // The address of the MMI Register
    string MMIRegisterAddress;
    // combination as MIPAddress
    MIPAddress MIPRegisterAddress;

    /// The port for the access
    int RemoteSceneAccessPort;
    /// The port for the access
    string RemoteSceneAccessAddress;
    // combination as MIPAddress
    MIPAddress MIPRemoteSceneAccessAddress;

    /// The port for the external write access
    int RemoteSceneWritePort;
    /// The port for the access
    string RemoteSceneWriteAddress;
    // combination as MIPAddress
    MIPAddress MIPRemoteSceneWriteAddress;

    /// Specifies whether the scene is accessible for external clients via thrift server
    bool AllowRemoteSceneConnections;

    /// The port for the skeleton access
    int RemoteSkeletonAccessPort;
    /// The port for the skeleton access
    string RemoteSkeletonAccessAddress;
    // combination as MIPAddress
    MIPAddress MIPRemoteSkeletonAccessAddress;

    /// The port for the skeleton access
    int RemoteCoSimulationAccessPort;
    /// The port for the skeleton access
    string RemoteCoSimulationAccessAddress;
    // combination as MIPAddress
    MIPAddress MIPRemoteCoSimulationAccessAddress;

    /// Specifies whether the skeleton is accessible for external clients via thrift server
    bool AllowRemoteSkeletonConnections;

private:
    MMISettings();
    static MMISettings* _instance;
};
