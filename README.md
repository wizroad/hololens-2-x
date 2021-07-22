# hololens-2-x
## HttpServer
### Requirements
>- flask
>  - opencv-python
>  - numpy
### Set server IP and Port Number
line 42 in app.py 
``` Python
app.run(host='192.168.0.3', port=5000)
```

---------------------
## Hololens2StreamClient
> This project is based on https://github.com/microsoft/HoloLens2ForCV/
> Currently, only RGB(PV) stream is supported. Other research mode sensor stream will be updated soon.
### Preparation
+ For Windows 10 Desktop
   > https://docs.microsoft.com/ko-kr/windows/mixed-reality/develop/install-the-tools?tabs=unity
   - Install Visual Studio 2019
   - Enable developer mode on your PC at Settings > Update & Security > For developers.
+ For Hololens2
     > https://docs.microsoft.com/en-us/windows/mixed-reality/develop/platform-capabilities-and-apis/using-the-windows-device-portal
     > https://docs.microsoft.com/en-us/windows/mixed-reality/develop/platform-capabilities-and-apis/research-mode
   - windows update (build 19041.1364.VB_RELEASE_SVC_SYDNEY.200807-1600 or newer)
   - Enabling Research Mode throuh the Device Portal
      - Settings > Update & Security > For developers > Developer Mode On & Device Portal On
      - Connect Hololens2 to Desktop with USB
      - Device portal from Desktop browser (https://127.0.0.1:10080)]
        - Set username and password
        - System > Recording mode, Research mode > Allow access to sensor streams check
+ For the project
  - Open the project with Visual Studio.
  - Install NuGet package "rmt_curl" from Referece > Manage NuGet packages.
  - Set the server IP and Port number at line 33 in CurlHttp.h
    ``` C++
    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.0.3:5000/Store");
    ```
### Bulid the project the first time
> https://docs.microsoft.com/en-us/windows/mixed-reality/develop/platform-capabilities-and-apis/using-visual-studio?tabs=hl2
 - Connect Hololens2 to Desktop with USB
 - Hololens2
   - Settings > Update & Security > For developers > Device discovery > Pair
   - Leave the Settings app at the PIN popup until you enter the PIN into Visual Studio during your first deployment.
 - Desktop
   - Open the project with Visual Studio.
   - Choose the ARM64 configuration option and select Device in the deployment target drop-down menu.
   - Start Debugging, you'll be prompted for a PIN.

### Start the server and hololens2 app - Hololens2StreamClient
  - Server
    ``` Python
    > python app.py
    
     * Running on http://192.168.0.3:5000/ (Press CTRL+C to quit)
     * Serving Flask app 'app' (lazy loading)
     * Environment: production
       WARNING: This is a development server. Do not use it in a production deployment.
       Use a production WSGI server instead.
     * Debug mode: off
    ```
  - Hololens2StreamClient
    - Select yes for permission request
    - Press the Start button.
    
    

