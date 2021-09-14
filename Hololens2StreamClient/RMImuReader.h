#pragma once
//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include "researchmode\ResearchModeApi.h"
#include "Tar.h"
#include "TimeConverter.h"

#include <mutex>
#include <winrt/Windows.Perception.Spatial.h>
#include <winrt/Windows.Perception.Spatial.Preview.h>

#include "UdpSocket.h"

#define SSLENGTH 64
class RMImuReader
{
public:
    RMImuReader(IResearchModeSensor* pAccelSensor, IResearchModeSensor* pGyroSensor, IResearchModeSensor* pMagSensor, HANDLE hasData, ResearchModeSensorConsent* pCamAccessConsent)
    {
        m_pAccelSensor = pAccelSensor;
        m_pGyroSensor = pGyroSensor;
        m_pMagSensor = pMagSensor;

        m_pAccelSensor->AddRef();
        m_pGyroSensor->AddRef();
        m_pMagSensor->AddRef();

        m_pImuUpdateThread = new std::thread(ImuUpdateThread, this, hasData, pCamAccessConsent);

    }
    virtual ~RMImuReader()
    {
        m_fExit = true;
        m_pImuUpdateThread->join();

        if (m_pAccelSensor)
        {
            m_pAccelSensor->CloseStream();
            m_pAccelSensor->Release();
        }
        if (m_pGyroSensor)
        {
            m_pGyroSensor->CloseStream();
            m_pGyroSensor->Release();
        }
        if (m_pMagSensor)
        {
            m_pMagSensor->CloseStream();
            m_pMagSensor->Release();
        }
    }
    
    bool StopSending = true;
    UdpSocket hololens2Sock;

private:
    static void ImuUpdateThread(RMImuReader* pReader, HANDLE hasData, ResearchModeSensorConsent* pCamAccessConsent);
    void ImuUpdateLoop();

    struct ImuSensorBind
    {
        float MagZ; float MagY; float MagX;
        float DeviceID = 2.f; long long DeviceTime;
        float ROTw = -999.f;        float ROTz = -999.f;        float ROTy = -999.f;        float ROTx = -999.f;
        float Az;        float Ay;        float Ax;
        float Gz;        float Gy;        float Gx;
    };
    
    
    IResearchModeSensor* m_pAccelSensor = nullptr;
    IResearchModeSensor* m_pGyroSensor = nullptr;
    IResearchModeSensor* m_pMagSensor = nullptr;
    IResearchModeSensorFrame* m_pSensorFrame;
    DirectX::XMFLOAT3 m_accelSample;
    DirectX::XMFLOAT3 m_gyroSample;
    DirectX::XMFLOAT3 m_magSample;
    std::mutex m_sampleMutex;

    std::thread* m_pImuUpdateThread;
    bool m_fExit = false ;
};

