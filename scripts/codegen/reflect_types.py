import os

# TODO: Pass this as arg 
wave_src = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../client/Wave"))
output_file = "ReflectedClassTypes.h"

"""
Identify project namespaces to reflect

Core - core::
Engine/Renderer - wv::
Vulkan - wvk::
"""
namespace_map =  { 
    "Core": "core",
    "Engine": "wv",
    "Renderer": "wv",
    "Vulkan": "wvk"
}

"""
Ignored header files

wpch.h - Pre-compiled header file shouldn't be parsed
"""
header_ignore = [
    "wpch.h"
]


def main():
    full_class_list = []

    # Find all subdirectories in the client/Wave project
    subdirs = [i[0] for i in os.walk(wave_src)]
    for abs_subdir in subdirs:
        # Check if the directory name has a valid namespace in the project
        dirname = os.path.basename(os.path.normpath(abs_subdir))
        if dirname in namespace_map:
            # Filter the contents of the directory to only the applicable headers
            sources = os.listdir(abs_subdir)
            headers = [h for h in filter(lambda f: f.endswith(".h"), sources)]
            class_map = find_classes(abs_subdir, dirname, headers)
            # Combine classes with their proper namespace definition
            class_names = [x for y in class_map.values() for x in y]
            for class_name in class_names:
                class_name = namespace_map[dirname] + "::" + class_name
                full_class_list.append(class_name)

    # Print list of reflected types
    log("Generated reflection types for:")
    for class_name in full_class_list:
        log(f"\t{class_name}")

    # Generate C++
    # Clear output file if it already exists
    with open(output_file, "w"): pass

    # Further problems...
    # This needs to include the classes in the generated file
    # This also has to write the macro in the correct namespaces
    # The file also has to be included somewhere?
        


"""
Retrieves all valid C++ classes in a list of header files
NOTE: This may not be comprehensive for the entire C++ standard... use with caution

absdir - The absolute path to the directory containing the list of headers
dirname - The name of the directory containing the list of headers
headers - The list of headers to retrieve C++ classes from 
"""
def find_classes(absdir: str, dirname: str, headers: list[str]) -> dict:
    class_map = {} 

    for header in headers:
        hpath = os.path.join(absdir, header)
        with open(hpath, "r") as hfile:
            # Split file by words, skip the file if no classes exist
            file_words = hfile.read().split()
            if "class" not in file_words:
                continue

            # Get all classes
            class_idxs = [i + 1 for i, s in enumerate(file_words) if s == "class"]
            for idx in class_idxs:
                # This may not be comprehensive for the entire C++ standard...
                fwd_decl = file_words[idx].endswith(";")
                enum_class = file_words[idx - 2] == "enum"
                if not fwd_decl and not enum_class:
                    if dirname not in class_map:
                        class_map[dirname] = [file_words[idx]]
                    else:
                        class_map[dirname].append(file_words[idx])

    return class_map


def log(msg):
    print(f"[{os.path.basename(__file__)}] {msg}")


if __name__ == "__main__":
    main()