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
        .frame(width: 800, height: 500)
        .onChange(of: settings.ip) { _ in settings.save() }
        .onChange(of: settings.password) { _ in settings.save() }
        .onChange(of: settings.rconPassword) { _ in settings.save() }
        .onChange(of: settings.nickname) { _ in settings.save() }
        .onChange(of: settings.resolutionIndex) { _ in settings.save() }
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
            Button(action: { loadBookmark(i) }) {
                Text(settings.bookmarks[i].name.isEmpty ? "(empty)" : settings.bookmarks[i].name)
                    .font(.system(size: 12))
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .lineLimit(1)
                    .foregroundColor(settings.bookmarks[i].name.isEmpty ? .secondary : .primary)
            }
            .buttonStyle(.plain)
            .disabled(settings.bookmarks[i].name.isEmpty)

            Button(action: {
                loadBookmark(i)
                GameLauncher.launch(settings: settings)
            }) {
                Image(systemName: "play.fill")
                    .font(.system(size: 10))
            }
            .disabled(settings.bookmarks[i].name.isEmpty)

            Button("Save") {
                let defaultName = !settings.bookmarks[i].name.isEmpty
                    ? settings.bookmarks[i].name
                    : (!settings.ip.isEmpty ? settings.ip : "Bookmark \(i + 1)")
                bookmarkNameInput = defaultName
                showBookmarkNaming = i
            }
            .font(.system(size: 11))

            Button(action: {
                settings.bookmarks[i] = Bookmark()
                settings.save()
            }) {
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
                        saveBookmark(i, name: bookmarkNameInput)
                        showBookmarkNaming = nil
                    }
                    .keyboardShortcut(.defaultAction)
                }
            }
            .padding()
            .frame(width: 260)
        }
    }

    private func loadBookmark(_ index: Int) {
        let bm = settings.bookmarks[index]
        settings.ip = bm.ip
        settings.password = bm.password
        settings.rconPassword = bm.rconPassword
    }

    private func saveBookmark(_ index: Int, name: String) {
        guard !name.isEmpty else { return }
        settings.bookmarks[index].name = name
        settings.bookmarks[index].ip = settings.ip
        settings.bookmarks[index].password = settings.password
        settings.bookmarks[index].rconPassword = settings.rconPassword
        settings.save()
    }
}
