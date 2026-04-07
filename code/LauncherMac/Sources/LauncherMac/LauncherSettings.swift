import Foundation

struct Bookmark {
    var name: String = ""
    var ip: String = ""
    var password: String = ""
    var rconPassword: String = ""
    var gameType: Int = 0
}

struct ResolutionEntry {
    let label: String
    let rMode: Int
}

let resolutionList: [ResolutionEntry] = [
    ResolutionEntry(label: "800x600 (4:3) FOV 80.00", rMode: 4),
    ResolutionEntry(label: "960x720 (4:3) FOV 80.00", rMode: 5),
    ResolutionEntry(label: "1024x768 (4:3) FOV 80.00", rMode: 6),
    ResolutionEntry(label: "1152x864 (4:3) FOV 80.00", rMode: 7),
    ResolutionEntry(label: "1280x720 (16:9) FOV 96.42", rMode: 0),
    ResolutionEntry(label: "1280x1024 (5:4) FOV 80.00", rMode: 8),
    ResolutionEntry(label: "1600x1200 (4:3) FOV 80.00", rMode: 9),
    ResolutionEntry(label: "1920x1080 (16:9) FOV 96.42", rMode: 1),
    ResolutionEntry(label: "2560x1440 (16:9) FOV 96.42", rMode: 10),
    ResolutionEntry(label: "3840x2160 (16:9) FOV 96.42", rMode: 11),
]

let maxBookmarks = 4

class LauncherSettings: ObservableObject {
    @Published var ip: String = ""
    @Published var password: String = ""
    @Published var rconPassword: String = ""
    @Published var nickname: String = ""
    @Published var gameType: Int = 0
    @Published var overrideResolution: Bool = false
    @Published var resolutionIndex: Int = 7  // default 1920x1080
    @Published var customWidth: Int = 1920
    @Published var customHeight: Int = 1080
    @Published var bookmarks: [Bookmark] = Array(repeating: Bookmark(), count: maxBookmarks)

    private var isLoading = false

    private var settingsPath: String {
        let dir = LauncherSettings.gameDirectory
        return (dir as NSString).appendingPathComponent("launcher.cfg")
    }

    static var gameDirectory: String {
        // Walk up from the executable until we exit the .app bundle
        var url = URL(fileURLWithPath: ProcessInfo.processInfo.arguments[0]).standardized
        while url.path != "/" {
            if url.pathExtension == "app" {
                return url.deletingLastPathComponent().path
            }
            url = url.deletingLastPathComponent()
        }
        // Fallback: directory containing the executable
        return URL(fileURLWithPath: ProcessInfo.processInfo.arguments[0])
            .deletingLastPathComponent().path
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
                if let g = Int(value), g >= 0, g <= 2 { gameType = g }
            case "override_resolution":
                overrideResolution = (Int(value) ?? 0) != 0
            case "resolution_index":
                if let r = Int(value), r >= 0, r <= resolutionList.count { resolutionIndex = r }
            case "custom_width":
                if let w = Int(value), w > 0 { customWidth = w }
            case "custom_height":
                if let h = Int(value), h > 0 { customHeight = h }
            default:
                for i in 0..<maxBookmarks {
                    let suffix = "_\(i)"
                    if key == "bookmark_name" + suffix { bookmarks[i].name = value }
                    else if key == "bookmark_ip" + suffix { bookmarks[i].ip = value }
                    else if key == "bookmark_pass" + suffix { bookmarks[i].password = value }
                    else if key == "bookmark_rcon" + suffix { bookmarks[i].rconPassword = value }
                    else if key == "bookmark_game" + suffix {
                        if let g = Int(value), g >= 0, g <= 2 { bookmarks[i].gameType = g }
                    }
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
        lines.append("override_resolution=\(overrideResolution ? 1 : 0)")
        lines.append("resolution_index=\(resolutionIndex)")
        lines.append("custom_width=\(customWidth)")
        lines.append("custom_height=\(customHeight)")

        for i in 0..<maxBookmarks {
            if !bookmarks[i].name.isEmpty {
                lines.append("bookmark_name_\(i)=\(bookmarks[i].name)")
                lines.append("bookmark_ip_\(i)=\(bookmarks[i].ip)")
                lines.append("bookmark_pass_\(i)=\(bookmarks[i].password)")
                lines.append("bookmark_rcon_\(i)=\(bookmarks[i].rconPassword)")
                lines.append("bookmark_game_\(i)=\(bookmarks[i].gameType)")
            }
        }

        try? lines.joined(separator: "\n").write(toFile: settingsPath, atomically: true, encoding: .utf8)
    }
}
