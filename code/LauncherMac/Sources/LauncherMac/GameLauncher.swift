import Foundation
import AppKit

struct GameLauncher {
    static func launch(settings: LauncherSettings) {
        let gameDir = LauncherSettings.gameDirectory
        let executable = (gameDir as NSString).appendingPathComponent("moh")

        guard FileManager.default.fileExists(atPath: executable) else {
            showAlert("Game executable not found. Make sure Launcher.app is in the same folder as moh.")
            return
        }

        var args: [String] = []
        args.append(contentsOf: ["+set", "fs_homepath", "."])
        args.append(contentsOf: ["+set", "com_target_game", "\(settings.gameType)"])

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

        if settings.overrideResolution {
            if settings.resolutionIndex < resolutionList.count {
                let res = resolutionList[settings.resolutionIndex]
                args.append(contentsOf: ["+set", "r_mode", "\(res.rMode)"])
            } else {
                // Custom resolution
                args.append(contentsOf: ["+set", "r_mode", "-1"])
                args.append(contentsOf: ["+set", "r_customwidth", "\(settings.customWidth)"])
                args.append(contentsOf: ["+set", "r_customheight", "\(settings.customHeight)"])
            }
        }

        // Auto-save bookmark if IP matches
        for i in 0..<maxBookmarks {
            if !settings.bookmarks[i].name.isEmpty && settings.bookmarks[i].ip == settings.ip {
                settings.bookmarks[i].password = settings.password
                settings.bookmarks[i].rconPassword = settings.rconPassword
                settings.bookmarks[i].gameType = settings.gameType
                break
            }
        }
        settings.save()

        let process = Process()
        process.executableURL = URL(fileURLWithPath: executable)
        process.arguments = args
        process.currentDirectoryURL = URL(fileURLWithPath: gameDir)

        do {
            try process.run()
            NSApplication.shared.terminate(nil)
        } catch {
            showAlert("Failed to launch game.")
        }
    }

    private static func showAlert(_ message: String) {
        let alert = NSAlert()
        alert.messageText = message
        alert.alertStyle = .warning
        alert.runModal()
    }
}
