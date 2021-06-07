#!/bin/bash

echo "+++ PKMN Asset Builder +++"

echo -e "\nAccording to the National Pok\u00e9mon Index, the first 151 entries served in Generation I."
echo -e "You may use any of these numbers to create new asset files to play this game.\n"

echo -n "Pokemon ID #1: "
read id1

echo -n "Pokemon ID #2: "
read id2

if [[ ! -d "venv/" ]]; then
    echo "Creating a new virtual environment . . ."
    python3 -m venv venv/
    source venv/bin/activate
    echo "Installing dependencies . . ."
    python3 -m pip install --upgrade pip
    python3 -m pip install -r requirements.txt --only-binary all
else
    source venv/bin/activate
fi

python3 gen_data.py --verbose make --id $id1 $id2
python3 gen_data.py manifest

echo "Done!"

deactivate # virtual environment
