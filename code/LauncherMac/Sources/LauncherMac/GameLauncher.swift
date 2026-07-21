import Foundation
import AppKit

struct GameLauncher {
    private static let dmFlagInfiniteAmmo = 1 << 14
    private static let dmFlagOldSniper = 1 << 19
    private static let dmFlagDisallowKar98Mortar = 1 << 20
    private static let dmFlagNoRocket = 1 << 26
    private static let dmFlagNoLandmine = 1 << 28

    static func launch(settings: LauncherSettings) {
        let gameDir = LauncherSettings.gameDirectory
        let executable = (gameDir as NSString).appendingPathComponent("openmohaa")

        guard FileManager.default.fileExists(atPath: executable) else {
            showNotFoundAlert("openmohaa")
            return
        }

        var args: [String] = []
        args.append(contentsOf: ["+set", "fs_homepath", "."])
        // Connect always launches Allied Assault (com_target_game 0)
        args.append(contentsOf: ["+set", "com_target_game", "0"])

        if !settings.ip.isEmpty {
            args.append(contentsOf: ["+connect", settings.ip])
        }
        if !settings.password.isEmpty {
            args.append(contentsOf: ["+set", "password", settings.password])
        }
        if !settings.rconPassword.isEmpty {
            args.append(contentsOf: ["+set", "rconpassword", settings.rconPassword])
        }
        if !settings.nickname.isEmpty {
            args.append(contentsOf: ["+set", "name", settings.nickname])
        }

        appendCommonArgs(&args, settings: settings)
        appendResolutionArgs(&args, settings: settings)

        // Auto-save bookmark if IP matches
        for i in 0..<maxBookmarks {
            if !settings.bookmarks[i].name.isEmpty && settings.bookmarks[i].ip == settings.ip {
                settings.bookmarks[i].password = settings.password
                settings.bookmarks[i].rconPassword = settings.rconPassword
                break
            }
        }
        settings.saveImmediately()

        run(executable: executable, args: args, cwd: gameDir)
    }

    static func launchBots(settings: LauncherSettings) {
        let gameDir = LauncherSettings.gameDirectory
        let bundlePath = (gameDir as NSString).appendingPathComponent("mohbots.app/Contents/MacOS/mohbots")

        guard FileManager.default.fileExists(atPath: bundlePath) else {
            showNotFoundAlert("mohbots.app")
            return
        }

        var args: [String] = []
        args.append(contentsOf: ["+set", "fs_homepath", "."])
        args.append(contentsOf: ["+set", "com_target_game", "\(settings.gameType)"])
        args.append(contentsOf: ["+set", "g_gametype", "\(settings.botGameType)"])
        args.append(contentsOf: ["+set", "dmflags", "\(botDmFlags(settings: settings))"])
        if settings.botGameType == 2 {
            // Spearhead and Breakthrough default team games to 15-second
            // spawn waves. Bot TDM should use immediate AA-style respawning.
            args.append(contentsOf: ["+set", "sv_team_spawn_interval", "0"])
        }
        let botCount = settings.validatedBotCount()
        settings.botCount = botCount
        args.append(contentsOf: ["+set", "sv_bots", "\(botCount)"])
        // A listen server with sv_maxclients <= 1 makes the loading screen stop
        // on a "Continue" button before entering the map. Bots are added on top
        // of sv_maxclients, so a value > 1 skips that prompt and drops us
        // straight into the match without losing any bot slots.
        args.append(contentsOf: ["+set", "sv_maxclients", "2"])
        // Bot matches are private LAN sessions. The second real-client slot is
        // reserved for one chase spectator; bots use their own slots.
        args.append(contentsOf: ["+set", "sv_gamespy", "0"])
        args.append(contentsOf: ["+set", "sv_lanOnly", "1"])
        args.append(contentsOf: ["+set", "g_bot_lan_observer", "1"])
        args.append(contentsOf: ["+set", "g_spectatefollow_firstperson", "0"])
        args.append(contentsOf: ["+set", "rconpassword", "bot"])

        if !settings.nickname.isEmpty {
            args.append(contentsOf: ["+set", "name", settings.nickname])
        }

        args.append(contentsOf: ["+set", "g_bot_team", settings.botTeam])
        let playerHealth = LauncherSettings.clampedPlayerHealth(settings.playerHealth)
        settings.playerHealth = playerHealth
        args.append(contentsOf: ["+set", "g_playerdmhealth", "\(playerHealth)"])
        args.append(contentsOf: [
            "+set", "sv_runspeed",
            "\(Int(LauncherSettings.clampedRunSpeed(settings.runSpeed)))",
        ])

        // Bot difficulty (slider-derived unless manual tuning is on)
        let tuning = settings.validatedTuning()
        args.append(contentsOf: ["+set", "g_bot_attack_react_min_delay", tuning.reactDelay])
        args.append(contentsOf: ["+set", "g_bot_turn_speed", tuning.turnSpeed])
        args.append(contentsOf: ["+set", "g_bot_turn_accel", tuning.turnAccel])
        args.append(contentsOf: ["+set", "g_bot_aim_error", tuning.aimError])
        args.append(contentsOf: ["+set", "g_bot_aim_settle_time", tuning.aimSettle])
        args.append(contentsOf: ["+set", "g_bot_aim_latency", tuning.aimLatency])
        args.append(contentsOf: ["+set", "g_bot_spread", tuning.spreadScale])
        let botSniper = LauncherSettings.clampedPercentage(settings.botSniper)
        let botStg = LauncherSettings.clampedPercentage(settings.botStg)
        settings.botSniper = botSniper
        settings.botStg = botStg
        args.append(contentsOf: ["+set", "g_bot_sniper", "\(botSniper)"])
        args.append(contentsOf: ["+set", "g_bot_stg", "\(Int(botStg))"])

        // Bot aim shape
        let aimHeights = settings.effectiveAimHeights()
        args.append(contentsOf: ["+set", "g_bot_aim_height_min", aimHeights.min])
        args.append(contentsOf: ["+set", "g_bot_aim_height_max", aimHeights.max])

        // Player accuracy: 0 normal, 1 high, 2 perfect
        args.append(contentsOf: ["+set", "g_accuracy", "\(settings.accuracy)"])
        args.append(contentsOf: ["+set", "g_aalean", settings.aaLean ? "1" : "0"])
        args.append(contentsOf: [
            "+set", "g_painanims",
            settings.painAnimations ? "1" : "0",
        ])
        args.append(contentsOf: ["+set", "g_godmode", settings.godMode ? "1" : "0"])
        // Server-side telemetry recorder; only meaningful when hosting.
        args.append(contentsOf: ["+set", "g_movelog", settings.moveLog ? "1" : "0"])

        appendCommonArgs(&args, settings: settings)
        appendResolutionArgs(&args, settings: settings)

        // Map must be last
        if !settings.botMap.isEmpty {
            args.append(contentsOf: ["+map", settings.botMap])
        }

        settings.saveImmediately()

        run(executable: bundlePath, args: args, cwd: gameDir)
    }

