import Foundation

struct Bookmark {
    var name: String = ""
    var ip: String = ""
    var password: String = ""
    var rconPassword: String = ""
}

struct ResolutionEntry {
    let label: String
    let rMode: Int
    let width: Int?
    let height: Int?

    init(label: String, rMode: Int, width: Int? = nil, height: Int? = nil) {
        self.label = label
        self.rMode = rMode
        self.width = width
        self.height = height
    }
}

let resolutionList: [ResolutionEntry] = [
    ResolutionEntry(label: "800x600 (4:3)", rMode: 4),
    ResolutionEntry(label: "960x720 (4:3)", rMode: 5),
    ResolutionEntry(label: "1024x768 (4:3)", rMode: 6),
    ResolutionEntry(label: "1152x864 (4:3)", rMode: 7),
    ResolutionEntry(label: "1280x720 (16:9)", rMode: -1, width: 1280, height: 720),
    ResolutionEntry(label: "1280x1024 (5:4)", rMode: 8),
    ResolutionEntry(label: "1600x1200 (4:3)", rMode: 9),
    ResolutionEntry(label: "1920x1080 (16:9)", rMode: -1, width: 1920, height: 1080),
]

let maxBookmarks = 3

class LauncherSettings: ObservableObject {
    // Connect tab
    @Published var ip: String = ""
    @Published var password: String = ""
    @Published var rconPassword: String = ""
    @Published var nickname: String = ""
    @Published var gameType: Int = 0
    @Published var resolutionIndex: Int = 7  // default 1920x1080
    @Published var bookmarks: [Bookmark] = Array(repeating: Bookmark(), count: maxBookmarks)

    // Bots tab
    @Published var botCount: Int = 3
    @Published var botGameType: Int = 1  // FFA
    @Published var botMap: String = "dm/downladder"
    @Published var botTeam: String = "axis"
    @Published var playerHealth: Int = 175
    @Published var botSniper: Int = 0
    @Published var botDifficulty: Double = 50
    @Published var botManualTuning: Bool = false
    @Published var botReactDelay: String = "0.2"
    @Published var botTurnSpeed: String = "5"
    @Published var botAimError: String = "40"
    @Published var botAimSettle: String = "0.4"
    @Published var botAimLatency: String = "120"
    @Published var botAimHeightMin: String = "0.49"
    @Published var botAimHeightMax: String = "0.65"
    @Published var botFirespreadScale: String = "2"
    @Published var accuracy: Int = 0  // 0 normal, 1 high, 2 perfect
    @Published var aaStyle: Bool = false
    @Published var godMode: Bool = false

    private var isLoading = false

    private var settingsPath: String {
        let dir = LauncherSettings.gameDirectory
        return (dir as NSString).appendingPathComponent("launcher.cfg")
    }

    static var gameDirectory: String {
        // Parent directory of launcher.app
        return Bundle.main.bundleURL.deletingLastPathComponent().path
    }

    /// Returns true if the app is running from a translocated path.
    /// macOS App Translocation copies quarantined apps to a random temp
    /// directory, hiding sibling files (mohbots.app, moh, etc.).
    /// Running install.command (xattr -cr .) prevents this.
    static var isTranslocated: Bool {
        return Bundle.main.bundlePath.contains("/AppTranslocation/")
    }

    func load() {
        isLoading = true
        defer { isLoading = false }
        guard let content = try? String(contentsOfFile: settingsPath, encoding: .utf8) else { return }

        for line in content.components(separatedBy: "\n") {
            guard let eqIndex = line.firstIndex(of: "=") else { continue }
            let key = String(line[line.startIndex..<eqIndex])
            let value = String(line[line.index(after: eqIndex)...])

            switch key {
            case "ip": ip = value
            case "password": password = value
            case "rcon": rconPassword = value
            case "nickname": nickname = value
            case "game":
                // Spearhead (1) was dropped from the launcher — fold it into
                // Breakthrough (2), which covers the same content.
                if let g = Int(value), g >= 0, g <= 2 { gameType = (g == 1 ? 2 : g) }
            case "resolution_index":
                if let r = Int(value), r >= 0, r < resolutionList.count { resolutionIndex = r }
            // Bots tab
            case "bot_count":
                if let n = Int(value), n >= 1, n <= 63 { botCount = n }
            case "bot_game_type":
                if let g = Int(value), (g == 1 || g == 2 || g == 4) { botGameType = g }
            case "bot_map": botMap = value
            case "bot_team": botTeam = value
            case "player_health":
                if let h = Int(value), h > 0 { playerHealth = h }
            case "bot_sniper":
                if let s = Int(value), s >= 0 { botSniper = s }
            case "bot_difficulty":
                if let d = Double(value), d >= 0, d <= 100 { botDifficulty = d }
            case "bot_manual": botManualTuning = (Int(value) ?? 0) != 0
            case "bot_react_delay": botReactDelay = value
            case "bot_turn_speed": botTurnSpeed = value
            case "bot_aim_error": botAimError = value
            case "bot_aim_settle": botAimSettle = value
            case "bot_aim_latency": botAimLatency = value
            case "bot_aim_height_min": botAimHeightMin = value
            case "bot_aim_height_max": botAimHeightMax = value
            case "bot_firespread_scale": botFirespreadScale = value
            case "accuracy":
                if let a = Int(value), a >= 0, a <= 2 { accuracy = a }
            case "player_spread":
                // migrate old string values
                switch value {
                case "capped": accuracy = 0
                case "laser":  accuracy = 1
                case "perfect": accuracy = 2
                default: break
                }
            case "aa_style": aaStyle = (Int(value) ?? 0) != 0
            case "god_mode": godMode = (Int(value) ?? 0) != 0
            default:
                for i in 0..<maxBookmarks {
                    let suffix = "_\(i)"
                    if key == "bookmark_name" + suffix { bookmarks[i].name = value }
                    else if key == "bookmark_ip" + suffix { bookmarks[i].ip = value }
                    else if key == "bookmark_pass" + suffix { bookmarks[i].password = value }
                    else if key == "bookmark_rcon" + suffix { bookmarks[i].rconPassword = value }
                }
            }
        }
    }

