from io import BufferedWriter
import struct
import time
from events import OPLWriteEvent

reg_slot_map = {
    0x00:  0, 0x01:   1,  0x02:  2,  0x03:  3,  0x04:  4,  0x05:  5,  0x08:  6,  0x09:  7,  0x0a:  8,  0x0b:  9,  0x0c: 10,  0x0d: 11,  0x10: 12,  0x11: 13,  0x12: 14,  0x13: 15,  0x14: 16,  0x15: 17,
    0x100: 18, 0x101: 19, 0x102: 20, 0x103: 21, 0x104: 22, 0x105: 23, 0x108: 24, 0x109: 25, 0x10a: 26, 0x10b: 27, 0x10c: 28, 0x10d: 29, 0x110: 30, 0x111: 31, 0x112: 32, 0x113: 33, 0x114: 34, 0x115: 35
}

note_fnums = [85, 90, 96, 101, 108, 114, 121, 128, 136, 144, 152, 161, 171, 181, 192, 203, 216, 228, 242, 256, 272,
              288, 305, 323, 342, 363, 384, 407, 432, 457, 484, 513, 544, 576, 611, 647, 685, 726, 769, 815, 864, 915, 969]


def get_slot(reg_offset):
    if reg_offset in reg_slot_map:
        return reg_slot_map[reg_offset]
    else:
        return 0


def get_channel(reg_offset):
    if reg_offset & 0x100 == 0x100:
        return (reg_offset & 0xff) + 9
    else:
        return reg_offset


def get_block_fnum(midi_note):
    note_fnums_len = len(note_fnums)
    if (midi_note < note_fnums_len):
        return (1, note_fnums[midi_note])
    else:
        index = (note_fnums_len - 12) + ((midi_note - note_fnums_len) % 12)
        block = ((midi_note - note_fnums_len) // 12) + 2
        return (block, note_fnums[index])


def get_ch_reg_offset(ch, op4=False):
    if op4:
        if ch >= 3:
            return (ch - 3) | 0x100
        else:
            return ch
    else:
        if ch >= 9:
            return (ch - 9) | 0x100
        else:
            return ch


def get_op_reg_offsets(ch, op4=False):
    if op4:
        ch1 = ((ch // 3) * 9) + (ch % 3)
        ch2 = ch1 + 3
        return (*get_op_reg_offsets(ch1), *get_op_reg_offsets(ch2))
    else:
        reg_base = 0x100 if ch >= 9 else 0
        ch = ch % 9
        reg_base = reg_base + ((ch // 3) * 8) + (ch % 3)
        return (reg_base, reg_base + 3)


def get_reg_channel(reg, op4=False):
    reg_type = reg & 0xf0
    if op4:
        if reg_type in (0xa0, 0xb0, 0xc0):
            ch = reg & 0x0f
            if ch > 5:
                return None
            ch = ch % 3
            if reg & 0x100 == 0x100:
                ch = ch + 3
            return ch
        else:
            raise NotImplementedError()
    else:
        if reg & 0x0f > 8:
            return None
        elif reg & 0x100 == 0x100:
            return (reg & 0x0f) + 9
        else:
            return (reg & 0x0f)


class OPLController(object):
    def __init__(self, chip):
        self.chip = chip
        self.registers = []
        self._clear_registers()

    def _clear_registers(self):
        self.registers.clear()
        for _ in range(0, 0x200):
            self.registers.append(None)

    def write_event(self, event):
        if type(event) == OPLWriteEvent:
            self.write(event.reg, event.value)

    def write(self, reg, value):
        reg = reg & 0x1ff
        if self.registers[reg] != value:
            if self.chip != None:
                self.chip.write(reg, value)
            self.registers[reg] = value

    def read(self, reg, default=None):
        reg = reg & 0x1ff
        if self.registers[reg] == None:
            return default
        else:
            return self.registers[reg]

    def wait(self, time):
        self.chip.wait(time)

    def flush(self):
        self.chip.flush()

    def reset(self):
        self._clear_registers()
        self.chip.reset()

    def close(self):
        self.chip.close()

    def all_notes_off(self):
        for ch in range(0, 18):
            self.note_off(ch)

    def note_on(self, ch, note):
        block, fnum = get_block_fnum(note)
        reg_offset = get_ch_reg_offset(ch)
        self.write(0xa0 + reg_offset, fnum & 0xff)
        self.write(0xb0 + reg_offset, ((fnum & 0x300) >> 8)
                   | ((block & 0x07) << 2) | 0x20)

    def note_off(self, ch):
        reg_offset = get_ch_reg_offset(ch)
        value = self.read(0xb0 + reg_offset)
        if value != None:
            value = value & ~0x20
            self.write(0xb0 + reg_offset, value)

    def is_4op_ch(self, ch):
        if ch < 0 or ch > 5:
            return False
        conn = self.read(0x104, 0) & 0x3f
        if conn == None:
            return None
        else:
            return (conn & (1 << ch)) != 0

    def get_reg_channel(self, reg):
        ch = get_reg_channel(reg, True)
        if ch == None or not self.is_4op_ch(ch):
            return get_reg_channel(reg, False)
        else:
            return ch

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        self.close()


class OPLChip(object):
    def write(self, reg, value):
        raise NotImplementedError()

    def flush(self):
        raise NotImplementedError()

    def wait(self, time):
        pass

    def reset(self):
        raise NotImplementedError()

    def close(self):
        pass

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        self.close()


class OPLUSBSerial(OPLChip):
    def __init__(self, device_path):
        from serial import Serial
        self.device = BufferedWriter(Serial(device_path, 115200))

    def write(self, reg, value):
        print(f"{reg:03x}: {value:02x}")
        port = (reg & 0x100) >> 8
        reg = reg & 0xff
        self.device.write(struct.pack('<BBB', port, reg, value))

    def flush(self):
        self.device.flush()

    def reset(self):
        self.device.write(b'\xff\x00\x01')
        self.flush()
        time.sleep(0.1)

    def close(self):
        self.device.close()
