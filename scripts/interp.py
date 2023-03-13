import subprocess

from utils import log, interp_dir, use_shell


def build_surf() -> None:
    """Builds the 'surf' interpreter using GNU Make"""
    log("Building surf using make and dune")

    make_cmd = subprocess.run(["make", "-C", interp_dir, "build"], shell=use_shell)

    # Ensure subprocess ran properly
    if make_cmd.returncode == 0:
        log("Successfully built dune project")
        log("Use 'make surf' to enter the surf repl")
    else:
        log("Failed to build dune project")