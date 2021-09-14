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

#include "RMImuReader.h"
#include <sstream>

using namespace winrt::Windows::Perception;
using namespace winrt::Windows::Perception::Spatial;
using namespace winrt::Windows::Foundation::Numerics;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
void RMImuReader::ImuUpdateLoop()
{

    winrt::check_hresult(m_pAccelSensor->OpenStream());
    winrt::check_hresult(m_pGyroSensor->OpenStream());
    winrt::check_hresult(m_pMagSensor->OpenStream());
    IResearchModeSensorFrame* pSensorAFrame = nullptr;
    IResearchModeSensorFrame* pSensorGFrame = nullptr;
    IResearchModeSensorFrame* pSensorMFrame = nullptr;
    IResearchModeAccelFrame* pAccelFrame = nullptr;
    IResearchModeGyroFrame* pGyroFrame = nullptr;
    IResearchModeMagFrame* pMagFrame = nullptr;
    ResearchModeSensorTimestamp timeStampA;
    ResearchModeSensorTimestamp timeStampG;
    ResearchModeSensorTimestamp timeStampM;
    
    ImuSensorBind stream;
    //wchar_t bitmapPath[MAX_PATH];
    byte* byteStream;


    while (!m_fExit)
    {
        if (!StopSending)
        {


            winrt::check_hresult(m_pAccelSensor->GetNextBuffer(&pSensorAFrame));
            winrt::check_hresult(m_pGyroSensor->GetNextBuffer(&pSensorGFrame));
            winrt::check_hresult(m_pMagSensor->GetNextBuffer(&pSensorMFrame));
            pSensorAFrame->GetTimeStamp(&timeStampA);
            pSensorGFrame->GetTimeStamp(&timeStampG);
            pSensorMFrame->GetTimeStamp(&timeStampM);


            winrt::check_hresult(pSensorAFrame->QueryInterface(IID_PPV_ARGS(&pAccelFrame)));
            winrt::check_hresult(pSensorGFrame->QueryInterface(IID_PPV_ARGS(&pGyroFrame)));
            winrt::check_hresult(pSensorMFrame->QueryInterface(IID_PPV_ARGS(&pMagFrame)));

            std::lock_guard<std::mutex> guard(m_sampleMutex);

            winrt::check_hresult(pAccelFrame->GetCalibratedAccelaration(&m_accelSample));
            winrt::check_hresult(pGyroFrame->GetCalibratedGyro(&m_gyroSample));
            winrt::check_hresult(pMagFrame->GetMagnetometer(&m_magSample));

            if (pAccelFrame)
            {
                pAccelFrame->Release();
            }

            if (pGyroFrame)
            {
                pGyroFrame->Release();
            }

            if (pMagFrame)
            {
                pMagFrame->Release();
            }
            if (pSensorAFrame)
            {
                pSensorAFrame->Release();
            }

            if (pSensorGFrame)
            {
                pSensorGFrame->Release();
            }

            if (pSensorMFrame)
            {
                pSensorMFrame->Release();
            }

            stream.Ax = m_accelSample.x; stream.Ay = m_accelSample.y; stream.Az = m_accelSample.z;
            stream.Gx = m_gyroSample.x; stream.Gy = m_gyroSample.y; stream.Gz = m_gyroSample.z;
            stream.MagX = m_magSample.x; stream.MagY = m_magSample.y; stream.MagZ = m_magSample.z;
            stream.DeviceTime = timeStampA.HostTicks;


            //swprintf_s(bitmapPath, L"Accel: timestamp - %lld, % 3.4f, % 3.4f, % 3.4f\n \
            //    Gyro: timestamp - % lld, % 3.4f, % 3.4f, % 3.4f\n   \
            //    Mag : timestamp - % lld, % 3.4f, % 3.4f, % 3.4f\n", 
            //    timeStampA.HostTicks, m_accelSample.x, m_accelSample.y, m_accelSample.z,
            //    timeStampG.HostTicks, m_gyroSample.x, m_gyroSample.y, m_gyroSample.z,
            //    timeStampM.HostTicks, m_magSample.x, m_magSample.y, m_magSample.z
            //    );
            //hololens2Sock.SendMsg(bitmapPath);
            byteStream = reinterpret_cast<byte*>(&stream);

            hololens2Sock.SendBytes(byteStream, SSLENGTH);            
        }
    }

    winrt::check_hresult(m_pAccelSensor->CloseStream());
    winrt::check_hresult(m_pGyroSensor->CloseStream());
    winrt::check_hresult(m_pMagSensor->CloseStream());
}

void RMImuReader::ImuUpdateThread(RMImuReader* pReader, HANDLE hasData, ResearchModeSensorConsent* pCamAccessConsent)
{
    HRESULT hr = S_OK;

    if (hasData != nullptr)
    {
        DWORD waitResult = WaitForSingleObject(hasData, INFINITE);

        if (waitResult == WAIT_OBJECT_0)
        {
            switch (*pCamAccessConsent)
            {
            case ResearchModeSensorConsent::Allowed:
                OutputDebugString(L"Access is granted");
                break;
            case ResearchModeSensorConsent::DeniedBySystem:
                OutputDebugString(L"Access is denied by the system");
                hr = E_ACCESSDENIED;
                break;
            case ResearchModeSensorConsent::DeniedByUser:
                OutputDebugString(L"Access is denied by the user");
                hr = E_ACCESSDENIED;
                break;
            case ResearchModeSensorConsent::NotDeclaredByApp:
                OutputDebugString(L"Capability is not declared in the app manifest");
                hr = E_ACCESSDENIED;
                break;
            case ResearchModeSensorConsent::UserPromptRequired:
                OutputDebugString(L"Capability user prompt required");
                hr = E_ACCESSDENIED;
                break;
            default:
                OutputDebugString(L"Access is denied by the system");
                hr = E_ACCESSDENIED;
                break;
            }
        }
        else
        {
            hr = E_UNEXPECTED;
        }
    }

    if (FAILED(hr))
    {
        return;
    }

    pReader->ImuUpdateLoop();
}

