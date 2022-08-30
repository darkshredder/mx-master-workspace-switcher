#include "common.h"

void map_init(Display *dpy);
void map(int *x, int *y);

int main(int argc, char *argv[])
{
  int fd;

  assert(argc==2);
  struct input_event ie;
  assert(strncmp(argv[1], "/dev/input/event", 16)==0);

  if((fd = open(argv[1], O_RDONLY)) == -1) {
    perror("opening device");
    exit(EXIT_FAILURE);
  }

  int event_count = 0;

  while(read(fd, &ie, sizeof(struct input_event))) {
    if (ie.type == EV_REL && (ie.code == REL_HWHEEL || ie.code == REL_HWHEEL_HI_RES)) {

      event_count++;

      if (event_count < 3) {
        continue;
      }

      event_count = 0;
        
      if (ie.code == REL_HWHEEL_HI_RES){

        Display *dpy = XOpenDisplay(NULL);
        if (dpy == NULL) {
            fprintf(stderr, "Cannot open display\n");
            exit(EXIT_FAILURE);
        }
        Window root = DefaultRootWindow(dpy);
        Atom atom = XInternAtom(dpy, "_NET_CURRENT_DESKTOP", False);
        Atom actual_type;
        int actual_format;
        unsigned long nitems, bytes_after;
        unsigned long *data = NULL;
        int status = XGetWindowProperty(dpy, root, atom, 0, 1, False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, (unsigned char **)&data);
        if (status != Success) {
            fprintf(stderr, "Cannot get _NET_CURRENT_DESKTOP property\n");
            exit(EXIT_FAILURE);
        }
        int current_desktop = data[0];

        XEvent xev;
        memset(&xev, 0, sizeof(xev));
        xev.type = ClientMessage;
        xev.xclient.window = root;
        xev.xclient.message_type = atom;
        xev.xclient.format = 32;

        if (ie.value > 0)
        {
            xev.xclient.data.l[0] = current_desktop - 1;
        }
        else
        {
            xev.xclient.data.l[0] = current_desktop + 1;
        }

        xev.xclient.data.l[1] = 0;

        XSendEvent(dpy, root, False, SubstructureNotifyMask, &xev);
        XFlush(dpy);
        XFree(data);
        XCloseDisplay(dpy);

        printf("Switching to desktop %ld from %d\n", xev.xclient.data.l[0], current_desktop);

      }
    }
  }

  return 0;
}