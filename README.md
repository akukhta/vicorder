# Description
Vicorder is C++ written dll for Windows OS to capture the screen. It allows user to select a display to record. Average FPS is equal to 40.

# Tools to build
- Conan 1.5.9
- Cmake

# Compiler
MSVC 19.32+

# Dependencies
- xz_utils 5.4.0
- opencv 4.5.5

# Build steps
		
    mkdir build
    cd build
    conan install ../conanfile.txt --profile ../profile.txt
    cmake .. -B .
    msbuild /m /p:Configuration=BUILD_CONFIGURATION vicorder.sln

# Example

    #include <iostream>
    #include "vicorder/VRBuilderDirector.h"
    #include "vicorder/VideoRecorder.h"

    int main()
    {
      auto recorder = VRBuilderDefDirector().getToAVI(VideoRecorder::getMonitors()[0]);
      recorder->startRecord();
      std::this_thread::sleep_for(std::chrono::seconds(600));
      recorder->stopRecord();
      recorder->save("Output.avi");

      std::cout << "Handled events (i.e. frames captured):\n";
      auto xxx = recorder->getHandledEventCounts();

      for (auto& count : recorder->getHandledEventCounts())
      {
        std::cout << count << "\t";
      }

      std::cout << std::endl;
    }