    func save() {
        guard !isLoading else { return }
        var lines: [String] = []
        lines.append("ip=\(ip)")
        lines.append("password=\(password)")
        lines.append("rcon=\(rconPassword)")
        lines.append("nickname=\(nickname)")
        lines.append("game=\(gameType)")
        lines.append("resolution_index=\(resolutionIndex)")

        // Bots tab
        lines.append("bot_count=\(botCount)")
        lines.append("bot_game_type=\(botGameType)")
        lines.append("bot_map=\(botMap)")
        lines.append("bot_team=\(botTeam)")
        lines.append("player_health=\(playerHealth)")
        lines.append("bot_sniper=\(botSniper)")
        lines.append("bot_difficulty=\(Int(botDifficulty))")
        lines.append("bot_manual=\(botManualTuning ? 1 : 0)")
        lines.append("bot_react_delay=\(botReactDelay)")
        lines.append("bot_turn_speed=\(botTurnSpeed)")
        lines.append("bot_aim_error=\(botAimError)")
        lines.append("bot_aim_settle=\(botAimSettle)")
        lines.append("bot_aim_latency=\(botAimLatency)")
        lines.append("bot_aim_height_min=\(botAimHeightMin)")
        lines.append("bot_aim_height_max=\(botAimHeightMax)")
        lines.append("bot_firespread_scale=\(botFirespreadScale)")
        lines.append("accuracy=\(accuracy)")
        lines.append("aa_style=\(aaStyle ? 1 : 0)")
        lines.append("god_mode=\(godMode ? 1 : 0)")

        for i in 0..<maxBookmarks {
            if !bookmarks[i].name.isEmpty {
                lines.append("bookmark_name_\(i)=\(bookmarks[i].name)")
                lines.append("bookmark_ip_\(i)=\(bookmarks[i].ip)")
                lines.append("bookmark_pass_\(i)=\(bookmarks[i].password)")
                lines.append("bookmark_rcon_\(i)=\(bookmarks[i].rconPassword)")
            }
        }

        try? lines.joined(separator: "\n").write(toFile: settingsPath, atomically: true, encoding: .utf8)
    }
}

struct BotTuning {
    var reactDelay: String
    var turnSpeed: String
    var aimError: String
    var aimSettle: String
    var aimLatency: String
    var spreadScale: String
}

extension LauncherSettings {
    /// Piecewise-linear interpolation through the casual / default / esports anchors.
    private static func lerp3(_ t: Double, _ casual: Double, _ mid: Double, _ pro: Double) -> Double {
        if t <= 50 {
            return casual + (mid - casual) * (t / 50)
        }
        return mid + (pro - mid) * ((t - 50) / 50)
    }

    /// Bot tuning derived from the difficulty slider (0 = casual, 50 = default, 100 = esports).
    func derivedTuning() -> BotTuning {
        let t = botDifficulty
        return BotTuning(
            reactDelay: String(format: "%.2f", Self.lerp3(t, 0.35, 0.2, 0.1)),
            turnSpeed: String(format: "%.0f", Self.lerp3(t, 3, 5, 10)),
            aimError: String(format: "%.0f", Self.lerp3(t, 60, 40, 20)),
            aimSettle: String(format: "%.2f", Self.lerp3(t, 0.6, 0.4, 0.2)),
            aimLatency: String(format: "%.0f", Self.lerp3(t, 250, 120, 40)),
            // Static bullet spread as a multiple of the weapon's base spread
            // (no bloom buildup): 1 = stock accuracy, casual bots spray wide.
            spreadScale: String(format: "%.1f", Self.lerp3(t, 4.0, 2.0, 1.0))
        )
    }

    /// Values passed to the game: manual fields when manual tuning is on, slider-derived otherwise.
    func effectiveTuning() -> BotTuning {
        if botManualTuning {
            return BotTuning(
                reactDelay: botReactDelay,
                turnSpeed: botTurnSpeed,
                aimError: botAimError,
                aimSettle: botAimSettle,
                aimLatency: botAimLatency,
                spreadScale: botFirespreadScale
            )
        }
        return derivedTuning()
    }
}
