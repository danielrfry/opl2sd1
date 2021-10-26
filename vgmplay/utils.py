import struct
import time


def read_struct(f, fmt, *keys):
    data = struct.unpack(fmt, f.read(struct.calcsize(fmt)))
    if len(keys) == 0:
        return data
    else:
        return dict(zip(keys, data))


def usleep(micros):
    time.sleep(micros / 1000000)
