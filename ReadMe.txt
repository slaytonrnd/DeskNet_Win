============
Installation
============

Unzip desknet_win someplace on a local Windows drive
	Referred to as <install_location> below
Browse to <install_location>/desknet_win/Release folder.
Install VeraMono font:
	Windows 7:
		Right click on <install_location>/desknet_win/Release/VeraMono.ttf
		Select "Install" from the right click menu
		Follow the on screen prompts to install
	Windows XP:
		Open the Control Panel ("Start" menu -> "Control Panel")
		Select "Appearance and Themes" under "Pick a catagory"
		Select "Fonts" under "See Also" panel on left
		Select "File" menu -> "Install New Font..." menu option from "Fonts" window
		Browse to <install_location>/desknet_win/Release/ in the "Add Fonts" dialogue
		Select "Bitstream Vera Sans Mono (TrueType)" and press "OK" button
Install data files:
	Windows 7:
		Files will generally go in "C:\Users\<user_name>\Documents\Desknet\" where:
			<user_name> is the user name for the currenly logged in user.
		Note that the "Documents" directory location is customizable in Windows and
		what is listed above is the default location.
	Windows XP:
		Files will generally go in "C:\Users\<user_name>\Documents\Desknet\" where:
			<user_name> is the user name for the currenly logged in user.
		Note that the "Documents" directory location is customizable in Windows and
		what is listed above is the default location.
	Directory structure under Desknet:

		Desknet -
			 |-Answerkeys-
			 |	      |-text1
			 |	      |-text2
			 |	      |-text3
			 |	      |-text4
			 |
			 |-Class1-
			 |	  |-Files-
			 |	  |	  |-filea
			 |	  |	  .
			 |	  |	  .
			 |	  |	  .
			 |	  |	  |-filez
			 |	  |
			 |	  |-Names
			 |	  |	  |-namea
			 |	  |	  .
			 |	  |	  .
			 |	  |	  .
			 |	  |	  |-namez
			 |	  |	  |-teacher
			 .	  |
			 .	  |-Settings-
			 .		     |-addpoints
			 |-Class9-	     |-divpoints
			 |	  |	     |-mulpoints
			 |    (Like Class1)  |-subpoints
			 |		     |-terma
			 |-net_size	     |-termb
			 |-pass		     |-type_line
					     |-win_score
				
Double click dnet.exe in <install_location>/desknet_win/Release folder to launch DeskNet

==================
Build Dependencies
==================
	dnet.exe depends on libfreetype-6.dll, libgcc_s_dw2-1.dll, SDL2.dll, SDL2_ttf.dll, and zlib1.dll
		So far I've been able to get libgcc_s_dw2-1.dll wrapped into the executable
		I am working on getting the other .dll files wrapped inthe the executable

-----------------------
Install MinGW with MSYS
-----------------------
	Using the Graphical User Interface installer is fairly simple
		Download: 		http://sourceforge.net/projects/mingw/files/
		Install Instructions:	http://www.mingw.org/wiki/Getting_Started
	The MinGW install instructions recommend creating a MSYS based shell
		Launching this shell will default to the Window's User's Home directory (~)

------------------
Install SDL2 2.0.3
------------------
	Download SDL2 2.0.3 development library:
		https://www.libsdl.org/release/SDL2-devel-2.0.3-mingw.tar.gz
	gunzip and tar -xvvf the file in "~" using MSYS based shell created during install procedure
	cd to SDL2-2.0.3 directory and run "make native" to install
	Some manual copying of files may be necessary for bin, include, lib, and share directories:
		Need to make sure that files are in subfolders under /mingw/msys/1.0
		Files may also need to be in subfolders under MinGW, but I'm not positive about that.
	
----------------------
Install freetype 2.4.8
----------------------
	Download freetype 2.4.8 freetype release:
		http://download.savannah.gnu.org/releases/freetype/freetype-2.4.8.tar.gz
	gunzip and tar -xvvf the file in "~" using MSYS based shell created during install procedure
	cd to freetype-2.4.8 directory
	Installation directions are in freetype-2.4.8/docs/INSTALL.UNIX
		Some manual copying of files is necessary unless ./configure is run with the "--prefix" option
			Sets the installation path:
			I haven't tried this yet, but believe "./configure --prefix=/mingw/msys/1.0" should work
			Or maybe just "./configure --prefix=/
		If ./configure is not run with --prefix the directory /mingw/msys/1.0/local is created:
			Files need to be copied from from/to subdirectories in /mingw/msys/1.0
			Or /mingw/msys/1.0/local paths need to be added to gcc linker

-----------------------
Install SDL2_ttf 2.0.12
-----------------------
	Download SDL2_ttf 2.0.12 development library:
		https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.12-mingw.tar.gz
	gunzip and tar -xvvf the file in "~" using MSYS based shell created during install procedure
	cd to SDL2_ttf-2.0.12 directory
	Installation directions are in README.txt
	Some manual copying may be necessary, not sure on this one.

==============
Compile dnet.c
==============

----------------------
Compile Flags for SDL2
----------------------
	Run sdl2-config --cflags --libs to show what flags to use for SDL2
		sdl2-config should be in the bin directory

--------------------------
Compile Flags for freetype
--------------------------
	Run freetype-config --cflags --libs to show what flags to use for freetype
		freetype-config should be in the bin directory

---------------------
Build Compile Command
---------------------
	Based on output from previous two sections I ended up with the following command:
		gcc -o ../build/dnet.exe dnet.c -Wall -D _WIN -I/usr/include/SDL2 -Dmain=SDL_main -I/usr/local/include/freetype2 -I/usr/local/include -L/usr/lib -L/usr/local/lib -lmingw32 -lSDL2main -lSDL2 -mwindows -lfreetype -lSDL2_ttf -lws2_32 -Wl,-Bstatic -static-libgcc -O3
	-Wl,-Bstatic is intended to include dll dependencies in the .exe file
		I haven't figured out how to get this to work for SDL2.dll, SDL2_ttf.dll, and libfreetype-6.dll yet, but in theory it should work.
	-static-libgcc includes the libgcc_s_dw2-1.dll dependency in the dnet.exe file