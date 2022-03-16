<!-- PROJECT SPLASH -->
<br />
<div align="center">

  <h3 align="center">SLoAn algorithm</h3>

  <p align="center">
    It's a <b>S</b>olver of <b>Lo</b>ng <b>An</b>agrams
    <br />
    <a href="#getting-started"><strong>How to install »</strong></a>
    <br />
    <br />
    <a href="https://github.com/othneildrew/Best-README-Template/issues">Report bug</a>
    ·
    <a href="https://github.com/othneildrew/Best-README-Template/issues">Request feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<!-- UNNECESSARY
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-sloan">About SLoAn</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
        <ul>
            <li><a href="#windows">Windows</a></li>    
        </ul>
      </ul>
    </li>
  </ol>
</details>
-->



<!-- ABOUT -->
## About SLoAn

This project is in development. In this case that means that there are several planned features yet to be 
implemented and major bugs to be fixed.

This is a command line program that will simply list all possible anagrams that can be made from an input 
sentence. It applies a user-submitted word list, although a default list is provided as well.

An anagram is a word or phrase that is formed by rearranging the letters of a different word or phrase. 
SLoAn uses all the original letters exactly once to arrive at its solutions.

SLoAn is a hobby project. I'm fascinated by the idea that this simple game with one rule becomes 
computationally exponentionally more intensive the more characters are added to the phrase. The challenge 
is to discover a way to drastically reduce the amount of letter rearrangements that have to be processed 
to arrive at an absolute answer. The goal is to deliver *all* possible anagrams, *quick*. 
And then accordingly, the short processing time will allow for *long* inputs to be admissible as well.

SLoAn "learns" by doing. It gets faster processing new unique phrases the more it solved in the past. 
Anytime a little bit of the puzzle is solved, that solution is stored for quick look-up in the future. 
In effect, the algorithm gets increasingly more familiar with its word list the more queries it completes.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- GETTING STARTED -->
<!-- NOTHING OF THIS IS VERIFIED TO WORK!
## Getting Started

### Prerequisites

To build the project, you will need to have the following libraries and software installed:
* <a href="https://cmake.org/download/">CMake</a>
* <a href="https://www.boost.org/users/download/">Boost</a> version 1.78.0+

_Installing the required Boost libraries using Vcpkg package manager._
1. Download and install Vcpkg following <a href="https://vcpkg.io/en/getting-started.html">the instructions</a> at their website.
2. Open a command line in your Vcpkg root folder.
2. Ensure you have the Vcpkg toolchain file added to your CMake.
      ```sh
      cmake .. "-DCMAKE_TOOLCHAIN_FILE=.\scripts\buildsystems\vcpkg.cmake"
      ```
3. Install the prerequisite Boost libraries
      ```sh
      .\vcpkg install boost-filesystem:x64-windows
      ```
      ```sh
      .\vcpkg install boost-chrono:x64-windows
      ```


### Installation

_Building the project can be achieved in a number of ways. Detailed is how to install using the command line._

#### Windows

1. Clone the repo
   ```sh
   git clone https://github.com/CyanMatter/SLoAn.git
   ```
2. Open a command line in the repo's root folder.
3. Build with CMake
   ```sh
   mkdir build
   ```
    ```sh
   cd build
   ```
    ```sh
   cmake -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=Release ..
   ```

#### Unix

1. Clone the repo
   ```sh
   git clone https://github.com/CyanMatter/SLoAn.git
   ```
2. Open a command line in the repo's root folder.
3. Build with CMake
   ```sh
   mkdir build
   ```
    ```sh
   cd build
   ```
   ```sh
   cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
   ```

<p align="right">(<a href="#top">back to top</a>)</p>
-->
