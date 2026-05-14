#!/usr/bin/env python3

import argparse
import json
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent


def get_project_version() -> str:
    cmake_file = REPO_ROOT / "CMakeLists.txt"
    for line in cmake_file.read_text().splitlines():
        if line.startswith("set(PROJECT_VER"):
            return line.split('"')[1]
    raise ValueError(f"failed to parse PROJECT_VER from {cmake_file}")


def parse_size(size_str: str) -> int:
    value = size_str.strip().upper()
    units = {
        "KB": 1024,
        "MB": 1024 * 1024,
    }
    for suffix, multiplier in units.items():
        if value.endswith(suffix):
            number = value[: -len(suffix)].strip()
            return int(number) * multiplier
    return int(value, 0)


def load_flasher_args(build_dir: Path) -> dict:
    flasher_args = build_dir / "flasher_args.json"
    if not flasher_args.exists():
        raise FileNotFoundError(f"missing {flasher_args}")
    return json.loads(flasher_args.read_text())


def collect_segments(build_dir: Path, flasher_args: dict) -> tuple[list[tuple[int, Path]], int | None]:
    flash_files = flasher_args.get("flash_files")
    if not flash_files:
        raise ValueError("flash_files is missing in flasher_args.json")

    segments: list[tuple[int, Path]] = []
    for offset_str, rel_path in flash_files.items():
        offset = int(offset_str, 16)
        bin_path = build_dir / rel_path
        if not bin_path.exists():
            raise FileNotFoundError(f"missing bin file: {bin_path}")
        segments.append((offset, bin_path))

    segments.sort(key=lambda item: item[0])

    flash_size = None
    flash_settings = flasher_args.get("flash_settings", {})
    flash_size_str = flash_settings.get("flash_size")
    if flash_size_str:
        flash_size = parse_size(flash_size_str)

    return segments, flash_size


def build_image(segments: list[tuple[int, Path]], flash_size: int | None) -> bytearray:
    max_end = 0
    used_ranges: list[tuple[int, int, Path]] = []

    for offset, bin_path in segments:
        size = bin_path.stat().st_size
        end = offset + size
        for used_start, used_end, used_path in used_ranges:
            if offset < used_end and end > used_start:
                raise ValueError(
                    f"overlap detected: {bin_path} @ 0x{offset:x}-0x{end:x} "
                    f"conflicts with {used_path} @ 0x{used_start:x}-0x{used_end:x}"
                )
        used_ranges.append((offset, end, bin_path))
        max_end = max(max_end, end)

    image_size = max_end
    if flash_size is not None and flash_size < max_end:
        raise ValueError(f"flash size 0x{flash_size:x} is smaller than required end offset 0x{max_end:x}")

    image = bytearray(b"\xFF" * image_size)
    for offset, bin_path in segments:
        data = bin_path.read_bytes()
        image[offset : offset + len(data)] = data

    return image


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Merge ESP-IDF build output binaries into a single flash image."
    )
    parser.add_argument(
        "-b",
        "--build-dir",
        default=None,
        help="ESP-IDF build directory (default: <repo>/build)",
    )
    parser.add_argument(
        "-o",
        "--output",
        default=None,
        help="Output file path (default: <repo>/releases/xiaozhi_v<version>_{merged|full}.bin)",
    )
    parser.add_argument(
        "--full",
        action="store_true",
        help="Pad the merged image to flash_size instead of matching official merge-bin behaviour",
    )
    args = parser.parse_args()

    build_dir = (Path(args.build_dir).resolve() if args.build_dir else REPO_ROOT / "build")
    if args.output:
        output = Path(args.output).resolve()
    else:
        project_version = get_project_version()
        image_kind = "full" if args.full else "merged"
        output = REPO_ROOT / "releases" / f"xiaozhi_v{project_version}_{image_kind}.bin"

    flasher_args = load_flasher_args(build_dir)
    segments, flash_size = collect_segments(build_dir, flasher_args)
    image = build_image(segments, flash_size)

    if args.full:
        if flash_size is None:
            raise ValueError("flash_size is missing, cannot create full image")
        image.extend(b"\xFF" * (flash_size - len(image)))

    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(image)

    print(f"build_dir: {build_dir}")
    print(f"output: {output}")
    if flash_size is not None:
        print(f"flash_size: 0x{flash_size:x} ({flash_size} bytes)")
    for offset, bin_path in segments:
        print(f"0x{offset:08x}  {bin_path.relative_to(build_dir)}")
    print(f"merged_size: 0x{len(image):x} ({len(image)} bytes)")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        print(f"error: {exc}", file=sys.stderr)
        raise SystemExit(1)
