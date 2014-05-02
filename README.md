# Evtablet
Single header libevdev wrapper for getting wacom tablet input in a simple way under linux. It has been useful to me in some projects and a colleague manifested interest in this simplified wrapper. I figured it might be useful to others. If you find anything wrong or need support for something, feel free to get in touch or contribute.

## Build
It's a single header, so just include evtablet.hpp and both link with libevdev and add it to the include path.

## Usage
You must derive from ev::Tablet using the [CRTP idiom](http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern) and one or more of the following member functions according to your need:
```c++
struct Derived : ev::Tablet<Derived> {
  void event_sync(bool report) {}
  void event_x(unsigned x) {}
  void event_y(unsigned x) {}
  void event_pressure(unsigned x) {}
  void event_distance(unsigned x) {}
  void event_in_range(bool x) {}
  void event_rubber_in_range(bool x) {}
  void event_stylus_contact(bool x) {}
  void event_stylus_button1(bool x) {}
  void event_stylus_button2(bool x) {}
};
```
These functions are called everytime their respective events are triggered. To process events that are in line, call the member function:
```c++
  //returns true if everything went okay and false if not
  bool process_events();
```

[Sync events](http://www.freedesktop.org/software/libevdev/doc/latest/syn_dropped.html) marks the end of frames of hardware events. The function is called with true if it's a normal sync event and false in case of dropped events. This is already handled, but sync events can be useful as a way to group events.
Minimum and maximum values for the x, y, pressure, and distance values can be obtained with the following member functions from ev::Tablet:
```c++
  unsigned x_min() const;
  unsigned x_max() const;
  unsigned y_min() const;
  unsigned y_max() const;
  unsigned pressure_min() const;
  unsigned pressure_max() const;
  unsigned distance_min() const;
  unsigned distance_max() const;
```

More detail can be obtained by calling the member function:
```c++
  libevdev_absinfo *query_properties(int axis)
```
Possible values for axis are ABS_X, ABS_Y, ABS_PRESSURE and ABS_DISTANCE. The pointer-to-libevdev_absinfo returned can be used to get access the fields value, minimum, maximum, fuzz, resolution and flat.
To start using a device, call open with the correct /dev/input path. It returns true in case the device was opened correctly and false if not. A flag 'good' is also set with this value (this flag is also set to false in case of error processing events). Opening new devices closes the old one, even if the operation. To close intetionally, call close.
```c++
  bool open(const char *path);
  void close();
  bool good;
```

## Example
A simple example is included in example.cpp. On my system, I can compile it with:
```
g++ -std=c++11 example.cpp -I/usr/include/libevdev-1.0 -levdev -oexample
```

## MIT License
Copyright (c) 2014 Raphael Sousa Santos, http://raphaelss.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
