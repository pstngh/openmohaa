import SwiftUI

struct ContentView: View {
    @Environment(\.scenePhase) private var scenePhase
    @ObservedObject var settings: LauncherSettings
    @State private var bookmarkNameInput: String = ""
    @State private var showBookmarkNaming: Int? = nil

    var body: some View {
        HStack(alignment: .top, spacing: Theme.sectionGap) {
            VStack(spacing: Theme.sectionGap) {
                connectPanel
                CrosshairView(settings: settings)
            }
            .frame(width: 270)

            BotsView(settings: settings)
                .frame(maxWidth: .infinity)
        }
        .padding(Theme.pagePadding)
        .frame(width: 800, height: 520)
        .onChange(of: settings.ip) { _ in settings.serverAddressChanged() }
        .onChange(of: scenePhase) { phase in
            if phase != .active {
                settings.flushPendingSave()
            }
        }
        .onDisappear { settings.flushPendingSave() }
    }

    private var connectPanel: some View {
        VStack(spacing: Theme.sectionGap) {
            Card {
                HStack(alignment: .top, spacing: 8) {
                    compactField("Nickname", text: $settings.nickname)
                    compactField("Server IP", text: $settings.ip)
                }

                HStack(alignment: .top, spacing: 8) {
                    compactField("Password", text: $settings.password, secure: true)
                    compactField("RCON", text: $settings.rconPassword, secure: true)
                }

                Text("BOOKMARKS")
                    .font(.system(size: 10, weight: .semibold))
                    .tracking(0.7)
                    .foregroundColor(.secondary)
                    .padding(.top, 2)

                ForEach(0..<maxBookmarks, id: \.self) { i in
                    bookmarkRow(index: i)
                }

                HStack(spacing: 12) {
                    Toggle("Compass", isOn: $settings.compassEnabled)
                        .toggleStyle(.checkbox)
                        .font(.system(size: 11))
                        .help("Show the compass. Top messages align left automatically when it is hidden.")

                    Toggle("Record my movement", isOn: $settings.clientMoveLog)
                        .toggleStyle(.checkbox)
                        .font(.system(size: 11))
                        .help("Record local movement, input, aim, target, and collision telemetry while connected.")

                    Spacer(minLength: 0)
                }
            }

            LaunchButton(title: "Connect", systemImage: "bolt.fill") {
                GameLauncher.launch(settings: settings)
            }
        }
    }

    private func compactField(
        _ label: String,
        text: Binding<String>,
        secure: Bool = false
    ) -> some View {
        VStack(alignment: .leading, spacing: 3) {
            Text(label)
                .font(.system(size: 10))
                .foregroundColor(.secondary)

            if secure {
                SecureField("", text: text)
                    .textFieldStyle(.roundedBorder)
                    .font(.system(size: 12))
            } else {
                TextField("", text: text)
                    .textFieldStyle(.roundedBorder)
                    .font(.system(size: 12))
            }
        }
        .frame(maxWidth: .infinity)
    }

    @ViewBuilder
    private func bookmarkRow(index i: Int) -> some View {
        HStack(spacing: 6) {
            Button(action: { settings.applyBookmark(i) }) {
                Text(settings.bookmarks[i].name.isEmpty ? "(empty)" : settings.bookmarks[i].name)
                    .font(.system(size: 12))
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .lineLimit(1)
                    .foregroundColor(settings.bookmarks[i].name.isEmpty ? .secondary : .primary)
            }
            .buttonStyle(.plain)
            .disabled(settings.bookmarks[i].name.isEmpty)

            Button(action: {
                settings.applyBookmark(i)
                GameLauncher.launch(settings: settings)
            }) {
                Image(systemName: "play.fill")
                    .font(.system(size: 10))
            }
            .disabled(settings.bookmarks[i].name.isEmpty)

            Button("Save") {
                let defaultName = !settings.bookmarks[i].name.isEmpty
                    ? settings.bookmarks[i].name
                    : (!settings.serverAddress.isEmpty ? settings.serverAddress : "Bookmark \(i + 1)")
                bookmarkNameInput = defaultName
                showBookmarkNaming = i
            }
            .font(.system(size: 11))

            Button(action: { settings.bookmarks[i] = Bookmark() }) {
                Image(systemName: "trash").foregroundColor(.red).font(.system(size: 10))
            }
            .frame(width: 20)
            .disabled(settings.bookmarks[i].name.isEmpty)
        }
        .sheet(isPresented: Binding(
            get: { showBookmarkNaming == i },
            set: { if !$0 { showBookmarkNaming = nil } }
        )) {
            VStack(spacing: 12) {
                Text("Bookmark Name").font(.headline)
                TextField("Name", text: $bookmarkNameInput)
                    .textFieldStyle(.roundedBorder)
                    .frame(width: 200)
                HStack {
                    Button("Cancel") { showBookmarkNaming = nil }
                    Button("OK") {
                        settings.saveBookmark(i, name: bookmarkNameInput)
                        showBookmarkNaming = nil
                    }
                    .keyboardShortcut(.defaultAction)
                }
            }
            .padding()
            .frame(width: 260)
        }
    }
}
