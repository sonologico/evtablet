#ifndef EVTABLET_HPP_INCLUDED
#define EVTABLET_HPP_INCLUDED
#include <memory>
#include <cstring>
#include <libevdev/libevdev.h>
#include <fcntl.h>

namespace ev {

namespace impl {
struct evdev_deleter {
  void operator()(libevdev *ptr)
  {
    libevdev_free(ptr);
  }
};
}

template <class T>
struct Tablet {
  Tablet(): good(false) {}

  bool open(const char *path)
  {
    libevdev *tmp;
    int fd = ::open(path, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
      std::fputs("Failed to open device\n", stderr);
      dev.reset();
      good = false;
      return false;
    }
    int rc = libevdev_new_from_fd(fd, &tmp);
    if (rc < 0) {
      std::fprintf(stderr,"Failed to init libevdev (%s)\n", std::strerror(-rc));
      dev.reset();
      good = false;
      return false;
    }
    dev.reset(tmp);
    good = true;
    return true;
  }

  bool process_events()
  {
    input_event ev;
    T *derived = static_cast<T*>(this);
    int rc;
    do {
      rc = libevdev_next_event(dev.get(), LIBEVDEV_READ_FLAG_NORMAL, &ev);
      if (rc == LIBEVDEV_READ_STATUS_SYNC) {
        derived->event_sync(ev.code != SYN_DROPPED);
        while (rc == LIBEVDEV_READ_STATUS_SYNC) {
          rc = libevdev_next_event(dev.get(), LIBEVDEV_READ_FLAG_SYNC, &ev);
        }
      } else if (rc == LIBEVDEV_READ_STATUS_SUCCESS) {
        switch(ev.type) {
        case EV_SYN:
          derived->event_sync(ev.code != SYN_DROPPED);
          break;
        case EV_ABS:
          switch(ev.code) {
          case ABS_X:
            derived->event_x(ev.value);
            break;
          case ABS_Y:
            derived->event_y(ev.value);
            break;
          case ABS_PRESSURE:
            derived->event_pressure(ev.value);
            break;
          case ABS_DISTANCE:
            derived->event_distance(ev.value);
            break;
          }
        case EV_KEY:
          switch(ev.code) {
          case BTN_TOOL_PEN:
            derived->event_in_range(ev.value);
            break;
          case BTN_TOOL_RUBBER:
            derived->event_rubber_in_range(ev.value);
            break;
          case BTN_TOUCH:
            derived->event_stylus_contact(ev.value);
            break;
          case BTN_STYLUS:
            derived->event_stylus_button1(ev.value);
            break;
          case BTN_STYLUS2:
            derived->event_stylus_button2(ev.value);
            break;
          }
        }
      }
    } while(rc == LIBEVDEV_READ_STATUS_SYNC
            || rc == LIBEVDEV_READ_STATUS_SUCCESS);
    if (rc != LIBEVDEV_READ_STATUS_SUCCESS && rc != -EAGAIN) {
      std::fprintf(stderr, "Failed to handle events: %s\n", std::strerror(rc));
      good = false;
      return false;
    }
    return true;
  }

  const input_absinfo *query_properties(int axis)
  {
    return libevdev_get_abs_info(dev.get(), axis);
  }

  unsigned x_min() const
  {
    return libevdev_get_abs_info(dev.get(), ABS_X)->minimum;
  }

  unsigned x_max() const
  {
    return libevdev_get_abs_info(dev.get(), ABS_X)->maximum;
  }

  unsigned y_min() const
  {
    return libevdev_get_abs_info(dev.get(), ABS_Y)->minimum;
  }

  unsigned y_max() const
  {
    return libevdev_get_abs_info(dev.get(), ABS_Y)->maximum;
  }

  unsigned press_min() const
  {
    return libevdev_get_abs_info(dev.get(), ABS_PRESSURE)->minimum;
  }

  unsigned press_max() const
  {
    return libevdev_get_abs_info(dev.get(), ABS_PRESSURE)->maximum;
  }

  unsigned dist_min() const
  {
    return libevdev_get_abs_info(dev.get(), ABS_DISTANCE)->minimum;
  }

  unsigned dist_max() const
  {
    return libevdev_get_abs_info(dev.get(), ABS_DISTANCE)->maximum;
  }

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

  bool good;
  std::unique_ptr<libevdev, impl::evdev_deleter> dev;
};

}
#endif