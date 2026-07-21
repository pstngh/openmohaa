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
    ResolutionEntry(label: "800x600 (4:3)", rMode: 4, width: 800, height: 600),
    ResolutionEntry(label: "960x720 (4:3)", rMode: 5, width: 960, height: 720),
    ResolutionEntry(label: "1024x768 (4:3)", rMode: 6, width: 1024, height: 768),
    ResolutionEntry(label: "1152x864 (4:3)", rMode: 7, width: 1152, height: 864),
    ResolutionEntry(label: "1280x720 (16:9)", rMode: -1, width: 1280, height: 720),
    ResolutionEntry(label: "1280x1024 (5:4)", rMode: 8, width: 1280, height: 1024),
    ResolutionEntry(label: "1600x1200 (4:3)", rMode: 9, width: 1600, height: 1200),
    ResolutionEntry(label: "1920x1080 (16:9)", rMode: -1, width: 1920, height: 1080),
]

let maxBookmarks = 3
let maxLauncherBots = 62  // MAX_CLIENTS (64) minus the two real-client slots

class LauncherSettings: ObservableObject {
    static let aaDefaultRunSpeed = 250.0
    static let expansionDefaultRunSpeed = 287.0
    static let minRunSpeed = aaDefaultRunSpeed
    static let maxRunSpeed = expansionDefaultRunSpeed
    static let defaultCrosshairLength = 9.0
    static let defaultCrosshairGap = 4.0
    static let defaultCrosshairThickness = 2.0
    static let defaultCrosshairColor = "FFFFFF"

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
    @Published var runSpeed: Double = LauncherSettings.aaDefaultRunSpeed
    @Published var botSniper: Int = 0
    @Published var botStg: Double = 5
    @Published var botDifficulty: Double = 50
    @Published var botManualTuning: Bool = false
    @Published var botReactDelay: String = "0.2"
    @Published var botTurnSpeed: String = "360"
    @Published var botTurnAccel: String = "5"
    @Published var botAimError: String = "40"
    @Published var botAimSettle: String = "0.4"
    @Published var botAimLatency: String = "120"
    @Published var botAimHeightMin: String = "0.32"
    @Published var botAimHeightMax: String = "0.55"
    @Published var botFirespreadScale: String = "2"
    @Published var accuracy: Int = 0  // 0 normal, 1 high, 2 perfect
    @Published var aaLean: Bool = false
    @Published var painAnimations: Bool = true
    @Published var godMode: Bool = false
    @Published var infiniteAmmo: Bool = true
    @Published var forceModels: Bool = false

    // Crosshair tab
    @Published var crosshairEnabled: Bool = false
    @Published var crosshairLength: Double = LauncherSettings.defaultCrosshairLength
    @Published var crosshairGap: Double = LauncherSettings.defaultCrosshairGap
    @Published var crosshairThickness: Double = LauncherSettings.defaultCrosshairThickness
    @Published var crosshairColor: String = LauncherSettings.defaultCrosshairColor

    private var isLoading = false
    private var hasLoaded = false
    private var pendingSave: DispatchWorkItem?
    private static let saveDebounce: TimeInterval = 0.25

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

    init() {
        load()
    }

