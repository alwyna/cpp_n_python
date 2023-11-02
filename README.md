# Say Hello
## Show message passing from python to cpp

Purpose is to prototype a quick hello-world message passing from python to c++

### How build works

```mermaid 
graph LR;
  CMake --> Ninja;
  Ninja --> Install;
```

Steps if you're not using an IDE (assuming you're in the project dir):

```shell
cmake -S . -B cmake-build-debug
cmake --build ./cmake-build-debug --target all -j 14
cmake --install cmake-build-debug
```

### Exporting templates

* Template must be ***fully*** specialized (ie. you cannot have open template)
  * Template must be declared and `externed` note this is reversed for `extern "C"`.
    * In the .h
    * ```c++
      extern template class A<int>;
      ```
    * In the .cpp
    * ```c++
      template
      class A<int>;
      ```

### Calling from python

#### A. Baseline
```python
from ctypes import cdll, create_string_buffer
lib = cdll.LoadLibrary('./cmake-build-debug/libsayhello.so')
buzz = create_string_buffer("buzz!".encode("UTF-8"))
lib.Hello(buzz)
```

expected result

```shell
You passed: buzz!
Hello, buzz!
1
```

#### B. Passing pointers
```python
from ctypes import cdll, create_string_buffer
import ctypes
lib = cdll.LoadLibrary('./cmake-build-debug/libsayhello.so')
lib.bFactory.restype = ctypes.c_void_p              # The next 3 lines are key!
lib.sayB.argtypes = [ctypes.c_void_p]               # Python casts pointers to integers, which blows up
lib.deleteB.argtypes = [ctypes.c_void_p]            # Unless you explicitly tells library what they are
b = lib.bFactory()                                  # Gets a specialized template instance of B (and A)
lib.sayB(b)                                         # Pass it back to native to reflect on its value
lib.deleteB(b)                                      # Clean up
```

Output

> Python 3.8.8 (default, Apr 13 2021, 19:58:26)
> 
> [GCC 7.3.0] :: Anaconda, Inc. on linux
> 
> Type "help", "copyright", "credits" or "license" for more information.
> 
> from ctypes import cdll, create_string_buffer
> 
> import ctypes
> 
> lib = cdll.LoadLibrary('./cmake-build-debug/libsayhello.so')
> 
> lib.bFactory.restype = ctypes.c_void_p              # The next 3 lines are key!
> 
> lib.sayB.argtypes = [ctypes.c_void_p]               # Python casts pointers to integers, which blows up
> 
> lib.deleteB.argtypes = [ctypes.c_void_p]            # Unless you explicitly tells library what they are
> 
> b = lib.bFactory()                                  # Gets a specialized template instance of B (and A)
> 
> lib.sayB(b)                                         # Pass it back to native to reflect on its value
> 
> Say B:0
> 
> 1404320608
> 
> lib.deleteB(b)                                      # Clean up
> 
> 0


### TIPS

* If you see `"It was created with incompatible generator "'Unix Makefiles'`
  * Invalidate all with all the following boxes ticked
    * ![img.png](invalidate-options.png)
* Automating the cmake build
  * When you select `build`
  * You can see that the command is `home/aaswin/.local/share/JetBrains/Toolbox/apps/clion/bin/cmake/linux/x64/bin/cmake --build /home/aaswin/CLionProjects/sayhello/cmake-build-debug --target all -j 14`
* Tutorial on cmake
  * https://www.jetbrains.com/help/clion/quick-cmake-tutorial.html#link-libs
* Installing latest cmake
  * https://askubuntu.com/questions/829310/how-to-upgrade-cmake-in-ubuntu
* Building the cmake file from CLI: https://earthly.dev/blog/ninjabuild-for-faster-build/
* Passing string results from unmanaged code back to calling python function
  * You DON'T - it causes memory leak, unless it's made static which's also botched
    * Even returning an array is problematic as none will be there to clean it up https://stackoverflow.com/questions/14887378/how-to-return-array-from-c-function-to-python-using-ctypes
    * A possibility is to call another extern function to free up the array when done.
  * You reserve the memory from the python side for example: https://stackoverflow.com/questions/45178140/returning-string-from-c-function-with-ctypes-gives-large-int-not-char-pointer
  * Here is how you can pass classes back and forth https://stackoverflow.com/questions/19198872/how-do-i-return-objects-from-a-c-function-with-ctypes
* Callback hint use `#include "python.h"` and `PyEval_CallObject(py_compare_func,arglist);` see https://code.activestate.com/recipes/65210-implementing-c-function-callbacks-using-python/ and https://sceweb.sce.uhcl.edu/helm/WEBPAGE-Python/documentation/python_tutorial/ext/callingPython.html
* Chaining lib dependencies with CMake https://stackoverflow.com/questions/39598323/how-to-properly-link-libraries-with-cmake
