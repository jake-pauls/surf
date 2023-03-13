import subprocess

from utils import log, api_dir, is_win32, use_shell


def build_api() -> None:
    """
    Builds the 'surf' api using GNU Make
    
    These make commands depend on the 'surf' codegen project being built
    and will build the surf project and install it as a side effect.
    """
    log("Building the surf api using make")

    so_ext = "dll" if is_win32 else "so" 
    so_cmd = subprocess.run(["make", "-C", api_dir, so_ext], shell=use_shell)

    # Ensure shared object subprocess ran properly
    if so_cmd.returncode == 0:
        log(f"Successfully built libsurf.{so_ext}")
    else:
        log("Failed to build the surf api")

    plat = "win32" if is_win32 else "unix"
    plat_cmd = subprocess.run(["make", "-C", api_dir, plat], shell=use_shell)

    if plat_cmd.returncode == 0:
        log("Successfully built the surf api executable")
        log("Start the server with './api/build/surf.exe'")
    else:
        log("Failed to compile and link surf api")
