# jsonParser
C++ console application for parcing of Json

# run tests

1. Open root folder in terminal
2. run `cmake -S . -B build`
3. run `cmake --build build`
4. run `cd build && ctest` or go into build folder and run ctest


# Design Decisions

## virtual base class

Since valid json can be many different things that need to be handled in different ways, To facilitate this, a factory pattern was introduced to handle the different objects that can be found inside the json file. The factory then passes the file into the correct base object to be created. Constructs like JsonObject, and JsonArray can also contain more jsonbases so those constructors can themselves call the JsonFactory to handle the creation of those object.

## unorderedmap

To handle the wide variablity of string options that a json object can be passed with, an optimization is to build the internal hash map to hold this key value stores that can be passed.

## optimizations ruled out

- There is a way to lazy evaluate the Json object. This could help increase runtime as we would then only parse the json components that are called in the functions. For example a 1000 line json file could be passed in and if we only care about `max(a[1], a[2])` then the rest of the json file could be ignored and overall speed would be improved. However, this comes at a cost of not reporting an invalid json file. As if the method doesn't hit a malformed part of the json file.
 