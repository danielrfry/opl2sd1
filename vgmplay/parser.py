from utils import read_struct
from events import OPLWriteEvent
import os.path
import gzip


class Parser(object):
    def __init__(self, input_file):
        self.input_file = input_file
        self.time_base = None
        self.duration = None

    def read_events(self):
        raise NotImplementedError()

    def close(self):
        pass

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        self.close()


class DROParser(Parser):
    def __init__(self, input_file):
        super(DROParser, self).__init__(input_file)
        self.time_base = 1000
        self._read_header()

    def _read_header(self):
        dro_header = read_struct(
            self.input_file, '<8sHH', 'cSignature', 'iVersionMajor', 'iVersionMinor')
        if not (dro_header['iVersionMajor'] == 2 and dro_header['iVersionMinor'] == 0):
            raise Exception('Only version 2.0 DRO files are supported')

        dro2_header = read_struct(self.input_file, '<IIBBBBBB', 'iLengthPairs', 'iLengthMS', 'iHardwareType',
                                  'iFormat', 'iCompression', 'iShortDelayCode', 'iLongDelayCode', 'iCodemapLength')
        if dro2_header['iFormat'] != 0:
            raise Exception('Only interleaved DRO files are supported')

        self.event_count = dro2_header['iLengthPairs']
        self.duration = dro2_header['iLengthMS']
        self.hardware_type = dro2_header['iHardwareType']

        self._short_delay = dro2_header['iShortDelayCode']
        self._long_delay = dro2_header['iLongDelayCode']

        self.codemap = self.input_file.read(dro2_header['iCodemapLength'])
        self._start_pos = self.input_file.tell()

    def read_events(self):
        self.input_file.seek(self._start_pos)
        cur_time = 0
        for _ in range(1, self.event_count):
            code = self.input_file.read(1)[0]
            value = self.input_file.read(1)[0]
            if code == self._short_delay:
                cur_time = cur_time + value + 1
            elif code == self._long_delay:
                cur_time = cur_time + ((value + 1) << 8)
            elif code & 0x80:
                reg = self.codemap[code & 0x7f] | 0x100
                yield OPLWriteEvent(cur_time, reg, value)
            else:
                reg = self.codemap[code]
                yield OPLWriteEvent(cur_time, reg, value)

    def close(self):
        self.input_file.close()


class VGMParser(Parser):
    def __init__(self, input_file):
        super(VGMParser, self).__init__(input_file)
        self.time_base = 44100
        self._read_header()

    def _read_header(self):
        vgm_header = read_struct(self.input_file, '<4sIIIIIIII', 'ident', 'end_offset', 'ver',
                                 'sn76489clock', 'ym2413clock', 'gd3_offset', 'total_samples', 'loop_offset', 'loop_samples')

        if vgm_header['ver'] < 0x150:
            raise Exception('Unsupported VGM version')

        self.input_file.seek(0x34)
        self._vgm_offset = read_struct(self.input_file, '<I', 'vgm_offset')[
            'vgm_offset'] + 0x34
        self._end_offset = vgm_header['end_offset'] + 4
        self.duration = vgm_header['total_samples']

    def read_events(self):
        self.input_file.seek(self._vgm_offset)

        cur_time = 0
        while self.input_file.tell() < self._end_offset:
            cmd = self.input_file.read(1)[0]
            if cmd == 0x5a or cmd == 0x5e:
                reg = self.input_file.read(1)[0]
                value = self.input_file.read(1)[0]
                yield OPLWriteEvent(cur_time, reg, value)
            elif cmd == 0x5f:
                reg = self.input_file.read(1)[0] | 0x100
                value = self.input_file.read(1)[0]
                yield OPLWriteEvent(cur_time, reg, value)
            elif cmd == 0x61:
                samples = read_struct(self.input_file, '<H', 'samples')[
                    'samples']
                cur_time = cur_time + samples
            elif cmd == 0x62:
                cur_time = cur_time + 735
            elif cmd == 0x63:
                cur_time = cur_time + 882
            elif cmd == 0x66:
                break
            else:
                if (cmd & 0xf0) == 0x70:
                    samples = (cmd & 0xf) + 1
                    cur_time = cur_time + samples
                else:
                    raise Exception('Unsupported VGM command')


def open_parser(path):
    (_, ext) = os.path.splitext(path)
    ext = ext.lower()
    if ext == '.dro':
        return DROParser(open(path, 'rb'))
    elif ext == '.vgm':
        return VGMParser(open(path, 'rb'))
    elif ext == '.vgz':
        return VGMParser(gzip.open(path))
    else:
        return None
