# Running the game

## Game selection

**Medal of Honor: Allied Assault** is the default game, but expansions are also supported.

### Start using launchers

Base game and expansions can be started from one of the 3 launchers:

- `launch_openmohaa_base`, use this to play **Medal of Honor: Allied Assault**
- `launch_openmohaa_spearhead`, use this to play **Medal of Honor: Allied Assault: Spearhead**
- `launch_openmohaa_breakthrough`, use this to play **Medal of Honor: Allied Assault: Breakthrough**

### Start using the macOS launcher

On Apple silicon Macs with macOS 15 or later, `launcher.app` in the game folder joins a server in Allied Assault, directly or from one of three saved shortcuts, or starts a local bot match in any of the three games with cheats enabled so `dog` and the other cheats work. It also sets the size and color of the crosshair and checks the retail pak files of each installed game. It keeps its settings in `launcher.cfg` beside it, and games it starts keep their configuration and other data in the game folder.

If macOS refuses to open a downloaded build, open `install.command` in the game folder first. To build the launcher on a Mac, run `code/LauncherMac/bundle.sh` with the game folder as argument. The launcher starts the `openmohaa` binary beside it, so build the game for it with `-DBUILD_MACOS_APP=OFF`, as the release build does, instead of as `openmohaa.app`.

### Start from the command-line

**Spearhead** and **Breakthrough** are supported in OpenMoHAA using the `com_target_game` variable.

To change the target game, append the following command-line arguments to the `openmohaa` and `omohaaded` executable:

- `+set com_target_game 0` for the default base game (mohaa, uses `main` folder)
- `+set com_target_game 1` for the Spearhead expansion (mohaas, uses `mainta` folder)
- `+set com_target_game 2` for the Breakthrough expansion (mohaab, uses `maintt` folder)

OpenMoHAA will also use the correct network protocol version accordingly. The default value of `com_target_game` is 0.

On Windows, a shortcut can be created to the `openmohaa` executable, with the command-line argument appended from above to play an expansion.

### Using a demo version

The argument `+set com_target_demo 1` must be appended to command-line to play the game or host a server using demo assets. Allied Assault, Spearhead and Breakthrough demos are supported.

## User data location

All user-writable data, like configuration files, the console logfile, saves,
screenshots and demos, is stored in the game installation folder, in the
subdirectory of the game being used: `main` for the base game or
`mainta`/`maintt` for the expansions. Configs are in `configs` there:
`omconfig.cfg` for the game, `omconfig_server.cfg` for the dedicated server, or
the file named by `+set config`. For example, an installation at
`/path/to/mohaa` stores the base-game config at
`/path/to/mohaa/main/configs/omconfig.cfg`. The installation folder is
`fs_basepath`: the folder of the executable, unless `fs_basepath` is set on
the command line or the executable is started without a folder in its path,
in which case it is the working directory. It must be writable.

After upgrading from a build that stored user data in `%APPDATA%\openmohaa`
(Windows), `~/.local/share/openmohaa` or `~/.openmohaa` (Linux) or `~/Library/Application Support/openmohaa`
(macOS), copy its `main`, `mainta` and `maintt` contents into the installation
folder to keep your settings, saves and demos.

If necessary, all user data except configs can be moved by setting
`fs_homepath` on the command line. The value can be a relative path (relative
to the current working directory) or an absolute path. Examples:
- `+set fs_homepath Z:\openmohaa_data` data will be written inside the fully qualified path `Z:\openmohaa_data`
- `+set fs_homepath homedata` will use the subfolder `homedata` in the process current working directory to write data (will be created automatically)

Note that the configuration file isn't created nor written automatically on a dedicated server (**omohaaded**).

## Configuration

For more settings such as configuring bots, see [Configuration and commands](../03-configuration/01-configuration.md).
