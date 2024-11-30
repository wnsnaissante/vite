2024, Sophomore 2nd semester Advanced Data Structure Project
20233100 김준영
---
Known Bug
1. Home, End key malfunction : Can outbound gap buffer or might not be in the exact location
2. Gap buffer Cursor <-> Screen Cursor position doesn't match in some case
3. Scrolling malfunction but anyway it works :)
4. Screen stuttering
5. When open files and insert any character at first, first origin character can be changed
6. Can't handle well big size file, Test on 200mb file :(
7. Print garbage character
8. MOTD screen doesn't print when program started.
9. if insert character after gap moves to left, total lines doesn't match
---
!TODO
1. Fix Memory leaks, overhead
2. Fix malfunctions
3. Optimization
4. Implement Search feature
5. Implement Up, down Arrow key
6. Fix,Implement MOTD(Init) screen
7. Fix Home, End key malfunction
---  
Project Initiation date : 2024/09/02
---  
Tested on
1. Windows 11, gcc (MinGW32 GCC-6.3.0-1) 6.3.0  // Has potential for bugs due to using 32bit. anyway i used it lol XD
2. Windows 11, Visual Studio 2022 MSVC 17.0
3. VM macOS Sonoma (version 14.5), Apple clang version 15.0.0 (clang-1500.3.9.4) Target: x86_64-apple-darwin23.5.0 POSIX
4. WSL Ubuntu 22.04.3 LTS (GNU/Linux 5.15.167.4-microsoft-standard-WSL2 x86_64), gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04), Target: x86_64-linux-gnu, POSIX
---  
Features
1. Make a new file
2. Open exist file
3. Status Bar
4. Message Bar
5. Text Edit
6. Move             // WIP
7. Save
8. Search           // WIP
9. Exit