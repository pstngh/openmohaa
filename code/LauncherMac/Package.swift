// swift-tools-version:5.7
import PackageDescription

let package = Package(
    name: "Launcher",
    platforms: [
        .macOS(.v13)
    ],
    targets: [
        .executableTarget(
            name: "Launcher",
            path: "Sources/LauncherMac"
        )
    ]
)
