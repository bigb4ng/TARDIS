import os

def main():
    bin_folder = "./tests/bin/"

    # Iterate over all the files in the bin folder
    for file in os.listdir(bin_folder):
        #  Check if the file is a regular file
        if os.path.isfile(os.path.join(bin_folder, file)) and not file.startswith("."):
            print(f"Testing [{file}]: ", end="")
            try:
                # Test no tardis timeout
                code = os.system(f"timeout 2 {bin_folder}{file}")
                assert os.WEXITSTATUS(code) == 124, f"original did not time out"

                # Test with tardis no timeout
                code = os.system(f"timeout 2 ./tardis 10 10 {bin_folder}{file}")
                assert os.WEXITSTATUS(code) != 124, f"timed out with TARDIS"
                print("\033[92mOK\033[0m")
            except AssertionError as e:
                    print(f"\033[91mFAILED\033[0m ({e})")

if __name__ == "__main__":
    main()