    private static func botDmFlags(settings: LauncherSettings) -> Int {
        // Bot matches always exclude rockets and landmines. Expansion games
        // additionally retain AA sniper behavior and replace the Kar98 mortar
        // with the shotgun. Infinite ammo is the only optional preset flag.
        var flags = dmFlagNoRocket | dmFlagNoLandmine
        if settings.gameType != 0 {
            flags |= dmFlagOldSniper | dmFlagDisallowKar98Mortar
        }
        if settings.infiniteAmmo {
            flags |= dmFlagInfiniteAmmo
        }
        return flags
    }

    private static func appendCommonArgs(_ args: inout [String], settings: LauncherSettings) {
        args.append(contentsOf: ["+set", "cl_playintro", "0"])
        args.append(contentsOf: ["+set", "r_primitives", "2"])
        args.append(contentsOf: ["+set", "r_uselod", "0"])
        args.append(contentsOf: ["+set", "cg_forceModel", settings.forceModels ? "1" : "0"])

        // The engine suppresses the stock crosshair whenever its overlay is
        // active. Restore ui_crosshair only when disabling the overlay, which
        // also repairs settings archived by older launcher versions.
        args.append(contentsOf: ["+set", "cg_crosshair_overlay", settings.crosshairEnabled ? "1" : "0"])
        if !settings.crosshairEnabled {
            args.append(contentsOf: ["+set", "ui_crosshair", "1"])
        }
        args.append(contentsOf: ["+set", "cg_crosshair_length", "\(Int(settings.crosshairLength))"])
        args.append(contentsOf: ["+set", "cg_crosshair_gap", "\(Int(settings.crosshairGap))"])
        args.append(contentsOf: ["+set", "cg_crosshair_thickness", "\(Int(settings.crosshairThickness))"])
        args.append(contentsOf: [
            "+set", "cg_crosshair_color",
            LauncherSettings.validatedCrosshairColor(settings.crosshairColor),
        ])
    }

    private static func appendResolutionArgs(_ args: inout [String], settings: LauncherSettings) {
        if settings.resolutionIndex < resolutionList.count {
            let res = resolutionList[settings.resolutionIndex]
            args.append(contentsOf: ["+set", "r_mode", "\(res.rMode)"])
            if res.rMode == -1, let w = res.width, let h = res.height {
                args.append(contentsOf: ["+set", "r_customwidth", "\(w)"])
                args.append(contentsOf: ["+set", "r_customheight", "\(h)"])
            }
        }
    }

    private static func run(executable: String, args: [String], cwd: String) {
        let process = Process()
        process.executableURL = URL(fileURLWithPath: executable)
        process.arguments = args
        process.currentDirectoryURL = URL(fileURLWithPath: cwd)

        do {
            try process.run()
            NSApplication.shared.terminate(nil)
        } catch {
            showAlert("Failed to launch game.")
        }
    }

    private static func showNotFoundAlert(_ name: String) {
        if LauncherSettings.isTranslocated {
            showAlert(
                "\(name) not found.\n\n"
                + "macOS App Translocation is active. This happens when the app "
                + "is launched without clearing its quarantine flag.\n\n"
                + "To fix this, run install.command first, or run this in Terminal "
                + "in the game folder:\n\nxattr -cr ."
            )
        } else {
            showAlert("\(name) not found. Make sure launcher.app is in the same folder as \(name).")
        }
    }

    private static func showAlert(_ message: String) {
        let alert = NSAlert()
        alert.messageText = message
        alert.alertStyle = .warning
        alert.runModal()
    }
}
