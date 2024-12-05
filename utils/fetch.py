import sys
import requests
from pathlib import Path


# The cookie is found by inspecting the requests for Day 1 in the browser.
# Firefox: Right-click > Inspect. 
#          Network tab > Reload (to forced a GET)
#          Look for the cookie called "session"
AOC_COOKIE = "Read from cookie.txt"
AOC_YEAR   = 2024       


# Usage: python3 ../utils/fetch.py 4

# The script should be run from the /build folder of the current year. 
# Be sure to change AOC_YEAR to match. 
# Be sure to update the value of AOC_COOKIE. 
#
# The folder structure is expected to be:
#    /aocYYYY
#        CMakeLists.txt
#        /build           << Build and run everything from here
#        /day01
#            input.txt    << The file fetched by this script
#            test.txt     << Test input given in the problem
#            solution.cpp
#        /day02
#        ...
#        /day25
#        /utils
def main():
    args = sys.argv[1:]
    print(sys.argv)
    if len(args) == 0:
        raise "Provide a day number"

    AOC_COOKIE = open('cookie.txt').readlines()[0].strip()

    year       = AOC_YEAR
    day        = int(args[0])
    input_url  = "https://adventofcode.com/%d/day/%d/input" % (year, day)
    input_path = Path(sys.argv[0]).parent.parent / ("day%02d" % day) / "input.txt"    

    # We don't want to spam the server. The input never changes for a given cookie.
    if not input_path.exists():
        print("Fetching input data from:", input_url)
        cookies = {"session": AOC_COOKIE}
        content = requests.get(input_url, cookies=cookies).content

        print("Saving input data to:", input_path)
        input_file = open(input_path, 'wb')
        input_file.write(content)

    else:
        print("Input file already exists:", input_path)


if __name__ == "__main__":
    main()
