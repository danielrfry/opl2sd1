from enum import Enum


class VGMEvent(object):
    def __init__(self, time):
        self.time = time


class OPLWriteEvent(VGMEvent):
    def __init__(self, time, reg, value):
        super().__init__(time)
        self.reg = reg & 0x1ff
        self.value = value
