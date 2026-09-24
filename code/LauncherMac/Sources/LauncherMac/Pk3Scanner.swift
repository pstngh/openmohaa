import CryptoKit
import SwiftUI

enum Pk3Status {
    case verified
    case corrupt
    case unknown
    case missing
    case scanning
}

struct Pk3Info {
    let folder: String
    let filename: String
    var status: Pk3Status = .scanning

    var key: String { "\(folder.lowercased())/\(filename.lowercased())" }
}

class Pk3Scanner: ObservableObject {
    @Published var files: [Pk3Info] = []
    @Published var isScanning = false
    @Published var hasScanned = false

    private let folders = ["main", "mainta", "maintt"]

    // Overall pass/fail rollup for the one-line status: the whole install
    // either checks out or it doesn't.
    var verifiedCount: Int { files.filter { $0.status == .verified }.count }
    var corruptCount: Int { files.filter { $0.status == .corrupt }.count }
    var missingCount: Int { files.filter { $0.status == .missing }.count }
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
                found.append(Pk3Info(folder: actualFolder, filename: pak))
            }

            // List the retail paks this game folder should have but does not,
            // so an incomplete install is never reported as fully verified.
            let present = Set(paks.map { $0.lowercased() })
            let prefix = "\(folder)/"
            let missing = Pk3Hashes.retail.keys
                .filter { $0.hasPrefix(prefix) && !present.contains(String($0.dropFirst(prefix.count))) }
                .sorted()
            for key in missing {
                let filename = String(key.dropFirst(prefix.count))
                found.append(Pk3Info(folder: actualFolder, filename: filename, status: .missing))
            }
        }

        // Show all files immediately so layout is stable
        files = found

        DispatchQueue.global(qos: .userInitiated).async { [weak self] in
            for i in 0..<found.count where found[i].status != .missing {
                let info = found[i]
                let fullPath = (gameDir as NSString)
                    .appendingPathComponent(info.folder)
                    .appending("/\(info.filename)")

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
                        self.files[i].status = status
                    }
                }
            }

            DispatchQueue.main.async {
                self?.isScanning = false
            }
        }
    }

    private static func computeMD5(path: String) -> String {
        guard let handle = FileHandle(forReadingAtPath: path) else { return "error" }
        defer { try? handle.close() }

        var md5 = Insecure.MD5()
        let bufferSize = 65536
        do {
            // read(upToCount:) reports read errors, where readData(ofLength:)
            // raises an exception that would crash the launcher.
            while try autoreleasepool(invoking: { () throws -> Bool in
                guard let data = try handle.read(upToCount: bufferSize), !data.isEmpty else {
                    return false
                }
                md5.update(data: data)
                return true
            }) {}
        } catch {
            return "error"
        }

        return md5.finalize().map { String(format: "%02x", $0) }.joined()
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
        } else if scanner.missingCount > 0 {
            return "\(scanner.verifiedCount)/\(scanner.files.count) verified, \(scanner.missingCount) missing"
        } else {
            return "\(scanner.verifiedCount)/\(scanner.files.count) verified"
        }
    }
}
