import sys

from utils import log
from interp import build_surf
from wave import build_wave
from api import build_api


def main() -> None:
    """
    Simple build script for the project that drops the CMake build into a directory
    Build configuration can be passed either as 'Debug' or 'Release' 

    Examples
    --------
    python surf.py surf 
    python surf.py wave debug
    python surf.py api
    """
    usage = "usage: surf.py <project> <optional_build_cfg>"
    if len(sys.argv) <= 1:
        log(usage)
        return

    project_name = sys.argv[1].lower()
    if project_name == "surf":
        build_surf()
    elif project_name == "wave":
        build_wave()
    elif project_name == "api":
        build_api()
    else:
        log(f"invalid project - {usage}")


if __name__ == '__main__':
    main()
