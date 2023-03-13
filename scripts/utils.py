import os
import platform
import inspect


"""
Directories

script_dir - this 'scripts/' directory
surf_dir - root 'surf/' directory
cmake_build_dir - client cmake 'build/' dir
interp_dir - interpreter 'surf/' directory
api_dir - api 'api' directory
"""
script_dir = os.path.dirname(os.path.realpath(__file__))
root_surf_dir = os.path.abspath(os.path.join(script_dir, os.pardir)) 
cmake_build_dir = os.path.join(root_surf_dir, 'build')
interp_dir = os.path.join(root_surf_dir, "surf")
api_dir = os.path.join(root_surf_dir, "api")


"""Use shell for subprocesses executed on windows"""
is_win32 = platform.system() == 'Windows' or platform.system().__contains__("CYGWIN")
use_shell = platform.system() == 'Windows'


def log(msg: str) -> None:
    """
    Simple logging function
    
    Parameters
    ----------
    msg : str
        Message to be logged
    """
    caller = inspect.stack()[1].filename
    print(f"[{os.path.basename(caller)}] {msg}")