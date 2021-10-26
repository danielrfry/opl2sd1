import argparse
import time
from parser import open_parser
from opl import OPLController, OPLUSBSerial


def get_device(path):
    return OPLController(OPLUSBSerial(path))


def main():
    parser = argparse.ArgumentParser(
        prog='vgmplay', description='Play back .DRO, .VGM and .VGZ files.')
    parser.add_argument('--device', '-d', metavar='DEVICE', required=True,
                        type=str, nargs=1, help='serial port device path', dest='device')
    parser.add_argument('file', metavar='FILE', nargs=1,
                        help='the file to play')

    args = parser.parse_args()

    with open_parser(args.file[0]) as vgmparser, get_device(args.device[0]) as chip:
        try:
            chip.reset()
            start_time = time.perf_counter()

            last_event_time = 0
            for event in vgmparser.read_events():
                now = time.perf_counter() - start_time
                delay = (event.time / vgmparser.time_base) - now
                if delay > 0:
                    chip.flush()
                    time.sleep(delay)
                chip.wait((event.time - last_event_time) / vgmparser.time_base)

                chip.write_event(event)

                last_event_time = event.time

            chip.wait((vgmparser.duration - last_event_time) /
                      vgmparser.time_base)

            chip.all_notes_off()
        except KeyboardInterrupt:
            chip.reset()
            print()


if __name__ == "__main__":
    main()
