2024, Sophomore 2nd semester Advanced Data Structure Project
20233100 김준영
---
Known Bug
1. Home, End key malfunction : Can outbound gap buffer or might not be in the exact location
2. Gap buffer Cursor <-> Screen Cursor position doesn't match in some case
3. Scrolling malfunction but anyway it works :)
4. Screen stuttering: when open 200mb there's fatal error, stuttering :(
5. When open files and insert any character at first, first origin character can be changed

TODO
1. Fix Memory leaks, overhead
2. Fix malfunctions
3. Optimization
4. Implement Search feature
---  
Project Initiation date : 2024/09/02
---  
Tested on
1. Windows 11, gcc (MinGW32 GCC-6.3.0-1) 6.3.0  // Has potential for bugs due to using 32bit. anyway i used it :(
2. Windows 11, Visual Studio 2022 MSVC 17.0
---  
Features
5. Make a new file
6. Open exist file
3. Status Bar
7. Message Bar
8. Text Edit
9. Move             // WIP
10. Save
8. Search           // TODO
11. Exit