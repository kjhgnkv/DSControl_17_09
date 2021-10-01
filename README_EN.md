# On a fluid implementation, what you need:

# Samples ZIP

> Build DSHost.
>> - About how to collect, it is written in DSHost README.
>
> Next, we create the Samples folder. And create sub-folders:
>> - cache
>> - components
>> - config
>> - exe
>
> In the "cache" folder, you need to add the libs:
>> - libDSUtils.[so|dll]
>
> > In the folder "exe" we throw:
>> - dshost[.exe]
>
> Then we make an archive:
>> - Samples.zip
>
> ### In the bin folder, create a Samples folder, create a Linux WinNT folder, and drop Samples.zip in accordance with the OS.

# macchina.io

> Location Path macchina, in:
> ### Linux
>> /usr/local/macchina
> ### Windows
>> C:/macchina

# ERROR

> ### If DSHost throws an error under Windows:
>> MqttClient:cann't connect to: [ip], error: 14
>
> In file "mosquitto.conf" to path "C:\Program Files\mosquitto", you need to add 2 lines:
>> - listener 1883
>> - allow_anonymous true
> ### If an error occurs in Linux:
>> error while loading shared libraries: libPocoXMLd.so.64: cannot open shared object file: No such file or directory
>
> Need to perform:
>> sudo ldconfig /usr/local/macchina/lib/
> ### If you run DSConstrol under Windows, it throws an error:
>> System Error: The program can't find the Qt library "qt[Name].dll"
>
> Run the command in the terminal:
>> $ C:\\Qt\\5.15.2\\msvc2019_64\\bin\\windeployqt.exe DSControl.exe
>
> ### or
>> System Error: The program can't find the Poco library "Poco[Name].dll"
>
> You need to copy from the folder C:\\macchina\\bin all *.dll, and throw everything in the bin folder.

## Deploy

> To make a deployment, you need to:
>> - Create a bndl folder in the bin folder
>> - We transfer there bundles *_1.0.0. bndl, which were collected on the DSHost side
>
> We throw the project, click the deploy, select the component & select the ip address
>
> And click Deploy
>
> In the Monitoring widget, the active machine and the application_ will be displayed