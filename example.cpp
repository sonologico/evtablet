/*
 *The MIT License (MIT)
 *
 *Copyright (c) 2014 Raphael Sousa Santos, http://raphaelss.com
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
 */

#include <cstdio>
#include <unistd.h>
#include "evtablet.hpp"

struct Conf : ev::Tablet<Conf> {
  void print_ranges()
  {
    std::printf("X - minimum: %u, maximum: %u\n", x_min(), x_max());
    std::printf("Y - minimum: %u, maximum: %u\n", y_min(), y_max());
    std::printf("Pressure - minimum: %u, maximum: %u\n", pressure_min(),
                pressure_max());
    std::printf("Distance - minimum: %u, maximum: %u\n", distance_min(),
                distance_max());
  }

  void event_sync(bool report)
  {
    std::printf("+++++++SYNC: %u++++++++\n", report);
  }

  void event_x(unsigned x)
  {
    std::printf("X: %u\n", x);
  }

  void event_y(unsigned x)
  {
    std::printf("Y: %u\n",x);
  }

  void event_pressure(unsigned x)
  {
    std::printf("Pressure: %u\n",x);
  }

  void event_distance(unsigned x)
  {
    std::printf("Distance: %u\n",x);
  }

  void event_in_range(bool x)
  {
    std::printf("In_range: %u\n",x);
  }

  void event_rubber_in_range(bool x)
  {
    std::printf("Rubber_in_range: %u\n",x);
  }

  void event_stylus_contact(bool x)
  {
    std::printf("Contact: %u\n",x);
  }

  void event_stylus_button1(bool x)
  {
    std::printf("Stylusb1: %u\n",x);
  }

  void event_stylus_button2(bool x)
  {
    std::printf("Stylusb2: %u\n",x);
  }
};

int main(int, char **argv)
{
  Conf t;
  if(!t.open(argv[1])) {
    return 1;
  }
  t.print_ranges();
  while(t.process_events()) {
    usleep(100);
  }
  return 0;
}
