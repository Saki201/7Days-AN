"""Read Hebdomad's SPK layout recovered from N900 v1.1.0.
Derived from FUN_0002ed90 (index) and FUN_0002fccc (payload read).
Real res.spk validation is pending: the supplied Drive link is unreachable here.
"""
import argparse
import json
import pathlib
import struct


def read_index(path):
    path = pathlib.Path(path)
    size = path.stat().st_size
    with path.open('rb') as stream:
        header = stream.read(2)
        if len(header) != 2:
            raise ValueError('Truncated SPK header')
        count, = struct.unpack('<h', header)
        if count <= 0:
            raise ValueError('Invalid signed 16-bit entry count')
        table_end = 2 + count * 68
        table = stream.read(count * 68)
        if len(table) != count * 68:
            raise ValueError('Truncated SPK index')
    entries = []
    for i in range(count):
        raw_name, offset = struct.unpack_from('<64sI', table, i * 68)
        name_bytes = raw_name.split(b'\0', 1)[0]
        # Keep raw bytes in the listing; actual filename encoding is not established.
        name = name_bytes.decode('utf-8', errors='surrogateescape')
        if offset < table_end or offset > size:
            raise ValueError('Entry offset outside payload area: %d' % i)
        if entries and offset < entries[-1]['offset']:
            raise ValueError('Entry offsets are not ordered')
        entries.append({'name': name, 'name_hex': name_bytes.hex(), 'offset': offset})
    for i, entry in enumerate(entries):
        end = entries[i+1]['offset'] if i+1 < count else size
        entry['size'] = end - entry['offset']
    return entries


def extract(path, destination, entries):
    root = pathlib.Path(destination).resolve()
    root.mkdir(parents=True, exist_ok=True)
    seen = set()
    with pathlib.Path(path).open('rb') as stream:
        for entry in entries:
            name = entry['name'].replace('\\', '/')
            relative = pathlib.PurePosixPath(name)
            if (not name or relative.is_absolute() or '..' in relative.parts
                    or ':' in name or any(ord(c) < 32 or 0xD800 <= ord(c) <= 0xDFFF for c in name)):
                raise ValueError('Unsafe or undecodable filename: ' + repr(name))
            target = root.joinpath(*relative.parts).resolve()
            if not target.is_relative_to(root):
                raise ValueError('Path escapes destination')
            key = str(target).casefold()
            if key in seen or target.exists():
                raise ValueError('Duplicate or existing extraction target: ' + name)
            seen.add(key)
            target.parent.mkdir(parents=True, exist_ok=True)
            stream.seek(entry['offset'])
            remaining = entry['size']
            with target.open('xb') as out:
                while remaining:
                    block = stream.read(min(remaining, 1024 * 1024))
                    if not block:
                        raise ValueError('Truncated payload')
                    out.write(block)
                    remaining -= len(block)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('pack', type=pathlib.Path)
    parser.add_argument('--extract', type=pathlib.Path)
    args = parser.parse_args()
    entries = read_index(args.pack)
    if args.extract:
        extract(args.pack, args.extract, entries)
    print(json.dumps(entries, ensure_ascii=True, indent=2))


if __name__ == '__main__':
    main()
