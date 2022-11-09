## WindowFinder
A simple console-based tool which allows the user to search, find, and set foreground windows. The tool is useful when dealing with systems that contain many windows and removes the manual labor involved in searching for the specific window. The searching and setting of the foreground window is fast and efficient.

<p align="center">
  <img src="https://user-images.githubusercontent.com/52585921/200727077-97137ee3-9d14-4783-bf73-239f6ef8ca2f.png?raw=true" alt="Window Finder Image"/>
</p>


## Instructions
To use this application, the user must either compile the project themselves or download and execute the binaries provided(x86 or x64). Below I have provided step-by-step instructions on how to run/use the Window Finder application.

### Step By Step
  * Download Window Finder application from the *Releases* section of this GitHub repository
  * Once the zip folder is downloaded extra the files to a safe place
  * If the user has a 32-bit OS, then run the x86 executable, otherwise run the x64 executable
  * Once the application opens up, then the user must provide a partial or full string of the desired window
  * If the window is found, then it will be set as the foreground. Otherwise, the applications will specify an error
  * The user will then have the option to try another string or to close the application
  

## Project Goal
My initial goal for this project was to create a simple application to find different windows on a system that runs a lot of applications(work-related). This was accomplished in the initial version of this project. My second goal was to optimize this application such that the search is instantaneous and the end user does not experience any delays. This was accomplished in v1.8.0, where both double buffering and multithreading were added.
 
## Help
All the code was written by me with most of my help coming from MSDN, StackOverflow, and CPPReference.
