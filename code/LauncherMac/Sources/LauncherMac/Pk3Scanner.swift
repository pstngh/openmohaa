import SwiftUI
import CommonCrypto

enum Pk3Status {
    case verified
    case corrupt
    case unknown
    case scanning
}

struct Pk3Info: Identifiable {
    let id = UUID()
    let folder: String
    let filename: String
    var size: UInt64 = 0
    var md5: String = ""
    var status: Pk3Status = .scanning

    var key: String { "\(folder.lowercased())/\(filename.lowercased())" }

    var sizeText: String {
        if size >= 1_073_741_824 {
            return String(format: "%.1f GB", Double(size) / 1_073_741_824)
        } else if size >= 1_048_576 {
            return String(format: "%.1f MB", Double(size) / 1_048_576)
        } else if size >= 1024 {
            return String(format: "%.1f KB", Double(size) / 1024)
        }
        return "\(size) B"
    }
}

class Pk3Scanner: ObservableObject {
    @Published var files: [Pk3Info] = []
    @Published var isScanning = false
    @Published var currentFile = ""
    @Published var hasScanned = false

    private let folders = ["main", "mainta", "maintt"]

    var summary: String {
        let verified = files.filter { $0.status == .verified }.count
        let corrupt = files.filter { $0.status == .corrupt }.count
        let unknown = files.filter { $0.status == .unknown }.count
        var parts: [String] = []
        if verified > 0 { parts.append("\(verified) verified") }
        if corrupt > 0 { parts.append("\(corrupt) corrupt") }
        if unknown > 0 { parts.append("\(unknown) unknown") }
        return parts.isEmpty ? "No pak files found" : parts.joined(separator: ", ")
    }

    // Overall pass/fail rollup for the Connect tab's one-line status:
    // the whole install either checks out or it doesn't.
    var verifiedCount: Int { files.filter { $0.status == .verified }.count }
    var corruptCount: Int { files.filter { $0.status == .corrupt }.count }
    var allVerified: Bool { !files.isEmpty && files.allSatisfy { $0.status == .verified } }

    func scan() {
        guard !isScanning else { return }
        isScanning = true
        hasScanned = true

        let gameDir = LauncherSettings.gameDirectory

        var found: [Pk3Info] = []

        let allFolders = (try? FileManager.default.contentsOfDirectory(atPath: gameDir)) ?? []

        for folder in folders {
            guard let actualFolder = allFolders.first(where: { $0.lowercased() == folder }) else {
                continue
            }
            let folderPath = (gameDir as NSString).appendingPathComponent(actualFolder)
            guard let contents = try? FileManager.default.contentsOfDirectory(atPath: folderPath) else {
                continue
            }
            let paks = contents.filter {
                let lower = $0.lowercased()
                return lower.hasPrefix("pak") && lower.hasSuffix(".pk3")
            }.sorted()
            for pak in paks {
                let fullPath = (folderPath as NSString).appendingPathComponent(pak)
                let size = (try? FileManager.default.attributesOfItem(atPath: fullPath)[.size] as? UInt64) ?? 0
                found.append(Pk3Info(folder: actualFolder, filename: pak, size: size))
            }
        }

        // Show all files immediately so layout is stable
        files = found

        DispatchQueue.global(qos: .userInitiated).async { [weak self] in
            for i in 0..<found.count {
                let info = found[i]
                let fullPath = (gameDir as NSString)
                    .appendingPathComponent(info.folder)
                    .appending("/\(info.filename)")

                DispatchQueue.main.async {
                    self?.currentFile = "\(info.folder)/\(info.filename)"
                }

                let md5 = Self.computeMD5(path: fullPath)

                let expected = Pk3Hashes.retail[info.key]
                let status: Pk3Status
                if let expected = expected {
                    status = (md5 == expected) ? .verified : .corrupt
                } else {
                    status = .unknown
                }

                DispatchQueue.main.async {
                    if let self = self, i < self.files.count {
                        self.files[i].md5 = md5
                        self.files[i].status = status
                    }
                }
            }

            DispatchQueue.main.async {
                self?.isScanning = false
                self?.currentFile = ""
            }
        }
    }

    private static func computeMD5(path: String) -> String {
        guard let handle = FileHandle(forReadingAtPath: path) else { return "error" }
        defer { handle.closeFile() }

        var context = CC_MD5_CTX()
        CC_MD5_Init(&context)

        let bufferSize = 65536
        while autoreleasepool(invoking: {
            let data = handle.readData(ofLength: bufferSize)
            if data.isEmpty { return false }
            data.withUnsafeBytes { ptr in
                _ = CC_MD5_Update(&context, ptr.baseAddress, CC_LONG(data.count))
            }
            return true
        }) {}

        var digest = [UInt8](repeating: 0, count: Int(CC_MD5_DIGEST_LENGTH))
        CC_MD5_Final(&digest, &context)

        return digest.map { String(format: "%02x", $0) }.joined()
    }
}

/// Install check as a single icon button: tap to verify, the checkmark turns
/// green when every pak checks out (red ✗ if any fail, spinner while checking).
/// Hover shows the file count. Owns its own scanner.
struct VerifyButton: View {
    @StateObject private var scanner = Pk3Scanner()

    var body: some View {
        Button(action: { scanner.scan() }) {
            icon
                .frame(width: 28, height: 28)
                .background(Circle().fill(Color.primary.opacity(0.06)))
                .contentShape(Circle())
        }
        .buttonStyle(.plain)
        .disabled(scanner.isScanning)
        .accessibilityLabel("Verify game files")
        .accessibilityValue(helpText)
        .help(helpText)
    }

    @ViewBuilder
    private var icon: some View {
        if scanner.isScanning {
            ProgressView()
                .scaleEffect(0.6)
        } else {
            Image(systemName: scanner.hasScanned && scanner.corruptCount > 0 ? "xmark" : "checkmark")
                .font(.system(size: 15, weight: .bold))
                .foregroundColor(iconColor)
        }
    }

    private var iconColor: Color {
        if !scanner.hasScanned {
            return .secondary
        } else if scanner.corruptCount > 0 {
            return .red
        } else if scanner.allVerified {
            return .green
        } else {
            return .secondary
        }
    }

    private var helpText: String {
        if scanner.isScanning {
            return "Verifying game files…"
        } else if !scanner.hasScanned {
            return "Verify game files"
        } else if scanner.corruptCount > 0 {
            return "\(scanner.corruptCount) of \(scanner.files.count) files failed"
        } else if scanner.allVerified {
            return "\(scanner.files.count) files verified"
        } else {
            return "\(scanner.verifiedCount)/\(scanner.files.count) verified"
        }
    }
}
