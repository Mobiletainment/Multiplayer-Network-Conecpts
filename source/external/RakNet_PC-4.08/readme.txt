RakNet 4.08
Copyright 2002-2005 Kevin Jenkins (rakkar@jenkinssoftware.com).
This API and the code herein created by and wholly and privately owned by Kevin Jenkins except where specifically indicated otherwise.
Licensed under the "RakNet" brand by Jenkins Software and subject to the terms of the relevant licensing agreement available at http://www.jenkinssoftware.com

------------------------------------------
C# support
------------------------------------------

See Help\swigtutorial.html

------------------------------------------
Upgrading from version 3
------------------------------------------

See 3.x_to_4.x_upgrade.txt

------------- Building Static Libraries for C++ -----------------

1. Open the solution file, RakNet_vc8.sln
2. Right click "LibStatic"
3. Click build.
4. The binary will be under the /Lib directory.
If you want to use source (recommended), just include all header and cpp files under /Source
See Help/compilersetup.html or go to the forums http://www.jenkinssoftware.com/raknet/forum/ if you need to

------------------------------------------
XBOX and PS3 users
-----------------------------------------
Contact us at sales@jenkinssoftware.com to request an evaluation.

------------------------------------------
Windows users (Visual Studio 2008 and 2010)
-----------------------------------------
Load RakNet_VS2008.sln and convert if necessary.
If it doesn't work, see Help/compilersetup.html or the training video at http://www.jenkinssoftware.com/raknet/manual/helloworldvideo.html on how to setup the project.

------------------------------------------
Windows users (Visual Studio 2005)
-----------------------------------------
Load RakNet_VS2005.sln

-----------------------------------------
Windows users (.NET 2003)
-----------------------------------------
Load RakNet_VS2003.sln

-----------------------------------------
Windows users (VC6)
-----------------------------------------
Not supported, but you can get by if you directly include the source.

-----------------------------------------
CYGWIN users
-----------------------------------------
Copy Include, Source, and whatever you want to run in the home directory.  Then type
g++ ../../lib/w32api/libws2_32.a *.cpp
You can run a.exe
You might have to copy *.dll from cygwin\bin as well.

-----------------------------------------
Linux users
-----------------------------------------
Use cmake, or "g++ -lpthread -g *.cpp" in the /Source directory.
With libcat, use "g++ -pthread -g -I./../DependentExtensions *.cpp" in the /Source directory.

64 bit use -m64 command line
Sometimes you need -pthread instead of -lpthread

Command to build 64 bit chat example server from the /Source directory:
g++ -m64 -g -lpthread -I./ "../Samples/Chat Example/Chat Example Server.cpp" *.cpp

