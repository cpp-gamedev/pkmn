#!/usr/bin/env python3

"""
PKMN Utility Tool (C) <dev.hentai-chan@outlook.com>

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
from urllib.parse import urljoin

import click
import requests
from click import style
from rich.console import Console

CONTEXT_SETTINGS = dict(max_content_width=120)

@click.group(invoke_without_command=True, context_settings=CONTEXT_SETTINGS, help=style("PKMN utility tool.", fg='bright_magenta'))
@click.version_option(version='0.0.1', prog_name="get-data", help=style("Show the version and exit.", fg='yellow'))
@click.pass_context
def cli(ctx):
    ctx.ensure_object(dict)
    assets = Path('assets/')
    assets.mkdir(parents=True, exist_ok=True)
    ctx.obj['ASSETS'] = assets
    ctx.obj['BASE_API'] = "https://pokeapi.co/api/v2/pokemon/"
    ctx.obj['SPRITE_API'] = "https://pokeres.bastionbot.org/images/pokemon/"
    ctx.obj['CONSOLE'] = Console()

@cli.command(context_settings=CONTEXT_SETTINGS, help=style("Create a new PKMN data file.", fg='bright_green'))
@click.option('--name', type=click.STRING, help=style("Name of a pokemon (English).", fg='bright_yellow'))
@click.option('--id', type=click.STRING, help=style("A pokemon ID.", fg='bright_yellow'))
@click.pass_context
def make(ctx, name, id):
    result = None
    query = name or id

    with ctx.obj['CONSOLE'].status('Collecting game data, this may take a while . . .', spinner='dots3') as _:
        response = requests.get(urljoin(ctx.obj['BASE_API'], query)).json()

        # formula for hp is a rough estimation
        level = randint(30, 60)
        result = {
            'id': response['id'],
            'name': response['name'],
            'level': level,
            'hp': int(2 * level * (0.8 + 0.4 * random())),
            'atk': 1, 
            'def': 1, 
        }

        # the base api does not provide detailed information about moves, 
        # so we need to make more calls to the api (+1 per move)
        moves = {}
        for index, endpoint in enumerate((move['move']['url'] for move in response['moves'])):
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

        with open(ctx.obj['ASSETS'].joinpath(f"{result['name']}.json"), mode='w', encoding='utf-8') as file_handler:
            json.dump(result, file_handler)

    click.secho(f"Done! A new game file was created in '{ctx.obj['ASSETS']}/'.", fg='bright_green')

@cli.command(context_settings=CONTEXT_SETTINGS, help=style("Create an ASCII image.", fg='bright_green'))
@click.option('--name', type=click.STRING, help=style("Name of a pokemon (English).", fg='bright_yellow'))
@click.option('--id', type=click.STRING, help=style("A pokemon ID.", fg='bright_yellow'))
@click.pass_context
def ascii(ctx, name, id):
    result = None
    query = name or id
    
    # the base api only contains very small sprites,
    # but there's another API which provides higher
    # quality sprites which are only searchable by id
    with ctx.obj['CONSOLE'].status('Creating new ASCII image . . .', spinner='dots3') as _:
        if name:
            query = requests.get(urljoin(ctx.obj['BASE_API'], query)).json()['id']
        
        # TODO: implement ascii function
        image = f"{query}.png"
        response = requests.get(urljoin(ctx.obj['SPRITE_API'], image))

if __name__ == '__main__':
    try:
        cli(obj={})
    except KeyboardInterrupt:
        pass
