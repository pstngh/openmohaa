// swift-tools-version:5.7
import PackageDescription

let package = Package(
    name: "launcher",
    platforms: [
        .macOS(.v13)
    ],
    targets: [
        .executableTarget(
            name: "launcher",
            path: "Sources/LauncherMac"
        )
    ]
)