Command to build NATCompleteServer from the Samples/NATCompleteServer directory:
g++ -g -lpthread -I./ -I./../../Source main.cpp -I./../CloudServer ./../../Source/*.cpp ./../CloudServer/CloudServerHelper.cpp

Command to build autopatcher server from /Source directory
g++ -lpthread -lpq -lssl -lbz2 -lssl -lcrypto -L/opt/PostgreSQL/9.0/lib -L../DependentExtensions/bzip2-1.0.6 -I/opt/PostgreSQL/9.0/include -I../DependentExtensions/bzip2-1.0.6 -I./ -I../DependentExtensions/Autopatcher -I../DependentExtensions/Autopatcher/AutopatcherPostgreRepository -I../DependentExtensions/PostgreSQLInterface -g *.cpp ../DependentExtensions/Autopatcher/AutopatcherServer.cpp ../DependentExtensions/Autopatcher/CreatePatch.cpp ../DependentExtensions/Autopatcher/MemoryCompressor.cpp ../DependentExtensions/Autopatcher/AutopatcherPostgreRepository/AutopatcherPostgreRepository.cpp ../DependentExtensions/PostgreSQLInterface/PostgreSQLInterface.cpp ../Samples/AutopatcherServer/AutopatcherServerTest.cpp

Command to build NATCompleteServer from /Source directory
g++ -lpthread -I./ -I../Samples/CloudServer ../Samples/CloudServer/CloudServerHelper.cpp ../Samples/NATCompleteServer/main.cpp *.cpp

Command to build BigPacketTest from /Source directory
g++ -lpthread -I./ ../Samples/BigPacketTest/BigPacketTest.cpp *.cpp
Or with debugging info on
g++ -g -lpthread -I./ ../Samples/BigPacketTest/BigPacketTest.cpp *.cpp

If you get /usr/local/lib/libraknet.so: undefined reference to `__sync_fetch_and_add_4 then build with -march=i686 

To debug:
http://www.unknownroad.com/rtfm/gdbtut/gdbstack.html
http://cs.baylor.edu/~donahoo/tools/gdb/tutorial.html
http://linux.bytesex.org/gdb.html
http://www.delorie.com/gnu/docs/gdb/gdb_29.html
gdb ./a.out
Set breakpoint:
b file:line
disable to disable a breakpoint, delete to delete it
info breakpoints to get a list of breakpoints
ignore <breakpointNumber> <count> to set breakpoint to be ignored that number of times
followed by run
Useful:
info stack
info locals
delete (Clears all breakpoints)
step (step into)
next (step over)
finish (step out)
continue to keep going after step or next
p <variableName>
For example: p users.orderedList.listArray[0].guid

Command to install g++
sudo apt-get install gcc-c++
sudo apt-get install build-essential
Or:
yum install gcc-c++
Or:
sudo apt-get update
sudo apt-get install g++

Command to install gdb
sudo apt-get install gdb

Command to install wget, used to download files from webpages
sudo apt-get install wget
wget http://www.raknet.com/raknet/downloads/RakNet_PC-4.068.zip

Text editor from console:
pico

Command to checkout RakNet
svn checkout <URL>

Series of commands for a new server:
1. sudo apt-get install wget
2. sudo apt-get update
3. sudo apt-get install --fix-missing g++
4. sudo apt-get install gdb
5. wget http://www.raknet.com/raknet/downloads/RakNet_PC-4.068.zip
6. sudo apt-get install unzip
7. unzip RakNet_PC-4.068.zip
8. cd Source
9. g++ -m64 -g -pthread -I./ "../Samples/Chat Example/Chat Example Server.cpp" *.cpp
10. ./a.out

-----------------------------------------
DevCPP Users
-----------------------------------------
Load RakNet.dev

-----------------------------------------
CodeBlocks Users
-----------------------------------------
Load RakNet.cbp

-----------------------------------------
Mac Users
-----------------------------------------
From http://www.jenkinssoftware.com/raknet/forum/index.php?topic=746.0;topicseen
Open a Terminal window and type: cd ~/Desktop/RakNet/Source

Give the following command:

Code:
g++ -c -DNDEBUG -I -isysroot /Developer/SDKs/MacOSX10.5u.sdk/ -arch i386 *.cpp 
Use whichver SDK you have. However, the 10.4 SDK is bugged and will not compile unless you use GCC 4.0 from inside XCODE

The sources should build cleanly. This gives you a bunch of PowerPC binaries, compiled against the 10.3.9 SDK which is a good thing.

Give the following command:

Code:
libtool -static -o raknetppc.a *.o

This will stitch together a static library for the PowerPC architecture. There may be warnings that some .o files do not have any symbols. If you want to be prudent, remove the named files (the .o files, not the .cpp files!) and re-run the libtool command.

Now, we build the source files for Intel:

Code:
gcc -c -I ../Include -isysroot /Developer/SDKs/MacOSX10.4u.sdk/ -arch i386 *.cpp

..and stitch it into a i386 library:

Code:
libtool -static -o rakneti386.a *.o

Now, type:

Code:
ls *.a

which should list the two .a files. Now, we make them into a universal binary:

Code:
lipo -create *.a -o libraknet.a

You now have a file named libraknet.a. This is the RakNet library, built to run on both PowerPC and Intel Macs. Enjoy! ;-)

-----------------------------------------
IPod
-----------------------------------------
Depending on what version you target, you may have to change two defines to not use 64 bit integers and floats or doubles. See 
See http://www.jenkinssoftware.com/forum/index.php?topic=2717.0

-----------------------------------------
Android
-----------------------------------------

You will need the latest CYWGIN and the android NDK to build native code on the android. Under CYWGIN, you will need to run ndk-build on a directory for RakNet.

1. Under cygwin, create the RakNet directory somewhere, such as under samples.
For example, if you create the path \cygwin\home\Kevin\android-ndk-r4b\samples\RakNet

2. I copied the Android.Manifest.xml and other files from another sample

3. Under jni, you will need the following Android.mk

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := RakNet
MY_PREFIX := $(LOCAL_PATH)/RakNetSources/
MY_SOURCES := $(wildcard $(MY_PREFIX)*.cpp)
LOCAL_SRC_FILES += $(MY_SOURCES:$(MY_PREFIX)%=RakNetSources/%) 
include $(BUILD_SHARED_LIBRARY)

This version of Android.mk assumes there is a directory called RakNetSources, for example 
cygwin/home/Kevin/android-ndk-r4b/samples/RakNet/jni/RakNetSources

Under RakNetSources should be the /Source directory to RakNet. Rather than copy the files I used junction.exe 
http://technet.microsoft.com/en-us/sysinternals/bb896768.aspx

The command I used to create the junction was:
D:/cygwin/home/Kevin/android-ndk-r4b/samples/RakNet/jni/junction.exe -s D:/cygwin/home/Kevin/android-ndk-r4b/samples/RakNet/jni/RakNetSources D:/RakNet4/Source

To unjunction I used:
D:/cygwin/home/Kevin/android-ndk-r4b/samples/RakNet/jni/junction.exe -d D:/cygwin/home/Kevin/android-ndk-r4b/samples/RakNet/jni/RakNetSources

From within the CYWGIN enviroment, navigate to home/Kevin/android-ndk-r4b/samples/RakNet. Then type
../../ndk-build

Everything should build and you should end up with a .so file.

You should then be able to create a project in eclipse, and import cygwin/home/Kevin/android-ndk-r4b/samples/RakNet

-----------------------------------------
Native client
-----------------------------------------
Solution: RakNet_NativeClient_VS2010. See Samples\nacl_sdk\RakNet_NativeClient_VS2010\HowToSetup.txt for detailed instructions on setup.

-----------------------------------------
Windows Phone 8
-----------------------------------------
Example solution: RakNet_WinPhone8_VS2012.
Add to your project DependentExtensions\WinPhone8\ThreadEmulation.cpp
Add DependentExtensions\WinPhone8\ to your include paths
Define _CRT_SECURE_NO_WARNINGS and WINDOWS_PHONE_8

-----------------------------------------
Windows Store 8
-----------------------------------------
Example solution: RakNet_WindowsStore8_VS2012.sln
Add to your project DependentExtensions\WinPhone8\ThreadEmulation.cpp
Add DependentExtensions\WinPhone8\ and DependentExtensions\WinRT to your include paths 
TCP is not supported, only UDP (RakPeer).
IPV4 only (not hard to also add IPV6 upon request).
Define:
_CRT_SECURE_NO_WARNINGS
WINDOWS_STORE_RT
_RAKNET_SUPPORT_TCPInterface=0
_RAKNET_SUPPORT_PacketizedTCP=0
_RAKNET_SUPPORT_EmailSender=0
_RAKNET_SUPPORT_HTTPConnection=0
_RAKNET_SUPPORT_HTTPConnection2=0
_RAKNET_SUPPORT_TelnetTransport=0
_RAKNET_SUPPORT_NatTypeDetectionServer=0
_RAKNET_SUPPORT_UDPProxyServer=0
_RAKNET_SUPPORT_UDPProxyCoordinator=0
_RAKNET_SUPPORT_UDPForwarder=0

-----------------------------------------
Consoles
-----------------------------------------
Contact rakkar@jenkinssoftware.com for code with Console support.

-----------------------------------------
Unreal engine
-----------------------------------------
See https://udn.epicgames.com/lists/showpost.php?list=unprog3&id=37697&lessthan=&show=20

-----------------------------------------
Package notes
-----------------------------------------
The Source directory contain all files required for the core of Raknet and is used if you want to use the source in your program or create your own dll
The Samples directory contains code samples and one game using an older version of Raknet.  The code samples each demonstrate one feature of Raknet.  The game samples cover several features.
The lib directory contains libs for debug and release versions of RakNet and RakVoice
The Help directory contains index.html, which is full help documentation in HTML format
There is a make file for linux users in the root directory.  Windows users can use projects under Samples\Project Samples

For support please visit
http://www.jenkinssoftware.com
