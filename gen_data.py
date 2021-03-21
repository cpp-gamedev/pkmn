#!/usr/bin/env python3

"""
PKMN Utility Tool (C) hentai-chan <dev.hentai-chan@outlook.com>

Description
-----------
This script uses the Pokemon API to create a JSON file for the pkmn game.

Usage
-----
Run 'python gen_data.py make --name <pokemon>' from the project's root directory
to create a new JSON file for pkmn. Append the '--help' option for more help.

Additionally, use `python gen_data.py ascii --name <pokemon>` to create a new
ascii image. In both instances, a ID may be used as primary argument instead.
"""

import json
from pathlib import Path
from random import randint, random
from typing import List
from urllib.parse import urljoin

import click
import colorama
import requests
from click import style
from colorama import Fore, Style
from PIL import Image, ImageOps
from rich.console import Console
from rich.progress import track

#region Image Processing

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

CONTEXT_SETTINGS = dict(max_content_width=120)

@click.group(invoke_without_command=True, context_settings=CONTEXT_SETTINGS, help=style("PKMN utility tool.", fg='bright_magenta'))
@click.option('--verbose', is_flag=True, default=False, help=style("Enable verbose terminal output.", fg='bright_yellow'))
@click.version_option(version='0.0.1', prog_name="get-data", help=style("Show the version and exit.", fg='bright_yellow'))
@click.pass_context
def cli(ctx, verbose):
    ctx.ensure_object(dict)
    assets = Path('assets/')
    assets.mkdir(parents=True, exist_ok=True)
    ctx.obj['ASSETS'] = assets
    ctx.obj['BASE_API'] = "https://pokeapi.co/api/v2/pokemon/"
    ctx.obj['SPRITE_API'] = "https://pokeres.bastionbot.org/images/pokemon/"
    ctx.obj['CONSOLE'] = Console()
    ctx.obj['VERBOSE'] = verbose

@cli.command(context_settings=CONTEXT_SETTINGS, help=style("Create a new PKMN data file.", fg='bright_green'))
@click.option('--name', type=click.STRING, help=style("Name of a pokemon (English).", fg='bright_yellow'))
@click.option('--id', type=click.STRING, help=style("A pokemon ID.", fg='bright_yellow'))
@click.pass_context
def make(ctx, name, id):
    result = None
    query = name or id
    console = ctx.obj['CONSOLE']

    # make result is stored in assets/{id}.json
    with console.status('Making initial request . . .', spinner='dots3') as _:
        response = requests.get(urljoin(ctx.obj['BASE_API'], query)).json()
        level = randint(30, 60)
        result = {
            'id': response['id'],
            'name': response['name'],
            'level': level,
            'hp': int(2 * level * (0.8 + 0.4 * random())), # formula for hp is a rough estimation
            'atk': 1, 
            'def': 1, 
        }

    # the base api does not provide detailed information about moves, 
    # so we need to make more calls to the api (+1 per move)
    moves = {}
    endpoints = [move['move']['url'] for move in response['moves']]
    for index, endpoint in track(enumerate(endpoints), "Sending Requests", total=len(endpoints), transient=True):
        move_response = requests.get(endpoint).json()
        moves[index] = {
            'name': move_response['names'][7]['name'],
            'accuracy': int(move_response['accuracy']) if move_response['accuracy'] is not None else None,
            'effect_changes': move_response['effect_changes'],
            'effect_chance': int(move_response['effect_chance']) if move_response['effect_chance'] is not None else None,
            'power': int(move_response['power']) if move_response['power'] is not None else None,
            'flavor_text_entries': [entry['flavor_text'] for entry in move_response['flavor_text_entries'] if entry['language']['name'] == 'en']
        }
    result['moves'] = moves

    with open(ctx.obj['ASSETS'].joinpath(f"{result['id']}.json"), mode='w', encoding='utf-8') as file_handler:
        json.dump(result, file_handler)

    if ctx.obj['VERBOSE']:
        console.print(result)
        click.echo('\n')

    click.secho(f"Done! A new JSON file was created in '{ctx.obj['ASSETS']}/'.", fg='bright_yellow')

@cli.command(context_settings=CONTEXT_SETTINGS, help=style("Create an ASCII image.", fg='bright_green'))
@click.option('--name', type=click.STRING, help=style("Name of a pokemon (English).", fg='bright_yellow'))
@click.option('--id', type=click.STRING, help=style("A pokemon ID.", fg='bright_yellow'))
@click.option('--mirror/--no-mirror', is_flag=True, default=False, help=style("Mirror image (Player).", fg='bright_yellow'))
@click.pass_context
def ascii(ctx, name, id, mirror):
    query = name or id
    
    colorama.init(autoreset=False)

    # the base api only contains very small sprites,
    # but there's another API which provides higher
    # quality sprites which are only searchable by id
    with ctx.obj['CONSOLE'].status('Creating new ASCII image . . .', spinner='dots3') as _:
        if name:
            query = requests.get(urljoin(ctx.obj['BASE_API'], query)).json()['id']
        
        # first find and download the pokemon sprite
        filename = f"{query}.png"
        image_path = ctx.obj['ASSETS'].joinpath(filename)
        response = requests.get(urljoin(ctx.obj['SPRITE_API'], filename), stream=True)
        with open(image_path, mode='wb') as file_handler:
            for chunk in response.iter_content(1024):
                file_handler.write(chunk)

        # then generate the ascii image and store the result in assets/{id}.txt
        ascii_art = img2ascii(Image.open(image_path), width=20, mirror_image=mirror)
        with open(ctx.obj['ASSETS'].joinpath(f"{query}.txt"), mode='w', encoding='utf-8') as file_handler:
            file_handler.writelines(ascii_art)

        # cleanup
        image_path.unlink(missing_ok=True)

    if ctx.obj['VERBOSE']:
        click.echo(f"\n{''.join(ascii_art)}")

    click.secho(f"Done! A new ASCII image was created in '{ctx.obj['ASSETS']}/'.", fg='bright_yellow')

if __name__ == '__main__':
    try:
        cli(obj={})
    except KeyboardInterrupt:
        pass
