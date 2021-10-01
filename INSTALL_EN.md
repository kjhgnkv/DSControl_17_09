1. Install mosquitto.
> https://mosquitto.org/download/
1.1. After installing mosquitto, add 2 lines to the file "mosquitto. conf" under the path "C:\Program Files\mosquitto":
> listener 1883
> allow_anonymous true
1.2. After that, rebut the computer.
1.3. It is also recommended to disable the "Firewall".

2. Install Visual Studio Community
> https://visualstudio.microsoft.com/
2.1. Install C++

3. Download macchina
> https://github.com/macchina-io/macchina.io/releases/tag/macchina-0.11.0-release
>> WINDOWS
3.1. Build libraries in the platform folder
    - CppParser
    - CppUnit
    - Foundation
    - JSON
    - Net
    - OSP
    - Util
    - XML
    - Zip
      3.2. We collect libs as a debag and release version
      3.3. Drop all [d].dll files into the bin folder and drop all [d].lib files into the lib folder
      3.4. Drop from all include/Poco folders to the include folder (top level)

>> LINUX
    3.1. Install python 2.7
    3.2. Install OpenSSL
    3.3. Build via make and then run make install