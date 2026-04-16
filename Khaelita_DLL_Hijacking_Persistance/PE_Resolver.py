import pefile
import os
import argparse

def generate_proxy_code(target_dll):
    try:
        if not os.path.exists(target_dll):
            print(f"[!] Error: File '{target_dll}' not found.")
            return

        pe = pefile.PE(target_dll)
        full_dll_name = os.path.basename(target_dll)
        dll_name_no_ext = os.path.splitext(full_dll_name)[0]
        system32_path = r"C:\\Windows\\System32\\"

        print(f"\n// --- Khaelita PE Resolver: Proxy Directives for {full_dll_name} ---")
        print(f"// Target Source: {target_dll}")
        print("// Copy and paste these lines at the top of your C++ project.\n")

        if hasattr(pe, 'DIRECTORY_ENTRY_EXPORT'):
            for export in pe.DIRECTORY_ENTRY_EXPORT.symbols:
                if export.name:
                    func_name = export.name.decode('utf-8')
                    pragma_line = (
                        f'#pragma comment(linker, "/export:{func_name}='
                        f'{system32_path}{dll_name_no_ext}.{func_name}")'
                    )
                    print(pragma_line)
        
        print("\n// --- End of Generated Code ---")

    except Exception as e:
        print(f"[!] Analysis Error: {e}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Khaelita PE Resolver - Automated DLL Proxy Directive Generator")
    parser.add_argument("-p", "--path", help="Full path to the target DLL (e.g., C:\\Windows\\System32\\version.dll)", required=True)
    args = parser.parse_args()
    generate_proxy_code(args.path)
