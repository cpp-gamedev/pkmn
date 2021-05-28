#!/usr/bin/env python3

"""
PKMN Utility Tool (C) hentai-chan <dev.hentai-chan@outlook.com>

Description
-----------
This script uses the Pokemon API to create a JSON file and an ASCII sprite for the
pkmn game. Run it from the root directory of the project and ensure that all dependencies
are installed.

Usage
-----

1. creates two new pkmns (bulbasaur and charmander)
>> python gen_data.py --verbose make --id 1 4

2. creates a manifest.json
>> python gen_data.py manifest
"""

import argparse
import json
from collections import Counter
from pathlib import Path
from pprint import pprint
from random import randint, random
from typing import List
from urllib.parse import urljoin

import colorama
import requests
from colorama import Fore, Style
from PIL import Image, ImageOps
from rich.console import Console
from rich.progress import track

__version__ = "0.0.1"
assets = Path('assets/')
assets.mkdir(parents=True, exist_ok=True)
ASSETS = assets
BASE_API = "https://pokeapi.co/api/v2/pokemon/"
SPRITE_API = "https://pokeres.bastionbot.org/images/pokemon/"
CONSOLE = Console()

#region image processing

CHARS = [' ', '.', 'o', 'v', '@', '#', 'W']

def resize_image(img: Image, new_width=20) -> Image:
    width, height = img.size
    aspect_ratio = height / float(width)
    return img.resize((new_width, int(aspect_ratio * new_width)), Image.BILINEAR)

def detect_color(c: str, r:int, g:int, b:int) -> str:
    # thresholds were determined experimentally
    if sum([r, g, b]) > 550:
        return ''.join((Fore.WHITE, c, Style.RESET_ALL))
    elif r + g > 250 and b < 100:
        return ''.join((Fore.YELLOW, c, Style.RESET_ALL))
    elif r + b > 250 and g < 100:
        return ''.join((Fore.MAGENTA, c, Style.RESET_ALL))
    elif g + b > 250 and r < 100:
        return ''.join((Fore.CYAN, c, Style.RESET_ALL))
    elif max(r, g, b) == r:
        return ''.join((Fore.RED, c, Style.RESET_ALL))
    elif max(r, g, b) == g:
        return ''.join((Fore.GREEN, c, Style.RESET_ALL))
    else:
        return ''.join((Fore.BLUE, c, Style.RESET_ALL))

def img2ascii(image, width=20, mirror_image=False) -> List[str]:
    image = resize_image(image, new_width=width).convert('RGB')

    if mirror_image:
        image = ImageOps.mirror(image)

    data = image.load()

    ascii_art = []
    for w in range(width):
        for h in (height:=range(width)):
            r, g, b = data[h, w]
            char = int(r/3 + g/3 + b/3)
            ascii_art.append(detect_color(CHARS[int(char * len(CHARS) // 256)], r, g, b))
        ascii_art.append('\n')
    return ascii_art

#endregion

def req_pkmn_data(id_: int, verbose: bool) -> None:
    result = None

    # result is stored in assets/{id}.json
    with CONSOLE.status('Making initial request . . .', spinner='dots3') as _:
        response = requests.get(urljoin(BASE_API, str(id_))).json()
        level = randint(30, 60)
        result = {
            'id': response['id'],
            'name': response['name'],
            'level': level,
            'hp': int(2 * level * (0.8 + 0.4 * random())),
            'atk': 100,
            'def': 100,
        }

    moves = {}
    endpoints = [move['move']['url'] for move in response['moves']]
    for index, endpoint in track(enumerate(endpoints), "Sending Requests", total=len(endpoints), transient=True):
        move_response = requests.get(endpoint).json()
        unescape = lambda text: text.replace('\u00e9','e').replace('\u2019', "'").replace('\n',' ').replace('\u00ad', '-')
        moves[index] = {
            'name': move_response['names'][7]['name'],
            'accuracy': int(move_response['accuracy']) if move_response['accuracy'] is not None else None,
            'effect_changes': move_response['effect_changes'] if len(move_response['effect_changes']) > 0 else [None],
            'effect_chance': int(move_response['effect_chance']) if move_response['effect_chance'] is not None else None,
            'power': int(move_response['power']) if move_response['power'] is not None else None,
            'flavor_text_entries': [unescape(entry['flavor_text']) for entry in move_response['flavor_text_entries'] if entry['language']['name'] == 'en']
        }
    result['moves'] = moves

    with open(ASSETS.joinpath(f"{result['id']}.json"), mode='w', encoding='utf-8') as file_handler:
        json.dump(result, file_handler)

    if verbose:
        pprint(result)

    print(f"{Fore.YELLOW}Done! A new JSON file was created in {str(ASSETS)!r}.{Style.RESET_ALL}")

def gen_sprite(id_: int,  mirror: bool, verbose: bool) -> None:    
    colorama.init(autoreset=False)

    with CONSOLE.status('Creating new ASCII image . . .', spinner='dots3') as _:        
        filename = f"{id_}.png"
        image_path = ASSETS.joinpath(filename)
        response = requests.get(urljoin(SPRITE_API, filename), stream=True)
        with open(image_path, mode='wb') as file_handler:
            for chunk in response.iter_content(1024):
                file_handler.write(chunk)

        ascii_art = img2ascii(Image.open(image_path), width=20, mirror_image=mirror)
        with open(ASSETS.joinpath(f"{id_}.txt"), mode='w', encoding='utf-8') as file_handler:
            file_handler.writelines(ascii_art)

        image_path.unlink(missing_ok=True)

    if verbose:
        print(f"\n{''.join(ascii_art)}")

    print(f"{Fore.YELLOW}Done! A new ASCII image was created in {str(ASSETS)!r}.{Style.RESET_ALL}")

def check_manifest(verbose: bool) -> None:
    extensions = ['.txt', '.json']

    files = list(filter(
        lambda file: file.suffix in extensions and file.stem.isnumeric(), 
        [file for file in ASSETS.glob(r'**/*')]
    ))
    
    ids = list(map(lambda file: int(file.stem), files))
    duplicates = [id_ for id_, count in Counter(ids).items() if count > 1]

    manifest = {
        'files': list(map(str, files)),
        'duplicates': duplicates,
        'game_ready': len(duplicates) >= 2,
    }

    with open("manifest.json", mode='w', encoding='utf-8') as file_handler:
        json.dump(manifest, file_handler)

    if verbose: pprint(manifest)

def main():
    parser = argparse.ArgumentParser()
    
    parser.add_argument('--version', action='version', version=f"%(prog)s {__version__}")
    parser.add_argument('--verbose', action='store_true', help="increase output verbosity")

    subparser = parser.add_subparsers(dest='command')

    make_parser = subparser.add_parser('make', help="create new pkmn data")
    make_parser.add_argument('--id', type=int, nargs='+', required=True, help="one or more pokemon id")
    make_parser.add_argument('--mirror', action='store_true', help="mirror sprite")

    manifest_parser = subparser.add_parser('manifest', help="validate manifest")

    args = parser.parse_args()

    if args.command == 'make':
        for id_ in args.id:
            req_pkmn_data(id_, verbose=False)
            gen_sprite(id_, args.mirror, args.verbose)
    elif args.command == 'manifest':
        check_manifest(args.verbose)
    else:
        raise NotImplementedError()


if __name__ == '__main__':
    main()
