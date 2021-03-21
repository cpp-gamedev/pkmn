<h1 align="center">PKMN</h1>

<p align="center">
    <a href="https://ci.appveyor.com/project/hentai-chan/pkmn" alt="CI" title="CI">
        <img src="https://ci.appveyor.com/api/projects/status/vcffeyqccc4pvi4d?branch=master&passingText=master%20-%20OK&svg=true">
    </a>
    <a href="https://ci.appveyor.com/project/hentai-chan/pkmn" alt="CI" title="CI">
        <img src="https://ci.appveyor.com/api/projects/status/vcffeyqccc4pvi4d?branch=integration&passingText=integration%20-%20OK&svg=true">
    </a>
    <a alt="C++ Standard Version" title="C++ Standard Version">
        <img src="https://img.shields.io/badge/Standard-C++17-blue">
    </a>
    <a alt="CMake Version" title="CMake Version">
        <img src="https://img.shields.io/badge/CMake-3.18+-blue">
    </a>
    <a alt="Python Version" title="Python Version">
        <img src="https://img.shields.io/badge/Python-3.8%20|%203.9-blue">
    </a>
    <a href="https://www.gnu.org/licenses/gpl-3.0.en.html" alt="License" title="License">
        <img src="https://img.shields.io/badge/License-GPLv3-blue.svg">
    </a>
</p>

## Build & Debug

### Generating new Pokemon

If this is your first time using a python script, use

```bash
$ python -m venv venv/
$ source venv/Scripts/activate
$ pip install -r requirements.txt
```

to install the dependencies in a virtual environment. Note that this script
assumes that it is being run from the project's root directory. After that
you should be able to use this script:

```bash
$ # creates a new JSON file in assets/
$ python gen_data.py make --id 1
$ # creates a new ascii image as txt file in assets/ and prints a preview
$ python gen_data.py --verbose ascii --id 1 --mirror
```

You can also use the `--name` option for identifying a new pokemon. Repeat both
steps - you need at least two pokemon to play this game. In case of doubt, use

```bash
$ python gen_data.py --help
```

to get more help. Once you've obtained these files, build the project to play
this game. Use the `--mirror` flag for your own pokemon (recommended).

### On Visual Studio (Windows)

Navigate to `Project` and select `Generate Cache for pkmn`. Set `Current Document (main.cpp)`
to start debugging this project (`F5`).

### On Visual Studio Code (Linux)

TODO
