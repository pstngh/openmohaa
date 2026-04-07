import SwiftUI

@main
struct LauncherMacApp: App {
    @StateObject private var settings = LauncherSettings()

    var body: some Scene {
        WindowGroup {
            ContentView(settings: settings)
        }
        .windowResizability(.contentSize)
    }
}
