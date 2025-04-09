import pefile
import sys

def generate_def_file(dll_path):
    try:
        # Load the DLL file
        dll = pefile.PE(dll_path)

        # Print the header for the DEF file
        print("EXPORTS")

        # Iterate through the exported symbols
        for export in dll.DIRECTORY_ENTRY_EXPORT.symbols:
            if export.name:
                # Decode the exported function name
                export_name = export.name.decode()
                ordinal = export.ordinal
                # Print in the format for DEF file
                print(f"{export_name}={dll_file_path}.{export_name} @{ordinal}")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    # Check if the script is called with the correct number of arguments
    if len(sys.argv) != 2:
        print("Usage: python gen_def.py ")
        sys.exit(1)

    # Get the DLL file path from the command-line arguments
    dll_file_path = sys.argv[1]
    generate_def_file(dll_file_path)