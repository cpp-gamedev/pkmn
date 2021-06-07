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

## For Users: Playing the Game

Go to [Releases](https://github.com/cpp-gamedev/pkmn/releases) and download the
latest version of `pkmn-x64-linux-windows-v1.0.0-*.zip`. Unzip this directory, then

- run `easy_install.sh` (Linux)
- run `EasyInstall.ps1` (Windows)

to configure the game. This process may take a minute or two depending on your
internet connection. After that, run the `pkmn` binary (`pkmn.exe` on Windows)
to start the game. The game takes up quite a bit of vertical space, so you may
want to adjust the size of your terminal.

## For Developers: Build & Debug the Game

Initialize and update all submodules after you have cloned this repository:

```bash
git submodule update --init --recursive
```

For python specifically, you may need to install `python3-venv` to create virtual
environments on Linux.

### Generating new Pokemon

If this is your first time using a python script, use

```bash
python -m venv venv/
source venv/bin/activate
python -m pip install --upgrade pip
pip install -r requirements.txt --only-binary all
```

to install the dependencies in a virtual environment. On Windows, you need to use
`.\venv\Scripts\Activate.ps1` to activate a virtual environment. Note that this script
assumes that it is being run from the project's root directory. You need to create
at least two new pkmn file sets before you can start playing this game:

```bash
# creates two new pkmns (bulbasaur and charmander)
python gen_data.py --verbose make --id 1 4
# creates a manifest.json
python gen_data.py manifest
```

In case of doubt, use

```bash
python gen_data.py --help
```

to get more help.

### On Visual Studio (Windows)

Navigate to `Project` and select `Generate Cache for pkmn`. Set `Current Document (main.cpp)`
to start debugging this project (`F5`).

### On Visual Studio Code (Linux)

Useful links for first timers:

- <https://cpp-gamedev.netlify.app/series/101/1_getting_started/>
- <https://cpp-gamedev.netlify.app/series/101/2_development_environment/>
