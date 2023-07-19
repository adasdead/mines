# Copyright (c) 2023 adasdead
# This software is licensed under the MIT License.

import os
import platform
import requests
import zipfile

url = "https://dl-game-sdk.discordapp.net/2.5.6/discord_game_sdk.zip"
filename = "discord_game_sdk.zip"
output = "discord_sdk"

def is_system_x64():
    return platform.machine().endswith('64')

def download_sdk_zip():
    print(f"Download \"{filename}\"")

    try:
        response = requests.get(url)

        with open(filename, "wb") as f:
            f.write(response.content)

    except requests.exceptions.RequestException as e:
        print(f"An error occurred while downloading Discord SDK: {e}")
        exit()

def unpack_sdk_zip():
    print(f"Unpack \"{filename}\" archive to \"{output}\" directory")

    try:
        with zipfile.ZipFile(filename, "r") as z:
            z.extractall(output)

    except zipfile.BadZipFile as e:
        print(f"An error occurred while unpacking the archive: {e}")
        exit()

def delete_sdk_zip():
    print(f"Delete \"{filename}\"")
    os.remove(filename)

if __name__ == '__main__':
    if not is_system_x64():
        print("Only x64 architecture is supported for SDK")
        exit()

    if os.name != 'nt':
        print("Currently only supported on Windows")
        exit()

    download_sdk_zip()
    unpack_sdk_zip()
    delete_sdk_zip()