    func load() {
        guard !hasLoaded else { return }
        hasLoaded = true
        isLoading = true
        defer { isLoading = false }
        guard let content = try? String(contentsOfFile: settingsPath, encoding: .utf8) else { return }

        let configLines = content.components(separatedBy: "\n")
        let settingsVersion = configLines.compactMap { line -> Int? in
            guard line.hasPrefix("settings_version=") else { return nil }
            return Int(line.dropFirst("settings_version=".count))
        }.first ?? 1
        var loadedRunSpeed = false

        for line in configLines {
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
                if let n = Int(value) { botCount = Self.clampedBotCount(n) }
            case "bot_game_type":
                if let g = Int(value), (g == 1 || g == 2) { botGameType = g }
            case "bot_map": botMap = value
            case "bot_team": botTeam = value
            case "player_health":
                if let h = Int(value) { playerHealth = Self.clampedPlayerHealth(h) }
            case "run_speed":
                if let speed = Double(value), speed.isFinite {
                    runSpeed = Self.clampedRunSpeed(speed)
                    loadedRunSpeed = true
                }
            case "bot_sniper":
                if let s = Int(value) { botSniper = Self.clampedPercentage(s) }
            case "bot_stg":
                if let percent = Double(value), percent.isFinite {
                    botStg = Self.clampedPercentage(percent)
                }
            case "bot_difficulty":
                if let d = Double(value), d >= 0, d <= 100 { botDifficulty = d }
            case "bot_manual": botManualTuning = (Int(value) ?? 0) != 0
            case "bot_react_delay": botReactDelay = value
            case "bot_turn_speed":
                if settingsVersion >= 2 {
                    botTurnSpeed = value
                } else {
                    // Version 1 mislabeled this value: it controlled angular
                    // acceleration, while the maximum rate stayed at 360.
                    botTurnAccel = value
                }
            case "bot_turn_accel": botTurnAccel = value
            case "bot_aim_error": botAimError = value
            case "bot_aim_settle": botAimSettle = value
            case "bot_aim_latency": botAimLatency = value
            case "bot_aim_height_min":
                // Version 7 broadens the stock target range downward to match
                // human shot placement. Preserve non-default custom values.
                if settingsVersion < 7,
                   let oldMin = Double(value),
                   abs(oldMin - 0.49) < 0.0001 {
                    botAimHeightMin = "0.32"
                } else {
                    botAimHeightMin = value
                }
            case "bot_aim_height_max":
                // Version 5 moves the stock aim ceiling out of the neck. Only
                // migrate the exact old default; preserve custom values.
                if settingsVersion < 5,
                   let oldMax = Double(value),
                   abs(oldMax - 0.65) < 0.0001 {
                    botAimHeightMax = "0.55"
                } else {
                    botAimHeightMax = value
                }
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
            case "aa_style":
                aaLean = (Int(value) ?? 0) != 0
                if settingsVersion < 3 {
                    // The old checkbox coupled AA movement with disabling the
                    // SH/BT pain animations. Preserve that combination.
                    painAnimations = !aaLean
                }
            case "shbt_pain_anims": painAnimations = (Int(value) ?? 0) != 0
            case "aa_lean": aaLean = (Int(value) ?? 0) != 0
            case "pain_anims": painAnimations = (Int(value) ?? 0) != 0
            case "god_mode": godMode = (Int(value) ?? 0) != 0
            case "infinite_ammo": infiniteAmmo = (Int(value) ?? 0) != 0
            case "force_models": forceModels = (Int(value) ?? 0) != 0
            // Crosshair tab
            case "crosshair_enabled": crosshairEnabled = (Int(value) ?? 0) != 0
            case "crosshair_length":
                if let n = Double(value), n.isFinite { crosshairLength = min(max(n, 2), 32) }
            case "crosshair_gap":
                if let n = Double(value), n.isFinite { crosshairGap = min(max(n, 1), 20) }
            case "crosshair_thickness":
                if let n = Double(value), n.isFinite { crosshairThickness = min(max(n, 1), 8) }
            case "crosshair_color": crosshairColor = Self.validatedCrosshairColor(value)
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

        if !loadedRunSpeed {
            runSpeed = gameType == 0 ? Self.aaDefaultRunSpeed : Self.expansionDefaultRunSpeed
        }
    }

    func save() {
        guard !isLoading else { return }
        pendingSave?.cancel()

        let workItem = DispatchWorkItem { [weak self] in
            guard let self = self else { return }
            self.pendingSave = nil
            self.writeSettings()
        }
        pendingSave = workItem
        DispatchQueue.main.asyncAfter(
            deadline: .now() + Self.saveDebounce,
            execute: workItem
        )
    }

    /// Persist any queued edits before the app becomes inactive or closes.
    func flushPendingSave() {
        guard !isLoading, pendingSave != nil else { return }
        pendingSave?.cancel()
        pendingSave = nil
        writeSettings()
    }

    /// Bypass the debounce when launching, because the app terminates immediately.
    func saveImmediately() {
        guard !isLoading else { return }
        pendingSave?.cancel()
        pendingSave = nil
        writeSettings()
    }

    private func writeSettings() {
        guard !isLoading else { return }
        var lines: [String] = []
        lines.append("settings_version=7")
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
        lines.append("player_health=\(Self.clampedPlayerHealth(playerHealth))")
        lines.append("run_speed=\(Int(Self.clampedRunSpeed(runSpeed)))")
        lines.append("bot_sniper=\(Self.clampedPercentage(botSniper))")
        lines.append("bot_stg=\(Int(Self.clampedPercentage(botStg)))")
        lines.append("bot_difficulty=\(Int(botDifficulty))")
        lines.append("bot_manual=\(botManualTuning ? 1 : 0)")
        lines.append("bot_react_delay=\(botReactDelay)")
        lines.append("bot_turn_speed=\(botTurnSpeed)")
        lines.append("bot_turn_accel=\(botTurnAccel)")
        lines.append("bot_aim_error=\(botAimError)")
        lines.append("bot_aim_settle=\(botAimSettle)")
        lines.append("bot_aim_latency=\(botAimLatency)")
        lines.append("bot_aim_height_min=\(botAimHeightMin)")
        lines.append("bot_aim_height_max=\(botAimHeightMax)")
        lines.append("bot_firespread_scale=\(botFirespreadScale)")
        lines.append("accuracy=\(accuracy)")
        lines.append("aa_lean=\(aaLean ? 1 : 0)")
        lines.append("pain_anims=\(painAnimations ? 1 : 0)")
        lines.append("god_mode=\(godMode ? 1 : 0)")
        lines.append("infinite_ammo=\(infiniteAmmo ? 1 : 0)")
        lines.append("force_models=\(forceModels ? 1 : 0)")

        // Crosshair tab
        lines.append("crosshair_enabled=\(crosshairEnabled ? 1 : 0)")
        lines.append("crosshair_length=\(Int(crosshairLength))")
        lines.append("crosshair_gap=\(Int(crosshairGap))")
        lines.append("crosshair_thickness=\(Int(crosshairThickness))")
        lines.append("crosshair_color=\(Self.validatedCrosshairColor(crosshairColor))")

        for i in 0..<maxBookmarks {
            if !bookmarks[i].name.isEmpty {
                lines.append("bookmark_name_\(i)=\(bookmarks[i].name)")
                lines.append("bookmark_ip_\(i)=\(bookmarks[i].ip)")
                lines.append("bookmark_pass_\(i)=\(bookmarks[i].password)")
                lines.append("bookmark_rcon_\(i)=\(bookmarks[i].rconPassword)")
            }
        }

        do {
            try lines.joined(separator: "\n").write(
                toFile: settingsPath,
                atomically: true,
                encoding: .utf8
            )
        } catch {
            NSLog("Launcher settings could not be saved to %@: %@", settingsPath, error.localizedDescription)
        }
    }

    static func validatedCrosshairColor(_ value: String) -> String {
        let trimmed = value.trimmingCharacters(in: .whitespacesAndNewlines)
        let hex = trimmed.hasPrefix("#") ? String(trimmed.dropFirst()) : trimmed
        guard hex.count == 6, Int(hex, radix: 16) != nil else {
            return defaultCrosshairColor
        }
        return hex.uppercased()
    }

    static func clampedRunSpeed(_ value: Double) -> Double {
        min(max(value.rounded(), minRunSpeed), maxRunSpeed)
    }

    static func clampedPlayerHealth(_ value: Int) -> Int {
        min(max(value, 1), 1000)
    }

    static func clampedPercentage(_ value: Int) -> Int {
        min(max(value, 0), 100)
    }

    static func clampedPercentage(_ value: Double) -> Double {
        min(max(value.rounded(), 0), 100)
    }

    func resetCrosshair() {
        crosshairLength = Self.defaultCrosshairLength
        crosshairGap = Self.defaultCrosshairGap
        crosshairThickness = Self.defaultCrosshairThickness
        crosshairColor = Self.defaultCrosshairColor
    }
}

struct BotTuning {
    var reactDelay: String
    var turnSpeed: String
    var turnAccel: String
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
            turnSpeed: String(format: "%.0f", Self.lerp3(t, 240, 360, 540)),
            turnAccel: String(format: "%.0f", Self.lerp3(t, 3, 5, 10)),
            aimError: String(format: "%.0f", Self.lerp3(t, 60, 40, 20)),
            aimSettle: String(format: "%.2f", Self.lerp3(t, 0.6, 0.4, 0.2)),
            aimLatency: String(format: "%.0f", Self.lerp3(t, 250, 120, 40)),
            // Bots do not accumulate weapon bloom, so even the hardest preset
            // needs more than stock base spread to avoid aimbot-like accuracy.
            spreadScale: String(format: "%.1f", Self.lerp3(t, 6.0, 3.5, 2.0))
        )
    }

    /// Values passed to the game: manual fields when manual tuning is on, slider-derived otherwise.
    func effectiveTuning() -> BotTuning {
        if botManualTuning {
            return BotTuning(
                reactDelay: botReactDelay,
                turnSpeed: botTurnSpeed,
                turnAccel: botTurnAccel,
                aimError: botAimError,
                aimSettle: botAimSettle,
                aimLatency: botAimLatency,
                spreadScale: botFirespreadScale
            )
        }
        return derivedTuning()
    }

    private static func clampedNumber(_ text: String, min: Double, max: Double, fallback: Double) -> String {
        guard let value = Double(text), value.isFinite else { return String(fallback) }
        return String(Swift.min(Swift.max(value, min), max))
    }

    /// Validate manual aim-height input before passing it to the engine. The
    /// engine repeats these checks so direct console/config use is safe too.
    func effectiveAimHeights() -> (min: String, max: String) {
        let minValue = Double(Self.clampedNumber(botAimHeightMin, min: 0, max: 1, fallback: 0.32)) ?? 0.32
        let maxValue = Double(Self.clampedNumber(botAimHeightMax, min: 0, max: 1, fallback: 0.55)) ?? 0.55
        return minValue <= maxValue
            ? (String(minValue), String(maxValue))
            : (String(maxValue), String(minValue))
    }

    /// Return launch-safe values while leaving the user's editable text intact.
    func validatedTuning() -> BotTuning {
        let tuning = effectiveTuning()
        return BotTuning(
            reactDelay: Self.clampedNumber(tuning.reactDelay, min: 0, max: 10, fallback: 0.2),
            turnSpeed: Self.clampedNumber(tuning.turnSpeed, min: 1, max: 1080, fallback: 360),
            turnAccel: Self.clampedNumber(tuning.turnAccel, min: 0.1, max: 100, fallback: 5),
            aimError: Self.clampedNumber(tuning.aimError, min: 0, max: 400, fallback: 40),
            aimSettle: Self.clampedNumber(tuning.aimSettle, min: 0, max: 10, fallback: 0.4),
            aimLatency: Self.clampedNumber(tuning.aimLatency, min: 0, max: 2000, fallback: 120),
            spreadScale: Self.clampedNumber(tuning.spreadScale, min: 0, max: 10, fallback: 2)
        )
    }

    static func clampedBotCount(_ value: Int) -> Int {
        return min(max(value, 1), maxLauncherBots)
    }

    func validatedBotCount() -> Int {
        return Self.clampedBotCount(botCount)
    }
}
