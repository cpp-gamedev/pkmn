Write-Host -ForegroundColor Yellow "+++ PKMN Asset Builder +++"

Write-Host "`nAccording to the National Pok\u00e9mon Index, the first 151 entries served in Generation I."
Write-Host "You may use any of these numbers to create new asset files to play this game.`n"

$ID1 = Read-Host "Pokemon ID #1"
$ID2 = Read-Host "Pokemon ID #2"

if ( -not (Test-Path -Path "venv" -PathType Container) )
{
    Write-Host -ForegroundColor Yellow "Creating a new virtual environment . . ."
    python -m venv venv/
    .\venv\Scripts\Activate.ps1
    Write-Host "Installing dependencies . . ."
    python -m pip install --upgrade pip
    python -m pip install -r requirements.txt --only-binary all
}
else
{
    .\venv\Scripts\Activate.ps1
}

python gen_data.py --verbose make --id $ID1 $ID2
python gen_data.py manifest

Write-Host -ForegroundColor Yellow "Done!"

deactivate # virtual environment